# 🚀 Hackintosh-And-Beyond Kexts v1.1.1: Expanding the Watchdog

Based on feedback and real-world usage, we've extended the powerful hardware-level recovery function inside **`AirPortUtility.kext`** to cover Ethernet Controllers too!

## 🌟 What's new in v1.1.1?

### 🛜 AirPortUtility.kext (Wired Network Guardian)
- **Ethernet Auto-Recovery:** The Watchdog now officially monitors and heals **Wired networks (Ethernet)**.
- We added the generic `IOEthernetController` superclass to the internal termination/re-probing recovery list. 
- If your wired network board (like `IntelMausi`, `RealtekRTL8111`, `AppleIntelI210`, or `LucyRTL8125Ethernet`) stops responding or freezes out of nowhere, the Hardware Watchdog will now detect the failure and trigger the exact same Self-Healing mechanisms it uses for Wi-Fi. 

Enjoy your panic-free, ultra-stable network, wired or wireless! 🌍
