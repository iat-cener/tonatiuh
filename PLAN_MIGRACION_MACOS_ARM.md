# Plan de migración de Tonatiuh a macOS Apple Silicon (M1–M4)

**Objetivo:** compilar y empaquetar Tonatiuh 2.2.4 como aplicación nativa arm64 para macOS 14.6 (Sonoma) o superior, sin Rosetta.

**Fecha del análisis:** 11 de agosto de 2026 · Repo analizado: `tonatiuh-macos` (rama `master`, v2.2.4)

---

## 1. Resultado del análisis del proyecto

### 1.1 Qué es y cómo está estructurado

Tonatiuh es un trazador de rayos Monte Carlo para sistemas de concentración solar, escrito en C++ (~224 .cpp / ~226 .h) con build system **qmake** (proyecto `subdirs`). La estructura de compilación tiene 5 subproyectos con este orden de dependencia:

```
TonatiuhProject.pro
├── geometry/   → libgeometry   (lib estática/din., sin Qt GUI: BBox, Matrix4x4, Transform, etc.)
├── fields/     → libfields     (campos custom de Open Inventor; depende de geometry, Coin, SoQt)
├── src/        → Tonatiuh      (la app: GUI Qt Widgets + raytracing + scripting)
├── plugins/    → 46 plugins    (Shapes, Materials, Trackers, Sunshapes, Transmissivity, PhotonMapExport, Random)
└── tests/      → tests GTest   (enlaza objetos .o ya compilados de release/debug)
```

El build depende de dos variables de entorno: `TONATIUH_ROOT` (raíz de `TonatiuhProject`) y `TDE_ROOT` (prefijo donde viven las dependencias externas en `$TDE_ROOT/local`).

La app carga los plugins en runtime desde `applicationDirPath()/plugins` (`main.cpp` → `PluginManager`), lo cual funciona sin cambios dentro de un bundle `.app` (el directorio sería `Tonatiuh.app/Contents/MacOS/plugins`).

### 1.2 Dependencias detectadas en el código

| Dependencia | Uso real en el código | Estado para arm64 |
|---|---|---|
| **Qt 5** (widgets, script, concurrent, printsupport) | Toda la GUI; `QT += script` en `src.pro` y `tests.pro` | `qt@5` 5.15.19 en Homebrew con binarios arm64 (sonoma/sequoia/tahoe). Incluye QtScript (verificar tras instalar) |
| **QtScript** | `ScriptEditorDialog`, `tonatiuh_script`, `ScriptRayTracer`, `main.cpp` | Existe en Qt 5.15 (deprecado). **Eliminado en Qt 6** → es el motivo principal para quedarse en Qt 5 en la fase 1 |
| **Coin3D** (Open Inventor) | 133 archivos incluyen `Inventor/...`; núcleo de la escena 3D | Coin 4.0.x compila nativo arm64 con CMake |
| **SoQt** | `GraphicView`, `CelestialWidget`, `HorizontalWidget`, `main.cpp` | SoQt 1.6.x compila arm64 con CMake. ⚠️ El paquete `coin3d` de Homebrew trae Coin 4.0.10 + SoQt 1.6.4 pero **ligados a Qt 6** → no sirve para la fase Qt 5; hay que compilarlos desde código fuente contra `qt@5` |
| **Marble** (mapa en SunPositionCalculatorDialog) | **Opcional**: todo el uso está protegido con `#ifndef NO_MARBLE` (2 archivos + una llamada en `MainWindow.cpp`). Ningún `.ui` lo referencia | No existe en Homebrew. `config.pri` actual ni siquiera lo enlaza → compilar con `NO_MARBLE` en fase 1 |
| **sqlite3** | Solo el plugin `PhotonMapExportDB` | macOS trae `libsqlite3` de sistema; trivial |
| **GTest** | `tests/` (21 archivos) — `LIBS += -L$$(TDE_ROOT)/local/lib -lgtest` | `brew install googletest` |
| **qcustomplot 1.3.1** | Incrustado en `src/source/gui/` (vendored) | OK con Qt 5; incompatible con Qt 6 (necesitaría 2.x) |

### 1.3 Qué ya está a favor

