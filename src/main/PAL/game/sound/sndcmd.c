/* RODATA 0x80011108 0x10 */
#include "types.h"

extern short D_800F4D44;
extern struct 
{
  char pad0[0x2C];
  short h2C;
  char pad2E[0x19D0 - 0x2E];
  int f19D0;
  int f19D4;
  int f19D8;
  int f19DC;
  int f19E0;
  unsigned short h19E4;
  short h19E6;
  short arr[1];
  char pad19EA[0x1A28 - 0x19EA];
  int f1A28;
} D_800F4DB8;
extern struct 
{
  char pad[0x31];
  unsigned char b31;
  short h32;
} D_800AD528;
extern int func_8007EA34();
extern int func_800857A4();
extern int func_80084534();
extern int func_8008A674();
extern int func_8008A644();
extern int func_8004FBF0();
extern int func_80050E58();
extern int func_80051248();
extern int func_800514FC();
extern int func_8004F2A0();
extern int func_800517D8();
extern int func_8004EE08();
extern int func_8004FB00();
extern int func_8004F82C();
extern int func_80055300();
extern int func_80055330();
extern int func_8005534C();
extern int func_8004FED4();
extern int D_800F2EC8;
extern struct { char p0[0x1C]; s32 x1C; char p1[0x28]; s32 x48; s32 x4C; char p2[0x38C]; s32 x3DC; } D_800AC650;
extern s32 D_800AC66C;

void func_8004F418();
void func_8004F600(void);
void func_8004F720(void);
void func_8004F7E0(void);

void func_8004F418()
{
  int s0 = 0;
  unsigned short s1;
  int v1;
  register int v0 asm("$2"); /* ASM_BARRIER: pin v0 so both copies read the sra result */
  int t0;
  int t1;
  unsigned short s2;
  if (D_800F4D44 == 1)
  {
    if (D_800F4DB8.f1A28 > 0)
    {
      D_800F4DB8.f1A28 = D_800F4DB8.f1A28 - 1;
    }
  }
  if (D_800F4DB8.f19D0 != 0)
  {
    s2 = D_800F4DB8.arr[D_800F4DB8.f19D8];
    if (D_800F4DB8.arr[D_800F4DB8.f19D8] >= 0)
    {
      if (D_800F4DB8.f19E0 != 0)
      {
        if (D_800F4DB8.f19D8 > 0)
        {
          goto b7;
        }
      }
      else
      {
        b7:
        if (func_8007EA34(0x10, D_800F4DB8.f19D8) == 1)
        {
          s0 = -1;
        }

      }
      if (s0 == 0)
      {
        v1 = D_800F4DB8.h19E6 * D_800AD528.b31;
        s1 = D_800F4DB8.h19E4;
        if (v1 < 0)
        {
          v1 += 0xF;
        }
        t1 = v1 >> 4;
        v0 = ((short) s1) * D_800AD528.b31;
        s0 = t1;
        if (v0 < 0)
        {
          v0 += 0xF;
        }
        t0 = v0 >> 4;
        s1 = t0;
        if (D_800AD528.h32 != 0)
        {
          v0 = (((short) t0) + ((short) t1)) / 2;
          s0 = v0;
          s1 = v0;
        }
        func_800857A4(4, D_800F4DB8.h2C, (short) s2, 0, 0x3C, 0, (short) s1, (short) s0);
        func_800857A4(5, D_800F4DB8.h2C, (short) s2, t1 = 1, 0x3C, 0, (short) s1, (short) s0);
        D_800F4DB8.f19D8 = D_800F4DB8.f19D8 + t1;
      }
    }
    else
    {
      D_800F4DB8.f19D0 = 0;
      D_800F4DB8.f19D8 = 0;
      D_800F4DB8.f19DC = 0;
    }
  }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[4]; s32 x4; char p2[4]; s32 xC; char p3[0x24-0x10]; s16 v[1]; char p4[0x2868-0x26]; s32 x2868; } (*))&D_800F4DB8)

void func_8004F600(void) {
    int idx[4] = {1, 4, 5, 6};
    int i;
    func_80084534(0);
    for (i = 0; i < 4; i++) {
        if (D_800F4DB8.v[idx[i]] >= 0) {
            func_8008A674(D_800F4DB8.v[idx[i]]);
            D_800F4DB8.v[idx[i]] = -1;
        }
    }
    func_8008A644(0x18);
    D_800F4DB8.xC = 0;
    func_8004FBF0();
    func_80050E58();
    func_80051248(0);
    func_800514FC();
    func_8004F2A0();
    func_800517D8();
    func_8004EE08();
    func_8004FB00();
    func_80084534(0);
    D_800F4DB8.x2868 = -1;
    D_800F4DB8.x4 = 0;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char p0[0x4]; s32 x4; char p1[0x4]; s32 xC; s32 x10; char p2[0x44]; s16 x58; char p3[0x61E]; s16 x678; char p4[0x61E]; s16 xC98; char p5[0x61E]; s16 x12B8; } (*))&D_800F4DB8)

void func_8004F720(void) {
    func_80084534(0);
    func_8008A644(4);
    D_800F4DB8.xC = 1;
    D_800F4DB8.x4 = 0;
    func_8004F82C(D_800F4DB8.x10, 1);
    func_80055300();
    D_800F2EC8 = 1;
    func_80055330(0, 0x90, 0x90, 0x90);
    func_8005534C(D_800AC650.x3DC);
    func_8004FED4();
    D_800F4DB8.x58 = 0x52;
    D_800F4DB8.x678 = 0x52;
    D_800F4DB8.xC98 = 0x4E;
    D_800F4DB8.x12B8 = 0x4E;
    D_800AC650.x4C = -1;
    D_800AC650.x1C = 1;
    D_800AC650.x48 = 0;
}

#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[4]; s32 a; char p[4]; s32 b; s32 c; } (*))&D_800F4DB8)

void func_8004F7E0(void) {
    func_80084534(0);
    func_8008A644(4);
    D_800F4DB8.b = 1;
    D_800F4DB8.a = 0;
    func_8004F82C(D_800F4DB8.c, 0);
    D_800AC66C = 0;
}

#undef D_800F4DB8
