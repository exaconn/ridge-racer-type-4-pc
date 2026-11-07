#!/bin/bash
# Build the PsyQ 4.5+ library compiler -> tools/toolchain/gcc-970318/cc1 (native macOS).
#
# WHAT: the GCC development tree of 1997-03-18 (gcc-mirror commit 2b3600ac), between the
# 2.7.2 and 2.8.0 releases, with the decompals/old-gcc PSX patches. It has GCC 2.8's 4-byte
# reload spill slots and a 1996 loop.c that no 2.7.2.x release carries, which is what Sony's
# SDK library objects show (e.g. libsnd _SsContDataEntry = _SsContDataEntry). Dating and the
# Linux/Docker recipe: bashalarmistdecomp/eternal-silver-sssc-decomp recipe/gcc-970318/.
# This script is that recipe plus old-gcc's gcc-2.7.2-psx-macos.sh host support.
#
# The one edit to the compiler proper: mips_split_addresses stays 0. The snapshot always
# splits symbol addresses into %hi/%lo for GAS and has no -mno-split-addresses yet; Sony's
# objects have the assembler's unsplit `lw $2,sym($3)` macro forms.
#
# Needs: curl, git, cc (Xcode CLT), python3, and bison >= 3.6 on PATH or docker
# (parsers are then generated in ubuntu:jammy, bison 3.8.2).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/../.." && pwd)
src=$root/tools/toolchain/gcc-970318-src
out=$root/tools/toolchain/gcc-970318
snap=2b3600acb4eceda3000312002ec78fecd5a39d30
tar_sha=7cd8bce5c3aeec59a72ecc2d3d5123864a817b14cdbd0680b1a969c3bccc5da5
# sha256 over `sha256sum` of every file of the snapshot's gcc/ directory, C-locale order
tree_sha=00695436c7ca42871205b8f566ddf15bf4fd5890e03570b4b9a0ba0173efa71b
oldgcc=b74211c9d959e9724802f3177c8229cd67202c87     # psx patches as measured
oldgcc_mac=cf70ec5                                  # adds xm-darwin.h, eprintf-darwin.c
sha() { shasum -a 256 "$@"; }

mkdir -p "$src"
[ -f "$src/gcc-2.7.2.tar.gz" ] || curl -fsSL https://ftp.gnu.org/old-gnu/gcc/gcc-2.7.2.tar.gz -o "$src/gcc-2.7.2.tar.gz"
echo "$tar_sha  $src/gcc-2.7.2.tar.gz" | sha -c --quiet
[ -f "$src/gcc-$snap.tar.gz" ] || curl -fsSL "https://github.com/gcc-mirror/gcc/archive/$snap.tar.gz" -o "$src/gcc-$snap.tar.gz"
[ -d "$src/old-gcc/.git" ] || git clone -q https://github.com/decompals/old-gcc.git "$src/old-gcc"
git -C "$src/old-gcc" fetch -q origin
git -C "$src/old-gcc" checkout -q "$oldgcc_mac"
# The PSX patches must be the ones the snapshot was measured with.
git -C "$src/old-gcc" diff --quiet "$oldgcc" -- patches/psx-2.5.7.patch patches/mipsel-2.7.patch \
    patches/obstack-2.7.2.h.patch patches/configure.patch
P=$src/old-gcc/patches

B=$(mktemp -d); trap 'rm -rf "$B"' EXIT
mkdir "$B/tree"
tar xzf "$src/gcc-$snap.tar.gz" -C "$B/tree" --strip-components=1
got=$(cd "$B/tree/gcc" && find . -type f | LC_ALL=C sort | xargs shasum -a 256 | shasum -a 256 | cut -d' ' -f1)
[ "$got" = "$tree_sha" ] || { echo "snapshot tree $got is not the measured $tree_sha" >&2; exit 1; }
mv "$B/tree/gcc" "$B/snap"; rm -rf "$B/tree"
if bison --version 2>/dev/null | head -1 | grep -Eq ' 3\.([6-9]|[1-9][0-9])'; then
    python3 "$here/genparsers.py" "$B/snap"
else
    docker run --rm -v "$B:/b" -v "$here:/r:ro" ubuntu:jammy bash -c \
        'apt-get update -qq >/dev/null && apt-get install -y -qq bison python3 >/dev/null && python3 /r/genparsers.py /b/snap'
