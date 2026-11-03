/* RODATA 0x80010550 0x60 */
#include "types.h"
#include "m2c_macros.h"
#include "rr/selection.h"
#include "game/data/ldscn1.h"
#include "game/gfx/gfxotag.h"
#include "game/menu/menutran.h"
#include "game/ui/barsel0.h"

typedef struct {
    u8 pad0[0x16];
    s16 unk16;
    u8 pad18[0x4];
    s8 unk1C;
    s8 unk1D;
    s8 unk1E;
    u8 unk1F;
    s32 unk20;
} Struct800FF68C;

typedef struct {
    u8 pad0[2];
    s16 unk2;
    u8 pad4[0x18];
} Struct800F3AFC;

struct S { int a; short b; short c; };

struct E { int a; short b; short c; };

typedef struct { char b[0x17]; signed char b17; char r[0x2C - 0x18]; } w3g2S;

extern signed char D_800AD4EF;
extern unsigned char D_800AD4F5;
extern short D_800F4C5C;
extern signed char *D_800FF68C;
extern short D_800FFC18;
extern s32 func_8001FCC8(s32, s32, s32);
extern s32 func_8001FD20(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_8002018C(s32, s32, void *, s32, s32, s32);
extern s32 func_80020508(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_8003DC9C(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_8003DCEC(s32);
extern void func_80043608(s32);
extern u8 D_8009F3F8[];
extern u8 D_8009F404[];
extern s32 D_800ACC14;
extern int func_8002AF8C();
extern int func_80032A14();
extern int func_80032DF0();
extern int func_8003D6F0();
extern struct S D_800F2A68[];
extern struct S *D_800F2DE8[];
extern short D_800AC59C;
extern int func_8002A604();
extern int func_8003D228();
extern s8 D_800ACC68;
extern s16 D_800F3A26;
extern s16 D_800F4C6A;
extern s16 D_800F4D3C;
extern s16 D_800F4D8A;
extern int func_800207D4();
extern int func_8002A344();
extern int func_8002AF8C();
extern int func_8002B330();
extern int func_80032A14();
extern int func_8003A198();
extern int func_8003D6F0();
extern int func_8004F24C();
extern struct { char pad[4]; char b4; char pad5; signed char b6; char pad2[0xC - 7]; } *D_800AC590;
extern short D_800AC5D0;
extern short D_800F4BE8;
extern int D_800ACC18;
extern int D_800F4D74;
extern w3g2S D_800F2E30[];
extern struct { char pad[0x28]; int w28, w2C; } D_800F3A38;
extern struct { char pad[8]; short s8, sA; char padC[4]; short s10; char pad12[0x2C - 0x12]; short s2C; } *D_800F3AFC;
extern char *D_800F4D80;
extern unsigned short D_800F3A42;
extern int D_8010BFC0;
extern int func_8002A344();
extern void func_8003E928(int,int,int,int,int,int,int,int,int,int,...);
extern s16 D_800AC64A;
extern s32 D_800F2DE4;
extern s16 D_800FFC00;

void func_8003159C(int a0, int a1);
void func_8003189C(void);
void func_80031CE4(void);
void func_80031DA0(void);
void func_80031DA8(void);
void func_80031DB0();
void func_80031DEC(void);
void func_80031F1C(void);
void func_80031FE8(void);
void func_8003244C(void);
void func_800324F8(void);

/* legacy typing of shared globals in this part of the unit */
#define func_80039F98 ((int (*)())func_80039F98)

void func_8003159C(int a0, int a1) {
    int sw;
    int idx, a3, v, t, r;
    signed char *P;
    int u;
    int i, sum;

    sw = D_800AD4F2;
    D_800FF68C[7] = 1;
    *(short *)(D_800FF68C + 0x16) = 0;
    D_800FF68C[0x1E] = -2;
    switch (sw) {
    case 0:
        D_800FF68C[7] = 0;
    case 1:
        D_800FF68C[4] = 0;
        D_800F4C5C = 0x109;
        D_800AD4F2 = 1;
        break;
    case 2:
        D_800FF68C[4] = D_800AD4F5 + 0x18;
        D_800F4C5C = 0x10F;
        break;
    case 4:
        idx = D_800AD4D8[0x17];
        if (idx < 4) {
            a3 = D_800AD4D8[idx];
        } else {
            a3 = 3 - D_800AD4D8[idx + 8];
            if (idx == 7) a3 = 0;
        }
        switch (idx) {
        case 0:
            a1 = D_800AD4D8[idx + 8] < 3;
            break;
        case 1:
        case 3:
            a1 = 4;
            if (D_800AD4D8[idx + 8] >= 3) a1 = D_800AD4D8[idx - 1];
            break;
        case 2:
            a1 = 4;
            if (D_800AD4D8[idx + 8] >= 3) a1 = (unsigned char)D_800AD4D8[0x14];
            break;
        case 4:
            a1 = D_800AD4EC;
            break;
        case 5:
            a1 = 3 - D_800AD4D8[idx + 7];
            break;
        case 6:
        case 7:
            a1 = 0;
            i = 0;
            sw = (int)D_800AD4D8;
            do {
                a1 += *(signed char *)(i + sw + 0xC);
                i++;
            } while (i < 4);
            if (a1 == 0xC) {
                a1 = 0;
            } else if (idx == 7) {
                a1 = 1;
            } else {
                a1 = 4 - D_800AD4D8[idx + 7];
            }
            break;
        }
        P = D_800FF68C;
        u = a3 * 5;
        t = a1 + 4;
        P[4] = u + t;
        v = D_800AD4EF;
        if (v == 3) goto L814;
        if (v < 4) {
            if (v == 1) goto L814;
            D_800F4C5C = 0x10F;
        } else {
            if (v == 6) {
L814:
                D_800AD4F2 = 6;
                func_80039F98(P, a1, sw, a3);
                D_800F4C5C = 0x12A;
            } else D_800F4C5C = 0x10F;
        }
        if (D_800AD4EF == 7) D_800F4C5C = 0x205;
        break;
    case 6:
        D_800FF68C[4] = 0x25 - D_800AD4EC;
        D_800F4C5C = 0x111;
        D_800FFC18 = 3;
        break;
    }
}

#undef func_80039F98

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((Struct800F3AFC *)D_800F3AFC)
#define D_800FF68C ((Struct800FF68C *)D_800FF68C)

void func_8003189C(void) {
    s32 var_a1;
    s32 temp_v0;
    s32 temp_s2;

    temp_v0 = D_800FF68C->unk1E;
    temp_s2 = D_800FF68C->unk1F;
    switch (temp_v0) {
    case -2:
        D_800FF68C->unk1F = 0;
        D_800FF68C->unk1E = -4;
        D_800FF68C->unk1D = -1;
        D_800FF68C->unk1C = -1;
        break;
    case -4:
        func_8003DC9C(0, 0x78, 0x60, 0x50, 0xB4, 0, 0);
        D_800FF68C->unk1E = -5;
        break;
    case -5:
        if (func_8003DCEC(0) != 0) {
            D_800FF68C->unk1E = -1;
        }
        break;
    case -3:
        break;
    case -1:
        {
            s32 a1 = *(s32 *)0x1F800000;
            *(s32 *)0x1F800000 = func_8001FCC8(
                D_800ACC14 + 0x74,
                func_80020508(D_800ACC14 + 0x74, a1, 0x78, 0x60, 0x50, 0xB4, (u16)temp_s2, (u16)temp_s2, (u16)temp_s2),
                0x45);
            if ((u16)temp_s2 >= 0x7C) {
                D_800FF68C->unk1E = 2;
                D_800FF68C->unk1F = 0;
                D_800F3AFC[D_800FF68C->unk16].unk2 = 1;
            } else {
                D_800FF68C->unk1F += 8;
            }
            func_8003DCEC(0);
        }
        break;
    case 0:
        var_a1 = *(s32 *)0x1F800000;
        if ((u16)temp_s2 >= 3) {
            var_a1 = func_8002018C(D_800ACC14 + 0x70, var_a1, D_8009F3F8, 1, 0xFF, 2);
        }
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x74, var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        if ((u16)temp_s2 >= 0x15) {
            D_800FF68C->unk1E = 1;
            D_800FF68C->unk1F = 0;
        } else {
            D_800FF68C->unk1F += 1;
        }
        if (temp_s2 == 1) {
            u8 temp_a0 = D_800FF68C->unk1C;
            if ((u32)(temp_a0 - 1) < 5U) {
                func_80043608(D_800FF68C->unk20 + (((s8)temp_a0 - 1) << 14));
            }
        }
        break;
    case 1:
        {
            s32 two = 2;
            *(s32 *)0x1F800000 = func_8001FD20(
                D_800ACC14 + 0x7C,
                func_8002018C(
                    D_800ACC14 + 0x70,
                    func_8002018C(D_800ACC14 + 0x70, *(s32 *)0x1F800000, D_8009F404, 1, temp_s2, 1),
                    D_8009F3F8, 1, (0x80 - temp_s2) * 2, two),
                0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
            if ((u32)temp_s2 >= 0x80U) {
                D_800FF68C->unk1F = 0;
                D_800FF68C->unk1E = two;
            } else {
                D_800FF68C->unk1F += 8;
            }
        }
        break;
    case 2:
    default:
        var_a1 = *(s32 *)0x1F800000;
        if (D_800FF68C->unk1C >= 0) {
            var_a1 = func_8002018C(D_800ACC14 + 0x70, var_a1, D_8009F404, 0, 0x80, 1);
        }
        *(s32 *)0x1F800000 = func_8001FD20(D_800ACC14 + 0x74, var_a1, 0x78, 0x60, 0x50, 0xB4, 8, 8, 8);
        break;
    }
}

#undef D_800F3AFC
#undef D_800FF68C

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC (*(s32 (*))&D_800F3AFC)
#define D_800FF68C ((void *)D_800FF68C)

void func_80031CE4(void) {
    void *temp_v0;

    if (M2C_FIELD(((M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC), s16 *, 0x10) == 3) {
        func_8002AF8C(1, 1, 0);
        func_80032DF0(D_800FF68C);
        temp_v0 = (M2C_FIELD(D_800FF68C, s16 *, 0x16) * 0x1C) + D_800F3AFC;
        func_80032A14(M2C_FIELD(temp_v0, s16 *, 8) + 0xA, M2C_FIELD(temp_v0, s16 *, 0xA) + 0x28, D_800FF68C);
    }
    func_8003D6F0(M2C_FIELD(D_800FF68C, s16 *, 0x16));
}

#undef D_800F3AFC
#undef D_800FF68C

void func_80031DA0(void) {
}

void func_80031DA8(void) {
}

void func_80031DB0() {
    int i = 0;
    struct S **q = D_800F2DE8;
    struct S *p = D_800F2A68;
    do { p->a = 0; p->b = i; *q++ = p; i++; p++; } while (i < 8);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F2A68 (*(struct E (*)[])&D_800F2A68)
#define D_800F2DE8 (*(struct E *(*)[])&D_800F2DE8)

void func_80031DEC(void) {
    int i = 0;
    struct E *e;
    struct E **t;
    int n;
    if (D_800AC59C > 0) {
        n = D_800AC59C;
        t = D_800F2DE8;
        e = D_800F2A68;
        do {
            e->a = 0;
            e->b = i;
            *t = e;
            t++;
            i++;
            e++;
        } while (i < n);
    }
}

#undef D_800F2A68
#undef D_800F2DE8

void func_80031E3C(void) {
    s32 a2 = 0;
    s32 a1;
    RrSelectionRow *a0;
    RrSelectionRow *v1;
    RrSelectionRows *b;
    unsigned char *page;
    const u8 *choices, *variants;
    const RrSelectionConfigView *config;
    u8 t;
    s32 x;
    unsigned char *stage;
    /* ASM_BARRIER: Preserve the indexed byte offset in the address temporary. */
    register u32 offset __asm__("$2");
    u32 attribute;
    page=(unsigned char *)0x800F0000U;
    /* ASM_BARRIER: Materialize the state page after the initial row index. */
    __asm__("" : "+r"(page) : "r"(a2));
    b=(RrSelectionRows *)(page+0x4D98);
    /* ASM_BARRIER: Retain the full frame pointer separately from its page. */
    __asm__("" : "+r"(b));
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Expose the selection-table page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    choices=stage-0xBDC;
    stage=(unsigned char *)0x800A0000U;
    /* ASM_BARRIER: Keep the separate variant-table page materialization. */
    __asm__("" : "+r"(stage));
    variants=stage-0xBE0;
    stage=(unsigned char *)0x800B0000U;
    /* ASM_BARRIER: Expose the configuration page before adding its signed offset. */
    __asm__("" : "+r"(stage));
    config=(const RrSelectionConfigView *)(stage-0x2B28);
    /* ASM_BARRIER: Form lookup bases before initializing the byte offset. */
    __asm__("" : : "r"(choices), "r"(variants), "r"(config) : "$5");
    a1=RR_FIELD_OFFSET(RrSelectionRows,rows);
    do {
        x = page[0x4D98];
        offset=x*8;
        offset=a2+offset;
        t=*(unsigned char *)((unsigned long)offset+(unsigned long)choices);
        a0 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        a0->selection = t;
        if (t == page[0x4D98]) {
            a0->variant = b->variant;
        } else {
            a0->variant = *(unsigned char *)((unsigned long)a0->selection+(unsigned long)variants);
            /* ASM_BARRIER: Retain the original row-pointer recomputation after the variant store. */
            __asm__("" : "+r"(a0));
            a0 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        }
        if (a0->selection != page[0x4D98]) {
            if (a0->variant == b->variant) {
                a0->variant = *(unsigned char *)((unsigned long)page[0x4D98]+(unsigned long)variants);
            }
        }
        v1 = (RrSelectionRow *)((unsigned long)a1+(unsigned long)b);
        /* ASM_BARRIER: Keep the current row and index before publishing its attribute. */
        __asm__("" : : "r"(v1), "r"(a2));
        attribute=config->parameter;
        /* ASM_BARRIER: Read the row attribute before advancing the loop index. */
        __asm__("" : "+r"(attribute) : "r"(a2));
        a2 += 1;
        v1->parameter = attribute;
        a1 += sizeof(RrSelectionRow);
    } while (a2 < 7);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((void *)D_800F3AFC)

void func_80031F1C(void) {
    func_8002A604();
    D_800ACC68 = 2;
    D_800F4D3C = 0;
    func_8003D228(1, 0x80, 0x58, 0xA8, 0x80, 0, 0x701);
    M2C_FIELD(D_800F3AFC, s16 *, 0x1E) = 1;
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    M2C_FIELD(D_800F3AFC, s16 *, 2) = 1;
    func_8002AF8C(0, 1, 1);
    D_800F4C6A = 1;
    D_800F3A26 = 0xE;
    D_800F4D8A = 0;
}

#undef D_800F3AFC

/* legacy typing of shared globals in this part of the unit */
#define D_800AD4D8 (*(w3g2S (*))&D_800AD4D8)
#define D_800F4D8A (*(unsigned short (*))&D_800F4D8A)

void func_80031FE8(void) {
    int s0;
    switch (D_800F4D8A) {
    case 0:
        *(D_800F4D80 + D_800F4D3C * 0x24 + 0xC) = 0;
        func_8002B330(0, 2, &D_800F4D3C, 3, 0);
        *(D_800F4D80 + D_800F4D3C * 0x24 + 0xC) = 2;
        if (D_800F3AFC->s10 == 3) {
            s0 = D_800AC590->b6;
            if (s0 == 4) {
                func_8002AF8C(1, 1, 1);
                if (D_800F3A38.w28 != 0) {
                    if (D_800F2E30[D_800F4D3C].b17 > 0) {
                        func_8004F24C(1);
                        D_800F4D8A = 1;
                        D_800AC5D0 = 0x40;
                        func_8003A198();
                        D_800ACC18 = D_800F4D74;
                        D_800F3AFC->s10 = s0;
                        D_800F3AFC->s2C = s0;
                    } else {
                        func_8004F24C(4);
                    }
                } else if (D_800F3A38.w2C != 0) {
                    func_8004F24C(2);
                    D_800F4D8A = 2;
                    D_800AC5D0 = 0x40;
                    func_8003A198();
                    D_800ACC18 = D_800F4D74;
                    func_8003B398(0);
                    D_800F3AFC->s10 = s0;
                    D_800F3AFC->s2C = s0;
                }
            }
        }
        goto common;
    case 1:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            if (func_8003B398(1) == 1) {
                D_800AD4D8 = D_800F2E30[D_800F4D3C];
                D_800F4C6A = 1;
                D_800F3A26 = 0xF;
                D_800F4D8A = 0;
                return;
            }
        }
        goto common;
    case 2:
        func_8002AF8C(0, 1, 1);
        if (D_800AC5D0 > 0) D_800AC5D0 = D_800AC5D0 - 1;
        func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) * 4, 0x45);
        if (D_800AC5D0 <= 0) {
            func_8002F688();
            D_800F4BE8 = 1;
            D_800F4C6A = 1;
            D_800F3A26 = 0x23;
            D_800F4D8A = 0;
            return;
        }
    default:
    common:
        func_8003D6F0(0);
        func_8003D6F0(1);
        if (D_800F3AFC->s10 == 3) {
            D_800AC590->b4 = 0x1C;
            func_80032A14(D_800F3AFC->s8 + 0xA, D_800F3AFC->sA + 0x28, D_800AC590);
        }
        func_8002A344(0x10086);
    }
}

#undef D_800AD4D8
#undef D_800F4D8A

void func_8003244C(void) {
    if (D_800F3A42 & 0x1000) {
        D_8010BFC0 = D_8010BFC0 - 1;
    } else if (D_800F3A42 & 0x4000) {
        D_8010BFC0 = D_8010BFC0 + 1;
    }
    if (D_8010BFC0 < 0) {
        D_8010BFC0 = 1;
    } else if (D_8010BFC0 >= 2) {
        D_8010BFC0 = 0;
    }
    if (D_800F3A40 & 0x800) {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
        D_800F4D8A = 0;
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3AFC ((void *)D_800F3AFC)

void func_800324F8(void) {
    D_800F2DE4 = 0;
    D_800FFC00 = 0;
    D_800FFC18 = 3;
    D_800AC64A = 0;
    func_8003E928(0, 0x8C, 0x88, 0x1C, 0x20, 0, 0, 1, 0, 0x68);
    func_8003E928(1, 0x8C, 0xB0, 0x1C, 0x20, 0, 0, 1, 0, 0x69);
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    D_800F4C6A = 1;
    D_800F3A26 = 0x2B;
    D_800F4D8A = 0;
    M2C_FIELD(D_800F3AFC, s16 *, 2) = 1;
    func_8002A344(0x10086);
}

#undef D_800F3AFC
