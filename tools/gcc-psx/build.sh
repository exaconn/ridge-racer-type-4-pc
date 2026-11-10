#!/bin/bash
# Build the release PSX C compiler on macOS, retaining the ILP32 real layout.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
version=${1:?usage: build.sh 2.7.2\|2.8.1 [output-directory]}
out=${2:-$root/tools/toolchain/gcc-$version}
if [[ "$out" != /* ]]; then
  out="$PWD/$out"
fi
case "$version" in
  2.7.2) archive_sha=7cd8bce5c3aeec59a72ecc2d3d5123864a817b14cdbd0680b1a969c3bccc5da5; gnu=old-gnu ;;
  2.8.1) archive_sha=3b30fbfdf93e628373d90d174243f3267b0eec9ebe792bb64fd15b8828c2ea4c; gnu=gnu ;;
  *) echo "Unsupported release: $version" >&2; exit 1 ;;
esac
if [ "$(uname -s)" != Darwin ]; then
  echo 'This recipe builds the native macOS host port.' >&2
  exit 1
fi
revision=cf70ec58bce58128732062bc9930045c82904cbb
cache=$root/tools/toolchain/gcc-psx-src
mkdir -p "$cache"
deps=$root/tools/toolchain/gcc-970318-src/old-gcc
if ! [ -d "$deps/.git" ] || ! [ "$(git -C "$deps" rev-parse HEAD)" = "$revision" ] ||
   ! git -C "$deps" diff --quiet "$revision" -- patches; then
  deps=$cache/old-gcc
  if [ ! -d "$deps/.git" ]; then
    git clone -q https://github.com/decompals/old-gcc.git "$deps"
    git -C "$deps" checkout -q --detach "$revision"
  fi
  test "$(git -C "$deps" rev-parse HEAD)" = "$revision"
  git -C "$deps" diff --quiet "$revision" -- patches
fi
patches=$deps/patches
archive=$cache/gcc-$version.tar.gz
if [ ! -f "$archive" ]; then
  curl -fsSL "https://ftp.gnu.org/$gnu/gcc/gcc-$version.tar.gz" -o "$archive"
fi
echo "$archive_sha  $archive" | shasum -a 256 -c --quiet
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
tar xzf "$archive" -C "$work"
cd "$work/gcc-$version"
chmod -R u+w .
perl -pi -e 's/include <varargs\.h>/include <stdarg.h>/g' *.c
for file in gcc.c cp/g++.c; do
  [ ! -f "$file" ] || perl -pi -e 's/^extern int sys_nerr;/extern const int sys_nerr;/' "$file"
done
if [ "$version" = 2.7.2 ]; then
  patch -u -p1 obstack.h -i "$patches/obstack-2.7.2.h.patch"
  patch -u -p1 configure -i "$patches/configure.patch"
  patch -u -p1 config/mips/mips.h -i "$patches/mipsel-2.7.patch"
  patch -su -p1 <"$patches/psx-2.5.7.patch"
else
  patch -u -p1 obstack.h -i "$patches/obstack-2.8.1.h.patch"
  patch -u -p1 config/mips/mips.h -i "$patches/mips.patch"
  patch -su -p1 <"$patches/psx.patch"
fi
curl -fsSL https://raw.githubusercontent.com/gcc-mirror/gcc/74af13c174714dd3b9f1ded4b39955f003c16361/config.guess -o config.guess
curl -fsSL https://raw.githubusercontent.com/gcc-mirror/gcc/6fad101f3063d722e3348d07dc93cf737f8709e4/config.sub -o config.sub
chmod +x config.guess config.sub
perl -pi -e 's/\| hiux\* \| abug \| nacl\*/| psx* | hiux* | abug | nacl*/' config.sub
cp "$patches/xm-darwin.h" config/
awk '/^\t\*\)$/ { buf=$0; next }
     buf != "" { if (!done && /echo.*Configuration.*not supported/) {
       print "\t*-apple-darwin*)"; print "\t\txm_file=xm-darwin.h"
       print "\t\tfixincludes=Makefile.in"; print "\t\t;;"; done=1 }
       print buf; buf="" } { print }' configure > configure.tmp
mv configure.tmp configure
chmod +x configure
export PATH="/usr/bin:/bin:/usr/sbin:/sbin:$PATH"
unset CC CXX
host=$(uname -m)-apple-darwin
export CFLAGS='-std=gnu89 -w -Wno-int-conversion -Wno-implicit-function-declaration -Wno-return-mismatch'
./configure --target=mips-sony-psx --host="$host" --build="$host" --prefix=/opt/cross \
  --with-endian-little --with-gnu-as --disable-gprof --disable-gdb --disable-werror
touch insn-config.h
"$root/.venv/bin/python" "$root/tools/scripts/repair_gcc_real_host.py" "$PWD"
cc -std=gnu89 -c "$patches/eprintf-darwin.c" -o eprintf-darwin.o
make -j4 cc1 LANGUAGES=c LDFLAGS="$PWD/eprintf-darwin.o" CFLAGS="$CFLAGS"
"$root/.venv/bin/python" "$root/tools/scripts/target_float_literals.py" --compiler "$PWD/cc1" --version "$version"
mkdir -p "$out"
cp cc1 "$out/cc1"
echo "Installed verified GCC $version at $out/cc1"
