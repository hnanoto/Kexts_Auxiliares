#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

shasum -a 256 \
  "$ROOT_DIR/kexts/AirPortUtility.kext/Contents/MacOS/AirPortUtility" \
  "$ROOT_DIR/kexts/BluetoothFileExchange.kext/Contents/MacOS/BluetoothFileExchange"
