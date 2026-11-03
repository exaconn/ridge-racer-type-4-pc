/* RODATA 0x80010420 0x20 */
#include "game/gfx/scene1.h"
#include "game/sound/sndcmd.h"
#include "game/ui/hudmap2.h"
#include "game/gfx/gfxdraw.h"
#include "game/sound/sndvab.h"
#include "game/system/pad.h"
#include "game/system/pad3.h"
#include "game/race/racesave.h"
typedef struct { int w[200]; } B800;

typedef struct {
    int x;
} __attribute__((packed)) U4;

typedef struct {
    unsigned char b0;
    unsigned char b1;
    unsigned short h2;
    unsigned char pad4[4];
    unsigned short h8;
    unsigned char padA[6];
    int w10;
    unsigned char pad14[0xE];
    short s22;
    short s24;
    short s26;
    unsigned char pad28[0x14];
    short s3C;
    unsigned char pad3E[0x1E];
} S5C;

typedef struct {
    short h0;
    short h2;
    short h4;
    short h6;
    short h8;
    short hA;
    short hC;
    short hE;
    short h10;
    short h12;
    short h14;
    short h16;
    short h18;
    short h1A;
    short h1C;
    short h1E;
    short h20;
    short h22;
    short h24;
    short h26;
    short h28;
    short h2A;
    short h2C;
    short h2E;
} S30;

typedef struct {
    short h0;
    short s12;
    short s14;
    short pad[5];
} S10_sub;

typedef struct {
    char pad[0x10];
    S10_sub sub[2];
} S_AD528;

typedef struct {
    char pad[0xB0];
    int sB0;
} SA0;

extern int D_800AD504;
extern int D_800ADAC8;
extern int D_800FF6B0;
extern short D_800AC59C;
extern B800 *D_800AC570[];
extern short D_8009FFB8[];
extern int D_800AC5A8;
extern int D_800ACBE0[];
extern int D_800ACBF0[];
extern short D_800AD508;
extern U4 *D_800AD510[];
extern S_AD528 D_800AD528;
extern unsigned char D_800ADAD8[][4];
extern S30 D_800F2F78[];
extern S5C D_800F3A38[];
extern int D_800F3AF0[];
extern short D_800F4D44;
extern int D_80107188;
extern short D_800AC5D0;
extern int D_800F2EB8;
extern int D_800F3A30;
extern int D_800F3A60;
extern int func_8004FC18();
extern int func_800207D4();
extern int func_800293D0();
extern int func_80077D90();
extern int func_80014A8C();
extern int D_800ABEF8[];
extern int D_800ACC14;
extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;
extern int D_8010BFB0;
extern int func_80020074();
extern int func_8002E0D8();
extern int func_8003660C();
extern int func_80036FD4();
extern int func_80037D88();
extern int func_8005007C();
extern int func_80052A30();
extern int func_8006E1AC();
extern void func_8006ECE8(s32);
extern int func_8006EF8C();
extern int func_800735D8();
extern int func_800747EC();
extern int func_80090424();
extern int D_800ABEB0;
extern int D_800F2A64;
extern short D_800F2AB4[];
extern int D_800F2DE4;
extern short D_800F4C70[];
extern struct { char pad[0x1E4]; short f1e4; } *D_800FFC20[];

void func_800295B8(int unused);
void func_80029660(char *arg0, int arg1);
void func_80029E0C();
void func_80029F34();
void func_8002A008(void);

/* legacy typing of shared globals in this part of the unit */
#define D_800FFC20 (*(B800 *(*)[])&D_800FFC20)

/* Callers explicitly supply this legacy argument; the body ignores it. */
void func_800295B8(int unused)
{
  int i;
  if (D_800AD504 == 0)
  {
    D_800ADAC8 = D_800FF6B0;
    for (i = 0; i < D_800AC59C; i++)
      *D_800AC570[i] = *D_800FFC20[i];
    D_800AD504 = 1;
  }
}

#undef D_800FFC20

