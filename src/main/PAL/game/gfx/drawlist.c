/* RODATA 0x800101f4 0xd8 */
/* CC1_FLAGS: -fno-strength-reduce */
#include "types.h"
#include "rr_asm.h"
#include "rr/animated_sprite_grid.h"
#include "game/data/cdfile2.h"
#include "game/gfx/gtemodel.h"
#include "game/gfx/light2.h"
#include "game/gfx/skyrot1.h"
#include "game/sound/cdstop.h"
#include "game/effect/spfade.h"
#include "game/gfx/skyrot1.h"

#define G D_8010BF78
#define H D_800F3AFC
#define K D_800AC590
#define SPP (*(struct P **)0x1F800000)
#define SPO (*(struct P **)0x1F800004)
#define SPAD ((int *)0x1F800000)
#define CURSOR (scratch->cursor)
#define ORDER (order_page->order)

struct GS {
    char pad0[0x40];
    short *p40;
    short s44;
    short s46;
    short s48;
    short s4A;
    short s4C;
    short s4E;
    short s50;
    short s52;
    short s54;
    short s56;
    short s58;
    short s5A;
    short s5C;
    short s5E;
    short s60;
    short s62;
    short s64;
    short s66;
    short s68;
    short s6A;
    short s6C;
    short s6E;
    short s70;
    short s72;
    short s74;
    short s76;
    short s78;
    short s7A;
    short s7C;
    short s7E;
    short s80;
    short s82;
    short s84;
    short s86;
    short s88;
    short s8A;
    short s8C;
    short s8E;
    short s90;
    short s92;
    short s94;
    short s96;
    short s98;
    short s9A;
    short s9C;
    short s9E;
    short sA0;
    unsigned short uA2;
    short sA4;
    short sA6;
    short sA8;
    short sAA;
    short sAC;
    short sAE;
    short sB0;
};

struct V16 { int w[4]; };

struct SPD { struct P *cur; struct OT *ot; };

struct HS { short s0; short s2; short s4; short s6; };

struct KS { char pad[4]; unsigned char b4; };

union H { struct P *p; struct { char pad[3]; unsigned char len; } b; };

struct P { union H h; int w[4]; };

struct OT { int x; union H h; };

extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A68();
extern int func_8001BAF8();
extern int func_800207D4();
extern int func_8002A344();
extern int func_8002A604();
extern int func_80032A14();
extern int func_8003A198();
extern int func_8003A1B4();
extern int func_8003B934();
extern int func_8003D228();
extern int func_8003D6F0();
extern int func_80041BF0();
extern int func_80043608();
extern int func_80052800();
extern int func_800528A8();
extern int func_80052A30();
extern int func_80052B8C();
extern int func_8006EF8C();
extern int func_80070108();
extern int func_800912B4();
extern int func_80096454();
extern struct GS *D_8010BF78;
extern struct HS *D_800F3AFC;
extern struct KS *D_800AC590;
extern int D_8010BF7C;
extern short D_800AC5D0;
extern int D_800ACC14;
extern char *D_800ACC18;
extern char D_800ACC68;
extern short D_800F3A26;
extern unsigned short D_800F3A40;
extern short D_800F4BE8;
extern short D_800F4C6A;
extern int D_800F4CA8;
extern short D_800F4D8A;
extern short D_8009E4EC;
extern char *D_800102F0;
extern char *D_800AC64C;
extern char D_800101EC[];
extern char D_8009E510[];
extern char D_8009E520[];
extern char D_8009E530[];
extern char D_8009E540[];

void func_8001C14C();
void func_8001E1F0(u32 intensity,RrSpriteGridClock *state);

