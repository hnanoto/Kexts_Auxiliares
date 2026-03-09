#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
XCODE_DIR="$ROOT_DIR/xcode/Hackintosh-And-Beyond-Kexts"
BUILD_DIR="$ROOT_DIR/.build/release"
OUT_DIR="$ROOT_DIR/kexts"
STAMP="$(date +%Y%m%d-%H%M%S)"

cd "$XCODE_DIR"

if [ ! -d "$XCODE_DIR/external/MacKernelSDK/.git" ]; then
  ./scripts/install_mackernelsdk.sh
fi

./scripts/generate_project.sh >/tmp/kexts_aux_generate.log

xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj \
  -scheme AirPortUtility \
  -configuration Release \
  ARCHS=x86_64 \
  ONLY_ACTIVE_ARCH=YES \
  CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
  build

xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj \
  -scheme BluetoothFileExchange \
  -configuration Release \
  ARCHS=x86_64 \
  ONLY_ACTIVE_ARCH=YES \
  CONFIGURATION_BUILD_DIR="$BUILD_DIR" \
  build

for KEXT in AirPortUtility BluetoothFileExchange; do
  SRC="$BUILD_DIR/$KEXT.kext"
  DST="$OUT_DIR/$KEXT.kext"

  if [ ! -d "$SRC" ]; then
    echo "Missing build output: $SRC"
    exit 1
  fi

  if [ -d "$DST" ]; then
    mv "$DST" "$OUT_DIR/$KEXT.kext.bak.$STAMP"
  fi

  cp -R "$SRC" "$DST"
  codesign --force --deep --sign - --timestamp=none "$DST" >/dev/null 2>&1 || true

done

echo "Release kexts updated in: $OUT_DIR"