fi

export PATH="/usr/bin:/bin:/usr/sbin:/sbin:$PATH"; unset CC CXX
cd "$B"; tar xzf "$src/gcc-2.7.2.tar.gz"; chmod -R u+w snap gcc-2.7.2
cd snap
# A VCS snapshot lacks what a release adds (configure, version.c, obstack, host configs):
# take those from 2.7.2, never overwriting a snapshot file.
for f in ../gcc-2.7.2/*; do b=$(basename "$f"); case $b in
  c-parse.c|c-parse.y|c-parse.h|cexp.c|bi-parser.c|bi-parser.h|objc-parse.c|objc-parse.y|objc|*.info*|ChangeLog*|TAGS) continue;; esac
  [ -e "$b" ] || cp -R "$f" "$b"; done
cp -Rn ../gcc-2.7.2/config/. config/ 2>/dev/null || true
chmod -R u+w .
perl -pi -e 's/include <varargs\.h>/include <stdarg.h>/g' *.c
perl -pi -e 's/^extern int sys_nerr;/extern const int sys_nerr;/' gcc.c
patch -u -p1 obstack.h -i "$P/obstack-2.7.2.h.patch"
patch -u -p1 configure -i "$P/configure.patch"
patch -u -p1 config/mips/mips.h -i "$P/mipsel-2.7.patch"
patch -su -p1 < "$P/psx-2.5.7.patch"
perl -pi -e 's/mips_split_addresses = 1;/mips_split_addresses = 0;/' config/mips/mips.c
test "$(grep -c 'mips_split_addresses = 0;' config/mips/mips.c)" = 2
# Darwin host support (from old-gcc gcc-2.7.2-psx-macos.sh).
curl -fsL "https://raw.githubusercontent.com/gcc-mirror/gcc/74af13c174714dd3b9f1ded4b39955f003c16361/config.guess" -o config.guess
curl -fsL "https://raw.githubusercontent.com/gcc-mirror/gcc/6fad101f3063d722e3348d07dc93cf737f8709e4/config.sub" -o config.sub
chmod +x config.guess config.sub
perl -pi -e 's/\| hiux\* \| abug \| nacl\*/| psx* | hiux* | abug | nacl*/' config.sub
cp "$P/xm-darwin.h" config/
awk '/^\t\*\)$/ { buf=$0; next }
     buf != "" { if (!done && /echo.*Configuration.*not supported/) {
       print "\t*-apple-darwin*)"; print "\t\txm_file=xm-darwin.h"
       print "\t\tfixincludes=Makefile.in"; print "\t\t;;"; done=1 }
       print buf; buf="" } { print }' configure > configure.tmp
mv configure.tmp configure; chmod +x configure
./configure --target=mips-sony-psx --prefix=/opt/cross --with-endian-little \
    --with-gnu-as --disable-gprof --disable-gdb --disable-werror
# 2.7.2's configure predates two Makefile.in conventions of the snapshot.
perl -pi -e 's|^   \$\(xmake_file\) \$\(tmake_file\) \\$|   \\|' Makefile
perl -pi -e 's|^target_alias=.*|target_alias=mips-sony-psx|' Makefile
! grep -q "configure' substitutes" Makefile
cc -std=gnu89 -c "$P/eprintf-darwin.c" -o eprintf-darwin.o
# Serial on purpose: this Makefile.in does not order every object after its insn-*.h.
make cc1 LANGUAGES=c LDFLAGS="$PWD/eprintf-darwin.o" \
    CFLAGS="-std=gnu89 -w -Wno-int-conversion -Wno-implicit-function-declaration -Wno-return-mismatch -Wno-incompatible-pointer-types"
./cc1 -quiet -O2 "$src/old-gcc/tests/little_endian.c" -o le.s && grep -Eq 'lbu\s\$2,0\(\$4\)' le.s
./cc1 -quiet -O2 "$src/old-gcc/tests/host_wide_int.c" -o hwi.s && grep -Eq '# 0x80000000$' hwi.s
printf 'extern int g[];\nint f(int i){return g[i];}\n' > t.c
./cc1 -quiet -O2 -G8 -mhard-float -fno-exceptions t.c -o t.s && grep -Eq 'lw\s\$2,g\(\$4\)' t.s
mkdir -p "$out"; cp cc1 "$out/cc1"
echo "installed $out/cc1"
