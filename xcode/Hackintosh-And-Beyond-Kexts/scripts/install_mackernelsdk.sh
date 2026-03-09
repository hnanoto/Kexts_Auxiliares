#!/usr/bin/env zsh
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SDK_DIR="$PROJECT_ROOT/external/MacKernelSDK"

if [ -d "$SDK_DIR/.git" ]; then
  echo "MacKernelSDK already installed in $SDK_DIR"
  exit 0
fi

rm -rf "$SDK_DIR"
git clone --depth 1 https://github.com/acidanthera/MacKernelSDK.git "$SDK_DIR"
echo "Installed MacKernelSDK at $SDK_DIR"
