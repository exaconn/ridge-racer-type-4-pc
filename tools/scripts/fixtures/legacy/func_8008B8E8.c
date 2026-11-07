/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern RrLibSavedState D_800A52C8;
extern unsigned short *I_MASK, *D_800A6354; extern int *D_800A635C;
extern void EnterCriticalSection(), ResetEntryInt();
RrLibSavedState *stopIntr(void) {
  RrLibSavedState *p = &D_800A52C8;
  unsigned short *a, *c; int *b; int t;
  if (p->active == 0) return 0;
  EnterCriticalSection();
  a = I_MASK;
  b = D_800A635C;
  p->register16 = *a;
  t = *b;
  c = D_800A6354;
  p->register32 = t;
  *a = 0;
  *c = *(volatile unsigned short *)a;
  *D_800A635C = *D_800A635C & 0x77777777;
  ResetEntryInt();
  p->active = 0;
  return p; }