void func_80029660(char *arg0, int arg1) {
    SA0 *a0;
    short t3;
    short t5;
    short t1;
    short a1;
    register int t2 asm("t2"); /* ASM_BARRIER: pin t2 for idx copy */
    int idx;
    int b;
    int f;
    int mode;

    a0 = (SA0 *)(arg0 + 0x1F4);
    if (D_800FF6B0 != 0) {
        if (D_800AD504 == 2) {
            if (D_800AC5A8 < D_80107188) {
                *(U4 *)D_800ADAD8[arg1] = D_800AD510[arg1][D_800AC5A8];
                if (D_800F4D44 == 4) {
                    if (arg1 == 3) {
                        D_800AC5A8 += 1;
                    }
                } else if (arg1 == 1 || D_800F4D44 < 2 || D_800F4D44 == 6) {
                    D_800AC5A8 += 1;
                }
            }
        } else {
            t3 = 0;
            t2 = idx = arg1 & 1;
            if (D_800FF6B0 < 4) {
                mode = D_800F3A38[idx].w10;
                if (mode == 0) {
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h4) {
                        t1 = 0xFF;
                    }
                    b = (D_800F3A38[idx].h2 & D_800F2F78[idx].h6) != 0;
                    {
                        register int f1 asm("v1"); /* ASM_BARRIER: pin f1 to v1 */
                        f1 = (D_800F3A38[idx].h2 & D_800F2F78[idx].h0) != 0;
                        if ((D_800F3A38[idx].h2 & D_800F2F78[idx].h2) > 0) {
                            f1 |= 2;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h8) > 0) {
                            f1 |= 4;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].hA) > 0) {
                            f1 |= 8;
                        }
                        f = f1;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].hE) > 0) {
                            f |= 0x10;
                        }
                    }
                    if (D_800F3A38[idx].b1 == 0x73) {
                        t5 = (D_800F3A38[idx].s3C * 0x7F) / 128;
                        t3 = f | 0x80;
                    } else {
                        t5 = 0;
                        t3 = f | 0x20;
                    }
                    a0->sB0 += b * 4;
                    if (b == 0) {
                        a0->sB0 = 0;
                    }
                    if (a0->sB0 >= 0x74) {
                        a0->sB0 = 0x73;
                    }
                    a1 = a0->sB0 + (b * 140);
                } else if (mode == 1) {
                    t5 = (D_800F3A38[idx].s3C * 0x7F) / D_8009FFB8[D_800AD528.sub[idx].s12];
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h14) {
                        t1 = 0xFF;
                    }
                    a1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h16) {
                        a1 = 0xFF;
                    }
                    {
                        int fm1;
                        fm1 = ((D_800F3A38[idx].h8 & D_800F2F78[idx].h18) != 0) * 4;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h1A) > 0) {
                            fm1 |= 8;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h1E) > 0) {
                            fm1 |= 0x10;
                        }
                        t3 = fm1 | 0x40;
                    }
                    switch (D_800AD528.sub[idx].s14) {
                    case 0:
                    case 5: {
                        int i0;
                        t1 = (D_800F3A38[({ i0 = t2; __asm__("" : "+r"(i0)); i0; })].s22 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i0].s24 * 0xFF) / 106;
                        break;
                    }
                    case 1:
                    case 6: {
                        int i1;
                        t1 = (D_800F3A38[({ i1 = t2; __asm__("" : "+r"(i1)); i1; })].s24 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i1].s22 * 0xFF) / 106;
                        break;
                    }
                    case 2: {
                        int i2;
                        a1 = (D_800F3A38[({ i2 = t2; __asm__("" : "+r"(i2)); i2; })].s26 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        break;
                    }
                    case 3: {
                        int i3;
                        t1 = (D_800F3A38[({ i3 = t2; __asm__("" : "+r"(i3)); i3; })].s24 * 0xFF) / 106; /* ASM_BARRIER: keep index copy */
                        a1 = (D_800F3A38[i3].s26 * 0xFF) / 106;
                        break;
                    }
                    case 4:
                    case 7:
                        break;
                    }
                } else if (mode == 2) {
                    t5 = (D_800F3A38[idx].s3C * 0x7F) / 128;
                    t1 = 0;
                    if (D_800F3A38[idx].h2 & D_800F2F78[idx].h24) {
                        t1 = 0xFF;
                    }
                    b = (D_800F3A38[idx].h2 & D_800F2F78[idx].h26) != 0;
                    a0->sB0 += b * 4;
                    if (b == 0) {
                        a0->sB0 = 0;
                    }
                    if (a0->sB0 >= 0x74) {
                        a0->sB0 = 0x73;
                    }
                    a1 = a0->sB0 + (b * 140);
                    {
                        register unsigned short u2 asm("a0"); /* ASM_BARRIER: pin h2 load to a0 */
                        register int tand asm("v1"); /* ASM_BARRIER: pin h20 mask to v1 */
                        int fb, fc;
                        u2 = D_800F3A38[idx].h2;
                        tand = u2 & D_800F2F78[idx].h20;
                        f = tand != 0;
                        if ((u2 & D_800F2F78[idx].h22) > 0) {
                            f |= 2;
                        }
                        fb = f;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h28) > 0) {
                            fb |= 4;
                        }
                        fc = fb;
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h2A) > 0) {
                            fc |= 8;
                        }
                        if ((D_800F3A38[idx].h8 & D_800F2F78[idx].h2E) > 0) {
                            fc |= 0x10;
                        }
                        t3 = fc | 0x60;
                    }
                } else {
                    a1 = 0;
                    t1 = 0;
                }
            } else {
                t1 = 0;
                a1 = 0;
            }
            D_800ADAD8[arg1][0] = t5;
            D_800ADAD8[arg1][1] = t1;
            D_800ADAD8[arg1][2] = a1;
            D_800ADAD8[arg1][3] = t3;
            if (D_800F4D44 == 4) {
                if (D_800AD508 != 0) {
                    if (arg1 >= 2) {
                        *(int *)D_800ADAD8[arg1] = D_800ACBE0[t2];
                    } else {
                        D_800F3AF0[t2] = *(int *)D_800ADAD8[arg1];
                        *(int *)D_800ADAD8[arg1] = D_800ACBF0[t2];
                    }
                } else {
                    if (arg1 < 2) {
                        *(int *)D_800ADAD8[arg1] = D_800ACBE0[t2];
                    } else {
                        D_800F3AF0[t2] = *(int *)D_800ADAD8[arg1];
                        *(int *)D_800ADAD8[arg1] = D_800ACBF0[t2];
                    }
                }
                __asm__ volatile("" : : "r"(t2)); /* ASM_BARRIER: keep t2 live so t2<<2 allocates v1 */
                if (D_800AD504 == 1) {
                    if (D_800AC5A8 < D_80107188) {
                        D_800AD510[arg1][D_800AC5A8] = *(U4 *)D_800ADAD8[arg1];
                        if (arg1 == 3) {
                            D_800AC5A8 += 1;
                        }
                    }
                }
            } else if (D_800AD504 == 1) {
                if (D_800AC5A8 < D_80107188) {
                    D_800AD510[arg1][D_800AC5A8] = *(U4 *)D_800ADAD8[arg1];
                    if (arg1 == 1 || D_800F4D44 < 2 || D_800F4D44 == 6) {
                        D_800AC5A8 += 1;
                    }
                }
            }
        }
    }
}

