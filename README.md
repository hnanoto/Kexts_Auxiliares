# Auxiliary Kexts — Guide (EN)

This repository ships six production-named kexts:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`
- `AudioStabilizer.kext`
- `BatteryWatchdog.kext`
- `USBStabilizer.kext`
- `IGPUWatchdog.kext`

## Kext responsibilities & Advantages

These kexts are advanced stabilizer drivers for both Hackintoshes and Real Macs running modern macOS. Rather than just monitoring or providing cosmetic injection, they feature a **hardware-level Watchdog Recovery System** capable of auto-repairing unresponsive controllers on the fly without causing memory leaks or System Panics.

- **`AirPortUtility.kext`**:
  - Automatically monitors Wi-Fi and Local/Wired Network services (`IO80211Controller`, `AirPort_BrcmNIC`, `itlwm`, `IOEthernetController`).
  - If the connection drops or the network card (Wi-Fi or Ethernet) becomes unresponsive (e.g., after waking up from sleep mode), the Watchdog triggers a synchronous `terminate()` over the crashed interface and immediately issues a `registerService()` to the PCIe/USB provider bus.
  - This simulates a forced "unplug and plug" hardware cycle at the kernel level, instantly reviving your Wi-Fi/Ethernet/Network.

- **`BluetoothFileExchange.kext`**:
  - Focuses on Bluetooth Controller and Transport protocol stabilization.
  - Functions exactly like its Wi-Fi counterpart, actively watching `IOBluetoothHCIController` and USB/UART transports.
  - Revives dead Bluetooth modules automatically by forcing the Apple Virtual Bus to reload the driver stack without requiring a system reboot.

- **`AudioStabilizer.kext`**:
  - Monitors high-level Audio controllers (`AppleHDAController`, `IOAudioEngine`, `VoodooHDAUserAudioDriver`).
  - Capable of clearing audio stack crashes and restoring sound after the system wakes from sleep without requiring a reboot.

- **`BatteryWatchdog.kext`**:
  - Ideal for laptops. Monitors the ACPI bus and SMC Battery services (`IOPMPowerSource`, `SMCBatteryManager`, `AppleSmartBatteryManager`).
  - Fixes stuck battery percentages or disappearing battery icons by forcing the VirtualSMC stack to reprobe hardware voltage values on reading pauses.

- **`USBStabilizer.kext`**:
  - Watches XHCI/EHCI/UHCI controllers. If your keyboard/mouse suddenly lags or your USB 3.0 external drive drops, it forces the board to virtually re-plug the internal USB lane within milliseconds.

- **`IGPUWatchdog.kext` (Universal GPU Watchdog)**:
  - Despite the name, this is a universal watchdog for **Intel IGPU**, **AMD IGPU** and **AMD dGPU**.
  - **Intel Integrated:** Covers UHD, Iris, HD series via `IntelAccelerator`, `IOGraphicsAccelerator2` and `AppleIntelFramebuffer`.
  - **AMD Integrated / APUs:** Covers Ryzen APUs (via NootedRed/etc) by injecting into `AMDFramebuffer` and `AMDSupport`.
  - **AMD Dedicated Cards:** Covers Polaris, Vega, and Navi architectures (`AMDRadeonX4000`, `X5000`, `X6000`).
  - Resolves complete system freezes or black screens upon waking from sleep by safely crashing the hung graphics session and reinitializing the GUI framebuffers in an instant.

**Why add them to your EFI / macOS?**
They guarantee **Self-Healing Capabilities** for your environment. If you ever experience random Wi-Fi drops, Bluetooth disappearing from the menu bar, or wake-from-sleep hardware issues, these Kexts will detect the failure and force the hardware back to life autonomously within 15 seconds.

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
