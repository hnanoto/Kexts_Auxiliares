#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
DIST_DIR="$ROOT_DIR/dist/release"
WORK_DIR="$ROOT_DIR/dist/.work"
OPENCORE_ROOT="$WORK_DIR/OpenCore/EFI/OC"
CLOVER_ROOT="$WORK_DIR/Clover/EFI/CLOVER"
VERSION="${1:-${KEXT_VERSION:-}}"
BUILD_DATE_UTC="$(date -u +%Y-%m-%dT%H:%M:%SZ)"
COMMIT_HASH="$(git -C "$ROOT_DIR" rev-parse --short HEAD)"
KEXTS=("AirPortUtility" "BluetoothFileExchange")

if [ -z "$VERSION" ]; then
  VERSION="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' \
    "$ROOT_DIR/kexts/AirPortUtility.kext/Contents/Info.plist" 2>/dev/null || true)"
fi

if [ -z "$VERSION" ]; then
  VERSION="1.0.0"
fi

PACKAGE_NAME="Hackintosh-And-Beyond-Kexts-${VERSION}"

rm -rf "$WORK_DIR" "$DIST_DIR"
mkdir -p "$DIST_DIR" "$OPENCORE_ROOT/Kexts" "$CLOVER_ROOT/kexts/Other"

for KEXT in "${KEXTS[@]}"; do
  SRC="$ROOT_DIR/kexts/$KEXT.kext"
  [ -d "$SRC" ] || { echo "Missing bundle: $SRC"; exit 1; }
  cp -R "$SRC" "$OPENCORE_ROOT/Kexts/$KEXT.kext"
  cp -R "$SRC" "$CLOVER_ROOT/kexts/Other/$KEXT.kext"
done

cp "$ROOT_DIR/templates/OpenCore-Kernel-Add-snippet.plist" \
  "$OPENCORE_ROOT/OpenCore-Kernel-Add-snippet.plist"

cat >"$WORK_DIR/OpenCore/README.txt" <<'EOF'
OpenCore package

1) Copy EFI/OC/Kexts/*.kext to your EFI/OC/Kexts/.
2) Merge OpenCore-Kernel-Add-snippet.plist into config.plist -> Kernel -> Add.
3) Rebuild snapshot (ProperTree/OCAT) and reboot.
EOF

cat >"$WORK_DIR/Clover/README.txt" <<'EOF'
Clover package

1) Copy EFI/CLOVER/kexts/Other/*.kext to your EFI/CLOVER/kexts/Other/.
2) Clover injects kexts from "Other" automatically.
3) Reboot and validate boot logs.
EOF

PACKAGE_ROOT="$WORK_DIR/$PACKAGE_NAME"
mkdir -p "$PACKAGE_ROOT/kexts" "$PACKAGE_ROOT/templates"
for KEXT in "${KEXTS[@]}"; do
  cp -R "$ROOT_DIR/kexts/$KEXT.kext" "$PACKAGE_ROOT/kexts/$KEXT.kext"
done
cp "$ROOT_DIR/templates/OpenCore-Kernel-Add-snippet.plist" "$PACKAGE_ROOT/templates/"
if [ -f "$ROOT_DIR/templates/Clover-Install-Guide.txt" ]; then
  cp "$ROOT_DIR/templates/Clover-Install-Guide.txt" "$PACKAGE_ROOT/templates/"
fi

cat >"$PACKAGE_ROOT/RELEASE-METADATA.txt" <<EOF
Project: Hackintosh-And-Beyond
Developer: Hnanoto
Version: $VERSION
Commit: $COMMIT_HASH
Built: $BUILD_DATE_UTC
Bundles:
- AirPortUtility.kext
- BluetoothFileExchange.kext
EOF

for KEXT in "${KEXTS[@]}"; do
  (
    cd "$ROOT_DIR/kexts"
    ditto -c -k --sequesterRsrc --keepParent "$KEXT.kext" "$DIST_DIR/$KEXT.kext.zip"
  )
done

(
  cd "$WORK_DIR/OpenCore"
  ditto -c -k --sequesterRsrc --keepParent EFI "$DIST_DIR/Hackintosh-And-Beyond-OpenCore-EFI.zip"
)

(
  cd "$WORK_DIR/Clover"
  ditto -c -k --sequesterRsrc --keepParent EFI "$DIST_DIR/Hackintosh-And-Beyond-Clover-EFI.zip"
)

(
  cd "$WORK_DIR"
  ditto -c -k --sequesterRsrc --keepParent "$PACKAGE_NAME" "$DIST_DIR/$PACKAGE_NAME.zip"
)

cp "$ROOT_DIR/templates/OpenCore-Kernel-Add-snippet.plist" \
  "$DIST_DIR/OpenCore-Kernel-Add-snippet.plist"

if [ -f "$ROOT_DIR/templates/Clover-Install-Guide.txt" ]; then
  cp "$ROOT_DIR/templates/Clover-Install-Guide.txt" "$DIST_DIR/Clover-Install-Guide.txt"
fi

"$ROOT_DIR/scripts/hash_kexts.sh" "$DIST_DIR/SHA256SUMS.txt" >/dev/null

echo "Release assets generated in: $DIST_DIR"
ls -1 "$DIST_DIR"