El proyecto ya contempla macOS parcialmente: hay bloques `macx` en `config.pri`, `ICON = icons/Tonatiuh.icns` en `src.pro` (el `.icns` existe), `Timer.h` ya es portable (usa `sys/time.h` fuera de Windows), no hay intrínsecos x86 (`_mm_*`, `xmmintrin`, etc.) en ningún fuente, y no usa QGLWidget/QtOpenGL directamente (el render lo hace SoQt). El único código específico de Windows es el de `Timer.h`, ya condicionado.

### 1.4 Problemas concretos encontrados (bloquean o ensucian el build arm64)

1. **`-mfpmath=sse` en el bloque `macx` de `config.pri`** (release). Es una bandera exclusiva de x86; clang para arm64 la rechaza. Bloquea el build release nativo. Basta dejar `-O3`.
2. **`NO_MARBLE` no está definido en ningún `.pro`/`.pri`**, pero `config.pri` (rama unix/mac) tampoco enlaza `-lmarblewidget`. Tal como está, el build unix falla en link por los símbolos de Marble. Hay que añadir `DEFINES += NO_MARBLE` al build de mac (el mecanismo ya existe en el código; no es código nuevo).
3. **`.gitignore` termina con `*.h`**: cualquier header nuevo que se cree durante la migración será ignorado por git silenciosamente. (Los 226 headers actuales ya están trackeados, pero es una trampa.)
4. **Typos preexistentes en `config.pri`**: `QMAKE_CLAGS` / `QMAKE_CXXLAGS` (faltan la F) en el bloque de plugins — el `-fPIC` nunca se está aplicando por esa vía (en mac no es grave, pero conviene corregirlo).
5. **`tests.pro` enlaza `.o` a mano** desde `$$(TONATIUH_ROOT)/release|debug` — frágil pero funcional: obliga a compilar `src` antes que `tests` y en la misma configuración.
6. **`config_win.pri` está desactualizado** (versión 1.2.0, enlaza Marble). No afecta a mac; ignorarlo.
7. Compiladores modernos (clang de Xcode 15/16) usan C++17 por defecto; código de 2008–2017 puede emitir warnings o errores puntuales (p. ej. `register`). Si aparece, fijar `CONFIG += c++11` en `config.pri` es el ajuste mínimo.

### 1.5 Decisión estratégica: Qt 5 ahora, Qt 6 después (opcional)

- **Ruta A — Qt 5.15 (recomendada para esta migración):** cambios mínimos, casi todo es configuración de build. QtScript, qcustomplot 1.3.1, `QRegExp` (usado en ≥5 archivos), `QApplication::setColorSpec` — todo sigue funcionando en 5.15. Riesgo bajo, tiempo corto.
- **Ruta B — Qt 6 (fase futura):** Homebrew ya marca `qt@5` como deprecado (fecha de desactivación: 2027-05-19), así que a mediano plazo conviene migrar. Requiere: portar QtScript→`QJSEngine` (3–4 clases), actualizar qcustomplot a 2.x, `QRegExp`→`QRegularExpression`, eliminar `setColorSpec`, revisar `QTime` usado como cronómetro→`QElapsedTimer`. Con Qt 6 se podría usar directamente `brew install coin3d` (ya trae Coin+SoQt para Qt 6). Como referencia existe la rama `upstream/TonatiuhProject3.0`.

Este plan desarrolla la Ruta A y deja la Ruta B como fase final opcional.

---

## 2. Requerimientos

- Mac con Apple Silicon (M1–M4), macOS 14.6+.
- **Xcode Command Line Tools**: `xcode-select --install`.
- **Homebrew** nativo arm64 (en `/opt/homebrew`).
- ~2 GB de disco para dependencias + builds.

### Dependencias a instalar

```bash
brew install qt@5 cmake googletest
# sqlite3: se usa el del sistema (o brew install sqlite si se prefiere)
```

Verificación importante tras instalar Qt:

```bash
ls "$(brew --prefix qt@5)/lib" | grep -i script   # debe aparecer QtScript
```

### Dependencias a compilar desde código fuente (contra qt@5)

- **Coin 4.0.x** — https://github.com/coin3d/coin (CMake)
- **SoQt 1.6.x** — https://github.com/coin3d/soqt (CMake, necesita apuntar a qt@5)

Ambas se instalan en `$TDE_ROOT/local` para respetar la convención que ya usa `config.pri` (`COINDIR = $$(TDE_ROOT)/local`), sin tocar los `.pro`.

