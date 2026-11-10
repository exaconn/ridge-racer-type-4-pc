#include "types.h"
#include "rr/audio_state_types.h"
#include "rr/audio_pan.h"
#include "psyq/libsnd/utrev3.h"
#include "game/sound/sndvab.h"
#include "rr/library.h"
#include "rr/menu_choice.h"
#include "rr/audio_bank_setup.h"
#include "rr/audio_cd_controller.h"

extern int PsyqExit();
extern char VabOpenHeadErrorStr[];
extern char D_800110F0[];
extern s32 D_800F7650;

void LoadVab(int *arg0, int arg1, int arg2);
void SetReverb(int a0, int a1);
void TriggerSfx(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3);

void LoadVab(int *arg0, int arg1, int arg2) {
    int k = &SoundState;
    short *p = (short *)(k + (arg2 << 1));
    short v;
    /* ASM_BARRIER: original keeps the first result sign-extended in $a1 */
    register int t asm("$5");
    short v2;
    if (p[0x12] >= 0) {
        SsVabClose(p[0x12]);
        p[0x12] = -1;
    }
    v = SsVabOpenHeadSticky(arg0[0], -1, arg1);
    p[0x12] = v;
    /* ASM_BARRIER: keep the result store ahead of the sign-extension in the schedule */
    __asm__ ("");
    t = v;
    if (t == -1) {
        DebugPrintStub(VabOpenHeadErrorStr);
        PsyqExit(1);
    }
    v2 = SsVabTransBody((u8 *)arg0[1], t);
    p[0x12] = v2;
    if (v2 == -1) {
        DebugPrintStub(D_800110F0);
        PsyqExit(1);
    }
    SsVabTransCompleted(1);
}

void ReverbReset(void) {
    SoundState.revType2894 = 0;
    SoundState.revDepthL2898 = 0;
    SoundState.revDepthR289C = 0;
    SoundState.revMask28A0 = 0xFFFFFF;
    SsUtReverbOff();
    SsUtSetReverbType(2);
    SsUtReverbOn();
    SsUtSetReverbDepth(0, 0);
}

void SetReverbDepthBoth(s32 arg0) {
    D_800F7650 = arg0;
    SsUtSetReverbDepth((s16) arg0, (s16) arg0);
}

void SetReverb(int a0, int a1) {
    SsUtReverbOff();
    if (a0 > 0) {
        SoundState.revType2894 = a0;
        SoundState.revDepthL2898 = a1;
        SoundState.revDepthR289C = a1;
        SsUtSetReverbType((s16)a0);
        SsUtReverbOn();
    } else {
        SoundState.revType2894 = 0;
        SoundState.revDepthL2898 = 0;
        SoundState.revDepthR289C = 0;
    }
}

void TriggerSfx(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
var_t0 = arg0;
    temp_t1 = (arg3 + 0x3CU);
    if (var_t0 < 0x1E) {
        var_a3 = D_800F4DDC;
    } else if (var_t0 < 0x3C) {
        var_a3 = D_800F4DDE;
        var_t0 -= 0x1E;
    } else {
        var_a3 = D_800F4DEA;
        var_t0 -= 0x3C;
    }
    table=PanGainTable;

    temp_a2 = arg2 & 0x7F;
    temp_a2_2 = (table[temp_a2] * (s16)arg1) / 16129;
    var_a0 = temp_a2_2;
    temp_v1 = (table[0x7F-temp_a2] * (s16)arg1) / 16129;
    var_a1 = temp_v1;
    if (Options.audio.mono32 != 0) {
        temp_v0 = (temp_v1 + temp_a2_2) / 2;
        var_a0 = (s16) temp_v0;
        var_a1 = (s16) temp_v0;
    }
    var_v1 = var_a0 * SoundMasterGain;
    if (var_v1 < 0) {
        var_v1 += 0x7F;
    }

    var_a0_2 = var_a1 * SoundMasterGain;
    first_div=var_v1>>7;
    if (var_a0_2 < 0) {
        var_a0_2 += 0x7F;
    }

    first_div=(first_div * 0x10000U);

    volume=Options.audio.volume31;
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
    if (arg4 == -1) {
        /* ASM_BARRIER: retain this phase value in its original MIPS register. */
        register s32 outbank asm("$4"),outprogram asm("$5"),mode asm("$6");
        register s32 outnote;
        register s32 hi;
        register s32 bank;
        register s32 program;
        hi=(var_a3 * 0x10000U); bank=hi>>16;
        hi=(var_t0 * 0x10000U); program=hi>>16;
        if (temp_t1 < 0) {
            var_s0 = temp_t1 + 0x7F;
        }
        outbank=bank;outprogram=program;mode=0;
        /* ASM_BARRIER: preserve argument preparation before the next phase. */
        __asm__("" : : "r"(outbank));
        temp_s0 = var_s0 >> 7;
        note=(s16)temp_s0;
        outnote=note;

        temp_s0_2 = temp_t1 - temp_s0 * 128U;
        SsUtKeyOn(outbank, outprogram, mode, outnote, temp_s0_2, (s16) temp_s2, (s16) temp_s1);
        SsUtKeyOn(bank, program, 1, note, temp_s0_2, (s16) temp_s2, (s16) temp_s1);
        return;
    }
    {
    register u32 voice=arg4*2U;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    s32 hi;
    register s32 bank;
    register s32 program;
    register s32 outvoice;
    /* ASM_BARRIER: retain this phase value in its original MIPS register. */
    register s32 outbank asm("$5"),outprogram asm("$6"),mode asm("$7");
    hi=(voice<<16);
    outvoice=hi>>16;

    hi=(var_a3 * 0x10000U); bank=hi>>16;
    hi=(var_t0 * 0x10000U); program=hi>>16;
    if (temp_t1 < 0) {
        var_s0 = temp_t1 + 0x7F;
    }
    outbank=bank;outprogram=program;mode=0;
    /* ASM_BARRIER: preserve argument preparation before the next phase. */
    __asm__("" : : "r"(outbank));
    temp_s0_3 = var_s0 >> 7;
    note=(s16)temp_s0_3;
    temp_s0_4 = temp_t1 - temp_s0_3 * 128U;
    SsUtKeyOnV(outvoice, outbank, outprogram, mode, note, temp_s0_4, (s16) temp_s2, (s16) temp_s1);
    SsUtKeyOnV((s16)(voice+1U), bank, program, 1, note, temp_s0_4, (s16) temp_s2, (s16) temp_s1);
}
}

void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3) {
    TriggerSfx(a0, a1, a2, 0x1E00, a3);
}

void PlaySfx(int a0) {
    PlaySfxVolPan(a0, 0x7F, 0x40, -1);
}

void PlaySfxBank2(s32 arg0) {
    PlaySfxVolPan(arg0 + 0x3C, 0x7F, 0x40, -1);
}

void SoundQueueClear(void) {
    s16 m = -1;
    int i = RR_SOUND_SEQUENCE_CAPACITY - 1;
    SoundState.sequence19D0.active = 0;
    SoundState.sequence19D0.readIndex = 0;
    SoundState.sequence19D0.unknown0C = 0;
    do { SoundState.sequence19D0.programs[i] = m; i--; } while (i >= 0);
}
