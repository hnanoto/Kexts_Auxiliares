# FAQ (PT-BR)

## Esses arquivos sao kexts reais?

Sim. Os binarios atuais sao `MH_KEXT_BUNDLE` com `CFBundlePackageType = KEXT`.

## Posso usar na EFI (OpenCore)?

Sim. Esse e o fluxo recomendado para Hackintosh.

## Posso instalar em /Library/Extensions?

Pode, mas e um fluxo legado/avancado e depende da politica de seguranca do macOS.

## Qual arquitetura e suportada neste repo?

`x86_64` no build padrao.

## Os kexts estao assinados com certificado Apple de producao?

No fluxo local padrao, nao. O pipeline aplica assinatura ad-hoc para testes.

## Como reconstruir os binarios da pasta kexts?

Na raiz do repositorio:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
```
