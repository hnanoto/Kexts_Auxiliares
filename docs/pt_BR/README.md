# 📦 Kexts Auxiliares Hackintosh and Beyond

Este repositório disponibiliza dois "kexts auxiliares" utilizados para **melhorar a estabilidade e funcionalidade das conexões Wi-Fi e Bluetooth** em ambientes **Hackintosh** e até mesmo em **Macs reais**:

- `AirPortUtility.kext`
- `BluetoothFileExchange.kext`

Apesar da extensão `.kext`, esses arquivos são na verdade **utilitários internos da Apple empacotados com estrutura de kext**, com funções específicas ativadas no sistema após o boot.

## ✅ Finalidade

| Kext | Função Principal |
|------|------------------|
| `AirPortUtility.kext` | Reativa recursos avançados do AirPort/Wi-Fi, inclusive suporte a base stations legadas, eventos de conexão, rede mesh e AirPlay |
| `BluetoothFileExchange.kext` | Restaura suporte ao protocolo OBEX (Bluetooth File Transfer) e melhora estabilidade de sessões BT |

## 🔧 Instalação Recomendada

⚠️ **Não coloque estes kexts na EFI** — eles não são drivers de boot.

### Instalar com script autoelevado:

```bash
./instalar_auxiliares_autoelevado.command
```

Insira sua senha quando solicitado e reinicie o sistema.

## 📁 Local de Instalação

```bash
/Library/Extensions
```

## 🖥️ Compatibilidade

| Plataforma | Compatível |
|------------|------------|
| Hackintosh | ✅ Totalmente compatível |
| Mac real   | ✅ Reativa recursos legados |
| EFI (OpenCore/Clover) | ❌ Sem efeito — não carregam no preboot |

## 📂 Conteúdo do Pacote

```
📁 Kexts_Auxiliares/
├── instalar_auxiliares_autoelevado.command
├── AirPortUtility.kext
└── BluetoothFileExchange.kext
```