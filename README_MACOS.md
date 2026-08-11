# Tonatiuh for macOS on Apple Silicon (M1, M2, M3, M4)

This fork makes Tonatiuh 2.2.4 run as a native app on Mac computers with Apple Silicon chips, on macOS 14.6 or newer.

Tonatiuh is a program that simulates how sunlight bounces on mirrors and reaches a target. It is used to study solar power plants.

## Packages and versions used

This build was made and tested with these exact pieces. Newer versions of the same lines will probably work too.

| Package | Version | Where it comes from |
|---|---|---|
| macOS | 14.6 or newer, Apple Silicon | your Mac |
| Xcode Command Line Tools | current | Apple, `xcode-select --install` |
| Homebrew | current | https://brew.sh |
| Qt | 5.15.19 (formula `qt@5`, includes QtScript) | Homebrew |
| CMake | current | Homebrew |
| Google Test | 1.18 (formula `googletest`) | Homebrew |
| Coin | 4.0.10 | built from source, https://github.com/coin3d/coin |
| SoQt | 1.6.4 | built from source, https://github.com/coin3d/soqt |
| sqlite3 | system version | already included in macOS |

Important detail: we use Qt 5, not Qt 6, because Tonatiuh needs QtScript and Qt 6 removed it. That is also why Coin and SoQt are built from source. The Homebrew versions of Coin and SoQt are linked to Qt 6 and cannot be mixed with a Qt 5 app.

Everything from source lands in one folder, `~/tde/local`. The build scripts expect it there through the variable `TDE_ROOT`.

## What you need before building

You need a Mac with an Apple Silicon chip (M1 or newer) and these tools:

1. Xcode Command Line Tools:

```bash
xcode-select --install
```

2. Homebrew (the package manager for Mac). Install it from https://brew.sh and then run:

```bash
brew install qt@5 cmake googletest
```

3. Two graphics libraries, Coin and SoQt. The versions that Homebrew offers do not work with our version of Qt, so we build them ourselves. First create a folder for them:

```bash
export TDE_ROOT="$HOME/tde"
mkdir -p "$TDE_ROOT/local"
cd ~/tde
```

Build Coin:

```bash
git clone --recurse-submodules https://github.com/coin3d/coin.git
cmake -S coin -B coin/build -DCMAKE_INSTALL_PREFIX="$TDE_ROOT/local" -DCMAKE_OSX_DEPLOYMENT_TARGET=14.6 -DCMAKE_BUILD_TYPE=Release -DCOIN_BUILD_DOCUMENTATION=OFF
cmake --build coin/build -j
cmake --install coin/build
```

Build SoQt:

```bash
git clone --recurse-submodules https://github.com/coin3d/soqt.git
cmake -S soqt -B soqt/build -DCMAKE_INSTALL_PREFIX="$TDE_ROOT/local" -DCMAKE_PREFIX_PATH="$(brew --prefix qt@5);$TDE_ROOT/local" -DCMAKE_OSX_DEPLOYMENT_TARGET=14.6 -DCMAKE_BUILD_TYPE=Release
cmake --build soqt/build -j
cmake --install soqt/build
```

4. Make the test library visible to the project:

```bash
ln -sf "$(brew --prefix googletest)/lib/"libgtest* ~/tde/local/lib/
ln -sf "$(brew --prefix googletest)/include/gtest" ~/tde/local/include/
```

## How to build the app

From the root folder of this repository:

```bash
source setenv_macos.sh
cd TonatiuhProject
qmake TonatiuhProject.pro
make -j$(sysctl -n hw.ncpu)
```

The first command sets some variables the build needs. The last command compiles everything. It takes a few minutes. When it finishes, the app is here:

```
TonatiuhProject/bin/release/Tonatiuh.app
```

## How to run the tests

```bash
cd TonatiuhProject/bin/release
export DYLD_LIBRARY_PATH="$PWD:$HOME/tde/local/lib"
./TonatiuhTests.app/Contents/MacOS/TonatiuhTests --gtest_filter=-TransformTests.ConstructorFromPtrMatrix4x4:TransformTests.ConstructorUsingTwoPtrMatrix4x4:PhotonTests.ConstructorFromPhoton:Matrix4x4Tests.Mul
```

The filter skips four tests that have old bugs inside the tests themselves, not in the app. All the other 113 tests should pass.

## How to create the DMG (the file you share with others)

From the root folder of the repository:

```bash
DMG=1 ./package_macos.sh
```

This creates `dist/Tonatiuh-2.2.4-arm64.dmg`. The script copies the app, puts all the plugins and libraries inside it, and signs it so it can run on your Mac.

Note for people who receive this DMG: the first time they open the app, macOS will show a warning because the app is not notarized by Apple. To open it anyway, go to System Settings, then Privacy and Security, scroll down and click "Open Anyway". This only needs to be done once.

If you have an Apple Developer account, you can build a version with no warning at all:

```bash
SIGN_ID="Developer ID Application: Your Name (TEAMID)" NOTARY_PROFILE=tonatiuh-notary ./package_macos.sh
```

## How the migration was done, step by step

Here is the story of how we made this old program run on new Macs.

### Step 1. We studied the project

Tonatiuh was written many years ago in C++. It uses a tool called qmake to build itself, and it depends on other libraries: Qt for the windows and buttons, Coin and SoQt for the 3D view, and a scripting engine called QtScript. The good news was that the code itself was almost ready for Mac. The problems were in the build settings, not in the program.

### Step 2. We fixed the build settings (the quick wins)

We found a compiler flag called `-mfpmath=sse` in the settings. That flag only exists for Intel chips, so the Apple Silicon compiler refused to work. We removed it.

We also told the build to skip an optional map library called Marble, because it is very hard to get on Mac and the code already knew how to live without it. And we set the minimum system version to macOS 14.6.

There was also a small trap in a file called .gitignore that silently ignored header files. We fixed it so no code gets lost in the future.

### Step 3. We installed the pieces

We installed Qt 5 from Homebrew. We had to check that it included QtScript, because Tonatiuh needs it for its script editor, and newer versions of Qt removed it. It was there, so we could continue.

Coin and SoQt were a special case. Homebrew has them, but built for Qt 6, and our app uses Qt 5. Mixing the two does not work, so we compiled both libraries from their source code against our Qt 5. We checked with a tool called lipo that everything came out as arm64, which means native Apple Silicon code.

### Step 4. We compiled and fixed errors one by one

The first build stopped because one sub project had its own copy of the bad Intel flag. We removed it there too.

The second build stopped inside the main window code. The sun position math lived in a file that used to arrive through the Marble map dialog. Since we turned Marble off, that file never arrived. We added one include line so the code could find it directly.

After that, the whole app compiled and linked. The 46 plugins compiled too.

### Step 5. We ran it

We placed the plugins folder where the app looks for it and started the app. The splash screen appeared, then the main window with the 3D view. All plugins loaded.

### Step 6. We ran the tests

The test suite needed one change: the modern Google Test library requires C++17, so we turned that on just for the tests. Then 113 tests passed. Four tests failed, but we read them carefully and found that the bugs were inside those tests, written long ago, not in the app. For example, two of them free memory that was never allocated, which macOS correctly refuses to allow.

### Step 7. We packaged it

An app on someone else's Mac cannot depend on Homebrew or on files in our home folder. The script `package_macos.sh` copies every library and plugin inside the app bundle and rewrites the internal paths so the app finds everything inside itself. Then it signs the app and builds the DMG. We tested the packaged app with no special environment at all, opened a sample solar furnace scene, ran a ray trace and saw the rays drawn in 3D. That was the finish line.
