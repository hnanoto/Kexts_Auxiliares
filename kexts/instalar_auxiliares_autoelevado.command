#!/bin/bash
# Script de instalação de kexts auxiliares para Wi-Fi e Bluetooth com autoelevação
# Desenvolvido com padrão profissional — Hackintosh and Beyond

# Reexecutar com sudo se não for root
if [[ $EUID -ne 0 ]]; then
  echo "⚙️ Este script precisa ser executado como root. Solicitando sudo..."
  exec sudo "$0" "$@"
fi

echo ""
echo "🔧 Instalando kexts auxiliares do sistema..."
echo ""

# Caminho onde estão os kexts (mesma pasta do script)
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Lista de kexts a instalar
KEXTS=(
  "AirPortUtility.kext"
  "BluetoothFileExchange.kext"
)

for KEXT in "${KEXTS[@]}"; do
  KEXT_PATH="$SCRIPT_DIR/$KEXT"

  if [ -d "$KEXT_PATH" ]; then
    echo "📦 Instalando $KEXT em /Library/Extensions..."
    cp -R "$KEXT_PATH" /Library/Extensions/
  else
    echo "❌ Kext não encontrado: $KEXT_PATH"
  fi
done

echo ""
echo "🔒 Corrigindo permissões..."
chown -R root:wheel /Library/Extensions/AirPortUtility.kext
chown -R root:wheel /Library/Extensions/BluetoothFileExchange.kext
chmod -R 755 /Library/Extensions/AirPortUtility.kext
chmod -R 755 /Library/Extensions/BluetoothFileExchange.kext

echo ""
echo "🧹 Limpando e reconstruindo cache de kexts..."
kextcache -i /

echo ""
echo "✅ Instalação concluída com sucesso!"
echo "🔁 Reinicie o sistema para aplicar totalmente os efeitos."
