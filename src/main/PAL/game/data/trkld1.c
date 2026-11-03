#include "types.h"
#include "m2c_macros.h"

extern int D_800A0220;
extern int D_800A02BC;
extern int D_8010C1F0;
extern int D_800A029C[];
extern short D_800F4D44;
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_8004D4BC();
extern int func_8004D61C();

int func_8004D628();
void func_8004D7B4(s32 arg0);
s32 func_8004D7C8(void);

int func_8004D628(a0, a1, a2)
int a0;
int a1;
int a2;
{
  int v;
  int t;
  int e;
  e = 0x44;
  v = 0x1E;
  switch (D_800A0220)
  {
    case 0:
      switch (D_800F4D44)
    {
      case 1:
        D_800A02BC = func_8001223C(2, D_800A029C[a0] + (a2 + v));
        break;

      case 3:
        D_800A02BC = func_8001223C(2, (a0 * 4) + (a2 + e));
        break;

      case 5:
        D_800A02BC = func_8001223C(2, a0 + 0x74);
        break;

      case 6:
        D_800A02BC = func_8001223C(2, (a0 * 2) + 0x64);
        break;

      default:
        D_800A0220 = 0;
        return 1;

    }

    case 1:
      v = func_8001292C(D_800A02BC, D_8010C1F0);
      if (v != 0)
    {
      D_800A0220++;
    case 2:
      t = func_80012A58();
      if (t != 0)
      {
        func_8004D61C(D_8010C1F0);
        v = func_8004D4BC(t);
        D_800A0220 = 0;
        D_8010C1F0 += v;
        return 1;
      }
      v = 0;
    }
      return v;
    default:
      return 0;
  }
}

void func_8004D7B4(s32 arg0) {
    D_8010C1F0 = arg0;
    D_800A0220 = 0;
}

s32 func_8004D7C8(void) {
    return D_8010C1F0;
}
