#include "rr/audio_pan.h"

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
