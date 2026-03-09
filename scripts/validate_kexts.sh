#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
KEXT_DIR="$ROOT_DIR/kexts"

check_kext() {
  local name="$1"
  local bundle="$KEXT_DIR/$name.kext"
  local bin="$bundle/Contents/MacOS/$name"
  local plist="$bundle/Contents/Info.plist"

  echo "== $name =="

  [ -d "$bundle" ] || { echo "Missing bundle: $bundle"; return 1; }
  [ -f "$bin" ] || { echo "Missing executable: $bin"; return 1; }
  [ -f "$plist" ] || { echo "Missing plist: $plist"; return 1; }

  file "$bin" | grep -q "kext bundle" || {
    echo "Binary is not MH_KEXT_BUNDLE";
    return 1;
  }

  local pkg
  pkg="$(/usr/libexec/PlistBuddy -c 'Print :CFBundlePackageType' "$plist" 2>/dev/null || true)"
  [ "$pkg" = "KEXT" ] || {
    echo "CFBundlePackageType is '$pkg' (expected KEXT)";
    return 1;
  }

  /usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$plist" >/dev/null
  /usr/libexec/PlistBuddy -c 'Print :IOKitPersonalities' "$plist" >/dev/null

  otool -hv "$bin" | awk 'NR==4{print "Mach-O filetype:", $5}'
  echo "Validation: OK"
}

check_kext AirPortUtility
check_kext BluetoothFileExchange

echo "All validations passed."