---

## 3. Plan paso a paso

### Fase 0 — Quick wins (minutos, desbloquean todo lo demás)

1. **Arreglar `.gitignore`**: quitar la línea final `*.h` (y revisar `*.rc`). Evita perder headers nuevos.
2. **`config.pri`, bloque `macx` release**: eliminar `-mfpmath=sse` (dejar `-O3`). Es la única bandera incompatible con arm64 en todo el proyecto.
3. **`config.pri`**: corregir `QMAKE_CLAGS`/`QMAKE_CXXLAGS` → `QMAKE_CFLAGS`/`QMAKE_CXXFLAGS`.
4. **Definir `NO_MARBLE` para mac**: en `config.pri` añadir dentro de un bloque `macx { DEFINES += NO_MARBLE }` (o global unix). El código ya está preparado para ello.
5. **Fijar target de despliegue**: en `config.pri`, `macx { QMAKE_MACOSX_DEPLOYMENT_TARGET = 14.6 }`.
6. **Crear un script de entorno** (p. ej. `setenv_macos.sh`) que exporte:
   ```bash
   export TONATIUH_ROOT="$PWD/TonatiuhProject"
   export TDE_ROOT="$HOME/tde"           # o la ruta que se elija
   export PATH="$(brew --prefix qt@5)/bin:$PATH"
   ```

### Fase 1 — Compilar dependencias nativas arm64 (½–1 día)

7. **Coin**:
   ```bash
   cmake -S coin -B coin/build \
     -DCMAKE_INSTALL_PREFIX="$TDE_ROOT/local" \
     -DCMAKE_OSX_DEPLOYMENT_TARGET=14.6 \
     -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_DOCUMENTATION=OFF
   cmake --build coin/build -j && cmake --install coin/build
   ```
8. **SoQt** (contra qt@5):
   ```bash
   cmake -S soqt -B soqt/build \
     -DCMAKE_INSTALL_PREFIX="$TDE_ROOT/local" \
     -DCMAKE_PREFIX_PATH="$(brew --prefix qt@5);$TDE_ROOT/local" \
     -DCMAKE_OSX_DEPLOYMENT_TARGET=14.6 -DCMAKE_BUILD_TYPE=Release
   cmake --build soqt/build -j && cmake --install soqt/build
   ```
9. **GTest visible para `tests.pro`**: o se enlaza `$TDE_ROOT/local/lib` a las libs de `brew --prefix googletest`, o se compila gtest con prefijo `$TDE_ROOT/local`. (El `.pro` busca `-lgtest` en `$TDE_ROOT/local/lib`.)
10. Verificar arquitectura de todo lo instalado: `lipo -info $TDE_ROOT/local/lib/libCoin*.dylib libSoQt*.dylib` → debe decir `arm64`.

### Fase 2 — Primer build de Tonatiuh (1–3 días, aquí aparecen los errores reales)

11. Compilar en el orden de dependencias, empezando por **release**:
    ```bash
    source setenv_macos.sh
    cd TonatiuhProject
    qmake TonatiuhProject.pro && make -j    # subdirs respeta geometry→fields→src→plugins→tests
    ```
    Si conviene iterar, compilar subproyecto por subproyecto: `geometry` → `fields` → `src` → `plugins`.
12. **Errores esperables y su tratamiento** (ajustes, no código nuevo):
    - Warnings/errores de C++ moderno → fijar `CONFIG += c++11` en `config.pri` si hace falta.
    - `PhotonMapExportDB`: `-lsqlite3` resuelve contra el SDK de macOS; si no, `brew install sqlite` y ajustar la ruta ya parametrizada por `TDE_ROOT`.
    - Enlace de plugins: los plugins re-compilan fuentes de `src/` (patrón ya existente); no requieren cambio.
13. **Humo inicial**: lanzar `bin/release/Tonatiuh` desde terminal, comprobar splash, carga de los 46 plugins (menús de Shapes/Materials/Trackers poblados), vista 3D SoQt, abrir `tests/SolarFurnace_normal.tnh`.

### Fase 3 — Validación funcional y numérica (1–2 días)

