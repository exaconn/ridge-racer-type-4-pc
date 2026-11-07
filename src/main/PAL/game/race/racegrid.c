/* RODATA 0x800105d0 0x30 */
#include "types.h"
#include "game/car/car.h"
#include "scratchpad.h"
#include "m2c_macros.h"
#include "game/system/pad.h"
#include "game/ui/hudmap.h"
#include "game/race/racegrid.h"
#include "rr/menu_selection.h"
#include "rr/matrix_workspace.h"
#include "rr/audio_loop.h"
#include "rr/driver_control.h"
#include "rr/bios_events.h"
#include "rr/serial_handles.h"
#include "game/gfx/gfxotag.h"
#include "game/gfx/drawbuf.h"

typedef struct { int a, b, c, d; } w3g2V;

struct w3g2p {
    int pad0[4]; int f10, f14, f18, f1C; char pad1[0x20]; int f40; int pad44; int f48; int pad4c; int f50, f54, f58, f5C;
    char pad2[0x108 - 0x60]; int f108; char pad3[0x1AA - 0x10C]; short s1AA, s1AC; unsigned short u1AE; char pad4[6]; unsigned short u1B6;
    char pad5[0x1E4 - 0x1B8]; unsigned short u1E4;
};

typedef struct {
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} Glyph4;

typedef struct { char pad[0x50]; short f; char pad2[0xA]; } S1;

typedef struct {
    short x, y;
} V;

typedef struct 
{
  char pad[0x84];
  int a84;
  int a88;
} T38;

extern int func_8002E828();
extern int func_8006E1AC();
extern short D_8009F694[];
extern short D_8009F6A0[];
extern struct 
{
  char pad[0x178];
  int v;
  char pad2[0x320 - 0x17C];
} D_800ABEF8[];
extern struct 
{
  short a;
  unsigned short b;
  char pad[0x58];
} D_800F3A38[];
extern short func_80025FBC();
extern int BuildRotMatrixZ();
extern int BuildRotMatrixY();
extern int BuildRotMatrixX();
extern int MulMatrix();
extern int ApplyMatrix();
extern int D_8010718C;
extern s32 func_8001FC28();
extern s32 func_8001FCC8();
extern s32 func_8001FF84();
extern Glyph4 D_8009F6AC[];
extern u16 D_8009F6CC[];
extern u16 D_8009F6D0[];
extern u8 D_8009F6D8[];
extern u8 D_8009F6DA;
extern u8 D_8009F6DB;
extern s32 D_800FF6B0;
extern int func_8004F2D8();
extern int D_800ABEC0;
extern short D_800AC5D0;
extern int func_8004BAD4();
extern int PlaySfx();
extern unsigned char D_8009F6DC[];
extern signed char D_800F2FDC;
extern int func_8003660C();
extern int D_8009F6E4[];
extern char D_800AD4D8[];
extern int D_800F2A68;
extern int D_800F2EB8;
extern short D_800F3A26;
extern int D_800F4C64;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int GameQueueSprite();
extern int func_8002071C();
extern unsigned char D_8009F71C[][4];
extern int D_800105C8[];
extern int D_800F2AAC;
extern int D_800F2AA8;
extern s16 D_800FF688;
extern int D_8009F728;
extern signed char D_800ABEBC[];
extern signed char D_800ABEBE;
extern int D_800AC5A4;
extern short D_800AD508;
extern int D_800F3A64;

void func_80033A74(int a);
void func_80033BE4(struct w3g2p *p);
void func_80033F3C(s32 arg0, s32 arg1);
void func_80034630(int a0);
void func_800347BC(void);
void func_80034844(int a0);
void func_800348E8(int arg0);
void func_800349F4(int a0, int a1);
int func_80034E70(int a0, unsigned int a1);
void func_80034F80(void);
int func_80034FC0(void);
int func_80035024(int a0);
int func_800353E4(void);

