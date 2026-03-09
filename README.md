# Kexts Auxiliares

Repositório oficial dos kexts auxiliares do projeto Hackintosh-And-Beyond:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Estado atual

- Binários de release atualizados no diretório `kexts/`
- Projeto fonte em `xcode/Hackintosh-And-Beyond-Kexts/`
- Templates de configuração em `templates/`
- Documentação PT-BR e EN em `docs/`

## Uso rápido (OpenCore)

1. Copie os kexts de `kexts/` para `EFI/OC/Kexts/`.
2. Adicione as entradas em `Kernel -> Add` usando:
   - `templates/OpenCore-Kernel-Add-snippet.plist`
3. Faça snapshot no config (ProperTree/OCAT) e reinicie.

## Build de produção

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
```

Isso recompila os dois kexts e atualiza automaticamente a pasta `kexts/`.

## Documentação

- Português: `docs/pt_BR/README.md`
- English: `docs/en/README.md`
- Guia Xcode (PT-BR): `docs/pt_BR/KEXT_XCODE_TAHOE.md`
- Xcode guide (EN): `docs/en/KEXT_XCODE_TAHOE.md`
