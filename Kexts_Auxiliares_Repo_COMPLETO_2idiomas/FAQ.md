# ❓ FAQ — Kexts Auxiliares Hackintosh and Beyond

## 🔸 Esses kexts são drivers de kernel?

❌ Não. Eles têm estrutura `.kext`, mas são aplicativos da Apple disfarçados como kexts. São utilitários que reativam funções legadas de Wi-Fi e Bluetooth.

---

## 🔸 Posso colocá-los na EFI do OpenCore ou Clover?

🚫 Não. A EFI carrega apenas drivers de kernel (`IOKit`). Esses utilitários não serão executados ou carregados se colocados na pasta EFI.

---

## 🔸 Onde devo instalar corretamente?

✅ Em `/Library/Extensions`, usando o script incluso no pacote:
```bash
sudo ./instalar_auxiliares_autoelevado.command
```

---

## 🔸 Eles funcionam em Hackintosh?

✅ Sim. Podem resolver problemas com AirDrop, OBEX Bluetooth, estabilidade de Wi-Fi ou dispositivos mesh legados.

---

## 🔸 Eles funcionam em Macs reais?

✅ Sim. Muitos usuários de Mac relatam melhorias em conexões após reinstalar esses kexts via Terminal.

---

## 🔸 Esses kexts substituem o Lilu ou outros?

🚫 Não. Eles complementam o sistema com utilitários extras, mas **não substituem** kexts como `Lilu.kext`, `BlueToolFixup.kext`, `AirportItlwm.kext` ou `BrcmPatchRAM`.

---

## 🔸 É necessário recompilar o cache?

✅ Sim. O script já faz isso automaticamente ao final da instalação:
```bash
kextcache -i /
```
