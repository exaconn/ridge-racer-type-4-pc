#!/usr/bin/env bash
# Compile one C file: cpp -> gcc 2.8.1 (PSX) cc1 -> maspsx -> GNU as -> .o
# Usage: cc.sh <input.c> <output.o> [extra cpp flags...]
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
TC="$ROOT/tools/toolchain"
IN="$1"; OUT="$2"; shift 2

CPP="${CPP:-$TC/bin/cpp}"
CC1="${CC1:-$TC/gcc-2.8.1/cc1}"
# Per-file compiler override: PsyQ library code was built with gcc 2.7.2 (marker: /* GCC_VERSION: 2.7.2 */)
if [ -z "${CC1_FORCE:-}" ] && grep -q 'GCC_VERSION:[[:space:]]*2\.7\.2' "$IN"; then
    CC1="$TC/gcc-2.7.2/cc1"
fi
AS="${AS:-$TC/bin/mipsel-none-elf-as}"
PYTHON="${PYTHON:-$ROOT/.venv/bin/python}"

CPP_FLAGS="-undef -D__GNUC__=2 -D__OPTIMIZE__ -Dmips -D__mips__ -D__LITTLE_ENDIAN__ -I$ROOT/include $*"
CC1_FLAGS="-w -O2 -G0 -funsigned-char -mips1 -mcpu=3000"
# Generic per-file cc1 flags: first-line style comment `/* CC1_FLAGS: -mno-split-addresses */` (any stock cc1 options, appended to CC1_FLAGS)
EXTRA=$(sed -n 's|^/\* CC1_FLAGS:[[:space:]]*\(.*[^[:space:]]\)[[:space:]]*\*/.*$|\1|p' "$IN" | head -1)
if [ -n "$EXTRA" ]; then CC1_FLAGS="$CC1_FLAGS $EXTRA"; fi
# DEPRECATED alias (to be removed once old markers are converted): /* NOSPLIT */ == /* CC1_FLAGS: -mno-split-addresses */
if [ "$CC1" = "$TC/gcc-2.8.1/cc1" ] && grep -q '^/\* NOSPLIT \*/' "$IN"; then CC1_FLAGS="$CC1_FLAGS -mno-split-addresses"; fi

mkdir -p "$(dirname "$OUT")"
TMP_I=$(mktemp -t rr4.XXXXXX.i); TMP_S=$(mktemp -t rr4.XXXXXX.s)
trap 'rm -f "$TMP_I" "$TMP_S"' EXIT

"$CPP" $CPP_FLAGS "$IN" -o "$TMP_I"

LOG=$(mktemp -t rr4log.XXXXXX)
trap 'rm -f "$TMP_I" "$TMP_S" "$LOG"' EXIT
"$CC1" $CC1_FLAGS "$TMP_I" -o "$TMP_S" >"$LOG" 2>&1 || { cat "$LOG" >&2; exit 1; }
grep -v "^time in\|^ *[A-Za-z_0-9]*$\|^ *$" "$LOG" >&2 || true
# Per-file stock MASPSX options (markers, same idea as parasite-pc): `/* ASPSX_VERSION: 2.21 */` selects --aspsx-version, `/* MASPSX_FLAGS: --dont-expand-li */` forwards extra stock maspsx flags
ASPSX_VER=$(sed -n 's/^\/\* ASPSX_VERSION:[[:space:]]*\([0-9][0-9.]*\).*/\1/p' "$IN" | head -1)
ASPSX_VER="${RR_ASPSX_VERSION:-${ASPSX_VER:-2.79}}"
MASPSX_EXTRA=$(sed -n 's|^/\* MASPSX_FLAGS:[[:space:]]*\(.*[^[:space:]]\)[[:space:]]*\*/.*$|\1|p' "$IN" | head -1)
MASPSX_EXTRA="${RR_MASPSX_FLAGS:-$MASPSX_EXTRA}"
"$PYTHON" "$ROOT/tools/maspsx/maspsx.py" --aspsx-version="$ASPSX_VER" --expand-div $MASPSX_EXTRA \
    --gnu-as-path="$AS" --run-assembler \
    -EL -G0 -march=r3000 -mtune=r3000 -no-pad-sections -I "$ROOT/include" \
    -o "$OUT" < "$TMP_S"
