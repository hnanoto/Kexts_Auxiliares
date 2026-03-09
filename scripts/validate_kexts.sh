#!/usr/bin/env zsh
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
KEXT_DIR="$ROOT_DIR/kexts"

check_kext() {
  local name="$1"
  local expected_bundle_id="$2"
  local expected_class="$3"
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

  local pkg bundle_id executable io_class provider_class short_version build_version
  pkg="$(/usr/libexec/PlistBuddy -c 'Print :CFBundlePackageType' "$plist" 2>/dev/null || true)"
  [ "$pkg" = "KEXT" ] || {
    echo "CFBundlePackageType is '$pkg' (expected KEXT)";
    return 1;
  }

  executable="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleExecutable' "$plist" 2>/dev/null || true)"
  [ "$executable" = "$name" ] || {
    echo "CFBundleExecutable is '$executable' (expected $name)";
    return 1;
  }

  bundle_id="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleIdentifier' "$plist" 2>/dev/null || true)"
  [ "$bundle_id" = "$expected_bundle_id" ] || {
    echo "CFBundleIdentifier is '$bundle_id' (expected $expected_bundle_id)";
    return 1;
  }

  io_class="$(/usr/libexec/PlistBuddy -c "Print :IOKitPersonalities:$name:IOClass" "$plist" 2>/dev/null || true)"
  [ "$io_class" = "$expected_class" ] || {
    echo "IOClass is '$io_class' (expected $expected_class)";
    return 1;
  }

  provider_class="$(/usr/libexec/PlistBuddy -c "Print :IOKitPersonalities:$name:IOProviderClass" "$plist" 2>/dev/null || true)"
  [ "$provider_class" = "IOResources" ] || {
    echo "IOProviderClass is '$provider_class' (expected IOResources)";
    return 1;
  }

  short_version="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleShortVersionString' "$plist" 2>/dev/null || true)"
  [ -n "$short_version" ] || {
    echo "CFBundleShortVersionString is empty";
    return 1;
  }

  build_version="$(/usr/libexec/PlistBuddy -c 'Print :CFBundleVersion' "$plist" 2>/dev/null || true)"
  [ -n "$build_version" ] || {
    echo "CFBundleVersion is empty";
    return 1;
  }

  /usr/libexec/PlistBuddy -c 'Print :OSBundleLibraries:com.apple.kpi.bsd' "$plist" >/dev/null
  /usr/libexec/PlistBuddy -c 'Print :OSBundleLibraries:com.apple.kpi.iokit' "$plist" >/dev/null
  /usr/libexec/PlistBuddy -c 'Print :OSBundleLibraries:com.apple.kpi.libkern' "$plist" >/dev/null
  /usr/libexec/PlistBuddy -c 'Print :OSBundleLibraries:com.apple.kpi.mach' "$plist" >/dev/null

  otool -hv "$bin" | awk 'NR==4{print "Mach-O filetype:", $5}'
  echo "Validation: OK"
}

check_kext AirPortUtility org.hackintosh-and-beyond.driver.airportutility AirPortUtility
check_kext BluetoothFileExchange org.hackintosh-and-beyond.driver.bluetoothfileexchange BluetoothFileExchange

echo "All validations passed."
