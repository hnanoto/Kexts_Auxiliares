#!/usr/bin/env zsh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
PROJECT_FILE="$PROJECT_ROOT/Hackintosh-And-Beyond-Kexts.xcodeproj/project.pbxproj"

cd "$PROJECT_ROOT"
xcodegen generate

# xcodegen does not expose "kernel-extension" target type directly.
# Patch product references and target productType to proper kext values.
perl -0pi -e 's/([A-Za-z0-9_]+)\.bundle/$1.kext/g' "$PROJECT_FILE"
perl -0pi -e 's/com\.apple\.product-type\.bundle/com.apple.product-type.kernel-extension/g' "$PROJECT_FILE"
perl -0pi -e 's/com\.apple\.product-type\.kext/com.apple.product-type.kernel-extension/g' "$PROJECT_FILE"

xcodebuild -list -project Hackintosh-And-Beyond-Kexts.xcodeproj
