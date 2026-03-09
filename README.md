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

## Uso rápido (Clover)

1. Copie os kexts de `kexts/` para `EFI/CLOVER/kexts/Other/`.
2. Clover injeta automaticamente os kexts em `Other`.
3. Guia rápido: `templates/Clover-Install-Guide.txt`.

## Build de produção

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
./scripts/package_release_assets.sh
```

Isso recompila os dois kexts, atualiza `kexts/` e gera assets em `dist/release/`.

## Releases no GitHub Actions

- Push em `main`: roda CI completa e publica artifact (`dist/release/*`).
- Tag `v*` (ex: `v1.2.0`): roda CI e publica GitHub Release com ZIPs e `SHA256SUMS.txt`.

## Documentação

- Português: `docs/pt_BR/README.md`
- English: `docs/en/README.md`
- Guia Xcode (PT-BR): `docs/pt_BR/KEXT_XCODE_TAHOE.md`
- Xcode guide (EN): `docs/en/KEXT_XCODE_TAHOE.md`
