/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libsnd/vmupd.h"
#include "psyq/libsnd/ssatr.h"
#include "rr/channels.h"
#include "rr/spu.h"
  int func_80089540(a0, a1, a2, a3) int a0; int a1; int a2; int a3; {
  register int s asm("$16");
  short i;
  int c0 = a0;
  int t2 = a2 << 16;
  int t1 = a1 << 16;
  register int c3 asm("$21") = a3;
  int t0;
  func_8008A564(t1 >> 16, t2 >> 16);
  i = 0;
  D_80110114 = c0;
  s = 0;
  if (D_801100FC > 0) { t0 = c0 << 16; c0 = c3 & 0xFFFF; do { s += func_80089344(i, t0 >> 16, t1 >> 16, t2 >> 16, c0); i++; } while (i < D_801100FC); }
  return s;
}
