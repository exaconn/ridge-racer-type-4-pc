/* RODATA 0x800106d0 0x1c */
#include "types.h"
#include "rr/ui.h"
#include "m2c_macros.h"
#include "game/menu/menuwork.h"
#include "game/ui/barsel0.h"
#include "rr/audio_loop.h"
#include "rr/menu_background.h"
#include "game/ui/barsel0.h"

typedef struct { short a, b; } X;

typedef struct { char p[4]; char a; char b; char c; char d; } Y;

extern int func_8001FCC8();
extern int func_8001FF84();
extern unsigned char D_8009F730[];
extern unsigned char D_8009F790[];
extern unsigned short D_800AC648;
extern int D_800ACC14;
extern signed char D_800AD4EF;
extern int func_8003D228();
extern int func_8003DC9C();
extern int func_8003E040();
extern int D_800ACC18;
extern int D_800F2DE4;
extern int D_800F4D40;
extern signed char D_800F4D7C;
extern int D_800F4D8C;
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern int func_8001E84C();
extern int func_8002AF8C();
extern void func_80036D40(void);
extern int func_8003A1B4();
extern int func_8003AE4C();
extern int func_8003EC88();
extern char D_800106EC[];
extern char D_800106F8[];
extern int D_8009FDA4;
extern int D_800F3A60;
extern struct { char pad[0x10]; short h10; } *D_800F3AFC;
extern struct { char pad[4]; signed char b4; char b5; signed char b6; } *D_800FF68C;

void func_8003662C();
void func_8003687C(int a0);
void func_800369E8(void);
void func_80036A44(void);

void func_8003662C(a0)
int a0;
{
  int s4;
  int a1;
  int sz2;
  int new_var;
  int s3;
  int s1;
  int s2;
  int s0;
  int t0;
  int d;
  /* ASM_BARRIER: pin t to $v1 like the original */
  register int t asm("$3");
  int u;
  unsigned char *p;
  unsigned char *q;
  unsigned char *base;
  s4 = 0x80;
  if (a0 < 0x41)
  {
    s4 = a0 * 2;
  }
  a1 = *((int *) 0x1F800000);
  s3 = 0x50;
  s1 = 0;
  do
  {
    if (s1 < 2)
    {
      d = D_800F4D44;
      if ((d != 1) && (s1 == 1))
      {
        goto next;
      }
      switch (d)
      {
        case 1:
          s2 = D_8009F790[(D_800AD4EF * 2) + s1];
          break;

        case 0:
          s2 = 3;
          break;

        case 2:

        case 3:
          s2 = 0;
          break;

        case 4:
          s2 = 1;
          break;

        case 6:
          s2 = 2;
          break;

      }

    }
    if (s1 == 2)
    {
      s2 = 0xC;
    }
    else
      if (s1 == 3)
    {
      t = s2 + 1;
      s2 = t + (D_800AC648 & 7);
    }
    p = D_8009F730 + (s2 * 4);
    a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s3, 0x48, p, 0x263A, (s4 + 1) * 2);
    u = s3 + 2;
    s3 = u + p[2];
    next:
    s1++;

  }
  while (s1 < 4);
  s2 = 0x16;
  if (D_800F4D44 != 1)
  {
    s0 = 1;
    s3 = 0xD6;
  }
  else
  {
    s2 = 0x15;
    s0 = 2;
    s3 = 0xE6;
  }
  t0 = 0x263A;
  s1 = 0;
  if (s0 != 0)
  {
    base = D_8009F730;
    do
    {
      new_var = s3 - 2;
      a1 = func_8001FF84(D_800ACC14 + 0x70, a1, s3, 0x24, (unsigned char *)((s2 * 4) + (int)base), t0, (s4 + 1) * 2);
      s2 = 0x17;
      s1++;
      q = base + 0x5C;
      t = new_var;
      s3 = t - q[2];
      t0 = 0x263B;
    }
    while (s1 < s0);
  }
  *((int *) 0x1F800000) = func_8001FCC8(D_800ACC14 + 0x70, a1, 0x2E);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((X *)D_800F3AFC)
#define D_800FF68C ((Y *)D_800FF68C)

void func_8003687C(int a0) {
    int i = 0;
    int a;
    int t;
    int p;
    do {
        a = a0 + i;
        t = a * 0x1C;
        *(int *)(t + D_800F4D8C + 0x18) = *(int *)(i * 0x1C + D_800F4D8C + 0x18) + 0x30000;
        func_8003E040(a, -2, 0, 0);
        i += 1;
        p = t + D_800F4D8C;
        func_8003DC9C(*(unsigned char *)(p + 7), *(short *)(p + 8), *(short *)(p + 0xA), *(short *)(p + 0xC), *(short *)(p + 0xE), 0, 0);
    } while (i < 4);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    func_8003E928(0, 0x28, 0x48, 0xC0, 0x20, 0, 0, 4, 3, 4);
    D_800F3AFC->b = 1;
    D_800FF68C->a = 1;
    D_800FF68C->d = 0;
    D_800F2DE4 = 0;
    D_800F4D7C = 0;
    D_800ACC18 = D_800F4D40;
}

#undef D_800F3AFC
#undef D_800FF68C

void func_800369E8(void) {
    func_8002A344(0x10006);
    func_8003687C(0);
    D_800F2DE4 = 0;
    D_800AC5DC = 0x80;
    D_800F4C6A = 1;
    D_800F3A26 = 0xA;
    D_800F4D8A = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2DE4 (*(unsigned (*))&D_800F2DE4)

void func_80036A44(void) {
    int a;
    int t;
    int r;
    int s1;
    D_800F2DE4 += 1;
    if (D_800FF68C->b4 >= 0x22) {
        func_8003EC88(0);
        a = 0x10006;
    } else {
        a = 0x10007;
    }
    func_8002A344(a);
    if (func_80036C08(D_8009FDA4) != 0) {
        if (D_800F3AFC->h10 == 3) {
            func_80036D40();
            return;
        }
    }
    if (D_800FF68C->b4 == 1) {
        if (D_800F3A60 != 0 || D_800F2DE4 >= 0x1F5) {
            t = D_800FF68C->b6;
            if (t == 4) {
                D_800FF68C->b4 = 0x22;
                D_800F3AFC->h10 = t;
                return;
            }
        }
    }
    if (D_800AD4F2 == 2) {
        r = func_8003AE4C(0, 1);
        if (r == 1) {
            if (D_800F3AFC->h10 == 0xB) {
                func_8002AF8C(0, 1, 0);
                func_8001E84C(D_800106EC);
                s1 = D_800ACC18;
                func_8003A1B4();
                func_80039988();
                D_800F4C6A = r;
                D_800F3A26 = 0x22;
                D_800F4D8A = 0;
                D_800ACC18 = s1;
                func_8001E84C(D_800106F8);
            }
        }
    }
}

#undef D_800F2DE4
