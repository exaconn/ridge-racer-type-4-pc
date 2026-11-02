/* Candidate module: menu transitions, 8007BD18..8007BFF4.
 * Contiguous functions indexed by the same menu state and entry table. */
#include "rr/tables.h"

extern s32 func_8004F24C(), func_80078E1C(), func_80078E40();
extern s32 func_8007A310(), func_8007A9CC();
extern RrEntry800A4258 D_800A4258[];
extern RrMenuChoice D_800A44F4[];
extern s32 D_800A4510[];
extern s16 D_800A4514[];
extern RrMenuChoice D_800A4520[];
extern s8 *D_800AC590;
extern s16 D_800AC594;
extern s32 D_800F2DE4;
extern s16 D_800F3A1C;
extern u8 D_800F3A24[];
extern RrMenuItemState5C D_800F3A38[];
extern s16 D_800FFC00;
extern u16 D_80109458;
extern s16 D_8010948C;

void func_8007BD18(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 a = D_800A44F4[i].a;
    s32 c = D_800A44F4[i].c;
    s32 b = D_800A44F4[i].b;
    s32 r;
    if (c == 1) r = func_8007A310(0, D_800FFC00);
    else r = func_8007A9CC(0, D_800FFC00);
    if (r == 4) {
        D_800F3A1C = a;
    } else if (r == 5) {
        D_800F3A1C = b;
    } else if (r == 6) {
        D_800F3A1C = b;
    }
    if (c == 0xD) {
        if (r == 4) D_8010948C = 1;
    }
}

void func_8007BDF8(void)
{
  s32 a1 = *(D_800A4510 + D_800A4258[D_800F3A1C].i);
  if ((&D_800F3A38[D_800AC594])->a != 0)
  {
    if (a1 == 0)
    {
      func_8004F24C(1);
      if (D_800F3A1C == 0xC)
      {
        D_800F3A1C = D_80109458;
        D_800F3A24[D_800FFC00] = 0x5A;
        func_80078E40(D_800FFC00, 0, 1);
        return;
      }
      D_800F3A1C = 0;
    }
  }
  else
    if ((&D_800F3A38[D_800AC594])->b != 0)
  {
    if (a1 == 0)
    {
      func_8004F24C(2);
      D_800F3A1C = 0;
    }
  }
}

void func_8007BEF8(void) {
    s32 i = D_800A4258[D_800F3A1C].i;
    s32 v = D_800A4514[i * 2];
    if (D_800AC590[6] == 4) {
        D_800F3A1C = v;
        D_800F2DE4 -= 1;
    }
}

void func_8007BF58(void) {
    s32 k;
    s32 i;
    s32 a, b;
    for (k = 0x63; k >= 0; k--) ;
    i = D_800A4258[D_800F3A1C].i;
    a = D_800A4520[i].a;
    b = D_800A4520[i].c;
    if (func_80078E1C(D_800FFC00, 0) == 1) {
        D_800F3A1C = a;
        return;
    }
    D_800F3A1C = b;
}
