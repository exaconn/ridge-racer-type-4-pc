#include "rr/ui.h"

extern int func_8001E84C();
extern int func_8001F254();
extern int func_80053CB0();
extern int func_80092CB8();
extern int func_8009AEAC();
extern int D_800102F0;
extern char D_800104A4[];
extern short D_800AC5D0;
extern int D_800AC5D4;
extern short D_800ACBD0[];
extern int D_800ACBF8;
extern int D_800ACC18;
extern short D_800AD508;
extern short D_800AD522;
extern int D_800F2EC8;
extern unsigned short D_800F3974;
extern short D_800F3A26;
extern short D_800F4BE8;
extern short D_800F4C6A;
extern int D_800F4CA8;
extern short D_800F4CD0;
extern int D_800F4D40;
extern unsigned short D_800F4D44;
extern short D_800F4D8A;
extern char D_800F4D98[];
extern short D_800FF688;
extern int D_8010BFB8;
extern int func_8001ECE8();
extern int func_8002F8E4();
extern int func_8003D228();
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
extern short D_800F4D2C;

void func_8002E9B0();
void func_8002EAF8();

void func_8002E9B0() {
    short *a0;
    char *a1;
    int a2;
    func_80092CB8(0);
    func_80053CB0();
    a2 = 0;
    a1 = D_800F4D98;
    a0 = D_800ACBD0;
    D_800AD508 = -1;
    D_800FF688 = 0;
    D_800AC5D0 = 0;
    D_800AC5D4 = 0;
    D_8010BFB8 = 0;
    D_800AD522 = 0;
    do {
        a0[0] = 0;
        a0[1] = 0;
        a1[3] = 0;
        a1 += 4;
        a2++;
        a0 += 2;
    } while (a2 < 4);
    D_800F4CA8 = 2;
    D_800F4BE8 = 4;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
    D_800ACC18 = D_800102F0;
    D_800F4D40 = D_800102F0;
    if ((unsigned)(D_800F4D44 - 2) < 3U) func_8001F254(D_800102F0, a1, a2);
    D_800F4D44 = 0;
    func_8009AEAC(1, 1, 0);
    D_800ACBF8 = 0;
    D_800F2EC8 = 0;
    D_800F4CD0 = (D_800F3974 & 1) + 7;
    func_8001E84C(D_800104A4);
}

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
