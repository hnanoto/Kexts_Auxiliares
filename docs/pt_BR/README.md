# Kexts Auxiliares — Guia (PT-BR)

Este repositório mantém dois kexts com nomes finais de produção:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Estrutura do repositório

- `kexts/`: binários prontos para teste/uso
- `xcode/Hackintosh-And-Beyond-Kexts/`: fonte e projeto de build
- `templates/`: snippets de `Info.plist` e OpenCore
- `docs/`: documentação

## OpenCore (recomendado para Hackintosh)

1. Copie os arquivos de `kexts/` para `EFI/OC/Kexts/`.
2. Use o snippet `templates/OpenCore-Kernel-Add-snippet.plist` para `Kernel -> Add`.
3. Gere snapshot da sua EFI e reinicie.

## Instalação em /Library/Extensions (opcional)

Use apenas se você souber exatamente o que está fazendo:

```bash
cd kexts
./instalar_auxiliares_autoelevado.command
```

## Build local (Xcode)

```bash
cd xcode/Hackintosh-And-Beyond-Kexts
./scripts/install_mackernelsdk.sh
./scripts/generate_project.sh
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme AirPortUtility -configuration Release build
xcodebuild -project Hackintosh-And-Beyond-Kexts.xcodeproj -scheme BluetoothFileExchange -configuration Release build
```

## Pipeline de release deste repositório

Na raiz do repo:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
```

## Observações técnicas

- Build padrão: `x86_64`
- Assinatura no fluxo local: ad-hoc
- Em ambientes com política rígida de segurança, assinatura Apple de kext pode ser exigida

## Guia tecnico adicional

- `docs/pt_BR/KEXT_XCODE_TAHOE.md`
