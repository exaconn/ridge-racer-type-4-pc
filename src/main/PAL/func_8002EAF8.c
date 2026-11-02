#include "rr/ui.h"
extern int func_8001ECE8();
extern int func_8002F8E4();
extern int func_8003D228();
extern int func_8009AEAC();
extern int func_8001E84C();
extern char D_800104B0[];
extern int D_800ABEB0;
extern char D_800ABEBC[];
extern char *D_800AC590;
extern int D_800AC5DC;
extern short D_800AC64A;
extern short D_800AD50A;
extern int D_800F2DE4;
extern int D_800F3970;
extern short *D_800F3AFC;
extern short D_800F4CD0;
extern short D_800F4D2C;
void func_8002EAF8()
{
  int i;
  int y;
  int v;
  short c7;
  func_8001ECE8(0, 0, 0, 3);
  c7 = 7;
  i = 0;
  D_800F2DE4 = 0;
  D_800ABEB0 = 0;
  D_800AC5DC = 0x80;
  D_800F4D2C = 0;
  D_800AC64A = -1;
  D_800ABEBC[3] = 0;
  D_800ABEBC[2] = 0;
  D_800ABEBC[1] = 0;
  D_800ABEBC[0] = 0;
  D_800AD50A = 0x104;
  while (i < (D_800F4CD0 + 1))
  {
    v = i;
    if (D_800F4CD0 == c7)
    {
      if (i == D_800F4CD0)
      {
        v = 8;
      }
    }
    func_8003E928(i, 0x6C, (i * 0x22) + 0x40, 0x54, 0x20, 0, 0, 1, 0x7F, v);
    i++;
  }

  if (func_8002F8E4() == 0x140)
  {
    D_800F3970 = 1;
  }
  D_800AC590[7] = 2;
  func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
  D_800F3AFC[1] = 1;
  func_8009AEAC(1, 1, 0);
  func_8001E84C(D_800104B0);
}
