#!/usr/bin/env bash
set -euo pipefail

if [[ $EUID -ne 0 ]]; then
  echo "This installer needs root privileges. Requesting sudo..."
  exec sudo "$0" "$@"
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_DIR="/Library/Extensions"
KEXTS=("AirPortUtility.kext" "BluetoothFileExchange.kext")

echo "Installing kexts to $TARGET_DIR"

for kext in "${KEXTS[@]}"; do
  src="$SCRIPT_DIR/$kext"
  dst="$TARGET_DIR/$kext"

  if [[ ! -d "$src" ]]; then
    echo "Missing kext: $src"
    exit 1
  fi

  if [[ -d "$dst" ]]; then
    mv "$dst" "$dst.bak.$(date +%Y%m%d-%H%M%S)"
  fi

  cp -R "$src" "$dst"
  chown -R root:wheel "$dst"
  chmod -R 755 "$dst"
  echo "Installed: $kext"
done

echo "Rebuilding kext cache..."
kextcache -i /

echo "Done. Reboot is recommended."
