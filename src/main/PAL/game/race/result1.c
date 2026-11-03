#include "types.h"
#include "m2c_macros.h"

extern int func_8001ECE8();
extern int func_8001F5B4();
extern int func_8001FCC8();
extern int func_8001FD20();
extern int func_800207D4();
extern int func_80092CB8();
extern int func_8009AEAC();
extern char D_80010678[];
extern char D_80010684[];
extern short D_800AC5D0;
extern int D_800AC5DC;
extern int D_800ACC14;
extern int D_800F3A60;
extern unsigned int D_800F2DE4;
extern unsigned short D_800F3A26;
extern unsigned short D_800F4D8A;
extern short D_800F4C6A;
extern char D_80010694[];
extern char D_800106A4[];
extern int func_8004B864();
extern int func_8004BB54();
extern int func_8004FC18();
extern s32 D_800F2EC8;

void func_80035B84();
void func_80035E28(void);
void func_80036030(void);
void func_80036068(void);

void func_80035B84()
{
  int s3 = D_800F4D8A;
  unsigned int new_var;
  int *g;
  switch (s3)
  {
    case 0:
      func_80092CB8(0);
      func_8001ECE8(0, 0, 0, 3);
      D_800AC5DC = 0x80;
      func_80036068();
      D_800F2DE4 = 0;
      D_800F4D8A += 1;
      return;

    case 1:
      func_80092CB8(1);
      D_800F4D8A += 1;
      return;

    case 2:
      D_800F2DE4 += 1;
      func_8009AEAC(1, 1, 0);
      g = (int *) 0x1F800000;
      new_var = *g;
      *g = func_8001FCC8(D_800ACC14 + 0x70, func_8001FD20(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, new_var, 0x78, 0x100, D_80010678, 0x7F38, 0, 1), 0x78, 0x114, D_80010684, 0x7F38, 0, 1), 0, 0, 0x140, 0x200, 0x80, 0, 0), 0x5E);
      if (D_800F3A60 == 0)
    {
      if (D_800F2DE4 < 0xFB)
      {
        return;
      }
    }
      D_800F4D8A = 3;
      D_800AC5D0 = 0x20;
      return;

    case 3:
      if (D_800AC5D0 > 0)
    {
      D_800AC5D0 = ((unsigned short) D_800AC5D0) - 1;
    }
      func_800207D4(D_800ACC14 + 0x70, (0x20 - D_800AC5D0) * 8, 0x45);
      *((int *) 0x1F800000) = func_8001FD20(D_800ACC14 + 0x74, *((int *) 0x1F800000), 0, 0, 0x140, 0x200, (0x20 - D_800AC5D0) * 4, 0, 0);
      if (D_800AC5D0 <= 0)
    {
      D_800F4C6A = 1;
      D_800F3A26 = s3;
      D_800F4D8A = 0;
    }
      break;

  }

}

void func_80035E28(void)
{
  int s = D_800F4D8A;
  int new_var;
  switch (s)
  {
    case 0:
      func_80092CB8(0);
      func_8001ECE8(0, 0, 0, 3);
      D_800AC5DC = 0x80;
      D_800F2DE4 = 0;
      D_800F4D8A++;
      return;

    case 1:
      func_80092CB8(1);
      D_800F4D8A++;
      return;

    case 2:
      D_800F2DE4++;
      new_var = *((int *) 0x1F800000);
      *((int *) 0x1F800000) = func_8001FCC8(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, func_8001F5B4(D_800ACC14 + 0x70, new_var, 0x78, 0x100, D_80010694, 0x7F38, 0, 1), 0x78, 0x114, D_800106A4, 0x7F38, 0, 1), 0x3E);
      if (D_800F3A60 == 0)
    {
      if (D_800F2DE4 < 0xFB)
      {
        return;
      }
    }
      D_800AC5D0 = 0x20;
      D_800F4D8A = 3;
      return;

    case 3:
      if (D_800AC5D0 > 0)
    {
      D_800AC5D0 = D_800AC5D0 - 1;
    }
      func_800207D4(D_800ACC14 + 0x70, (0x20 - D_800AC5D0) * 8, 0x45);
      if (D_800AC5D0 <= 0)
    {
      D_800F4C6A = 1;
      D_800F3A26 = s;
      D_800F4D8A = 0;
    }
      break;

  }

}

void func_80036030(void) {
    func_8004BB54(0);
    func_8004BB54(1);
    func_8004B864(0);
    func_8004B864(1);
}

void func_80036068(void) {
    func_8004FC18(0, 0);
    func_80036030();
    D_800F2EC8 = 0;
}
