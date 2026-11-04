#include "scratchpad.h"
#include "game/course/trkintp2.h"
#include "game/course/trktext.h"
#include "game/gfx/gfxdraw.h"
#include "game/gfx/scene1.h"
#include "game/sound/cdstop.h"
#include "game/sound/sndslot.h"
#include "game/ui/hudmap2.h"
#include "psyq/libc/printf.h"
#include "game/demo/attract.h"
#include "game/demo/opening.h"
#include "game/menu/title.h"
#include "game/ui/hudmap2.h"
#include "game/course/trktext.h"
#include "rr/frame_environment.h"
#include "rr/color_conversion.h"
#include "rr/driver_control.h"
#include "game/gfx/gfxotag.h"

extern int func_80014A8C();
extern int func_800172DC();
extern int func_800207D4();
extern int func_8002D784();
extern int func_8002E0D8();
extern int func_80036FD4();
extern int func_80037D88();
extern int func_8004C5A4();
extern int func_8004FC18();
extern int func_800528A8();
extern int func_80052A30();
extern int func_80052B8C();
extern int func_80055330();
extern int func_8005534C();
extern int func_80059168();
extern int func_8006E1AC();
extern int func_8006ECE8();
extern int func_8006EF8C();
extern int func_800735D8();
extern int func_800747EC();
extern char D_800112CC[];
extern char D_800112D4[];
extern char D_800A1A6C[];
extern short D_800AC5D0;
extern int D_800ACA2C;
extern int D_800ACB0C;
extern int D_800ACB28;
extern int D_800ACB2C;
extern int D_800ACC14;
extern int D_800F2A64;
extern int D_800F2EC8;
extern short D_800F3A26;
extern int D_800F3A60;
extern int D_800F3A68;
extern int D_800F3A6C;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int D_8010718C;
extern int D_800FFC20[];
typedef struct 
{
  char pad[0xF0];
  int wF0;
  char padF4[0x318 - 0xF4];
  short h318;
  char pad2[0x320 - 0x31A];
} T320;
extern T320 D_80107198[];
extern struct 
{
  char pad[0x28A4];
  int w28a4;
  int w28a8;
} D_800F4DB8;
typedef struct 
{
  char pad0[0x18];
  int w18;
  int w1c;
  char pad20[0x2C];
  int w4c;
  char pad50[0x38C];
  int w3dc;
  char pad3E0[0xDC];
  int w4bc;
  int w4c0;
  int w4c4;
  int w4c8;
  int w4cc;
  int w4d0;
  int w4d4;
  int w4d8;
  int w4dc;
  unsigned short h4e0;
  short h4e2;
  char pad4E4[0x4];
  unsigned char b4e8;
  unsigned char b4e9;
  unsigned char b4ea;
  char pad4EB[0x1];
  int w4ec;
  char pad4F0[0x10];
  int w500;
  char pad504[0x2C];
  int w530;
  int w534;
  int w538;
} AC;
extern AC D_800AC650;
void func_8005DDF4()
{
  char sp18[16];
  int i;
  int old;
  int v;
  int one;
  int vv;
  AC *p;
  int c8;
  T320 *tp;
  int *ip;
  if (D_800AC650.w4bc == 0)
  {
    SetDispMask(0);
    func_8001ECE8(0, 0, 0, 1);
    func_80059168();
    func_80052A30(0);
    D_800AC650.w4dc = 0xFA;
    for (i = 0; i < 7; i++)
    {
      if (D_8010E270[i & 3] < 2)
      {
        D_80107198[i].h318 = 1;
      }
      else
      {
        D_80107198[i].h318 = 0;
      }
    }

    for (i = 0; i < 4; i++)
    {
      func_80050028(i, 1);
    }

    D_800F2EC8 = 1;
  }
  else
    if (D_800AC650.w4bc > 0)
  {
    if (D_800AC650.w4bc == 4)
    {
      SetDispMask(1);
      func_8001ECE8(0, 0, 0, 0);
    }
  }
  func_80037D88(5);
  func_800735D8(0);
  p = &D_800AC650;
  D_800F2EC8 = (one = 1);
  p->w4bc = p->w4bc + one;
  p->w4c4 = p->w4c4 + one;
  if (p->w4bc >= 0x1771)
  {
    p->w4bc = 0x1770;
  }
  if (p->w4c8 == 0)
  {
    vv = D_800F3A60;
  }
  else
  {
    vv = D_800F3A6C;
  }
  if (vv != 0)
  {
    if (D_800AC5D0 == 0)
    {
      if (p->w4bc >= 0x1A)
      {
        p->w4bc = 0xBB8;
        p->w4cc = one;
      }
    }
  }
  if (D_800AC650.w4c8 == 0)
  {
    if (D_800AC650.w4bc == 0xBB8)
    {
      D_800AC5D0 = 0x40;
      func_8004FC18(0, 0x3C);
    }
  }
  else
    if (D_800AC650.w4cc == 1)
  {
    D_800AC5D0 = 0x40;
    func_8004FC18(0, 0x3C);
    D_800AC650.w4cc = 2;
  }
  {
    register int c8_1 asm("a1") = D_800AC650.w4c8; /* ASM_BARRIER: pin w4c8 to a1 across switch cases and func_800172DC second argument */
    register int one_v0 asm("v0") = 1; /* ASM_BARRIER: pin constant 1 to v0 for w4c8 comparison */
    if (c8_1 == one_v0)
    {
      if (D_800F3A40 & 0x10)
      {
        D_800AC650.w534 = D_800AC650.w534 + 1;
        if (D_800AC650.w534 >= 4)
        {
          D_800AC650.w534 = 0;
        }
        switch (D_800AC650.w534)
        {
          case 0:
            D_800AC650.w3dc = 0;
            D_800AC650.w530 = 0;
            func_800172DC(&D_800AC650, c8_1);
            break;

          case 1:
            D_800AC650.w3dc = c8_1;
            D_800AC650.w530 = 0;
            break;

          case 2:
            D_800AC650.w3dc = 0;
            __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: keep w3dc store before w530 store in delay slot */
            D_800AC650.w530 = c8_1;
            func_800172DC(&D_800AC650, c8_1);
            break;

          case 3:
            D_800AC650.w3dc = c8_1;
            D_800AC650.w530 = c8_1;
            break;

        }

        func_8005534C(D_800ACA2C);
      }
    if (D_800AC650.w3dc == 1)
    {
      if (D_800AC650.w530 == 0)
      {
        if (D_800F3A3A & 8)
        {
          D_800AC650.h4e0 = D_800AC650.h4e0 + 1;
        }
        if (D_800F3A3A & 4)
        {
          D_800AC650.h4e0 = D_800AC650.h4e0 - 1;
        }
        if (((short) D_800AC650.h4e0) < 0)
        {
          D_800AC650.h4e0 = 0x167;
        }
        if (!(((short) D_800AC650.h4e0) < 0x168))
        {
          D_800AC650.h4e0 = 0;
        }
        if (D_800F3A3A & 2)
        {
          D_800AC650.h4e2 = 0x800;
        }
        if (D_800F3A3A & 1)
        {
          D_800AC650.h4e2 = 0;
        }
        if (D_800AC650.h4e2 >= 0x801)
        {
          D_800AC650.h4e2 = 0x800;
        }
        if (D_800AC650.h4e2 < 0)
        {
          D_800AC650.h4e2 = 0x800;
        }
        func_8004C138(&D_800AC650.h4e0, &D_800AC650.b4e8);
        func_80055330(0, D_800AC650.b4e8, D_800AC650.b4e9, D_800AC650.b4ea);
      }
    }
    if (D_800AC650.w530 == 1)
    {
      if (D_800F3A3A & 4)
      {
        D_800AC650.w500 = D_800AC650.w500 - 0x10;
      }
      if (D_800F3A3A & 8)
      {
        D_800AC650.w500 = D_800AC650.w500 + 0x10;
      }
      if (D_800AC650.w500 < 0)
      {
        D_800AC650.w500 = D_800AC650.w500 + 0x1000;
      }
      if (D_800AC650.w500 >= 0x1001)
      {
        D_800AC650.w500 = D_800AC650.w500 - 0x1000;
      }
    }
      if (D_800F3A40 & 0x100)
      {
        D_800AC650.w538 = D_800AC650.w538 ^ 1;
      }
    }
  }
  func_8006EF8C(0);
  if ((D_800AC650.w4c8 == 0) || (D_800AC650.w530 == 0))
  {
    func_8004417C(2, D_80107198 + D_800AC650.w4c0);
  }
  else
  {
    D_8010718C = 1;
    D_800ABFE8 = D_80107198[0].wF0;
    func_800437D8(D_80107198, &D_800AC650.w4ec);
  }
  if (D_800AC650.w4c4 >= 0x178)
  {
    if (D_800AC650.w1c != 0)
    {
      if (D_800AC650.w530 == 0)
      {
        v = D_800AC650.w4c0;
        D_800AC650.w4c0 = v - 2;
        if ((v - 2) < 0)
        {
          D_800AC650.w4c0 = v + 5;
        }
        D_800AC650.w4c = -1;
        D_800AC650.w4c4 = 0;
        func_8004417C(2, D_80107198 + D_800AC650.w4c0);
      }
    }
  }
  func_8006E1AC();
  if (D_800AC650.w4bc >= 5)
  {
    if (D_800AC650.w4c8 == 0)
    {
      func_800556B4();
    }
    else
    {
      func_80055660();
    }
  }
  if (D_800AC5D0 > 0)
  {
    D_800AC5D0 = D_800AC5D0 - 1;
    func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
    if (D_800AC5D0 == 0)
    {
      if (D_800AC650.w4c8 == 0)
      {
        D_800F4C6A = 1;
        D_800F3A26 = 3;
      }
      else
      {
        D_800AC650.w18 = 1;
        D_800F4C6A = 2;
        D_800F3A26 = 8;
      }
      D_800F4D8A = 0;
      D_800F2EC8 = 0;
      __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: prevent hoisting argument setup for func_8004FC18 before stores */
      func_8004FC18(0, 0);
    }
  }
  else
    if (D_800ACB0C < 0x1A)
  {
    func_800207D4(D_800ACC14 + 0x70, (0x19 - D_800ACB0C) * 8, 0x45);
  }
  {
    register int c8_2 asm("a2") = D_800AC650.w4c8; /* ASM_BARRIER: pin w4c8 to a2 across w28a4 clamping and func_800534B4 third argument */
    if (c8_2 == 1)
    {
      if (D_800AC650.w538 == 0)
      {
        old = D_800F4DB8.w28a4;
        if (D_800F3A40 & 0x8000)
        {
          D_800F4DB8.w28a4 = D_800F4DB8.w28a4 - 1;
        }
        if (D_800F3A40 & 0x2000)
        {
          D_800F4DB8.w28a4 = D_800F4DB8.w28a4 + 1;
        }
        if (D_800F4DB8.w28a4 <= 0)
        {
          D_800F4DB8.w28a4 = D_800F4DB8.w28a8 - 1;
        }
        if (D_800F4DB8.w28a4 >= D_800F4DB8.w28a8)
        {
          D_800F4DB8.w28a4 = c8_2;
        }
        if (old != D_800F4DB8.w28a4)
        {
          D_800AC650.w4d4 = 0x1D;
          if (func_800534B4(&D_800F4DB8, old, c8_2) >= 0)
        {
          if (D_800AC650.w4d0 == 0)
          {
            func_80052B8C(0, 0x11);
            D_800AC650.w4d0 = 0x19;
          }
        }
      }
      if (D_800AC650.w4dc > 0)
      {
        D_800AC650.w4dc = D_800AC650.w4dc - 1;
      }
      if (D_800AC650.w4d4 == 0)
      {
        if (D_800AC650.w4dc == 0)
        {
          if (func_800534B4() < 0)
          {
            if (D_800AC650.w4d8 == 0)
            {
              D_800F4DB8.w28a4 = D_800F4DB8.w28a4 + 1;
              if (D_800F4DB8.w28a4 >= D_800F4DB8.w28a8)
              {
                D_800F4DB8.w28a4 = 1;
              }
            }
            func_80052B8C(0, 0x11);
            D_800AC650.w4d0 = 0x19;
            D_800AC650.w4d4 = 0x1D;
          }
        }
      }
      D_800AC650.w4d0 = D_800AC650.w4d0 - 1;
      if (D_800AC650.w4d0 < 0)
      {
        D_800AC650.w4d0 = 0;
      }
      if (D_800AC650.w4d4 > 0)
      {
        D_800AC650.w4d4 = D_800AC650.w4d4 - 1;
        if (D_800AC650.w4d4 == 0)
        {
          func_800528A8(0, D_800A0C9C[D_800F765C - 1]);
          if (D_800AC650.w4d8 == 0)
          {
            func_80052A30(0);
          }
          else
          {
            func_80052A30(1);
          }
          D_800ACB2C = 0xFA;
        }
      }
      if (D_800F3A68 != 0)
      {
        if (D_800AC650.w4d8 == 0)
        {
          D_800AC650.w4d8 = 1;
        }
        else
        {
          D_800AC650.w4d8 = 0;
        }
      }
      if (D_800ACB28 != 0)
      {
        old = 0xC8;
        func_8004C894(0x131, old, D_800112CC, 0xFF, 0);
      }
      func_8004C894(0xFA, 0xD4, D_800A095C + (D_800A0C9C[D_800F4DB8.w28a4 - 1] << 5), 0xFF, 0);
      func_80059B1C(D_800A1A6C, 0xFC, 0xD4, 0xFF);
      func_80059B1C(D_800A1A6C - 4, 0x105, 0xD4, 0xFF);
      func_80096484(sp18, D_800112D4, D_800F4DB8.w28a4);
      func_8004C5A4(0x112, 0xD4, sp18, 0xFF, 0);
      func_80059B1C(D_800A1A6C - 0xC, 0x121, 0xD4, 0xFF);
        func_80059B1C(D_800A1A6C - 8, 0x12A, 0xD4, 0xFF);
        func_8004C928(0);
      }
    }
  }
  func_80014A8C(0xA);
  if ((D_800AC650.w4c8 == 0) || (D_800AC650.w530 == 0))
  {
    func_8002E0D8(6);
  }
  else
  {
    register int i_s1 asm("s1") = 1; /* ASM_BARRIER: pin loop counter to s1 */
    register int *ip_v0 asm("v0") = D_800FFC20; /* ASM_BARRIER: load base address into v0 before adding 4 into s0 */
    register int *ip_s0 asm("s0"); /* ASM_BARRIER: pin array pointer to s0 */
    __asm__ volatile("" : "+r"(ip_v0)); /* ASM_BARRIER: prevent folding +1 into address lo */
    ip_s0 = ip_v0 + 1;
    do
    {
      register int a0 asm("a0") = *ip_s0; /* ASM_BARRIER: load *ip_s0 into a0 first */
      register int a1 asm("a1") = 2; /* ASM_BARRIER: load 2 into a1 before ip_s0 increment */
      __asm__ volatile("" : "+r"(a0), "+r"(a1)); /* ASM_BARRIER: order argument setup before pointer increment */
      ip_s0++;
      __asm__ volatile("" : "+r"(ip_s0)); /* ASM_BARRIER: keep pointer increment before jal so i_s1++ fills delay slot */
      func_8002D784(a0, a1);
      i_s1++;
    }
    while (i_s1 < 7);
  }
  *((int *) (SCRATCHPAD + 0x78)) = D_800F2A64;
  func_8006ECE8(0);
  func_800747EC(0);
  func_80036FD4(0);
  func_800555E0();
}
