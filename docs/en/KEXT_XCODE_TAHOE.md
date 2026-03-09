# Real Kext Build in Xcode (Tahoe)

## Goal

Build real kernel extensions with final production names:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Requirements

- Xcode 26+
- `xcodegen`
- MacKernelSDK

## Steps

```bash
cd xcode/Hackintosh-And-Beyond-Kexts
./scripts/install_mackernelsdk.sh
./scripts/generate_project.sh
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme AirPortUtility -configuration Release build
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme BluetoothFileExchange -configuration Release build
```

## Repository release flow

From repository root:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
```

This updates `kexts/` with fresh release binaries.

## OpenCore snippet

Use:

- `templates/OpenCore-Kernel-Add-snippet.plist`

## Notes

- Default architecture in this repo: `x86_64`
- Local flow signs ad-hoc for testing
- For stricter environments, use a proper Apple kext signing identity
