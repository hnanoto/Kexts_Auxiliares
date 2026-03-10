#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
XCODE_DIR="$ROOT_DIR/xcode/Hackintosh-And-Beyond-Kexts"
BUILD_DIR="$ROOT_DIR/.build/release"
OUT_DIR="$ROOT_DIR/kexts"
VERSION="${KEXT_VERSION:-1.3.0}"
BUILD_NUMBER="${KEXT_BUILD_NUMBER:-$VERSION}"
SCHEMES=("AirPortUtility" "BluetoothFileExchange" "AudioStabilizer" "BatteryWatchdog" "USBStabilizer" "IGPUWatchdog")
GENERATE_LOG="/tmp/kexts_aux_generate.log"
BUILD_LOG="/tmp/kexts_aux_build.log"

if [ "${1:-}" != "" ]; then
  VERSION="$1"
fi

if [ "${2:-}" != "" ]; then
  BUILD_NUMBER="$2"
fi

echo "Building release kexts"
echo "Version: $VERSION"
echo "Build number: $BUILD_NUMBER"

cd "$XCODE_DIR"

if [ ! -d "$XCODE_DIR/external/MacKernelSDK/Headers" ]; then
  ./scripts/install_mackernelsdk.sh
fi

: >"$BUILD_LOG"
if ! ./scripts/generate_project.sh >"$GENERATE_LOG" 2>&1; then
  echo "Failed generating Xcode project."
  echo "----- generate_project.sh log -----"
  cat "$GENERATE_LOG" || true
  exit 74
fi

for SCHEME in "${SCHEMES[@]}"; do
  if ! xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj \
    -scheme "$SCHEME" \
    -configuration Release \
    ARCHS=x86_64 \
    ONLY_ACTIVE_ARCH=YES \
    MARKETING_VERSION="$VERSION" \
    CURRENT_PROJECT_VERSION="$BUILD_NUMBER" \
    CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
    build 2>&1 | tee -a "$BUILD_LOG"; then
    echo "xcodebuild failed for scheme: $SCHEME"
    echo "----- xcodebuild log tail -----"
    tail -n 200 "$BUILD_LOG" || true
    exit 74
  fi
done

for KEXT in "${SCHEMES[@]}"; do
  SRC="$BUILD_DIR/$KEXT.kext"
  DST="$OUT_DIR/$KEXT.kext"

  if [ ! -d "$SRC" ]; then
    echo "Missing build output: $SRC"
    exit 1
  fi

  rm -rf "$DST"
  cp -R "$SRC" "$DST"
  codesign --force --deep --sign - --timestamp=none "$DST" >/dev/null 2>&1 || true
done

echo "Release kexts updated in: $OUT_DIR"
