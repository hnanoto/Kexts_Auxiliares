# Kext real no Xcode (Hackintosh-And-Beyond / develop Hnanoto)

## 1) Ponto importante antes de comecar

Neste repositorio, os binarios atuais ja sao kexts reais (`MH_KEXT_BUNDLE` e `CFBundlePackageType = KEXT`).

Se voce estiver migrando de um pacote antigo no formato app (`APPL`), nao existe conversao automatica para kext real.
O caminho correto e sempre compilar um driver novo com `mh_kext_bundle`.

## 2) Estrutura minima de um kext real

```text
NomeDoDriver.kext/
  Contents/
    Info.plist
    MacOS/
      NomeDoDriver
```

Chaves obrigatorias no `Info.plist`:
- `CFBundlePackageType = KEXT`
- `CFBundleExecutable`
- `CFBundleIdentifier`
- `OSBundleLibraries`
- `IOKitPersonalities`

## 3) Fluxo recomendado (Xcode)

1. Crie um target novo para driver (C++).
2. Configure saida como bundle `.kext`.
3. Defina build settings para kext legado:
- `MACH_O_TYPE = mh_kext_bundle`
- `WRAPPER_EXTENSION = kext`
- `INFOPLIST_FILE` apontando para o `Info.plist` do driver
4. Implemente uma classe derivada de `IOService`.
5. Compile e confira se o bundle final ficou no formato acima.

Codigo minimo de classe:

```cpp
#include <IOKit/IOService.h>

class AirPortUtility : public IOService {
  OSDeclareDefaultStructors(AirPortUtility)
public:
  bool start(IOService* provider) override;
  void stop(IOService* provider) override;
};

OSDefineMetaClassAndStructors(AirPortUtility, IOService)

bool AirPortUtility::start(IOService* provider) {
  if (!IOService::start(provider)) return false;
  registerService();
  return true;
}

void AirPortUtility::stop(IOService* provider) {
  IOService::stop(provider);
}
```

## 4) Plists modelo

Use os templates:
- `templates/AirPortUtility.Info.plist`
- `templates/BluetoothFileExchange.Info.plist`

Eles ja incluem:
- identidade `Hackintosh-And-Beyond`
- autoria `develop Hnanoto`
- estrutura basica de `OSBundleLibraries` e `IOKitPersonalities`

## 5) OpenCore `config.plist` (Kernel -> Add)

Snippet pronto em:
- `templates/OpenCore-Kernel-Add-snippet.plist`

Copie cada `dict` para `Kernel -> Add` e ajuste:
- `BundlePath`
- `ExecutablePath`
- `PlistPath`

## 6) Clover

Para Clover, nao existe `Kernel -> Add`.
Instalacao padrao:
- copiar os kexts para `EFI/CLOVER/kexts/Other/`
- reiniciar

Guia pronto:
- `templates/Clover-Install-Guide.txt`

## 7) Validacao antes de boot

```bash
plutil -lint NomeDoDriver.kext/Contents/Info.plist
kmutil inspect --bundle-path NomeDoDriver.kext
```

## 7) Observacoes de Tahoe / macOS moderno

- Kext legado e cada vez mais restrito no macOS moderno.
- Em hardware Apple Silicon, normalmente exige politica de seguranca reduzida para carregar kext de terceiros.
- Sempre prefira DriverKit quando possivel; mantenha kext legado apenas se realmente necessario.

## 8) Empacotamento de release

Na raiz do repositorio:

```bash
./scripts/build_release_kexts.sh
./scripts/validate_kexts.sh
./scripts/package_release_assets.sh
```

Assets gerados:
- `dist/release/Hackintosh-And-Beyond-OpenCore-EFI.zip`
- `dist/release/Hackintosh-And-Beyond-Clover-EFI.zip`
- `dist/release/SHA256SUMS.txt`
