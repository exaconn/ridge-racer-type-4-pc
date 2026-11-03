/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern RrLibSavedState D_800A52C8;
extern unsigned short *D_800A6358, *D_800A6354; extern int *D_800A635C;
extern void func_80096F04(), func_80096EE4();
RrLibSavedState *func_8008B8E8(void) {
  RrLibSavedState *p = &D_800A52C8;
  unsigned short *a, *c; int *b; int t;
  if (p->active == 0) return 0;
  func_80096F04();
  a = D_800A6358;
  b = D_800A635C;
  p->register16 = *a;
  t = *b;
  c = D_800A6354;
  p->register32 = t;
  *a = 0;
  *c = *(volatile unsigned short *)a;
  *D_800A635C = *D_800A635C & 0x77777777;
  func_80096EE4();
  p->active = 0;
  return p; }
