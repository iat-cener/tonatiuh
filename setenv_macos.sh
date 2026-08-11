#!/bin/bash
# Build environment for Tonatiuh on macOS (Apple Silicon)
# Usage:  source setenv_macos.sh   (works in zsh and bash, from any directory)

# Path of this script, compatible with zsh and bash
if [ -n "$ZSH_VERSION" ]; then
  _SETENV_DIR="$(cd "$(dirname "${(%):-%N}")" && pwd)"
elif [ -n "$BASH_SOURCE" ]; then
  _SETENV_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
else
  _SETENV_DIR="$HOME/Documents/GitHub/tonatiuh-macos"
fi

export TDE_ROOT="$HOME/tde"
export TONATIUH_ROOT="$_SETENV_DIR/TonatiuhProject"
export PATH="$(brew --prefix qt@5)/bin:$PATH"
unset _SETENV_DIR

echo "TDE_ROOT      = $TDE_ROOT"
echo "TONATIUH_ROOT = $TONATIUH_ROOT"
echo "qmake         = $(which qmake)"