14. Correr la suite GTest (`tests/`) en release.
15. Ejecutar el trazado del escenario de referencia (`tests/RayTracerTest.tnhs`, `SolarFurnace_normal.tnh`) y comparar potencia/​distribución de fotones contra un build de referencia (Windows/Intel o resultados publicados). Nota: al pasar de x86-SSE a ARM-NEON puede haber diferencias de redondeo en el último bit; lo relevante es la convergencia estadística, no la igualdad bit a bit.
16. Probar de punta a punta: crear escena, undo/redo, ray trace con `PhotonMapExportFile` y `PhotonMapExportDB`, editor de scripts (QtScript), export/import `.tnh`.
17. Verificar el diálogo de posición solar: con `NO_MARBLE` queda deshabilitado (así lo decide el código actual). Documentarlo como limitación conocida de la v1 para mac; el cálculo de sol por azimut/zenit directo en la luz sigue disponible.

### Fase 4 — Empaquetado .app distribuible (1–2 días)

18. Estructura del bundle: `src.pro` ya genera `Tonatiuh.app` con su `.icns`. Copiar `bin/release/plugins/` a `Tonatiuh.app/Contents/MacOS/plugins` (la app ya busca ahí; sin cambios de código).
19. `macdeployqt` (el de qt@5) para incrustar frameworks Qt; añadir `libCoin` y `libSoQt` al bundle y corregir rutas con `install_name_tool` (o dejar que macdeployqt las arrastre al estar enlazadas al ejecutable). Revisar también las dylib de cada plugin.
20. Firmar: mínimo `codesign --force --deep -s -` (ad-hoc) para uso local; para distribuir fuera del equipo, firma con Developer ID + notarización (`notarytool`), si no Gatekeeper bloqueará la app en macOS 14+.
21. Prueba limpia en otra Mac (o cuenta de usuario nueva) sin Homebrew, para confirmar que el bundle es autocontenido.

### Fase 5 — (Opcional, lo más complejo) Modernización

22. **Marble**: si el mapa del diálogo solar se considera imprescindible, compilar Marble (KDE) desde código fuente contra qt@5 e integrarlo (`config.pri` de unix hoy no lo enlaza; habría que añadirlo simétrico a `config_win.pri`). Esfuerzo alto para una función secundaria — por eso queda al final.
23. **Migración a Qt 6** (elimina la dependencia del deprecado `qt@5` de Homebrew y permite usar `brew install coin3d` directamente): QtScript→QJSEngine, qcustomplot 1.3.1→2.x, `QRegExp`→`QRegularExpression` (NodeNameDelegate, ContainerEditorMFVec2/3, ShapeBezierSurfaceFactory, ComponentHeliostatField), quitar `setColorSpec`, revisar deprecaciones menores. Referencia útil: rama `upstream/TonatiuhProject3.0`.
24. **CI**: workflow de GitHub Actions con runner `macos-14`/`macos-15` (ya son arm64) que reproduzca las fases 1–2 y publique el `.app`.

---

## 4. Riesgos y puntos de decisión

| Riesgo | Prob. | Mitigación |
|---|---|---|
| `qt@5` de Homebrew sin QtScript | Baja | Verificar (paso de la sección 2); si faltara, compilar el módulo qtscript 5.15 desde fuente, o adelantar la migración a QJSEngine |
| Errores de compilación por clang/C++17 en código antiguo | Media | `CONFIG += c++11`; corregir puntualmente |
| Diferencias numéricas x86 vs ARM en validación | Media | Comparación estadística, no bit a bit (paso 15) |
| Gatekeeper bloquea la app sin notarizar | Alta (si se distribuye) | Paso 20 |
| `qt@5` desaparece de Homebrew (2027) | Cierta a futuro | Fase 5 / Qt 6 |

## 5. Resumen ejecutivo

La base del código está sorprendentemente bien preparada: sin intrínsecos x86, con `Timer` portable, icono de mac ya presente, Marble ya opcional y carga de plugins compatible con bundles. Los bloqueadores reales son de *build*, no de código: una bandera de compilador x86 (`-mfpmath=sse`), la falta de `DEFINES += NO_MARBLE`, y que Coin3D/SoQt deben compilarse contra Qt 5 porque el paquete de Homebrew viene para Qt 6. La ruta corta (Qt 5.15 arm64) llega a una app nativa funcional en aproximadamente una semana de trabajo efectivo; la modernización a Qt 6 queda planificada como fase opcional posterior.
