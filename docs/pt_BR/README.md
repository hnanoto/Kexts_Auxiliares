# Kexts Auxiliares — Guia (PT-BR)

Este repositório mantém dois kexts com nomes finais de produção:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

## Responsabilidades dos Kexts & Vantagens

Estes kexts são drivers estabilizadores avançados tanto para Hackintoshes quanto para Macs Reais rodando versões modernas do macOS. Em vez de apenas monitorar ou prover injeção cosmética, eles contam com um **Sistema de Recuperação Watchdog a nível de hardware** capaz de auto-reparar controladores que não estejam respondendo, em tempo real, e sem causar vazamentos de memória (Memory Leaks) ou "System Panics".

- **`AirPortUtility.kext`**:
  - Monitora ativamente os serviços de Wi-Fi e Rede Local (`IO80211Controller`, `AirPort_BrcmNIC`, `itlwm`).
  - Se a conexão cair ou a placa Wi-Fi travar (por ex., após voltar do repouso/Sleep mode), o Watchdog dispara um comando síncrono `terminate()` sobre a interface crashada e imediatamente emite um `registerService()` direto na base do barramento (PCIe/USB).
  - Isso simula um "desconectar e reconectar" físico direto no kernel, ressuscitando sua Rede/Wi-Fi instantaneamente.

- **`BluetoothFileExchange.kext`**:
  - Focado na estabilização de Controladoras e Protocolos de Transporte Bluetooth.
  - Funciona de modo idêntico ao seu equivalente de Wi-Fi, monitorando o `IOBluetoothHCIController` e transportes USB/UART.
  - Revive módulos Bluetooth "mortos" forçando o Barramento Virtual da Apple a recarregar o pacote de drivers sem você precisar reiniciar a máquina.

**Por que adicioná-los à sua EFI / macOS?**
Eles garantem **Capacidades de Auto-Cura** para o seu ambiente. Caso você sofra quedas aleatórias de Wi-Fi, ícone de Bluetooth sumindo da barra de menus ou bugs ao acordar o PC do repouso, esses Kexts vão achar a falha em background e forçar o hardware a voltar à vida de forma 100% autônoma dentro de no máximo 15 segundos.

## Estrutura do repositório

- `kexts/`: binários prontos para teste/uso
- `xcode/Hackintosh-And-Beyond-Kexts/`: fonte e projeto de build
- `templates/`: snippets de `Info.plist` e OpenCore
- `docs/`: documentação

## OpenCore (recomendado para Hackintosh)

1. Copie os arquivos de `kexts/` para `EFI/OC/Kexts/`.
2. Use o snippet `templates/OpenCore-Kernel-Add-snippet.plist` para `Kernel -> Add`.
3. Gere snapshot da sua EFI e reinicie.

## Clover

1. Copie os arquivos de `kexts/` para `EFI/CLOVER/kexts/Other/`.
2. Clover injeta automaticamente os kexts da pasta `Other`.
3. Guia objetivo: `templates/Clover-Install-Guide.txt`.

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
./scripts/package_release_assets.sh
```

Assets de distribuicao sao gerados em `dist/release/`.

## CI/Release (GitHub Actions)

- Push em `main`: CI + artifacts de distribuicao.
- Tag `v*`: CI + publicacao de release com ZIPs e `SHA256SUMS.txt`.

## Observações técnicas

- Build padrão: `x86_64`
- Assinatura no fluxo local: ad-hoc
- Em ambientes com política rígida de segurança, assinatura Apple de kext pode ser exigida

## Guia tecnico adicional

- `docs/pt_BR/KEXT_XCODE_TAHOE.md`
