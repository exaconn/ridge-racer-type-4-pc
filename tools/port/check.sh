#!/bin/sh
# Compile game sources with the host flags of the root CMake build.
#   tools/port/check.sh                 every file under src/main and src/overlays
#   tools/port/check.sh src/main/gfx    one directory
#   tools/port/check.sh a.c b.c         given files
# Prints each failing file with its errors and a summary line.
cd "$(dirname "$0")/../.." || exit 1
CFLAGS="-std=c11 -c -o /dev/null -D__psyz -Iinclude -isystem third_party/psyz/psyz/include
 -fno-strict-aliasing -fwrapv -Wno-everything -Wint-conversion -Wpointer-to-int-cast
 -Wint-to-pointer-cast -Wimplicit-function-declaration -Wincompatible-pointer-types
 -Werror -ferror-limit=0"
[ $# -eq 0 ] && set -- src/main src/overlays
LIST=$(for a in "$@"; do if [ -d "$a" ]; then find "$a" -name '*.c'; else echo "$a"; fi; done | sort)
TMP=$(mktemp -d)
for f in $LIST; do
  ( cc $CFLAGS "$f" > "$TMP/$(echo "$f" | tr / _).log" 2>&1 || echo "$f" >> "$TMP/failed.$$.$(echo "$f" | tr / _)" ) &
  while [ "$(jobs -r | wc -l)" -ge 8 ]; do sleep 0.05; done
done
wait
total=$(echo "$LIST" | wc -l | tr -d ' ')
failed=0
for f in $LIST; do
  key=$(echo "$f" | tr / _)
  if ls "$TMP"/failed.*."$key" >/dev/null 2>&1; then
    failed=$((failed + 1))
    echo "=== $f"
    grep -E 'error:' "$TMP/$key.log"
  fi
done
echo "check: $failed failing of $total files"
rm -rf "$TMP"
[ "$failed" -eq 0 ]