void func_80029E0C(a0) char *a0; {
    char *a2 = a0 + 0x1F4;
    int a1 = *(short *)(a2 + 0xB6);
    if (*(int *)(a0 + 0x188) + *(int *)(a0 + 0x184) >= a1 * D_800F2EB8) {
        unsigned short u = *(unsigned short *)(a2 + 0xB6);
        if (D_800F3A30 >= a1) {
            *(short *)(a2 + 0xB6) = u + 1;
            if (D_800F3A30 < (short)(u + 1)) {
                if (D_800FF6B0 < 4) {
                    if (D_800AC5D0 == -1) {
                        D_800FF6B0 = 4;
                        D_800AC5D0 = 0x4B;
                        func_8004FC18(0, 0x47);
                    }
                }
            }
        }
    }
    if (D_800AC5A8 >= D_80107188 - 0x4B && D_800AC5D0 == -1) {
        D_800AC5D0 = 0x25;
        func_8004FC18(0, 0x21);
    } else if (D_800F3A60 != 0) {
        if (D_800AC5D0 == -1) {
            D_800AC5D0 = 0x25;
            func_8004FC18(0, 0x21);
        }
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D44 (*(unsigned short (*))&D_800F4D44)

void func_80029F34() {
    func_8004F720();
    func_800590DC();
    func_800207D4(D_800ACC14 + 0x70, 0xFF, 0x25);
    func_800293D0(0, 2);
    D_800FF6B0 = D_800ADAC8;
    func_80077D90(1);
    func_80014A8C(10);
    D_8010BFB0 = 0;
    if ((unsigned)(D_800F4D44 - 2) < 2U) {
        if (D_800ABEF8[0x184 / 4] < D_800ABEF8[0x4A4 / 4]) D_8010BFB0 = 1;
    }
    D_800AC5D0 = -1;
    D_800F4C6A = 1;
    D_800F3A26 = 0x16;
    D_800F4D8A = 0;
}

#undef D_800F4D44

/* legacy typing of shared globals in this part of the unit */
#define D_800ABEF8 (*(struct { char pad[0x184]; int f184; char pad2[0x320 - 0x188]; } (*)[])&D_800ABEF8)

void func_8002A008(void) {
    int i;
    short t;
    struct { char pad[0x1E4]; short f1e4; } **pp;
    D_800F2DE4 += 1;
    if (D_800AC5A8 < 0x1A) {
        func_800207D4(D_800ACC14 + 0x70, (0x19 - D_800AC5A8) * 10, 0x25);
        if (D_800AC5A8 == 10) {
            func_80052A30(0);
        }
    } else {
        func_800556B4();
    }
    D_800ABEB0 += 1;
    if (D_800AC5D0 >= 0) {
        if (D_800AC5D0 == 0) {
            func_8004EE60(0);
            func_8003660C();
        }
        t = (unsigned short)D_800AC5D0 - 1;
        D_800AC5D0 = t;
        if (t < 0x19) {
            func_800207D4(D_800ACC14 + 0x70, (0x19 - t) * 10, 0x45);
        }
    }
    if (func_80090424((D_800AC5A8 << 7) & 0xF80) > 0) {
        { int tt = *(int *)0x1F800000; *(int *)0x1F800000 = func_80020074(D_800ACC14 + 0x70, tt, 0x10, 0x10, 0x4C, 0xC, 0x64, 0xC0, 5, 0x7FF8, 0, 0x80); }
    }
    for (i = 0; i < D_800AC59C; i++) {
        if (D_800FFC20[i]->f1e4 >= 0x10) {
            func_80029E0C(D_800FFC20[i]);
        }
    }
    func_80037D88(D_800F4D44);
    if ((unsigned)((unsigned short)D_800F4D44 - 2) < 2U) {
        if ((unsigned char)D_800AC5A8 == 0) {
            if (D_800ABEF8[0].f184 < D_800ABEF8[1].f184) {
                D_8010BFB0 = 1;
            } else {
                D_8010BFB0 = 0;
            }
        }
    }
    func_8006EF8C(0);
    func_8004417C(2, &D_800ABEF8[D_8010BFB0]);
    func_800735D8(0);
    func_8006E1AC();
    func_8002E0D8(6);
    *(int *)0x1F800078 = D_800F2A64;
    func_8006ECE8(0);
    func_800747EC(0);
    func_80036FD4(0);
    if (D_800AC5A8 >= 0x19) {
        func_800555E0();
    }
    func_8005007C(D_800F4C70[0], D_800F2AB4[0], 0);
    func_8005007C(D_800F4C70[1], D_800F2AB4[1], 1);
    func_8004B864(0);
    func_8004B864(1);
    func_8004BB54(0);
    func_8004BB54(1);
}

#undef D_800ABEF8
