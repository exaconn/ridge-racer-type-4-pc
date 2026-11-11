#!/bin/sh
# Copy the documentation, notices and license texts that every release
# package carries into a package directory:
#   packaging/stage.sh <package-dir> <build-dir> [--no-licenses]
# <package-dir>/  README.md CHANGELOG.md THIRD_PARTY_NOTICES.md [LICENSE.md]
#                 source-commit.txt
#                 licenses/<source path of each packaging/licenses.txt entry>
#                 licenses/libwebp/{COPYING,PATENTS}
# The macOS app carries the license texts in Contents/Resources/licenses
# already (CMake RR4_MACOS_BUNDLE), so its package passes --no-licenses.
set -eu
root=$(cd "$(dirname "$0")/.." && pwd)
dest=$1
build=$2
licenses=1
[ "${3:-}" = "--no-licenses" ] && licenses=0

mkdir -p "$dest"
for doc in README.md CHANGELOG.md THIRD_PARTY_NOTICES.md LICENSE.md; do
    if [ -f "$root/$doc" ]; then
        cp "$root/$doc" "$dest/"
    elif [ "$doc" != LICENSE.md ]; then
        echo "stage.sh: missing $doc" >&2
        exit 1
    fi
done
git -C "$root" rev-parse HEAD > "$dest/source-commit.txt"

[ "$licenses" -eq 1 ] || exit 0
grep -v '^#' "$root/packaging/licenses.txt" | while read -r notice; do
    [ -n "$notice" ] || continue
    mkdir -p "$dest/licenses/$(dirname "$notice")"
    cp "$root/$notice" "$dest/licenses/$notice"
done
webp="$build/_deps/psyz_webp-src"
if [ ! -f "$webp/COPYING" ]; then
    echo "stage.sh: libwebp license not found in $webp" >&2
    exit 1
fi
mkdir -p "$dest/licenses/libwebp"
cp "$webp/COPYING" "$webp/PATENTS" "$dest/licenses/libwebp/"
