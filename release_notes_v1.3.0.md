# 🌟 Hackintosh-And-Beyond Kexts v1.3.0: The IGPU Watchdog

You asked, we delivered! The stability suite just got bigger with our latest addition focusing on integrated graphics. 

## 🆕 New in this Release: IGPUWatchdog.kext

For those using Intel and AMD integrated graphics in laptops and selected desktops, graphics glitches, freezes or black screens after waking from Sleep are a thing of the past.

### 🖥️ IGPUWatchdog.kext
- **The Problem it Fixes:** Screen remaining black, graphical artifacts, or complete system freeze when the integrated GPU fails to wake up correctly.
- **The Watchdog:** Actively monitors critical graphics controller classes like `IOGraphicsAccelerator2`, `IntelAccelerator`, and `AppleIntelFramebuffer`. When the GPU stalls, our advanced Watchdog acts in milliseconds to terminate the active graphics state and force macOS to reprobe and reinitialize the IGPU, bringing your display back to life autonomously!

## 📦 Download & Install
You now have access to a suite of **6 Kexts** in the Release package.
- Install them into your OpenCore / Clover like any other kext.
- Snapshot your config, reboot, and enjoy the ultimate stable macOS experience. 🌍
