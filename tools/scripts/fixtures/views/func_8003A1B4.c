/* CC1_FLAGS: -fno-strength-reduce */

#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define SetDispMask ((int (*)())SetDispMask)

#define func_8005CA88 ((int (*)())func_8005CA88)

#define D_800F4D70 (*(int (*))&D_800F4D70)

#define D_800F4C6A (*(short (*))&D_800F4C6A)

#undef SetDispMask

#undef func_8005CA88

#undef D_800F4D70

#undef D_800F4C6A

extern int func_8001E84C();

extern int func_8003D1D4();

extern int func_8003DC50();

extern int func_8003DFEC();

extern int func_8003E8D4();

extern int func_8003FAA0();

extern int D_800102F0;

extern char D_8001075C[];

extern int D_800AC540;

extern int D_800ACC18;

extern int D_800ACC1C;

extern int D_800ACC24;

extern int D_800F2ECC;

extern int D_800F4BEC;

extern int D_800F4C1C;

extern int MenuTextRowCount;

extern int D_800F4D70[];

void func_8003A1B4()
{
  /* ASM_BARRIER: pin the 0x2D000 constant to $a2 so the addu and the call share one lui/ori like the original */
  register int c asm("$6") = 0x2D000;
  D_800F2ECC = 10;
  D_800AC540 = 4;
  D_800F4C1C = 4;
  D_800F4BEC = 0x28;
  D_800ACC24 = 1;
  MenuTextRowCount = 0x37;
  D_800F4D70[1] = D_800102F0 + c;
  D_800ACC18 = D_800F4D70[1];
  func_8001E84C(D_8001075C, D_800ACC18, 0x2D000);
  D_800ACC18 = D_800ACC18 + func_8003D1D4(D_800ACC18);
  D_800ACC18 = D_800ACC18 + func_8003DC50(D_800ACC18);
  D_800ACC18 = D_800ACC18 + func_8003DFEC(D_800ACC18);
  D_800ACC18 = D_800ACC18 + func_8003FAA0(D_800ACC18);
  D_800ACC18 = D_800ACC18 + func_8003E8D4(D_800ACC18);
  D_800ACC1C = D_800ACC18;
  D_800F4D70[1] = D_800ACC18 + 0x19000;
  D_800ACC18 = D_800ACC18 + 0x19000;
}
