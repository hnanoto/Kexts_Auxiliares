# Auxiliary Kexts — Guide (EN)

This repository ships two production-named kexts:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Kext responsibilities

- `AirPortUtility.kext`:
  - focuses on Wi-Fi and local network service stability at boot/runtime
  - runs a watchdog for Wi-Fi/network service availability
  - performs gentle recovery cycles when repeated degraded checks are detected
- `BluetoothFileExchange.kext`:
  - focuses on Bluetooth service stability at boot/runtime
  - runs a watchdog for Bluetooth controller/transport availability
  - performs gentle recovery cycles when repeated degraded checks are detected

These are auxiliary stabilizer kexts for EFI-based workflows. They are not full hardware replacement drivers.

## Repository layout

- `kexts/`: ready-to-test binaries
- `xcode/Hackintosh-And-Beyond-Kexts/`: source and build project
- `templates/`: `Info.plist` and OpenCore snippets
- `docs/`: documentation

## OpenCore (recommended for Hackintosh)

1. Copy kexts from `kexts/` to `EFI/OC/Kexts/`.
2. Use `templates/OpenCore-Kernel-Add-snippet.plist` for `Kernel -> Add`.
3. Re-snapshot your EFI config and reboot.

## Clover

1. Copy kexts from `kexts/` to `EFI/CLOVER/kexts/Other/`.
2. Clover injects kexts from `Other` automatically.
3. Quick guide: `templates/Clover-Install-Guide.txt`.

## /Library/Extensions install (optional)

Use only if you know the implications:

```bash
cd kexts
./instalar_auxiliares_autoelevado.command
```

## Local build (Xcode)

```bash
cd xcode/Hackintosh-And-Beyond-Kexts
./scripts/install_mackernelsdk.sh
./scripts/generate_project.sh
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme AirPortUtility -configuration Release build
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme BluetoothFileExchange -configuration Release build
```

## Release pipeline in this repo

From repo root:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
./scripts/package_release_assets.sh
```

Distribution assets are generated in `dist/release/`.

## CI/Release (GitHub Actions)

- Push to `main`: full CI + distribution artifacts.
- Tag `v*`: full CI + GitHub Release publish with ZIPs and `SHA256SUMS.txt`.

## Technical notes

- Default build arch: `x86_64`
- Local signing flow: ad-hoc
- On stricter macOS security policy, Apple kext signing may be required

## Additional technical guide

- `docs/en/KEXT_XCODE_TAHOE.md`
