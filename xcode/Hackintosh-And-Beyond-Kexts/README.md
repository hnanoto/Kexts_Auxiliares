# Hackintosh-And-Beyond Kexts (Xcode skeleton)

This project is a starter skeleton with two legacy kernel extension targets:

- AirPortUtility.kext
- BluetoothFileExchange.kext

Identity in plist metadata:

- Project: Hackintosh-And-Beyond
- Developer: Hnanoto

## Requirements

- Xcode 26+
- xcodegen (already installed on this machine)
- MacKernelSDK headers in:
  - `external/MacKernelSDK/Headers`

The code is intentionally minimal and is meant as a base to evolve.

## Generate Xcode project

```bash
cd xcode/Hackintosh-And-Beyond-Kexts
./scripts/generate_project.sh
```

If `external/MacKernelSDK/Headers` is missing, install it first:

```bash
./scripts/install_mackernelsdk.sh
./scripts/generate_project.sh
```

This creates:

- `Hackintosh-And-Beyond-Kexts.xcodeproj`
- with target product type patched to `com.apple.product-type.kernel-extension`

## Validate project

```bash
xcodebuild -list -project Hackintosh-And-Beyond-Kexts.xcodeproj
```

Optional compile check:

```bash
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj \
  -scheme AirPortUtility \
  -configuration Debug \
  build
```

## Build notes

- Modern macOS heavily restricts third-party legacy kext loading.
- For Apple Silicon, reduced security policy is usually required for third-party kext tests.
- Prefer DriverKit when possible.
