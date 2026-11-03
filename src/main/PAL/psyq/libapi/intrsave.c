/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/library.h"

extern RrLibSavedState D_800A52C8;
extern unsigned short *D_800A6358, *D_800A6354;
extern int *D_800A635C;
extern void func_80096F04(), func_80096EE4();
extern u16 *D_800A6358;
extern void func_80096EF4(void *data);
extern void func_80096F14(void);

RrLibSavedState *func_8008B8E8(void);
RrLibSavedState *func_8008B988(void);
void func_8008BA00();

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

RrLibSavedState *func_8008B988(void) {
    RrLibSavedState *state = &D_800A52C8;
    if (state->active != 0) return 0;
    func_80096EF4(state->callback_data);
    state->active = 1;
    *D_800A6358 = state->register16;
    {
        s32 *destination = D_800A635C;
        s32 value = state->register32;
        /* ASM_BARRIER: retain the original load-delay nop before the call/store pair. */
        __asm__ volatile("nop");
        *destination = value;
    }
    func_80096F14();
    return state;
}

void func_8008BA00(p, n) int *p; int n; {
    while (n--) *p++ = 0;
}