void func_80033A74(int a)
{
  int b1 = (int) D_8009F6A0;
  int o = a * 6;
  short *p = (short *) (o + b1);
  short *q;
  int i;
  /* ASM_BARRIER: pin scratchpad base to $a0 like the original */
  register int *g asm("$4");
  g = (int *) SCRATCHPAD;
  g[2] = p[0];
  g[3] = p[1];
  g[4] = p[2];
  q = (short *) (o + ((int) D_8009F694));
  g[6] = q[0];
  g[7] = q[1];
  g[8] = q[2];
  func_8006E1AC(g, q);
  {
    int t;
    t = SCRATCH_ROT_MATRIX;
    /* ASM_BARRIER: forces the SCRATCH_ROT_MATRIX li to be scheduled before the li a0,2 (original delay-slot layout) */
    asm volatile("" : "=r"(t) : "0"(t));
    func_8002E828(2, t);
  }
  ;
  if (D_800F3A38[D_800AC5D4 & 1].b & 4)
  {
    D_800ABEF8[D_800AC5D4 & 1].v += 0xC0;
  }
  else
    if (D_800F3A38[D_800AC5D4 & 1].b & 8)
  {
    D_800ABEF8[D_800AC5D4 & 1].v -= 0xC0;
  }
  i = D_800ABEF8[D_800AC5D4 & 1].v;
  if (i > 0xF00)
  {
    D_800ABEF8[D_800AC5D4 & 1].v = 0xF00;
  }
  else
    if (i < (-0xF00))
  {
    D_800ABEF8[D_800AC5D4 & 1].v = -0xF00;
  }
}

void func_80033BE4(struct w3g2p *p) {
    struct { char a[0x20]; char b[0x20]; short v[3]; short pad; int w[3]; } s;
    int *g = (int *)SCRATCHPAD;
    int r;
    short t;
    unsigned short a1, a2;
    D_8010718C = 1;
    if (p->u1E4 & 0x100) {
        BuildRotMatrixY(s.a, 0x800 - p->f54);
        BuildRotMatrixX(s.b, p->f50);
        MulMatrix(s.a, s.b);
        BuildRotMatrixZ(s.b, p->f58);
        MulMatrix2(s.a, s.b);
        s.v[0] = 0;
        s.v[1] = -0x64;
        if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x8C;
        s.v[2] = 0;
        ApplyMatrix(s.b, s.v, s.w);
        *(w3g2V *)(SCRATCHPAD + 0x8) = *(w3g2V *)&p->f10;
        g[2] -= p->f40;
        g[4] -= p->f48;
        *(w3g2V *)(SCRATCHPAD + 0x18) = *(w3g2V *)&p->f50;
        g[6] += p->s1AC * 2;
        a1 = p->u1B6;
        a2 = p->u1AE;
        g[2] += s.w[0];
        g[3] += s.w[1];
        g[4] += s.w[2];
        g[8] += ((short)a1 / 2) + ((short)a2 / 2);
        return;
    }
    s.v[0] = func_80025FBC(g[6], p->f50) / 8;
    r = func_80025FBC(g[7], p->f54);
    t = r / 6;
    g[6] += s.v[0];
    s.v[1] = t;
    g[7] += t;
    BuildRotMatrixY(s.a, 0x800 - g[7], g[7]);
    BuildRotMatrixX(s.b, g[6]);
    MulMatrix(s.a, s.b);
    BuildRotMatrixZ(s.b, g[8]);
    MulMatrix2(s.a, s.b);
    s.v[0] = 0;
    s.v[1] = -0xFA;
    if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x122;
    s.v[2] = 0x2D0;
    ApplyMatrix(s.b, s.v, s.w);
    *(w3g2V *)(SCRATCHPAD + 0x8) = *(w3g2V *)&p->f10;
    g[8] = p->f58 - p->s1AA;
    g[2] += s.w[0];
    g[3] += s.w[1];
    g[4] += s.w[2];
}

