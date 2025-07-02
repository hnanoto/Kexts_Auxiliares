# ❓ FAQ — Auxiliary Kexts Hackintosh and Beyond

## 🔸 Are these real kernel drivers?

❌ No. They use the `.kext` structure but are Apple utility apps packaged as kexts. They reactivate legacy Wi-Fi and Bluetooth functionality.

---

## 🔸 Can I place them in OpenCore or Clover EFI?

🚫 No. The EFI only loads valid IOKit drivers. These tools will not be executed or loaded if placed in the EFI.

---

## 🔸 Where should I install them properly?

✅ In `/Library/Extensions`, using the script included:
```bash
sudo ./instalar_auxiliares_autoelevado.command
```

---

## 🔸 Do they work on Hackintosh systems?

✅ Yes. They can help fix AirDrop issues, Bluetooth OBEX support, Wi-Fi instability or mesh sync with legacy AirPort base stations.

---

## 🔸 Do they work on real Macs?

✅ Yes. Some Mac users report improved behavior after manually reinstalling these legacy tools via Terminal.

---

## 🔸 Do these kexts replace Lilu or other drivers?

🚫 No. They complement your system with extra utilities, but do **not replace** `Lilu.kext`, `BlueToolFixup.kext`, `AirportItlwm.kext`, or `BrcmPatchRAM.kext`.

---

## 🔸 Do I need to rebuild kext cache?

✅ Yes. The script already does that automatically:
```bash
kextcache -i /
```
