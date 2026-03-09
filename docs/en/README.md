# Auxiliary Kexts — Guide (EN)

This repository ships two production-named kexts:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Repository layout

- `kexts/`: ready-to-test binaries
- `xcode/Hackintosh-And-Beyond-Kexts/`: source and build project
- `templates/`: `Info.plist` and OpenCore snippets
- `docs/`: documentation

## OpenCore (recommended for Hackintosh)

1. Copy kexts from `kexts/` to `EFI/OC/Kexts/`.
2. Use `templates/OpenCore-Kernel-Add-snippet.plist` for `Kernel -> Add`.
3. Re-snapshot your EFI config and reboot.

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
```

## Technical notes

- Default build arch: `x86_64`
- Local signing flow: ad-hoc
- On stricter macOS security policy, Apple kext signing may be required

## Additional technical guide

- `docs/en/KEXT_XCODE_TAHOE.md`