void func_80033F3C(s32 arg0, s32 arg1) {
    Glyph4 sp20;
    s32 s0;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 s5;
    s32 s7;
    s32 fp;
    s32 a1;

    if (D_800FF6B0 != 0 && (arg1 - 0x4B) < 0xAF) {
        s5 = arg1 % 25;
        s3 = (arg1 - 0x4B) / 25;
        if ((D_800ACC14 + 0x70) == arg0) {
            s4 = 0;
            if (s3 == 6) {
                s4 = -(s5 << 7) / 25;
            }
        } else {
            *(s32 *)SCRATCHPAD = func_8001FC28(arg0, *(s32 *)SCRATCHPAD, 0, 0, 0x140, 0x100);
            s4 = 0x40;
        }
        a1 = func_8001FCC8(arg0, *(s32 *)SCRATCHPAD, 5);
        if (s3 == 1) {
            fp = ((s5 << 6) / 25) + 0x40;
        } else if (s3 >= 4) {
            fp = 0xA4;
            if (s3 >= 5) {
                fp = 0x80;
            }
        } else {
            fp = 0x80;
            if (s3 <= 0) {
                fp = 1;
            }
        }
        if (D_800F4D44 == 1) {
            if (s3 >= 4) {
                s7 = ((arg1 >> 2) & 1) + 2;
                sp20 = D_8009F6AC[7];
                a1 = func_8001FF84(arg0, a1, D_8009F6DB + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
            } else if (s3 > 0) {
                s2 = 0;
                for (s1 = 0; s1 < 0x10;) {
                    s32 b, c;

                    if (s1 > s5 - 7) {
                        break;
                    }
                    b = s1 & 1;
                    c = b << 1;
                    a1 = func_8001FF84(arg0, a1, s2 + 0x78, s4 + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    s1++;
                    s2 += 1 << (c + 1);
                }
                s7 = 0;
                sp20 = D_8009F6AC[5];
                sp20 = D_8009F6AC[6];
                sp20.b0 += (s3 - 1) * 12;
                a1 = func_8001FF84(arg0, a1, D_8009F6DA + 0x60, s4 + 0x39, &sp20, D_8009F6D0[0], 0);
            }
        } else {
            if (s3 == 0) {
                s2 = 0;
                for (s1 = 0; s1 < 0x10;) {
                    s32 b, c;

                    if (s1 > s5 - 6) {
                        break;
                    }
                    b = s1 & 1;
                    c = b << 1;
                    a1 = func_8001FF84(arg0, a1, s2 + 0x78, s4 + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    s1++;
                    s2 += 1 << (c + 1);
                }
            } else if (s3 >= 4) {
                s7 = ((arg1 >> 2) & 1) + 2;
                sp20 = D_8009F6AC[5];
                sp20.b0 += 0x60;
                for (s1 = 0; s1 < 3; s1++) {
                    if (s1 == 2) {
                        sp20 = D_8009F6AC[7];
                        s1 = 3;
                    }
                    a1 = func_8001FF84(arg0, a1, D_8009F6D8[s1] + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
                }
            } else {
                s7 = 0;
                sp20 = D_8009F6AC[5];
                for (s1 = 0; s1 < 3; s1++) {
                    if (s1 == 0) {
                        sp20.b0 += ((s5 * 9) / 25) * 12;
                    } else if (s1 == 1) {
                        sp20.b0 = D_8009F6AC[5].b0 + s3 * 0x18;
                    } else {
                        sp20 = D_8009F6AC[6];
                        sp20.b0 += (s3 - 1) * 12;
                    }
                    a1 = func_8001FF84(arg0, a1, D_8009F6D8[s1] + 0x60, s4 + 0x39, &sp20, D_8009F6D0[s7], 0);
                }
            }
        }
        if (s3 > 0) {
            s0 = s4 + 0x39;
            s2 = 0;
            for (s1 = 0; s1 < 10; s1++) {
                a1 = func_8001FF84(arg0, a1, s2 + 0x78, s0, &D_8009F6AC[4], D_8009F6D0[(u32)(s1 - 3) < 4 ? 3 : s7], 0);
                s2 += 8;
            }
        }
        a1 = func_8001FF84(arg0, a1, 0x68, s4 + 0x1A, &D_8009F6AC[1], D_8009F6CC[s3 >= 4], (fp << 1) | 1);
        a1 = func_8001FF84(arg0, a1, 0x60, s4, &D_8009F6AC[0], 0x797C, 0);
        *(s32 *)SCRATCHPAD = func_8001FCC8(arg0, a1, 0x3F);
        if (arg0 == D_800ACC14 + 0xB70) {
            s32 y;
            if (s3 == 6) {
                fp = ((0x19 - s5) * 0x18) / 25;
            } else {
                fp = 0x18;
            }
            y = (0x18 - fp) / 2 + 0x34;
            *(s32 *)SCRATCHPAD = func_8001FC28(arg0, *(s32 *)SCRATCHPAD, 0x70, s4 + y, 0x60, fp);
        }
    }
}

void func_80034630(int a0) {
    if (D_800FF6B0 != 0 && a0 < 0x115) {
        if (a0 == 0xC3) {
            D_800AC5D0 = 0x1E;
        }
        if (a0 == 0x65) {
            if (!(D_800ABEC0 & 1)) {
                func_8004F2D8(0, 0);
                D_800ABEC0 |= 1;
                return;
            }
        }
        if (a0 == 0x7E) {
            if (!(D_800ABEC0 & 2)) {
                func_8004F2D8(1, 0);
                D_800ABEC0 |= 2;
                return;
            }
        }
        if (a0 == 0x97) {
            func_8004F2D8(2, 0);
            return;
        }
        if (a0 == 0xB0) {
            if (!(D_800ABEC0 & 4)) {
                func_8004F2D8(3, 0);
                D_800ABEC0 |= 4;
                return;
            }
        }
        if (a0 == 0xE2) {
            if (!(D_800ABEC0 & 8)) {
                func_8004F2D8(0x17, 0);
                D_800ABEC0 |= 8;
                return;
            }
        }
        if (a0 == 0x114) {
            if (!(D_800ABEC0 & 0x10)) {
                func_8004F2D8(0x18, 0);
                D_800ABEC0 |= 0x10;
            }
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(Car (*)[])&D_800ABEF8)
#define D_800F3A38 (*(S1 (*)[])&D_800F3A38)

void func_800347BC(void) {
    int i;
    func_8004B864(0);
    func_8004B864(1);
    for (i = 0; i < 2; i++) {
        if (D_800ABEF8[i].unk1E4 != 0) {
            D_800F3A38[i].f = 1;
            func_8004BAD4(i, 0, 0xF);
        }
    }
}

#undef D_800ABEF8
#undef D_800F3A38

void func_80034844(int a0) {
    if (D_800F3A40 & 0x1000) {
        if (D_800F2FDC > 0) {
            D_800F2FDC = D_800F2FDC - 1;
            PlaySfx(0);
        }
    }
    if (D_800F3A40 & 0x4000) {
        if (D_800F2FDC < D_8009F6DC[a0]) {
            D_800F2FDC = D_800F2FDC + 1;
            PlaySfx(0);
        }
    }
    func_800347BC();
}

void func_800348E8(int arg0) {
    int *t;
    int b;
    if (D_800F2FDC == 0) {
        D_800AC5D0 = 0x1E;
        return;
    }
    D_800AD4D8[0x19] = 0;
    D_800F4C64 = -1;
    if (arg0 == 1) {
        D_800AD4D8[0x19] = 9;
        D_800FF6B0 = 6;
        D_800F2A68 = -D_800F2EB8;
        if (D_800F2FDC == 2) {
            D_800F4D8A = 0;
            D_800F4C6A = D_8009F6E4[3] >> 8;
            D_800F3A26 = *(unsigned char *)&D_8009F6E4[3];
            return;
        }
        func_8003660C(D_800AD4D8, D_800F2FDC);
        return;
    }
    b = (int)D_8009F6E4;
    D_800FF6B0 = 6;
    t = (int *)((D_800F2FDC - 1) * 4 + arg0 * 8 + b);
    D_800F4C6A = *t >> 8;
    {
        int v = *(unsigned char *)t;
        D_800F4D8A = 0;
        D_800F3A26 = v;
    }
}

void func_800349F4(int a0, int a1) {
    V v[4];
    unsigned char rgb[4];
    int p;
    int i, j;
    unsigned char (*base)[4];
    unsigned char *t;

    p = GameQueueSprite(a0, *(int *)SCRATCHPAD, 0x90, 0x5C, 0x1C, 8, 0x60, 0xA0, 0x7FF8);
    p = GameQueueSprite(a0, p, 0x90, 0x6A, 0x20, 8, 0xC4, 0xA0, 0x7FF8);
    if (D_800F4D44 == 1) {
        p = GameQueueSprite(a0, p, 0x90, 0x74, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    } else {
        p = GameQueueSprite(a0, p, 0x90, 0x74, 0x24, 8, 0x9C, 0xA0, 0x7FF8);
        p = GameQueueSprite(a0, p, 0x90, 0x7E, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    }
    p = GameQueueSprite(a0, p, 0x88, a1 * 10 + 0x6A, 8, 8, 0xE4, 0xA0, 0x7FF8);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 4; j++) {
            int kx = ((j & 1) << 5) + 0x70;
            v[j].x = ((i % 3) << 5) + kx;
            {
                int ky = ((j >> 1) << 4) + 0x56;
                v[j].y = ((i / 3) << 4) + ky;
            }
        }
        rgb[0] = 0xFF;
        rgb[2] = 0x20;
        rgb[1] = 0x20;
        j = i / 3;
        j = j == 1;
        base = D_8009F71C;
        t = base[j];
        p = func_8002071C(a0, p, v[t[0]], v[t[1]], v[t[2]], rgb, 1);
        rgb[0] = 0xFF;
        rgb[2] = 0xFF;
        rgb[1] = 0xFF;
        p = func_8002071C(a0, p, v[t[1]], v[t[2]], v[t[3]], rgb, 1);
    }
    *(int *)SCRATCHPAD = func_8001FCC8(a0, p, 5);

    for (j = 0; j < 4; j++) {
        if (j == 1 || j == 2) {
            v[j].x = 0xD0;
        } else {
            v[j].x = 0x70;
        }
        if (j >= 2) {
            v[j].y = 0x86;
        } else {
            v[j].y = 0x56;
        }
    }

    for (i = 0; i < 3; i++) {
        j = 0x40;
        func_80022694(a0, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y, 0x100, 0, j, j, j, -1);
    }
}

void func_80034E14(void) {
    func_8009AEAC(1, 1, 1);
}

int func_80034E3C(void) {
    return (unsigned)(((unsigned)func_8009AEAC(0, 0, 0) >> 7) & 3) - 1 < 2;
}

int func_80034E70(int a0, unsigned int a1) {
    return a1 <= 0x1869E;
}

void func_80034E84(void) {
    func_8009AEAC(1, 3, 0x7E900);
    func_8009AEAC(1, 4, 1);
    D_800F2AAC = func_80096F24(D_800105C8, 2);
    D_800F2AA8 = func_80096F24(D_800105C8, 1);
    func_8009AEAC(4, 0, (int)func_80034E70);
    func_8009AEAC(2, 0, 0);
}

void func_80034F18(void) {
    func_8009AEAC(2, 3, 0);
    func_8009AEAC(2, 0, 0);
    func_80096F64(D_800F2AAC);
    func_80096F64(D_800F2AA8);
    func_8009AEAC(4, 0, 0);
}

void func_80034F80(void) {
    if (D_800FF688 == 2) {
        D_800FF688 = 3;
        func_8009AEAC(1, 1, 2);
    }
}

int func_80034FC0(void) {
    if (D_800FF688 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 0x2D;
        D_800F4D8A = 0;
        func_8009AEAC(1, 1, 0);
        return 1;
    }
    return 0;
}

/* legacy typing of shared globals in this part of the unit */
#define func_8009AEAC ((unsigned int (*)())func_8009AEAC)
#define D_800F3A38 (*(T38 (*))&D_800F3A38)
#define D_800FF688 (*(unsigned short (*))&D_800FF688)

int func_80035024(int a0)
{
  int r = 0;
  long new_var2;
  int st;
  int mm;
  signed char *t;
  signed char *bp;
  short z;
  int xx;
  int q3;
  T38 *e;
  signed char *new_var;
  unsigned int k = (func_8009AEAC(0, 0, 0) >> 7) & 3;
  short new_var3;
  st = (short) (D_800FF688 + 1);
  switch (st)
  {
    case 1:
      if (func_8009AEAC(3, 1, 0) != 0)
    {
      func_8009AEAC(1, 1, 2);
      if (a0 == 0)
      {
        D_800AD508 = 0;
        D_800ABEBE = 1;
        D_8009F728 = 3;
        D_800FF688 = 10;
      }
      else
      {
        goto s2;
      }
      break;
    }
      func_8009AEAC(1, 1, 2);
      if (a0 == 0)
    {
      D_800AD508 = 1;
      D_800ABEBC[0] = 1;
      D_800FF688 = 1;
    }
    else
      if (k != 1)
    {
      D_800FF688 = 1;
    }
    else
    {
      goto s2;
    }
      break;

    case 11:
      if (D_8009F728 > 0)
    {
      func_8009AEAC(1, 1, 2);
      D_8009F728 -= 1;
      if (k == 1)
      {
        D_800FF688 = 2;
        D_8009F728 = 0;
      }
    }
    else
    {
      goto s2;
    }
      break;
      s2:
    D_800FF688 = 2;

      break;

    case 2:
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
    else
      if (k == 2)
    {
      D_800FF688 = k;
      D_8009F728 = 4;
    }
      break;

    case 3:
      func_8009AEAC(1, 1, 1);
      if (D_8009F728 > 0)
    {
      if (a0 == 0)
      {
        D_8009F728 -= 1;
        break;
      }
    }
    else
      if (a0 == 0)
    {
      new_var = D_800ABEBC;
      xx = D_800AD508;
      z = 3;
      new_var2 = xx * 2;
      bp = new_var;
      t = &bp[z - new_var2];
      if ((*t) == 0)
      {
        e = &D_800F3A38;
        new_var3 = z;
        if (e->a84 != 0)
        {
          PlaySfx(1);
          bp[new_var3 - (D_800AD508 * 2)] = 1;
        }
        else
          if (e->a88 != 0)
        {
          *t = -1;
        }
        if (D_800F3A64 != 0)
        {
          D_800ABEBC[1 - (q3 = D_800AD508)] = -1;
          func_8009AEAC(1, 1, 0, q3);
        }
      }
    }
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
    else
      if (a0 == 0)
    {
      int v = D_800AC5A4 - 1;
      D_800AC5A4 = v;
      if (v <= 0)
      {
        D_800FF688 = 4;
        func_8009AEAC(1, 1, 2);
      }
      else
        if (D_800ABEBC[3 - (D_800AD508 * 2)] != 0)
      {
        D_800FF688 = 3;
        func_8009AEAC(1, 1, 2);
      }
    }
      break;

    case 4:
      if (a0 == 0)
    {
      if (D_800AC5A4 > 0)
      {
        D_800AC5A4 -= 1;
      }
    }
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
    else
      if (k != 1)
    {
      goto c5;
    }
      break;

    case 5:
      c5:
    if (a0 != 2)
    {
      D_800FF688 = 0;
    }

      r = 1;
      break;

    case 0:
      r = -1;
      break;
      m1:
    *((short *) (&D_800FF688)) = -1;

      break;

  }

  return r;
}

#undef func_8009AEAC
#undef D_800F3A38
#undef D_800FF688

/* legacy typing of shared globals in this part of the unit */
#define func_8009AEAC ((unsigned (*)())func_8009AEAC)

int func_800353E4(void) {
    int r = 0;
    int t = (func_8009AEAC(0, 0, 0) >> 7) & 3;
    if (t == 0 || t == 3) {
        D_800FF688 = -1;
        r = -1;
    } else if (t == 1) {
        D_800FF688 = t;
    } else if (t == 2) {
        if (D_800FF688 == 3) r = 1;
    }
    return r;
}

#undef func_8009AEAC