void func_8001C14C() {
    int temp_s5;
    register struct GS *g asm("$2"); /* ASM_BARRIER: pin G reload pointer to $v0 */
    register struct GS *g_a0 asm("$4"); /* ASM_BARRIER: pin G reload pointer to $a0 in case 0x2F */
    register struct GS *g_v1 asm("$3"); /* ASM_BARRIER: pin G reload pointer to $v1 in case 0x32 */
    int temp_a0;
    int temp_a0_10;
    int temp_a0_2;
    int temp_a0_3;
    int temp_a0_4;
    int temp_a0_5;
    int temp_a0_6;
    int temp_a0_7;
    int temp_a0_8;
    int temp_a0_9;
    short *temp_a1;
    short *temp_a1_10;
    short *temp_a1_11;
    short *temp_a1_12;
    short *temp_a1_13;
    short *temp_a1_14;
    short *temp_a1_15;
    short *temp_a1_16;
    short *temp_a1_2;
    short *temp_a1_3;
    short *temp_a1_4;
    short *temp_a1_5;
    short *temp_a1_6;
    short *temp_a1_7;
    short *temp_a1_8;
    short *temp_a1_9;
    register int temp_t0 asm("$8"); /* ASM_BARRIER: pin clamped color temp to $t0 */
    int temp_v0;
    int temp_v1;
    int temp_v1_10;
    int temp_v1_2;
    int temp_v1_3;
    int temp_v1_4;
    int temp_v1_5;
    int temp_v1_6;
    int temp_v1_7;
    int temp_v1_8;
    int temp_v1_9;
    int var_a0;
    int var_a0_2;
    int var_a0_3;
    int var_a0_4;
    int var_a0_5;
    int v0;
    struct V16 *dv;
    register char *src asm("$2"); /* ASM_BARRIER: pin src to $2 as in original copy sequence */
    register int *st asm("$4"); /* ASM_BARRIER: pin state pointer to $a0 */
    register int *st_v1 asm("$3"); /* ASM_BARRIER: pin state pointer to $v1 in case 0x2F */
    register int c80 asm("$3"); /* ASM_BARRIER: pin hoisted 0x80 constant to $v1 in case 0x2F */
    register int c8 asm("$2"); /* ASM_BARRIER: pin 8 constant to $v0 in case 0x32 */
    int *sp1, *sp2, *sp3, *sp4;
    register int *tabq asm("$3"); /* ASM_BARRIER: pin GTE IR table pointer to $v1 */
    int *tabr;
    register int c_800060 asm("$7"); /* ASM_BARRIER: pin 0x800060 constant to $a3 in block_241 */
    register int c_800080 asm("$6"); /* ASM_BARRIER: pin 0x800080 constant to $a2 in block_241 */
    register int c_e1000240 asm("$5"); /* ASM_BARRIER: pin 0xE1000240 constant to $a1 in block_241 */
    int var_a1;
    int var_a1_2;
    int var_a1_3;
    int var_a1_4;
    int var_a1_5;
    int var_a2;
    int var_v0;
    int var_v0_2;
    int var_v0_3;
    register int var_v0_4 asm("$2"); /* ASM_BARRIER: pin clamp result to $v0 */
    char *temp_s0, *temp_s1, *temp_s2;
    register struct P *temp_t1 asm("$9"); /* ASM_BARRIER: pin packet pointer to $t1 */
    struct SPD *spd, *spd2;
    int col;
    temp_s5 = func_800534B4();
    if (D_8010BF7C < 5) {
        goto block_9;
    }
    if (D_800AC5D0 == 0) {
        goto block_7;
    }
    if (D_800AC5D0 != 4) {
        goto block_4;
    }
    func_80052B8C(0, 0x40);
    goto block_6;
block_4:
    if (D_800AC5D0 < 0x100) {
        goto block_6;
    }
    D_800F4CA8 = 0;
    D_800F4BE8 = 1;
    D_800F4C6A = 1;
    D_800F3A26 = 0x23;
    D_800F4D8A = 0;
block_6:
    func_800207D4(D_800ACC14 + 0x70, D_800AC5D0, 0x45);
    D_800AC5D0 = (u16) D_800AC5D0 + 4;
    goto block_9;
block_7:
    if (!(D_800F3A40 & 0x800)) {
        goto block_9;
    }
    D_800AC5D0 = 4;
block_9:
    if ((u32) D_8010BF7C >= 0x36U) {
        goto block_224;
    }
    switch (D_8010BF7C) {
case 0x0:
    D_800ACC18 = D_800102F0;
    func_8002A604();
    D_800ACC68 = 2;
    func_8003A1B4();
    func_8003A198();
    D_800F4CA8 = 4;
    func_8003D228(0, 0xC, 0x130, 0x128, 0x58, 0, 0);
    D_8010BF7C = 1;
case 0x1:
    if (func_8001292C(func_8001223C(0, 0x13), D_800ACC18) == 0) {
        goto block_248;
    }
    D_8010BF7C = 2;
case 0x2:
    temp_v0 = func_80012A58();
    if (temp_v0 == 0) {
        goto block_248;
    }
    temp_s1 = D_800ACC18 + ((int *)D_800ACC18)[1];
    temp_s2 = D_800ACC18 + ((int *)D_800ACC18)[2];
    temp_s0 = D_800ACC18 + ((int *)D_800ACC18)[4];
    func_80043608(D_800ACC18 + ((int *)D_800ACC18)[3]);
    func_80043608(temp_s0);
    func_80070108(temp_s1, 4);
    D_800AC64C = temp_s2;
    func_80012A68(temp_s2);
    D_8010BF7C = 3;
    D_800ACC18 += temp_v0;
case 0x3:
    if (func_8003B934() == 0) {
        goto block_248;
    }
    func_80052800();
    func_800528A8(0, 0);
    func_80052A30(0);
    func_8001B308();
    D_8010BF7C = 4;
case 0x4:
    if (temp_s5 < 0) {
        goto block_248;
    }
    D_8010BF7C = 5;
    G->s48 = 0x80;
    G->s9C = 0x1000;
    G->s4A = 0;
    G->p40 = &D_8009E4EC;
    G->s46 = 0;
    G->s44 = 0;
case 0x5:
    if (G->s46 < 0xB4) {
        goto block_224;
    }
    if (G->s9C <= 0) {
        goto block_23;
    }
    G->s9C = (s16) ((u16) G->s9C - 0x100);
block_23:
    if ((u32) ((u16) G->s46 - 0xA0) >= 0xAAU) {
        goto block_25;
    }
    sp1 = (int *)0x1F800000;
    sp1[2] = sp1[2] + 3;
block_25:
    temp_a0 = G->s46;
    if (temp_a0 == 0xB0) {
        G->s76 = 4;
    } else if (temp_a0 == 0xA0) {
        G->s8A = 0x80;
        G->s8C = 0x80;
        G->s8E = 0x80;
    } else if (temp_a0 == 0x15E) {
        H->s2 = 1;
    } else if (temp_a0 == 0x129) {
        G->s8A = -0x80;
        G->s8C = -0x80;
        G->s8E = -0x80;
    } else if (temp_a0 == 0x175) {
        G->s74 = -4;
        G->s76 = -4;
    }
block_38:
    temp_a1 = G->p40;
    if (G->s46 < *temp_a1) {
        goto block_197;
    }
    G->p40 = (s16 *) (temp_a1 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 6;
    G->s44 = 0;
case 0x6:
    if (G->s44 < 0x20) {
        goto block_197;
    }
    func_80096454(0x1F800018, 0, 0x10);
    src = D_8009E540;
    st = &D_8010BF7C;
    __asm__ volatile("" ::: "$8", "$9"); /* ASM_BARRIER: reserve $t0-$t1 so sp2 allocates to $t5 */
    sp2 = (int *)0x1F800008;
    RR_COPY16_T(sp2, src, 16);
    g = G;
    *st = 7;
    g->s56 = 0x100;
    g->uA2 = 3U;
    g->s8A = 0x80;
    g->s48 = 0;
    g->s5E = 0;
    g->s66 = 0;
    g->s68 = 0;
    g->s6A = 0;
    g->s74 = 4;
    g->s44 = 0;
case 0x7:
    temp_v1 = G->s44;
    if (temp_v1 >= 0x38) {
        goto block_49;
    }
    if (temp_v1 != 8) {
        goto block_45;
    }
    G->s82 = 0x200;
    goto block_198;
block_45:
    if (temp_v1 != 0xC) {
        goto block_47;
    }
    G->s76 = 4;
    goto block_198;
block_47:
    if (temp_v1 != 0x18) {
        goto block_199;
    }
    G->s78 = 4;
    goto block_199;
block_49:
    G->s44 = 0;
    D_8010BF7C = 8;
case 0x8:
    temp_a1_2 = G->p40;
    if (G->s46 < *temp_a1_2) {
        goto block_199;
    }
    G->p40 = (s16 *) (temp_a1_2 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 9;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->uA2 = 0xCU;
    G->s9E = 2;
    G->sA0 = 0x1000;
    G->s8A = -0x80;
    G->s8C = 0x80;
    G->s44 = 0;
case 0x9:
    if (G->s44 < 0x20) {
        goto block_199;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->sAC = 0;
    G->sAA = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0xA;
case 0xA:
    temp_v1_2 = G->s44;
    if (temp_v1_2 >= 0x38) {
        goto block_61;
    }
    if (temp_v1_2 != 8) {
        goto block_57;
    }
    G->s82 = 0x200;
    goto block_200;
block_57:
    if (temp_v1_2 != 0xC) {
        goto block_59;
    }
    G->s76 = 4;
    goto block_200;
block_59:
    if (temp_v1_2 != 0x18) {
        goto block_201;
    }
    G->s78 = 4;
    goto block_201;
block_61:
    G->s44 = 0;
    D_8010BF7C = 0xB;
case 0xB:
    temp_a1_3 = G->p40;
    if (G->s46 < *temp_a1_3) {
        goto block_201;
    }
    G->p40 = (s16 *) (temp_a1_3 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0xC;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->uA2 = 0xF0U;
    G->s9E = 3;
    G->sA0 = 0x1000;
    G->s8C = -0x80;
    G->s8E = 0x80;
    G->s44 = 0;
case 0xC:
    if (G->s44 < 0x20) {
        goto block_201;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->sB0 = 0;
    G->sAE = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0xD;
case 0xD:
    temp_v1_3 = G->s44;
    if (temp_v1_3 >= 0x50) {
        goto block_77;
    }
    if (temp_v1_3 != 8) {
        goto block_69;
    }
    G->s82 = 0x200;
    goto block_202;
block_69:
    if (temp_v1_3 != 0xC) {
        goto block_71;
    }
    G->s76 = 4;
    goto block_202;
block_71:
    if (temp_v1_3 != 0x18) {
        goto block_73;
    }
    G->s78 = 4;
    goto block_202;
block_73:
    if (temp_v1_3 != 0x24) {
        goto block_75;
    }
    G->s7A = 4;
    goto block_202;
block_75:
    if (temp_v1_3 != 0x30) {
        goto block_203;
    }
    G->s7C = 4;
    goto block_203;
block_77:
    G->s44 = 0;
    D_8010BF7C = 0xE;
case 0xE:
    temp_a1_4 = G->p40;
    if (G->s46 < *temp_a1_4) {
        goto block_203;
    }
    G->p40 = (s16 *) (temp_a1_4 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0xF;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s7A = -4;
    G->s7C = -4;
    G->s82 = -8;
    G->s8A = 0x80;
    G->s8C = 0x80;
    G->s9E = 4;
    G->uA2 = 0U;
    G->sA0 = 0x1000;
    G->s44 = 0;
case 0xF:
    if (G->s44 < 0x20) {
        goto block_203;
    }
    if (G->sA4 != 0) {
        goto block_203;
    }
    G->s56 = 0x100;
    G->s5E = 0;
    G->s4A = 0;
    G->s48 = 0;
    G->s70 = 0;
    G->s6E = 0;
    G->s6C = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0x10;
case 0x10:
    temp_a0_2 = G->s44;
    if (temp_a0_2 >= 0x38) {
        goto block_92;
    }
    if (temp_a0_2 != 8) {
        goto block_86;
    }
    G->s82 = 0x200;
    goto block_205;
block_86:
    if (temp_a0_2 != 0xC) {
        goto block_88;
    }
    G->s90 = 0x40;
    G->s92 = 0x40;
    G->s94 = 0x40;
    goto block_205;
block_88:
    if (temp_a0_2 != 0x18) {
        goto block_90;
    }
    G->s76 = 4;
    goto block_205;
block_90:
    if (temp_a0_2 != 0x4C) {
        goto block_206;
    }
    G->s94 = 0;
    G->s92 = 0;
    G->s90 = 0;
    goto block_206;
block_92:
    G->sA6 = 0xFFF;
    G->s44 = 0;
    D_8010BF7C = 0x11;
case 0x11:
    temp_a1_5 = G->p40;
    if (G->s46 < *temp_a1_5) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_5 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x12;
case 0x12:
    G->sA6 = 0;
    D_8010BF7C = 0x13;
    G->uA2 = 1;
    G->s44 = 0;
    goto block_205;
case 0x13:
    temp_a1_6 = G->p40;
    if (G->s46 < *temp_a1_6) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_6 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x14;
case 0x14:
    D_8010BF7C = 0x15;
    G->sA6 = 1;
    G->uA2 = 2;
    G->s44 = 0;
    goto block_205;
case 0x15:
    temp_a1_7 = G->p40;
    if (G->s46 < *temp_a1_7) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_7 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x16;
case 0x16:
    G->sA6 = 0;
    D_8010BF7C = 0x17;
    G->uA2 = 0U;
    G->s44 = 0;
    goto block_205;
case 0x17:
    if (G->sA4 != 0) {
        goto block_206;
    }
    G->s76 = -4;
    G->s8A = -0x40;
    G->s8C = -0x40;
    G->s8E = -0x40;
    G->s90 = -0x40;
    G->s92 = -0x40;
    G->s94 = -0x40;
    G->s44 = 0;
    D_8010BF7C = 0x18;
case 0x18:
    if (G->s44 < 0x20) {
        goto block_206;
    }
    G->s44 = 0;
    D_8010BF7C = 0x1B;
case 0x1B:
    temp_a1_8 = G->p40;
    if (G->s46 < *temp_a1_8) {
        goto block_206;
    }
    G->p40 = (s16 *) (temp_a1_8 + 1);
    K->b4 = K->b4 + 1;
    st = &D_8010BF7C;
    g = G;
    *st = 0x19;
    g->s56 = 0x100;
    g->s58 = 0x100;
    g->s5A = 0x100;
    g->s90 = 0x40;
    g->s92 = 0x40;
    g->s94 = 0x40;
    g->s54 = 0;
    g->s52 = 0;
    g->s50 = 0;
    g->s4E = 0;
    g->s4C = 0;
    g->s4A = 0;
    g->s48 = 0;
    g->s5E = 0;
    g->s60 = 0;
    g->s62 = 0;
    g->s74 = 8;
    g->s44 = 0;
case 0x19:
    if (G->s44 != 0x20) {
        goto block_111;
    }
    G->s94 = 0;
    G->s92 = 0;
    G->s90 = 0;
block_111:
    temp_a1_9 = G->p40;
    if (G->s46 < *temp_a1_9) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_9 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x1C;
    G->s90 = 0x40;
    G->s44 = 0;
case 0x1C:
    temp_v1_4 = G->s44;
    if (temp_v1_4 >= 0x44) {
        goto block_120;
    }
    if (temp_v1_4 != 0x10) {
        goto block_116;
    }
    G->s76 = 4;
    goto block_209;
block_116:
    if (temp_v1_4 != 0x18) {
        goto block_118;
    }
    G->s82 = 0x200;
    goto block_209;
block_118:
    if (temp_v1_4 != 0x24) {
        goto block_210;
    }
    G->s78 = 4;
    goto block_210;
block_120:
    G->s44 = 0;
    D_8010BF7C = 0x1D;
case 0x1D:
    temp_a1_10 = G->p40;
    if (G->s46 < *temp_a1_10) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_10 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x1E;
    G->s92 = 0x40;
    G->s44 = 0;
case 0x1E:
    temp_v1_5 = G->s44;
    if (temp_v1_5 >= 0x44) {
        goto block_130;
    }
    if (temp_v1_5 != 0x10) {
        goto block_126;
    }
    G->s7A = 4;
    goto block_209;
block_126:
    if (temp_v1_5 != 0x18) {
        goto block_128;
    }
    G->s84 = 0x200;
    goto block_209;
block_128:
    if (temp_v1_5 != 0x24) {
        goto block_210;
    }
    G->s7C = 4;
    goto block_210;
block_130:
    G->s44 = 0;
    D_8010BF7C = 0x1F;
case 0x1F:
    temp_a1_11 = G->p40;
    if (G->s46 < *temp_a1_11) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_11 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x20;
    G->s94 = 0x40;
    G->s44 = 0;
case 0x20:
    temp_v1_6 = G->s44;
    if (temp_v1_6 >= 0x44) {
        goto block_140;
    }
    if (temp_v1_6 != 0x10) {
        goto block_136;
    }
    G->s7E = 4;
    goto block_209;
block_136:
    if (temp_v1_6 != 0x18) {
        goto block_138;
    }
    G->s86 = 0x200;
    goto block_209;
block_138:
    if (temp_v1_6 != 0x24) {
        goto block_210;
    }
    G->s80 = 4;
    goto block_210;
block_140:
    G->s44 = 0;
    D_8010BF7C = 0x21;
case 0x21:
    temp_a1_12 = G->p40;
    if (G->s46 < *temp_a1_12) {
        goto block_210;
    }
    G->p40 = (s16 *) (temp_a1_12 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x22;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s7A = -4;
    G->s7C = -4;
    G->s7E = -4;
    G->s80 = -4;
    G->s82 = -8;
    G->s84 = -8;
    G->s86 = -8;
    G->s44 = 0;
case 0x22:
    if (G->s44 < 0x20) {
        goto block_210;
    }
    G->s9E = 5;
    G->sA0 = 0x1000;
    G->s44 = 0;
    D_8010BF7C = 0x23;
case 0x23:
    if (G->s44 < 0x20) {
        goto block_210;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0x24;
case 0x24:
    temp_v1_7 = G->s44;
    if (temp_v1_7 >= 0x44) {
        goto block_154;
    }
    if (temp_v1_7 != 0x10) {
        goto block_150;
    }
    G->s76 = 4;
    goto block_211;
block_150:
    if (temp_v1_7 != 0x18) {
        goto block_152;
    }
    G->s82 = 0x200;
    goto block_211;
block_152:
    if (temp_v1_7 != 0x24) {
        goto block_212;
    }
    G->s78 = 4;
    goto block_212;
block_154:
    G->s44 = 0;
    D_8010BF7C = 0x25;
case 0x25:
    temp_a1_13 = G->p40;
    if (G->s46 < *temp_a1_13) {
        goto block_212;
    }
    G->p40 = (s16 *) (temp_a1_13 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x26;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s90 = -0x80;
    G->s44 = 0;
case 0x26:
    if (G->s44 < 0x20) {
        goto block_212;
    }
    G->s9E = 6;
    G->sA0 = 0x1000;
    G->s44 = 0;
    D_8010BF7C = 0x28;
case 0x28:
    if (G->s44 < 0x20) {
        goto block_212;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0x29;
case 0x29:
    temp_v1_8 = G->s44;
    if (temp_v1_8 >= 0x44) {
        goto block_168;
    }
    if (temp_v1_8 != 0x10) {
        goto block_164;
    }
    G->s76 = 4;
    goto block_213;
block_164:
    if (temp_v1_8 != 0x18) {
        goto block_166;
    }
    G->s82 = 0x200;
    goto block_213;
block_166:
    if (temp_v1_8 != 0x24) {
        goto block_214;
    }
    G->s78 = 4;
    goto block_214;
block_168:
    G->s44 = 0;
    D_8010BF7C = 0x2A;
case 0x2A:
    temp_a1_14 = G->p40;
    if (G->s46 < *temp_a1_14) {
        goto block_214;
    }
    G->p40 = (s16 *) (temp_a1_14 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x2B;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s92 = -0x80;
    G->s44 = 0;
case 0x2B:
    if (G->s44 < 0x20) {
        goto block_214;
    }
    G->s9E = 7;
    G->sA0 = 0x1000;
    G->s44 = 0;
    D_8010BF7C = 0x2C;
case 0x2C:
    if (G->s44 < 0x20) {
        goto block_216;
    }
    G->s56 = 0x100;
    G->s48 = 0;
    G->s5E = 0;
    G->s74 = 4;
    G->s44 = 0;
    D_8010BF7C = 0x2D;
case 0x2D:
    temp_v1_9 = G->s44;
    if (temp_v1_9 >= 0x44) {
        goto block_182;
    }
    if (temp_v1_9 != 0x10) {
        goto block_178;
    }
    G->s76 = 4;
    goto block_215;
block_178:
    if (temp_v1_9 != 0x18) {
        goto block_180;
    }
    G->s82 = 0x200;
    goto block_215;
block_180:
    if (temp_v1_9 != 0x24) {
        goto block_216;
    }
    G->s78 = 4;
    goto block_216;
block_182:
    G->s44 = 0;
    D_8010BF7C = 0x2E;
case 0x2E:
    temp_a1_15 = G->p40;
    if (G->s46 < *temp_a1_15) {
        goto block_216;
    }
    G->p40 = (s16 *) (temp_a1_15 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x2F;
    G->s74 = -4;
    G->s76 = -4;
    G->s78 = -4;
    G->s82 = -8;
    G->s94 = -0x80;
    G->s44 = 0;
case 0x2F:
    if (G->s44 < 0x20) {
        goto block_216;
    }
    c80 = 0x80;
    __asm__ volatile("" : "+r"(c80)); /* ASM_BARRIER: hoist 0x80 load into $v1 before s56..s76 stores */
    G->s56 = 0x100;
    G->s5E = 0x1000;
    G->s76 = 4;
    __asm__ volatile(""); /* ASM_BARRIER: keep src load after s76 store */
    src = D_8009E540;
    __asm__ volatile("" : "+r"(src)); /* ASM_BARRIER: emit src load before s48..s96 stores */
    G->s48 = c80;
    G->s4A = 0;
    G->s72 = 0;
    G->s96 = c80;
    __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: force G reload after s96 store */
    g_a0 = G;
    __asm__ volatile("" : "+r"(g_a0) :: "$8", "$9"); /* ASM_BARRIER: emit G reload before state lui and reserve $t0-$t1 for sp2 in $t5 */
    st_v1 = &D_8010BF7C;
    sp2 = (int *)0x1F800008;
    RR_COPY16_T(sp2, src, 128);
    *st_v1 = 0x30;
    g_a0->s44 = 0;
case 0x30:
    if (G->s44 < 0x20) {
        goto block_219;
    }
    G->s44 = 0;
    D_8010BF7C = 0x31;
case 0x31:
    temp_a1_16 = G->p40;
    if (G->s46 < *temp_a1_16) {
        goto block_219;
    }
    G->p40 = (s16 *) (temp_a1_16 + 1);
    K->b4 = K->b4 + 1;
    D_8010BF7C = 0x32;
    G->s74 = -4;
    G->s76 = -4;
    G->s82 = -8;
    G->s96 = -0x80;
    G->s44 = 0;
case 0x32:
    if (G->s44 < 0x20) {
        goto block_219;
    }
    sp4 = (int *)0x1F800000;
    sp4[6] = 0x400;
    sp4[7] = 0;
    sp4[8] = 0;
    __asm__ volatile("" : "=r"(st) :: "memory"); /* ASM_BARRIER: clobber $a0 so %hi(D_8010BF78) stays in $a1 across sp4 stores */
    src = D_8009E540;
    g_v1 = G;
    __asm__ volatile("" : "+r"(src), "+r"(g_v1) :: "$8", "$9"); /* ASM_BARRIER: emit src and G reload before state lui and reserve $t0-$t1 for sp2 in $t5 */
    st = &D_8010BF7C;
    sp2 = (int *)0x1F800008;
    RR_COPY16_T(sp2, src, 144);
    *st = 0x33;
    __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: keep *st = 0x33 before c8 = 8 */
    c8 = 8;
    __asm__ volatile("" : "+r"(c8)); /* ASM_BARRIER: schedule 8 load before s48=0 store */
    g_v1->s48 = 0;
    __asm__ volatile(""); /* ASM_BARRIER: keep s48=0 before s74=8 */
    g_v1->s74 = c8;
    g_v1->s98 = c8;
    g_v1->s44 = 0;
case 0x33:
    if (G->s44 < 0x20) {
        goto block_222;
    }
    G->s44 = 0;
    D_8010BF7C = 0x34;
case 0x34:
    if (G->s46 < *G->p40) {
        goto block_222;
    }
    D_800AC5D0 = 4;
    D_8010BF7C = 0x35;
    goto block_221;
block_197:
    func_8001B8B0(G->s48, G->s9C);
    func_8001BAF8(G->s4A, 0);
    goto block_223;
block_198:
block_199:
    func_8001BAF8(G->s48, 1);
    func_8001BAF8(G->s48, 2);
    func_8001BAF8(G->s4A, 3);
    func_8001BAF8(G->s4C, 4);
    func_8001BBC4(G->s56, G->s5E, 0x6C0018, 0x78);
    goto block_225;
block_200:
block_201:
    func_8001BAF8(G->s48, 5);
    func_8001BAF8(G->s4A, 6);
    func_8001BAF8(G->s4C, 7);
    func_8001BBC4(G->s56, G->s5E, 0x6C0018, 0x78);
    goto block_225;
block_202:
block_203:
    func_8001BAF8(G->s48, 8);
    func_8001BAF8(G->s4A, 9);
    func_8001BAF8(G->s4C, 0xA);
    func_8001BAF8(G->s4E, 0xB);
    func_8001BAF8(G->s50, 0xC);
    func_8001BBC4(G->s56, G->s5E, 0x6C0018, 0x78);
    goto block_225;
block_205:
block_206:
    temp_a0_3 = G->sA6;
    if (temp_a0_3 == 0) {
        goto block_208;
    }
    func_8001BC84(temp_a0_3, G->s44);
block_208:
    func_8001BAF8(G->s48, 0xD);
    func_8001BAF8(G->s48, 0xE);
    func_8001BBC4(G->s56, G->s5E, 0x680010, 0x120);
    func_8001BAF8(G->s4A, 0xF);
    func_8001BAF8(G->s4A, 0x10);
    func_8001BAF8(G->s4A, 0x11);
    goto block_223;
block_209:
block_210:
    func_8001BAF8(G->s48, 0xF);
    func_8001BAF8(G->s48, 0x10);
    func_8001BAF8(G->s48, 0x11);
    func_8001BAF8(G->s4A, 0x12);
    func_8001BAF8(G->s4C, 0x13);
    func_8001BAF8(G->s4E, 0x14);
    func_8001BAF8(G->s50, 0x15);
    func_8001BAF8(G->s52, 0x16);
    func_8001BAF8(G->s54, 0x17);
    func_8001BBC4(G->s56, G->s5E, 0xB0001C, 0x28);
    func_8001BBC4(G->s58, G->s60, 0xB00070, 0x28);
    func_8001BBC4(G->s5A, G->s62, 0xB000E0, 0x28);
    goto block_218;
block_211:
block_212:
    func_8001BAF8(G->s48, 0x18);
    func_8001BAF8(G->s48, 0x19);
    func_8001BAF8(G->s4A, 0x1A);
    func_8001BAF8(G->s4C, 0x1B);
    func_8001BBC4(G->s56, G->s5E, 0xE0008C, 0x28);
    goto block_218;
block_213:
block_214:
    func_8001BAF8(G->s48, 0x1C);
    func_8001BAF8(G->s4A, 0x1D);
    func_8001BAF8(G->s4C, 0x1E);
    func_8001BBC4(G->s56, G->s5E, 0xE0008C, 0x28);
    goto block_218;
block_215:
block_216:
    func_8001BAF8(G->s48, 0x1F);
    func_8001BAF8(G->s4A, 0x20);
    func_8001BAF8(G->s4C, 0x21);
    func_8001BBC4(G->s56, G->s5E, 0xE0008C, 0x28);
block_218:
    func_8001BAF8(0x80, 0xD);
    func_8001BAF8(0x80, 0xE);
    func_8001BBC4(0x100, 0x1000, 0x680010, 0x120);
    goto block_220;
block_219:
    func_8001BAF8(G->s48, 0xD);
    func_8001BAF8(G->s48, 0xE);
    func_8001BAF8(G->s4A, 0x22);
    func_8001BAF8(G->s4A, 0x23);
    func_8001BBC4(G->s56, G->s5E, 0x680010, 0x120);
block_220:
    goto block_225;
case 0x35:
block_221:
block_222:
    func_8001BAF8(G->s48, 0x24);
block_223:
case 0x1A:
case 0x27:
block_224:
block_225:
    G->s44 = (s16) ((u16) G->s44 + 1);
    G->s46 = (s16) ((u16) G->s46 + 1);
    spd = (struct SPD *)0x1F800000;
    spd->cur->w[0] = 0xE1000229;
    spd->cur->h.p = spd->ot->h.p;
    spd->cur->h.b.len = 1;
    spd->ot->h.p = spd->cur;
    spd->ot->h.b.len = 0;
    spd->cur = (struct P *)((char *)spd->cur + 8);
    func_8001B754(&G->s48, &G->s74, 0x80);
    func_8001B754(&G->s4A, &G->s76, 0x80);
    func_8001B754(&G->s4C, &G->s78, 0x80);
    func_8001B754(&G->s4E, &G->s7A, 0x80);
    func_8001B754(&G->s50, &G->s7C, 0x80);
    func_8001B754(&G->s52, &G->s7E, 0x80);
    func_8001B754(&G->s54, &G->s80, 0x80);
    func_8001B754(&G->s9A, &G->s98, 0x100);
    func_8001B7C8(&G->s56, &G->s5E, &G->s82);
    func_8001B7C8(&G->s58, &G->s60, &G->s84);
    func_8001B7C8(&G->s5A, &G->s62, &G->s86);
    func_8001B7C8(&G->s5C, &G->s64, &G->s88);
    func_8001B83C(&G->s66, &G->s8A);
    func_8001B83C(&G->s68, &G->s8C);
    func_8001B83C(&G->s6A, &G->s8E);
    func_8001B83C(&G->s6C, &G->s90);
    func_8001B83C(&G->s6E, &G->s92);
    func_8001B83C(&G->s70, &G->s94);
    func_8001B83C(&G->s72, &G->s96);
    g_v1 = G;
    v0 = (unsigned short)g_v1->sA0;
    if (g_v1->sA0 == 0) {
        goto block_227;
    }
    c8 = v0 - 0x80;
    g_v1->sA0 = c8;
    RR_GTE_MTC2_IR0(c8);
    c8 = g_v1->s9E;
    st = (int *)D_8009E540;
    tabq = st + c8 * 4;
    RR_GTE_LWC2_IR(tabq);
    tabr = st + (c8 - 1) * 4;
    RR_GTE_SETFC_P(tabr);
    RR_GTE_INTPL();
    __asm__ volatile("" ::: "$8", "$9"); /* ASM_BARRIER: reserve $t0-$t1 so sp3 allocates to $t5 */
    sp3 = (int *)0x1F800008;
    RR_GTE_STORE_IR(sp3);
block_227:
    func_8001B640();
    func_8006EF8C(6);
    func_800912B4(0xA0, 0xC0);
    temp_a0_4 = G->s66;
    G->sA4 = 0;
    if (temp_a0_4 == 0) {
        goto block_229;
    }
    func_8001B688(temp_a0_4);
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(0, G->uA2 & 1));
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(1, G->uA2 & 2, D_8010BF78));
block_229:
    temp_a0_5 = G->s68;
    if (temp_a0_5 == 0) {
        goto block_231;
    }
    func_8001B688(temp_a0_5);
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(2, G->uA2 & 4));
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(3, G->uA2 & 8, D_8010BF78));
block_231:
    temp_a0_6 = G->s6A;
    if (temp_a0_6 == 0) {
        goto block_233;
    }
    func_8001B688(temp_a0_6);
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(4, G->uA2 & 0x10));
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(5, G->uA2 & 0x20, D_8010BF78));
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(6, G->uA2 & 0x40, D_8010BF78));
    G->sA4 = (s16) ((u16) G->sA4 | func_8001BD40(7, G->uA2 & 0x80, D_8010BF78));
block_233:
    temp_a0_7 = G->s6C;
    if (temp_a0_7 == 0) {
        goto block_235;
    }
    func_8001B688(temp_a0_7);
    func_8001BFA8(1, D_800101EC, D_8009E510);
block_235:
    temp_a0_8 = G->s6E;
    if (temp_a0_8 == 0) {
        goto block_237;
    }
    func_8001B688(temp_a0_8);
    func_8001BFA8(1, D_800101EC, D_8009E520);
block_237:
    temp_a0_9 = G->s70;
    if (temp_a0_9 == 0) {
        goto block_239;
    }
    func_8001B688(temp_a0_9);
    func_8001BFA8(2, D_800101EC, D_8009E530);
block_239:
    temp_a0_10 = G->s72;
    if (temp_a0_10 == 0) {
        goto block_241;
    }
    func_8001B688(temp_a0_10);
    func_8001BEF8();
block_241:
    c80 = G->s9A;
    if (c80 == 0) {
        goto block_247;
    }
    var_v0_4 = 0x100;
    __asm__ volatile("" : "+r"(var_v0_4)); /* ASM_BARRIER: fill beq delay slot with addiu $v0,$zero,256 */
    temp_t1 = spd->cur;
    temp_t0 = var_v0_4 - c80;
    __asm__ volatile("" : "+r"(temp_t1), "+r"(temp_t0)); /* ASM_BARRIER: emit spd->cur load and subu before bltz */
    if (temp_t0 >= 0) {
        if (temp_t0 > 0x100) {
            var_v0_4 = 0x100;
            goto block_clamp;
        }
    }
    __asm__ volatile("" :: "r"(c80)); /* ASM_BARRIER: keep $v1 live across >0x100 check so slti uses $v0 */
    if (temp_t0 < 0) {
        var_v0_4 = 0;
    } else {
        var_v0_4 = temp_t0;
    }
block_clamp:
    __asm__ volatile("" : "+r"(var_v0_4)); /* ASM_BARRIER: materialize clamped value in $v0 before copying to $t0 */
    temp_t0 = var_v0_4;
    __asm__ volatile("" : "+r"(temp_t0)); /* ASM_BARRIER: keep addu $t0,$v0,$zero at clamp merge label */
    c_800060 = 0x800060;
    c_800080 = 0x800080;
    c_e1000240 = 0xE1000240;
    __asm__ volatile("" : "+r"(c_800060), "+r"(c_800080), "+r"(c_e1000240)); /* ASM_BARRIER: hoist constant loads into $a3,$a2,$a1 before color shifts */
    c80 = temp_t0 << 0x10;
    var_v0_4 = temp_t0 << 8;
    st = (int *)0x62000000;
    __asm__ volatile("" : "+r"(c80), "+r"(var_v0_4), "+r"(st)); /* ASM_BARRIER: order sll/sll/lui before bitwise ors */
    var_v0_4 |= (int)st;
    c80 |= var_v0_4;
    c80 |= temp_t0;
    temp_t1->w[1] = c80;
    __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: keep w[1] store before loading 0x1F800000 into $v1 */
    spd2 = (struct SPD *)0x1F800000;
    temp_t1->w[2] = c_800060;
    temp_t1->w[3] = c_800080;
    temp_t1->w[0] = c_e1000240;
    temp_t1->h.p = spd2->ot->h.p;
    temp_t1->h.b.len = 4;
    __asm__ volatile(""); /* ASM_BARRIER: prevent hoisting call arg setup into packet header load delay slots */
    spd2->ot->h.p = temp_t1;
    spd2->ot->h.b.len = 0;
    temp_t1++;
    spd2->cur = temp_t1;
    func_80041BF0(0, &spd2->ot->h, 1, c_800060);
block_247:
    H->s2 = 1;
    func_8003D6F0(0);
    K->b4 = 0U;
    func_80032A14(H->s4 + 0xA, H->s6 + 0x28, D_800AC590);
    func_8002A344(0x1001C);
block_248:
    return;
    }
}

void func_8001E1F0(u32 intensity,RrSpriteGridClock *state) {
    s32 row;
    /* ASM_BARRIER: retain the horizontal position counter in a3. */
    register s32 x __asm__("$7");
    s32 phase;
    s32 index;
    u32 hi;
    u32 middle;
    u32 size;
    u32 uv;
    s32 limit;
    s32 length;
    s32 quotient;
    /* ASM_BARRIER: materialize the packed row step only after each sprite row. */
    register u32 row_step __asm__("$2");
    u32 mode_command;
    u32 y;
    /* ASM_BARRIER: keep the raw/color word in its original temporary. */
    register u32 color __asm__("$9")=intensity;
    RrGpuScratchPointers *order_page;
    RrGpuScratchPointers *scratch;
    u8 *packet;
    u8 *fields;
    hi=color<<16;
    middle=color<<8;
    middle|=0x66000000U;
    hi|=middle;
    color|=hi;
    /* ASM_BARRIER: finish color packing before initializing loop constants. */
    __asm__("" : "+r"(color) : : "$15");
    row=0;
    /* ASM_BARRIER: keep the row counter distinct from later coordinate zero. */
    __asm__("" : "+r"(row));
    limit=15;size=0x180010;uv=0x2F29A400U;
    order_page=(RrGpuScratchPointers*)0x1F800000U;
    /* ASM_BARRIER: stage the scratch page before the packed-y constant. */
    __asm__("" : "+r"(order_page) : : "$8");
    length=4;
    y=0xB00000;
    packet=*(u8**)0x1F800000U;
    state->tick0E++;
    phase=state->tick0E;
    /* ASM_BARRIER: capture tick phase before starting the horizontal counter. */
    __asm__("" : "+r"(phase) : : "$7");
    x=0;
    do {
        fields=packet+3;
        do {
            quotient=phase;
            if(phase<0)quotient=phase+3;
            index=(quotient>>2)&15;
            if(index==limit)index=0;
            *(u32*)(fields+5)=y|(x+172);
            *(u32*)(fields+1)=color;
            *(u32*)(fields+13)=size;
            *(u32*)(fields+9)=index*16+uv;
            phase+=5;x+=26;
            *(u32*)packet=*ORDER;
            fields[0]=length;
            fields+=sizeof(RrSpritePacket);
            *ORDER=(u32)(unsigned long)packet;
            ((u8*)ORDER)[3]=0;
            packet+=sizeof(RrSpritePacket);
        }while(x<130);
        row_step=0x300000;
        y+=row_step;
        row+=48;x=0;
    }while(row<144);
    mode_command=0xE100022A;
    scratch=(RrGpuScratchPointers*)0x1F800000U;
    CURSOR=packet;
    *(u32*)(packet+4)=mode_command;
    *(u32*)CURSOR=*scratch->order;
    CURSOR[3]=1;
    *scratch->order=(u32)(unsigned long)CURSOR;
    ((u8*)scratch->order)[3]=0;
    CURSOR+=sizeof(RrGpuSingleCommandPacket);
}
