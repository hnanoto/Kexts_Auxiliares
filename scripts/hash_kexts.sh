#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
OUT_FILE="${1:-$ROOT_DIR/dist/release/SHA256SUMS.txt}"
DIST_DIR="$ROOT_DIR/dist/release"

mkdir -p "$(dirname "$OUT_FILE")"
tmp_file="$(mktemp)"
trap 'rm -f "$tmp_file"' EXIT

shasum -a 256 \
  "$ROOT_DIR/kexts/AirPortUtility.kext/Contents/MacOS/AirPortUtility" \
  "$ROOT_DIR/kexts/BluetoothFileExchange.kext/Contents/MacOS/BluetoothFileExchange" >>"$tmp_file"

if [ -d "$DIST_DIR" ]; then
  while IFS= read -r asset; do
    shasum -a 256 "$asset" >>"$tmp_file"
  done < <(find "$DIST_DIR" -maxdepth 1 -type f -name "*.zip" | sort)
fi

sed "s#${ROOT_DIR}/##g" "$tmp_file" >"$OUT_FILE"
cat "$OUT_FILE"
