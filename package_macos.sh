#!/bin/bash
# Packages Tonatiuh.app as a self-contained bundle for macOS arm64 (14.6+)
#
# Local use (ad-hoc signature, this Mac only):
#   ./package_macos.sh
#
# For DISTRIBUTION (Developer ID signature + notarization + stapled DMG):
#   SIGN_ID="Developer ID Application: Your Name (TEAMID)" \
#   NOTARY_PROFILE=tonatiuh-notary \
#   ./package_macos.sh
#
# Distribution requirements: a "Developer ID Application" certificate installed and
# credentials stored with `xcrun notarytool store-credentials tonatiuh-notary ...`
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
REL="$ROOT/TonatiuhProject/bin/release"
DIST="$ROOT/dist"
QT5="$(brew --prefix qt@5)"
TDE="${TDE_ROOT:-$HOME/tde}"
VERSION="2.2.4"
DMG_PATH="$DIST/Tonatiuh-$VERSION-arm64.dmg"

[ -x "$REL/Tonatiuh.app/Contents/MacOS/Tonatiuh" ] || { echo "ERROR: release build not found. Build the project first."; exit 1; }

echo "==> Copying bundle to dist/"
rm -rf "$DIST"; mkdir -p "$DIST"
cp -R "$REL/Tonatiuh.app" "$DIST/"
APP="$DIST/Tonatiuh.app"
MACOS="$APP/Contents/MacOS"
FW="$APP/Contents/Frameworks"
mkdir -p "$FW"

echo "==> Copying plugins into the bundle"
rm -rf "$MACOS/plugins"            # removes the testing symlink if it came with the copy
cp -R "$REL/plugins" "$MACOS/plugins"

echo "==> Embedding project libraries and Coin/SoQt"
cp -f "$REL/libgeometry.1.dylib" "$REL/libfields.1.dylib" "$FW/"
cp -f "$TDE/local/lib/libCoin.80.dylib" "$TDE/local/lib/libSoQt.20.dylib" "$FW/"

# Rewrites references to these 4 libraries so they point to @rpath/ inside the bundle
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
  fix_refs "$FW/$lib"              # cross references: SoQt->Coin, fields->geometry/Coin/SoQt
done

fix_refs "$MACOS/Tonatiuh"

echo "==> Fixing references in the plugins"
PLUGIN_ARGS=()
while IFS= read -r -d '' dylib; do
  fix_refs "$dylib"
  PLUGIN_ARGS+=( "-executable=$dylib" )
done < <(find "$MACOS/plugins" -name "*.dylib" -print0)

echo "==> macdeployqt (embeds Qt frameworks and fixes paths)"
"$QT5/bin/macdeployqt" "$APP" "${PLUGIN_ARGS[@]}"

if [ -n "${SIGN_ID:-}" ]; then
  echo "==> Signing with Developer ID (hardened runtime + timestamp)"
  # 1) standalone dylibs (project libs, Coin/SoQt, the 46 plugins, Qt plugins)
  find "$APP" -type f -name "*.dylib" -print0 | while IFS= read -r -d '' f; do
    codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$f"
  done
  # 2) Qt frameworks
  for fwdir in "$FW"/*.framework; do
    [ -d "$fwdir" ] && codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$fwdir"
  done
  # 3) the whole bundle (executable + resource sealing)
  codesign --force --timestamp --options runtime --sign "$SIGN_ID" "$APP"

  echo "==> Verifying signature"
  codesign --verify --deep --strict "$APP" && echo "codesign OK"

  if [ -n "${NOTARY_PROFILE:-}" ]; then
    echo "==> Notarizing the app (may take a few minutes)"
    ditto -c -k --keepParent "$APP" "$DIST/Tonatiuh-app.zip"
    xcrun notarytool submit "$DIST/Tonatiuh-app.zip" --keychain-profile "$NOTARY_PROFILE" --wait
    rm -f "$DIST/Tonatiuh-app.zip"
    echo "==> Stapling ticket to the app"
    xcrun stapler staple "$APP"
  fi

  echo "==> Creating DMG"
  hdiutil create -volname Tonatiuh -srcfolder "$APP" -ov -format UDZO "$DMG_PATH"
  codesign --force --timestamp --sign "$SIGN_ID" "$DMG_PATH"

  if [ -n "${NOTARY_PROFILE:-}" ]; then
    echo "==> Notarizing the DMG"
    xcrun notarytool submit "$DMG_PATH" --keychain-profile "$NOTARY_PROFILE" --wait
    echo "==> Stapling ticket to the DMG"
    xcrun stapler staple "$DMG_PATH"
    echo "==> Final Gatekeeper assessment"
    spctl -a -t open --context context:primary-signature -vv "$DMG_PATH" || true
    spctl -a -t exec -vv "$APP" || true
  fi

  echo ""
  echo "Ready to distribute: $DMG_PATH"
else
  echo "==> Ad-hoc signature (local use on this Mac only)"
  codesign --force --deep --sign - "$APP"
  codesign --verify --deep "$APP" && echo "codesign OK"
  if [ "${DMG:-0}" = "1" ]; then
    echo "==> Creating DMG (not notarized: Gatekeeper will warn on other Macs)"
    hdiutil create -volname Tonatiuh -srcfolder "$APP" -ov -format UDZO "$DMG_PATH"
  fi
  echo ""
  echo "Done: $APP"
fi
