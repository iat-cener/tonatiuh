#!/bin/bash
# Empaqueta Tonatiuh.app como bundle autocontenido para macOS arm64 (14.6+)
#
# Uso local (firma ad-hoc, solo para esta Mac):
#   ./package_macos.sh
#
# Uso para DISTRIBUIR (firma Developer ID + notarización + DMG engrapado):
#   SIGN_ID="Developer ID Application: Tu Nombre (TEAMID)" \
#   NOTARY_PROFILE=tonatiuh-notary \
#   ./package_macos.sh
#
# Requisitos para distribuir: certificado "Developer ID Application" instalado y
# credenciales guardadas con `xcrun notarytool store-credentials tonatiuh-notary ...`
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
REL="$ROOT/TonatiuhProject/bin/release"
DIST="$ROOT/dist"
QT5="$(brew --prefix qt@5)"
TDE="${TDE_ROOT:-$HOME/tde}"
VERSION="2.2.4"
DMG_PATH="$DIST/Tonatiuh-$VERSION-arm64.dmg"

[ -x "$REL/Tonatiuh.app/Contents/MacOS/Tonatiuh" ] || { echo "ERROR: no existe el build release. Compila primero."; exit 1; }

echo "==> Copiando bundle a dist/"
rm -rf "$DIST"; mkdir -p "$DIST"
cp -R "$REL/Tonatiuh.app" "$DIST/"
APP="$DIST/Tonatiuh.app"
MACOS="$APP/Contents/MacOS"
FW="$APP/Contents/Frameworks"
mkdir -p "$FW"

echo "==> Copiando plugins dentro del bundle"
rm -rf "$MACOS/plugins"
cp -R "$REL/plugins" "$MACOS/plugins"

echo "==> Incrustando librerias propias y Coin/SoQt"
cp -f "$REL/libgeometry.1.dylib" "$REL/libfields.1.dylib" "$FW/"
cp -f "$TDE/local/lib/libCoin.80.dylib" "$TDE/local/lib/libSoQt.20.dylib" "$FW/"

fix_refs() {
  local bin="$1"
  for lib in libgeometry.1.dylib libfields.1.dylib libCoin.80.dylib libSoQt.20.dylib; do
    local ref
    ref="$(otool -L "$bin" | awk '{print $1}' | grep "$lib" | head -1 || true)"
    if [ -n "$ref" ] && [ "$ref" != "@rpath/$lib" ]; then
      install_name_tool -change "$ref" "@rpath/$lib" "$bin"
    fi
  done
}

for lib in libgeometry.1.dylib libfields.1.dylib libCoin.80.dylib libSoQt.20.dylib; do
  install_name_tool -id "@rpath/$lib" "$FW/$lib"
  fix_refs "$FW/$lib"
done

fix_refs "$MACOS/Tonatiuh"

echo "==> Corrigiendo referencias en los plugins"
PLUGIN_ARGS=()
while IFS= read -r -d '' dylib; do
  fix_refs "$dylib"
  PLUGIN_ARGS+=( "-executable=$dylib" )
done < <(find "$MACOS/plugins" -name "*.dylib" -print0)

echo "==> macdeployqt (incrusta frameworks de Qt y corrige rutas)"
"$QT5/bin/macdeployqt" "$APP" "${PLUGIN_ARGS[@]}"

if [ -n "${SIGN_ID:-}" ]; then
  echo "==> Firmando con Developer ID (runtime endurecido + timestamp)"
  # 1) dylibs sueltas (libs propias, Coin/SoQt, los 46 plugins, plugins de Qt)
  find "$APP" -type f -name "*.dylib" -print0 | while IFS= read -r -d '' f; do
    codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$f"
  done
  # 2) frameworks de Qt
  for fwdir in "$FW"/*.framework; do
    [ -d "$fwdir" ] && codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$fwdir"
  done
  # 3) el bundle completo (ejecutable + sellado de recursos)
  codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$APP"

  echo "==> Verificando firma"
  codesign --verify --deep --strict "$APP" && echo "codesign OK"

  if [ -n "${NOTARY_PROFILE:-}" ]; then
    echo "==> Notarizando la app (puede tardar unos minutos)"
    ditto -c -k --keepParent "$APP" "$DIST/Tonatiuh-app.zip"
    xcrun notarytool submit "$DIST/Tonatiuh-app.zip" --keychain-profile "$NOTARY_PROFILE" --wait
    rm -f "$DIST/Tonatiuh-app.zip"
    echo "==> Engrapando ticket a la app"
    xcrun stapler staple "$APP"
  fi

  echo "==> Creando DMG"
  hdiutil create -volname Tonatiuh -srcfolder "$APP" -ov -format UDZO "$DMG_PATH"
  codesign --force --timestamp --sign "$SIGN_ID" "$DMG_PATH"

  if [ -n "${NOTARY_PROFILE:-}" ]; then
    echo "==> Notarizando el DMG"
    xcrun notarytool submit "$DMG_PATH" --keychain-profile "$NOTARY_PROFILE" --wait
    echo "==> Engrapando ticket al DMG"
    xcrun stapler staple "$DMG_PATH"
    echo "==> Evaluacion final de Gatekeeper"
    spctl -a -t open --context context:primary-signature -vv "$DMG_PATH" || true
    spctl -a -t exec -vv "$APP" || true
  fi

  echo ""
  echo "Listo para distribuir: $DMG_PATH"
else
  echo "==> Firma ad-hoc (solo para uso local en esta Mac)"
  codesign --force --deep --sign - "$APP"
  codesign --verify --deep "$APP" && echo "codesign OK"
  if [ "${DMG:-0}" = "1" ]; then
    echo "==> Creando DMG (sin notarizar: mostrara aviso de Gatekeeper en otras Macs)"
    hdiutil create -volname Tonatiuh -srcfolder "$APP" -ov -format UDZO "$DMG_PATH"
  fi
  echo ""
  echo "Listo: $APP"
fi
