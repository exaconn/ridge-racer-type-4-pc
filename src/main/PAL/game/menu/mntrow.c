#include "types.h"
#include "rr/gpu.h"
#include "m2c_macros.h"

#define ROW(i) ((RrRow60 *)((s32)D_800FFC14 + (i) * 0x3C))

typedef struct {
    u8 unk0;
    u8 unk1;
    s8 unk2;
    s8 unk3;
    u8 unk4[24];
    u8 unk1C[24];
    u16 unk34;
    u16 unk36;
    s16 unk38;
    s16 unk3A;
} RrRow60;

struct O {
    char pad[0x70];
    u32 addr:24;
    u32 len:8;
};

typedef struct 
{
  char pad[0x1C];
  char c;
  char pad2[0x17];
  short d;
  unsigned short e;
} E;

typedef struct 
{
  int pad[6];
  int f;
} E_1;

typedef struct { short f0, f2; } U;

typedef struct { char pad[4]; char f4; char pad2[2]; char f7; } T;

extern RrSpritePacket *func_8001FCC8();
extern s32 func_80090424();
extern s32 func_800904F4();
extern u8 D_8009ED38[];
extern struct O *D_800ACC14;
extern RrRow60 *D_800FFC14;
extern unsigned short D_8009FDA0[];
extern E_1 *D_800F4D8C;
extern int func_8001ECE8();
extern s32 D_800AC5DC;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D8A;
extern void func_8003D228();
extern void func_8003DC9C();
extern void func_8003E040();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern void func_80053BB4();
extern short D_800AC5D0;
extern short D_800AD50A;
extern short D_800AD520;
extern int D_800F2DE4;
extern short D_800F3826;
extern U *D_800F3AFC;
extern char D_800F4D98;
extern T *D_800FF68C;
extern int func_8002A344();
extern int func_8003EC88();
extern int func_800416B4();
extern int func_800417EC();
extern int D_8009FDA4;
extern int D_800F3A60;
extern char *D_800F4D80;

void func_80040CB4(s32 a0);
void func_800411D4();
void func_80041320();
void func_8004139C();
void func_800413E0(void);
void func_80041434(void);
void func_80041588();

