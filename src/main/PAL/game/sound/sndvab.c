#include "types.h"
#include "m2c_macros.h"
#include "rr/audio_pan.h"
#include "psyq/libsnd/utrev3.h"
#include "game/sound/sndvab.h"
#include "rr/library.h"
#include "rr/menu_choice.h"

extern int func_8008A674();
extern short func_8008AA74();
extern int func_8008B074();
extern int func_800963C4();
extern int func_8001E84C();
extern char D_800110D8[];
extern char D_800110F0[];
extern s32 D_800F7650;

void func_8004ED38(int *arg0, int arg1, int arg2);
void func_8004EE90(int a0, int a1);
void func_8004EF04(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8004F218(int a0, s16 a1, s16 a2, int a3);

/* legacy typing of shared globals in this part of the unit */
#define func_8008AEC4 ((short (*)())func_8008AEC4)
#define D_800F4DB8 (*(short (*)[])&D_800F4DB8)

void func_8004ED38(int *arg0, int arg1, int arg2) {
    int k = (int)D_800F4DB8; short *p = (short *)(k + (arg2 << 1));
    short v;
    /* ASM_BARRIER: original keeps the first result sign-extended in $a1 */
    register int t asm("$5");
    short v2;
    if (p[0x12] >= 0) {
        func_8008A674(p[0x12]);
        p[0x12] = -1;
    }
    v = func_8008AA74(arg0[0], -1, arg1);
    p[0x12] = v;
    /* ASM_BARRIER: keep the result store ahead of the sign-extension in the schedule */
    __asm__ volatile("");
    t = v;
    if (t == -1) {
        func_8001E84C(D_800110D8);
        func_800963C4(1);
    }
    v2 = func_8008AEC4(arg0[1]);
    p[0x12] = v2;
    if (v2 == -1) {
        func_8001E84C(D_800110F0);
        func_800963C4(1);
    }
    func_8008B074(1);
}

#undef func_8008AEC4
#undef D_800F4DB8

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x2894]; s32 a, b, c, d; } (*))&D_800F4DB8)

void func_8004EE08(void) {
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    D_800F4DB8.d = 0xFFFFFF;
    func_800867A4();
    func_800865E4(2);
    func_800867C4();
    func_80086554(0, 0);
}

#undef D_800F4DB8

void func_8004EE60(s32 arg0) {
    D_800F7650 = arg0;
    func_80086554((s16) arg0, (s16) arg0);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x2894]; s32 a, b, c; } (*))&D_800F4DB8)

void func_8004EE90(int a0, int a1) {
    func_800867A4();
    if (a0 > 0) {
        D_800F4DB8.a = a0;
        D_800F4DB8.b = a1;
        D_800F4DB8.c = a1;
        func_800865E4((s16)a0);
        func_800867C4();
    } else {
        D_800F4DB8.a = 0;
        D_800F4DB8.b = 0;
        D_800F4DB8.c = 0;
    }
}

#undef D_800F4DB8

