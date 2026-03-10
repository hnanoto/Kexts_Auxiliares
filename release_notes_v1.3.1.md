# 🌟 Hackintosh-And-Beyond Kexts v1.3.1: Expanding the GPU Watchdog

Following up on our brand new `IGPUWatchdog.kext`, we're already deploying an extended net for maximum stability.

## 🆕 New in this Release: AMD Support

The new watchdog is no longer just for Integrated Graphics! We've injected monitoring rules for the entire AMD ecosystem. The kext now guards against sleep failures and black screens on dedicated AMD Radeon cards as well.

### 🖥️ IGPUWatchdog.kext (Updated)
The internal monitor now covers:
- **Intel Integrated Graphics:** `IOGraphicsAccelerator2`, `IntelAccelerator`, `AppleIntelFramebuffer`
- **AMD Dedicated Graphics:** `AMDRadeonX4000_AMDRadeonController`, `AMDRadeonX5000_AMDRadeonController`, `AMDRadeonX6000_AMDRadeonController`, `AMDFramebuffer`, `AMDSupport`

If your powerful AMD Radeon card (Polaris, Vega, or Navi) fails to initialize video output after the system wakes from sleep, our Watchdog intercepts the fatal timeout and instantly forces an ACPI/PCIe re-initialization of the display driver. No more hard reboots!

## 📦 Download & Install
The suite continues to pack **6 Kexts** inside the Release package, with the `IGPUWatchdog.kext` now supercharged.
- Install them into your OpenCore / Clover like any other kext.
- Snapshot your config, reboot, and enjoy the ultimate stable macOS experience. 🌍