void func_80040CB4(s32 a0) {
    RrSpritePacket *s4;
    s32 fp;
    s32 sp10;
    s32 sp14;
    s32 sp18;
    u8 *sp1C;
    u16 s5;
    u16 sp20;
    s32 s2;
    s32 s0;
    s32 a3;
    s32 s1;
    s16 d34;
    s32 v1;

    s4 = *(RrSpritePacket **)0x1F800000;
    fp = ROW(a0)->unk38;
    sp1C = ROW(a0)->unk1C;
    sp10 = ROW(a0)->unk3A;
    s5 = ROW(a0)->unk36;
    /* ASM_BARRIER: keep fp and s7 increments before sp1C reload in outer loop latch */
    for (s2 = 0; *sp1C != 0; fp += 0xD, s2++, ({ __asm__("" : "=m"(*s4) : "r"(fp), "r"(s2 * 0x61)); })) {
        s0 = *sp1C - 0x20;
        sp1C++;
        if (s0 != 0) {
            s0 = D_8009ED38[s0 * 2] + (D_8009ED38[s0 * 2 + 1] << 5);
            a3 = 0;
            sp14 = (s0 % 21) * 12;
            sp18 = (s0 / 21) * 24;
            d34 = ROW(a0)->unk34;
            s1 = ROW(a0)->unk4[s2];
            {
                s32 ext = d34;
                if (ROW(a0)->unk3 >= s2) {
                    /* ASM_BARRIER: keep sign-extension of d34 before unk3 >= s2 branch */
                    __asm__("" : : "r"(ext));
                    if (d34 == 1) {
                        if (s2 & 1) {
                            s5 = 0x2638;
                            sp20 = 0x2639;
                        } else {
                            sp20 = 0x2638;
                            s5 = 0x2639;
                        }
                        if (func_80090424(s1 << 6) > 0) {
                            a3 = s1 * 4;
                        } else {
                            a3 = 0xFF - (s1 - 0x20) * 4;
                        }
                        if (s1 >= 0x40) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 == 3) {
                        sp20 = 0x2639;
                        s5 = 0x2638;
                        a3 = (func_800904F4(((s1 << 5) - s2 * 0x61) & 0xFFF) * 0x4A) / 4096 + 0x4A;
                        if (s1 >= 0x80) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 == 2) {
                        s5 = 0x2638;
                        a3 = (func_800904F4(((s1 << 5) - s2 * 0x61) & 0xFFF) * 0x4A) / 4096 + 0x4A;
                        if (s1 >= 0x80) {
                            ROW(a0)->unk4[s2] = 0;
                        }
                        ROW(a0)->unk4[s2]++;
                    } else if (d34 >= 0) {
                        s0 = d34 >> 8;
                        v1 = func_80090424(ROW(a0)->unk0 << 6) / 256;
                        a3 = v1 + 0x80;
                        if (s0 - 1 == s2) {
                            a3 = v1 + 0xE4;
                            s5 = 0x2638;
                        } else {
                            s5 = ROW(a0)->unk36;
                            if (s1 < 0x15) {
                                a3 = (func_80090424((s1 * 0x555) / 20) * 0x94) / 4096;
                                ROW(a0)->unk4[s2]++;
                            }
                        }
                    } else {
                        break;
                    }
                }
            }
            for (s0 = 0; s0 < (ROW(a0)->unk34 & 1) + 1; s0++) {
                s4->len = 4;
                s4->code = 0x64;
                s4->x = fp;
                s4->y = sp10;
                s4->w = 0xC;
                s4->h = 0x18;
                s4->u = sp14;
                s4->v = sp18;
                if (s0 == 1) {
                    s4->clut = sp20;
                    a3 = 0x94 - a3;
                } else {
                    s4->clut = s5;
                }
                s4->r = a3;
                s4->g = a3;
                s4->b = a3;
                s4->code |= 2;
                s4->addr = D_800ACC14->addr;
                D_800ACC14->addr = (u32)s4;
                s4++;
                /* ASM_BARRIER: count one more reg in inner loop so loop_optimize does not hoist 0x64 */
                __asm__("" : "=r"(v1));
            }
        }
    }
    *(RrSpritePacket **)0x1F800000 = func_8001FCC8((char *)D_800ACC14 + 0x70, s4, 0x2E);
}

/* legacy typing of shared globals in this part of the unit */
#define func_8001FCC8 (*(unsigned *(*)())&func_8001FCC8)
#define D_800ACC14 ((char *)D_800ACC14)

void func_800411D4()
{
    unsigned *a1;
    int a3;
    unsigned char *q;
    char *d;
    unsigned t;
    a1 = func_8001FCC8(D_800ACC14 + 0x98, *(int *)0x1F800000, 5);
    q = (unsigned char *)a1 + 7;
    a3 = 0;
    do {
        q[a3 * 20 - 4] = 4;
        q[a3 * 20] = 0x64;
        *(short *)(q + a3 * 20 + 9) = 0xC;
        *(short *)(q + a3 * 20 + 0xB) = 0x18;
        q[a3 * 20 + 5] = 0xF4;
        q[a3 * 20 + 6] = 0x68;
        *(short *)(q + a3 * 20 + 7) = 0x2678;
        q[a3 * 20] |= 1;
        *(short *)(q + a3 * 20 + 1) = (a3 % 22) * 13 + 0x11;
        *(short *)(q + a3 * 20 + 3) = (a3 / 22) * 0x1C + 0x60;
        d = D_800ACC14;
        a3++;
        t = (unsigned)a1 & 0xFFFFFF;
        *a1 = (*a1 & 0xFF000000) | (*(unsigned *)(d + 0x98) & 0xFFFFFF);
        *(unsigned *)(d + 0x98) = (*(unsigned *)(d + 0x98) & 0xFF000000) | t;
        a1 += 5;
    } while (a3 < 0xF2);
    *(unsigned **)0x1F800000 = func_8001FCC8(d + 0x98, a1, 0xE, a3);
}

