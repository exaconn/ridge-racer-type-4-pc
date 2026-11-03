/* GCC_VERSION: 2.7.2 */
#include "types.h"
#include "m2c_macros.h"

extern int func_80087DE4();
extern int func_80088340();

void func_800884A8();
int func_80088594(int a0, int a1, unsigned short a2);
void func_800885C8(void);

void func_800884A8(a0, a1, a2, a3, a4, a5)
int a0;
int a1;
int a2;
int a3;
int a4;
int a5;
{
  int new_var;
  int y;
  int x;
  int v;
  int r;
  x = a4 & 0xFFFF;
  y = a5 & 0xFFFF;
  if (x == y)
  {
    v = 0x40;
    r = a4;
  }
  else
    if (((unsigned) y) < ((unsigned) x))
  {
    new_var = (y << 6) / x;
    r = a4;
    v = new_var;
  }
  else
  {
    r = a5;
    v = 0x7F - ((x << 6) / y);
  }
  func_80087DE4(0x21, (short) a0, (short) a1, a2 & 0xFFFF, r & 0xFFFF, v & 0xFFFF);
}

int func_80088594(int a0, int a1, unsigned short a2) { return func_80088340(0x21, (short)a0, (short)a1, a2); }

void func_800885C8(void) {

}
