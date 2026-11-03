#include "types.h"

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
extern int D_800AC5D4;
extern struct 
{
  short a;
  unsigned short b;
  char pad[0x58];
} D_800F3A38[];
extern short func_80025FBC();
extern int func_8006E030(), func_8006E09C(), func_8006E108(), func_80090CA4(), func_80090DB4(), func_80090EC4();
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
extern s32 D_800ACC14;
extern s16 D_800F4D44;
extern s32 D_800FF6B0;
extern int func_8004F2D8();
extern int D_800ABEC0;
extern short D_800AC5D0;

void func_80033A74(int a);
void func_80033BE4(struct w3g2p *p);
void func_80033F3C(s32 arg0, s32 arg1);
void func_80034630(int a0);

void func_80033A74(int a)
{
  int b1 = (int) D_8009F6A0;
  int o = a * 6;
  short *p = (short *) (o + b1);
  short *q;
  int i;
  /* ASM_BARRIER: pin scratchpad base to $a0 like the original */
  register int *g asm("$4");
  g = (int *) 0x1F800000;
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
    t = 0x1F800028;
    /* ASM_BARRIER: forces the 0x1F800028 li to be scheduled before the li a0,2 (original delay-slot layout) */
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
    int *g = (int *)0x1F800000;
    int r;
    short t;
    unsigned short a1, a2;
    D_8010718C = 1;
    if (p->u1E4 & 0x100) {
        func_8006E09C(s.a, 0x800 - p->f54);
        func_8006E108(s.b, p->f50);
        func_80090CA4(s.a, s.b);
        func_8006E030(s.b, p->f58);
        func_80090DB4(s.a, s.b);
        s.v[0] = 0;
        s.v[1] = -0x64;
        if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x8C;
        s.v[2] = 0;
        func_80090EC4(s.b, s.v, s.w);
        *(w3g2V *)0x1F800008 = *(w3g2V *)&p->f10;
        g[2] -= p->f40;
        g[4] -= p->f48;
        *(w3g2V *)0x1F800018 = *(w3g2V *)&p->f50;
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
    func_8006E09C(s.a, 0x800 - g[7], g[7]);
    func_8006E108(s.b, g[6]);
    func_80090CA4(s.a, s.b);
    func_8006E030(s.b, g[8]);
    func_80090DB4(s.a, s.b);
    s.v[0] = 0;
    s.v[1] = -0xFA;
    if (p->f108 < 0x28) s.v[1] = (unsigned short)p->f108 - 0x122;
    s.v[2] = 0x2D0;
    func_80090EC4(s.b, s.v, s.w);
    *(w3g2V *)0x1F800008 = *(w3g2V *)&p->f10;
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
            *(s32 *)0x1F800000 = func_8001FC28(arg0, *(s32 *)0x1F800000, 0, 0, 0x140, 0x100);
            s4 = 0x40;
        }
        a1 = func_8001FCC8(arg0, *(s32 *)0x1F800000, 5);
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
        *(s32 *)0x1F800000 = func_8001FCC8(arg0, a1, 0x3F);
        if (arg0 == D_800ACC14 + 0xB70) {
            s32 y;
            if (s3 == 6) {
                fp = ((0x19 - s5) * 0x18) / 25;
            } else {
                fp = 0x18;
            }
            y = (0x18 - fp) / 2 + 0x34;
            *(s32 *)0x1F800000 = func_8001FC28(arg0, *(s32 *)0x1F800000, 0x70, s4 + y, 0x60, fp);
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