#undef func_8001FCC8
#undef D_800ACC14

/* legacy typing of shared globals in this part of the unit */
#define D_800FFC14 ((E *)D_800FFC14)

void func_80041320(a0, a1, a2)
int a0;
int a1;
int a2;
{
  E *p;
  ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->e = D_8009FDA0[a1];
  if (a2 < (-0xFF))
  {
    ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->d = -a2;
    return;
  }
  switch (a2)
  {
    case 0:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x20;
      return;

    case 1:
      ((E *) (((int) D_800FFC14) + (a0 * 0x3C)))->c = 0x3E;
      return;

    case -1:
      return;

  }

}

#undef D_800FFC14

void func_8004139C(a0, a1)
int a0;
int a1;
{
  int i;
  int new_var;
  for (i = 0; i < 4; i++)
  {
    new_var = i;
    D_800F4D8C[a1 + new_var].f = a0;
    new_var = 0xC000;
    a0 = a0 + new_var;
  }

}

void func_800413E0(void) {
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    D_800F4C6A = 1;
    D_800F3A26 = 8;
    D_800F4D8A = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define func_8001ECE8 ((void (*)())func_8001ECE8)
#define D_800F4D8C ((char *)D_800F4D8C)

void func_80041434(void) {
    int i;
    func_8001ECE8(0, 0, 0, 2);
    D_800AC5DC = 0x80;
    for (i = 0; i < 4; i++) {
        func_8003E040(i, -2, 0, 0);
    }
    D_800AD520 = 2;
    D_800F4D98 = 2;
    D_800F3826 = 0;
    func_80053BB4(0);
    func_8003DC9C(*(unsigned char *)(D_800F4D8C + 7), *(short *)(D_800F4D8C + 8), *(short *)(D_800F4D8C + 0xA), *(short *)(D_800F4D8C + 0xC), *(short *)(D_800F4D8C + 0xE), 0, 0);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    func_8003E928(0, 0xC, 0x48, 0xF0, 0x20, 0, 0, 4, 2, 0);
    D_800F3AFC->f2 = 1;
    D_800FF68C->f4 = 1;
    D_800FF68C->f7 = 0;
    D_800F2DE4 = 0;
    D_800AD50A = 0x108;
    D_800AC5D0 = 0;
}

#undef func_8001ECE8
#undef D_800F4D8C

/* legacy typing of shared globals in this part of the unit */
#define D_800AD520 (*(unsigned char (*))&D_800AD520)
#define D_800F2DE4 (*(unsigned (*))&D_800F2DE4)
#define D_800F3AFC ((char *)D_800F3AFC)
#define D_800FF68C ((signed char *)D_800FF68C)

void func_80041588()
{
    int a2;
    D_800F2DE4 += 1;
    if (D_800FF68C[4] >= 0x22) {
        func_8003EC88(0);
        func_8002A344(0x10002);
    } else {
        func_8002A344(0x10003);
    }
    if (func_800416B4(D_8009FDA4) != 0) {
        if (*(short *)(D_800F3AFC + 0x10) == 3) {
            func_800417EC();
            return;
        }
    }
    if (D_800FF68C[4] == 1) {
        if (D_800F3A60 == 0) {
            if (D_800F2DE4 < 0x1F5) return;
        }
        a2 = D_800FF68C[6];
        if (a2 == 4) {
            D_800FF68C[4] = D_800AD520 + 0x22;
            D_800F4D80[0x16] = D_800AD520;
            *(short *)(D_800F3AFC + 0x10) = a2;
        }
    }
}

#undef D_800AD520
#undef D_800F2DE4
#undef D_800F3AFC
#undef D_800FF68C
