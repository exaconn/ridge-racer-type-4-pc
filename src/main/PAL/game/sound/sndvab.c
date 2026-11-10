#include "psyq/libc/exit.h"
#include "types.h"
#include "rr/audio_state_types.h"
#include "rr/audio_pan.h"
#include "psyq/libsnd/utrev3.h"
#include "game/sound/sndvab.h"
#include "rr/library.h"
#include "rr/menu_choice.h"
#include "rr/audio_bank_setup.h"
#include "rr/audio_cd_controller.h"

extern char VabOpenHeadErrorStr[];
extern char VabTransBodyErrorStr[];
extern s32 ActiveVabTransferSlot;

void SetReverb(int a0, int a1);
void TriggerSfx(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void PlaySfxVolPan(int a0, s16 a1, s16 a2, int a3);

void LoadVab(const VabData *data, s32 mode, s32 bankIndex) {
    s32 opened;
    s32 transferred;
    if (SoundState.banks24.ids[bankIndex] >= 0) {
        SsVabClose(SoundState.banks24.ids[bankIndex]);
        SoundState.banks24.ids[bankIndex] = -1;
    }
    opened = SsVabOpenHeadSticky(data->header, -1, mode);
    SoundState.banks24.ids[bankIndex] = opened;
    if ((s16)opened == -1) {
        DebugPrintStub(VabOpenHeadErrorStr);
        PsyqExit(1);
    }
    transferred = SsVabTransBody(data->samples, (s16)opened);
    SoundState.banks24.ids[bankIndex] = transferred;
    if ((s16)transferred == -1) {
        DebugPrintStub(VabTransBodyErrorStr);
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
    ActiveVabTransferSlot = arg0;
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
    s16 temp_s0_2;
    s16 temp_s0_4;
    s16 temp_v1;
    s16 var_a0;
    s16 var_a1;
    s32 var_t0;
    s32 temp_a2;
    s32 temp_s1;
    s32 temp_s2;
    s32 temp_t1;
    s32 temp_v0;
    s32 var_a0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_v0;
    s32 first_div,second_div;
    s32 volume;
    s16 *table;
    s16 note;
    s32 var_v1;
    u16 var_a3;
var_t0 = arg0;
    temp_t1 = (arg3 + 0x3CU);
    if (var_t0 < 0x1E) {
        var_a3 = AudioPanCenterAngle;
    } else if (var_t0 < 0x3C) {
        var_a3 = AudioPanRearAngle;
        var_t0 -= 0x1E;
    } else {
        var_a3 = AudioPanFrontAngle;
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
        s16 bank = var_a3;
        s16 program = var_t0;
        SsUtKeyOn(bank, program, 0, note = temp_t1 / 128, temp_s0_2 = temp_t1 % 128, (s16) temp_s2, (s16) temp_s1);
        SsUtKeyOn(bank, program, 1, note, temp_s0_2, (s16) temp_s2, (s16) temp_s1);
        return;
    }
    {
        u32 voice = arg4 * 2U;
        s16 bank = var_a3;
        s16 program = var_t0;
        SsUtKeyOnV((s16)voice, bank, program, 0, note = temp_t1 / 128, temp_s0_4 = temp_t1 % 128, (s16) temp_s2, (s16) temp_s1);
        SsUtKeyOnV((s16)(voice + 1U), bank, program, 1, note, temp_s0_4, (s16) temp_s2, (s16) temp_s1);
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
