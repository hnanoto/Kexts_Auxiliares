# Real Kext Build in Xcode (Tahoe)

## Goal

Build real kernel extensions with final production names:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Runtime role split

- `AirPortUtility.kext`: Wi-Fi and local network stabilizer.
- `BluetoothFileExchange.kext`: Bluetooth stabilizer.

Both targets keep the same architecture and boot-injection logic already established in this repository, while adding role-specific watchdog/recovery behavior.

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

## Clover

For Clover there is no `Kernel -> Add`.
Standard installation:
- copy kexts to `EFI/CLOVER/kexts/Other/`
- reboot

Guide:
- `templates/Clover-Install-Guide.txt`

## Notes

- Default architecture in this repo: `x86_64`
- Local flow signs ad-hoc for testing
- For stricter environments, use a proper Apple kext signing identity

## Release packaging

From repository root:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
./scripts/package_release_assets.sh
```

Generated assets include OpenCore/Clover ZIP bundles and `SHA256SUMS.txt`.