void func_8004EF04(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s16 temp_a2_2;
    register s16 temp_s0_2;
    register s16 temp_s0_4;
    s16 temp_v1;
    s16 var_a0;
    s16 var_a1;
    register s32 var_t0;
    s32 temp_a2;
    register s32 temp_s0;
    register s32 temp_s0_3;
    register s32 temp_s1;
    register s32 temp_s2;
    register s32 temp_t1;
    s32 temp_v0;
    s32 var_a0_2;
    register s32 var_s0;
    s32 var_s1;
    s32 var_v0;
    s32 first_div,second_div;
    register s32 volume;
    s16 *table;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    register s32 note asm("$19");
    s32 var_v1;
    register u16 var_a3;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    register s32 selector asm("$11")=arg4;

    var_t0 = arg0;
    temp_t1 = (s32)((u32)arg3 + 0x3CU);
    if (var_t0 < 0x1E) {
        var_a3 = D_800F4DDC;
    } else if (var_t0 < 0x3C) {
        var_a3 = D_800F4DDE;
        var_t0 -= 0x1E;
    } else {
        var_a3 = D_800F4DEA;
        var_t0 -= 0x3C;
    }
    table=D_800A043C;

    temp_a2 = arg2 & 0x7F;
    temp_a2_2 = (table[temp_a2] * (s16)arg1) / 16129;
    var_a0 = temp_a2_2;
    temp_v1 = (table[0x7F-temp_a2] * (s16)arg1) / 16129;
    var_a1 = temp_v1;
    if (D_800AD528.audio.mono32 != 0) {
        temp_v0 = (s32) (temp_v1 + temp_a2_2) / 2;
        var_a0 = (s16) temp_v0;
        var_a1 = (s16) temp_v0;
    }
    var_v1 = var_a0 * D_800F4DD0;
    if (var_v1 < 0) {
        var_v1 += 0x7F;
    }

    var_a0_2 = var_a1 * D_800F4DD0;
    first_div=var_v1>>7;
    if (var_a0_2 < 0) {
        var_a0_2 += 0x7F;
    }

    first_div=(s32)((u32)first_div<<16);

    volume=D_800AD528.audio.volume31;
    var_s1 = (first_div>>16) * volume;
    second_div=var_a0_2>>7;
    if (var_s1 < 0) {
        var_s1 += 0xF;
    }

    var_v0 = (s16)second_div * volume;
    temp_s1 = var_s1 >> 4;
    if (var_v0 < 0) {
        var_v0 += 0xF;
    }
    temp_s2 = var_v0 >> 4;
    var_s0 = temp_t1;
    if (selector == -1) {
        /* ASM_BARRIER: retain this phase value in its original MIPS register. */
        register s32 outbank asm("$4"),outprogram asm("$5"),mode asm("$6");
        register s32 outnote;
        register s32 hi;
        register s32 bank;
        register s32 program;
        hi=(s32)((u32)var_a3<<16); bank=hi>>16;
        hi=(s32)((u32)var_t0<<16); program=hi>>16;
        if (temp_t1 < 0) {
            var_s0 = temp_t1 + 0x7F;
        }
        outbank=bank;outprogram=program;mode=0;
        /* ASM_BARRIER: preserve argument preparation before the next phase. */
        __asm__("" : : "r"(outbank));
        temp_s0 = var_s0 >> 7;
        note=(s16)temp_s0;
        outnote=note;

        temp_s0_2 = temp_t1 - (u32)temp_s0 * 128U;
        func_80085304(outbank, outprogram, mode, outnote, (s32) temp_s0_2, (s32) (s16) temp_s2, (s32) (s16) temp_s1);
        func_80085304(bank, program, 1, note, (s32) temp_s0_2, (s32) (s16) temp_s2, (s32) (s16) temp_s1);
        return;
    }
    {
    register u32 voice=(u32)selector*2U;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    register s32 hi asm("$2");
    register s32 bank;
    register s32 program;
    register s32 outvoice;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    register s32 outbank asm("$5"),outprogram asm("$6"),mode asm("$7");
    /* ASM_BARRIER: preserve argument preparation before the next phase. */
    __asm__("" : : "r"(voice));
    hi=(s32)((u32)voice<<16);
    outvoice=hi>>16;

    hi=(s32)((u32)var_a3<<16); bank=hi>>16;
    hi=(s32)((u32)var_t0<<16); program=hi>>16;
    if (temp_t1 < 0) {
        var_s0 = temp_t1 + 0x7F;
    }
    outbank=bank;outprogram=program;mode=0;
    /* ASM_BARRIER: preserve argument preparation before the next phase. */
    __asm__("" : : "r"(outbank));
    temp_s0_3 = var_s0 >> 7;
    note=(s16)temp_s0_3;
    temp_s0_4 = temp_t1 - (u32)temp_s0_3 * 128U;
    func_800857A4(outvoice, outbank, outprogram, mode, note, (s32) temp_s0_4, (s32) (s16) temp_s2, (s32) (s16) temp_s1);
    func_800857A4((s16)(voice+1U), bank, program, 1, note, (s32) temp_s0_4, (s32) (s16) temp_s2, (s32) (s16) temp_s1);
}
}

void func_8004F218(int a0, s16 a1, s16 a2, int a3) {
    func_8004EF04(a0, a1, a2, 0x1E00, a3);
}

void func_8004F24C(int a0) {
    func_8004F218(a0, 0x7F, 0x40, -1);
}

void func_8004F274(s32 arg0) {
    func_8004F218(arg0 + 0x3C, 0x7F, 0x40, -1);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(struct { char pad[0x19D0]; s32 a; char p1[4]; s32 b, c; char p2[8]; s16 arr[8]; } (*))&D_800F4DB8)

void func_8004F2A0(void) {
    s16 m = -1;
    int i = 7;
    D_800F4DB8.a = 0;
    D_800F4DB8.b = 0;
    D_800F4DB8.c = 0;
    do { D_800F4DB8.arr[i] = m; i--; } while (i >= 0);
}

#undef D_800F4DB8
