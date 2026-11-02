#!/usr/bin/env bash
# usage: m2c.sh func_XXXXXXXX  -> first-draft C for one function (types from include/types.h + m2c_macros.h)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
exec "$ROOT/.venv/bin/python" "$ROOT/tools/m2c/m2c.py" --target mipsel-gcc-c --valid-syntax --no-andor \
    "$ROOT/build/baseline/asm/nonmatchings/$1/$1.s"
