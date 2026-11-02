#!/usr/bin/env bash
# usage: perm.sh func_XXXXXXXX [seconds=300] [source.c]  - random-permutation search for near-miss functions.
# Prints the best score; a zero-score result is in build/perm/<func>/output-0-*/source.c (then run fd.py on it).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
export PATH="$ROOT/tools/toolchain/bin:$PATH"
FN="$1"; SECS="${2:-300}"; SRC="${3:-}"
"$ROOT/.venv/bin/python" "$ROOT/tools/scripts/perm_setup.py" "$FN" ${SRC:+"$SRC"} >/dev/null
timeout "$SECS" "$ROOT/.venv/bin/python" "$ROOT/tools/decomp-permuter/permuter.py" "$ROOT/build/perm/$FN" -j"${PERM_JOBS:-4}" --best-only --stop-on-zero >"$ROOT/build/perm/$FN/log.txt" 2>&1 || true
ls -d "$ROOT"/build/perm/"$FN"/output-* 2>/dev/null | sed 's#.*/output-##' | sort -n | head -3 | sed "s#^#$FN best: output-#" || true
tr '\r' '\n' < "$ROOT/build/perm/$FN/log.txt" | grep -o "score = [0-9]*" | sort -t= -k2 -n | head -1
