# 🌟 Hackintosh-And-Beyond Kexts v1.3.2: Complete Ecosystem Integration & Universal GPU Watchdog

You asked, we double-checked! The kexts were compiling and uploading fine, but the *install templates* were slightly behind, causing some confusion for our users.

## 🛠️ What's Fixed in v1.3.2
- **Clover Install Guide (`Clover-Install-Guide.txt`)**: Now correctly lists all 6 kexts (including the new Audio, Battery, USB, and IGPU Watchdogs). 
- **OpenCore Snippet (`OpenCore-Kernel-Add-snippet.plist`)**: Was previously missing the new kexts in the config injection arrays. Now fully updated to map `AudioStabilizer`, `BatteryWatchdog`, `USBStabilizer` and `IGPUWatchdog` natively into OpenCore. 

The `EFI` zip bundles always had the kexts inside, but the config templates are now correctly updated to match the latest ecosystem upgrades. Install and inject with peace of mind!

## 🚀 IGPUWatchdog: It's a Universal Video Watchdog!

Despite the name "IGPU", we've done some heavy engineering on it and transformed it into a **Universal Video Watchdog**. It covers and injects hardware correction rules into **3 distinct environments** at once:

1. **Integrated Intel (UHD, Iris, HD Series):** Covers the roots `IntelAccelerator`, `IOGraphicsAccelerator2`, and `AppleIntelFramebuffer`.
2. **Integrated AMD (Ryzen APUs):** Covers computers with Vega graphics via *NootedRed* through injection into `AMDFramebuffer` and `AMDSupport`.
3. **Dedicated AMD Cards (Polaris, Vega, and Navi):** Actively monitors desktop cards via `AMDRadeonX4000`, `X5000`, and `X6000`.

**What does it do in practice?**
If you use Intel or AMD graphics and usually suffer from the famous "black screens", Kernel Panics, or graphical freezes when the computer **wakes from Sleep**, the problem is over. 
If the kext detects that macOS is failing to reanimate the video PCIe bus after waking up, it intercepts this logical shock in milliseconds, shuts down the frozen graphics path, and forces macOS to restart the card's driver in real-time. The monitor turns on by itself and the system doesn't crash! 💻⚡

## 📦 Download & Install
All **6 Kexts** are securely packed into the Release assets below. Enjoy the stability! 🌍
