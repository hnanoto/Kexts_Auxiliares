#!/usr/bin/env zsh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SDK_DIR="$PROJECT_ROOT/external/MacKernelSDK"
SDK_REPO="https://github.com/acidanthera/MacKernelSDK.git"
README_FILE="$SDK_DIR/README.md"

if [ -d "$SDK_DIR/Headers" ] && [ -d "$SDK_DIR/Library" ]; then
  echo "MacKernelSDK headers and libraries already available in $SDK_DIR"
  exit 0
fi

mkdir -p "$SDK_DIR"
tmp_dir="$(mktemp -d)"
trap 'rm -rf "$tmp_dir"' EXIT

git clone --depth 1 "$SDK_REPO" "$tmp_dir/MacKernelSDK"
rm -rf "$SDK_DIR/Headers" "$SDK_DIR/Library"
cp -R "$tmp_dir/MacKernelSDK/Headers" "$SDK_DIR/Headers"
cp -R "$tmp_dir/MacKernelSDK/Library" "$SDK_DIR/Library"

if [ ! -f "$README_FILE" ]; then
  cat >"$README_FILE" <<'EOF'
Place MacKernelSDK headers here.

Expected path used by project settings:

external/MacKernelSDK/Headers

Install with:

./scripts/install_mackernelsdk.sh
EOF
fi

echo "Installed MacKernelSDK headers and libraries at $SDK_DIR"
