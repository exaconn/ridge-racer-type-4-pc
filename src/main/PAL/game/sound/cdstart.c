#include "types.h"
#include "rr/audio_cd_controller.h"
#include "rr/overlay668_stream.h"
#include "game/sound/cdstart.h"

void CdAudioBeginPlay(int a0) {
    SoundState.action2670 = 1;
    SoundState.playActive26BC = 1;
    if (a0 == 1) SoundState.flag26A0 = a0;
    else SoundState.flag26A0 = 0;
    SoundState.step2678 = 0;
    SoundState.timer26A8 = 0;
    SoundState.reserved26B0 = 0;
    SoundState.request26B4 = 0;
    if (SoundState.state269C == 2) CdAudioSetVolume(SoundState.volume26CE);
    else CdAudioFade(SoundState.targetVol26C0.halves.value, 0);
    CdAudioSetVolumeRaw(0);
}

void CdAudioMute(void) {
    CdAudioFade(0, 0);
}

void CdAudioSetVolume(u16 a0) {
    s16 s;
    if (SoundState.currentVol26C4 != a0) {
        SoundState.currentVol26C4 = (u8)a0;
        s = (SoundState.currentVol26C4 * D_800AD558) / 16;
        SsSetSerialVol(0, s, s);
    }
}

void CdAudioSetVolumeRaw(s32 arg0) {
    s32 var_a0;
    u32 temp_a1;

    var_a0 = (arg0 & 0xFF) * D_800AD558;
    if (var_a0 < 0) {
        var_a0 += 0xF;
    }
    temp_a1 = (u32) var_a0 >> 4;
    SsSetSerialVol(0, temp_a1, temp_a1);
}

void CdAudioFade(s32 a0_in, s32 a1) {
    u16 a0 = a0_in;
    if (a1 >= 2) {
        u16 v = SoundState.currentVol26C4;
        SoundState.fadeLength26C6 = a1;
        SoundState.fadeTick26C8 = 0;
        SoundState.fadeTarget26CA = a0;
        SoundState.fadeStart26CC = v;
    } else {
        CdAudioSetVolume(a0);
        SoundState.fadeLength26C6 = 0;
        SoundState.fadeTick26C8 = 0;
        SoundState.fadeTarget26CA = 0;
        SoundState.fadeStart26CC = 0;
        if (a0 == 0) {
            int r = SoundState.state269C;
            if (r != -1) {
                if (SoundState.action2670 != 1) {
                    SoundState.action2670 = 4;
                    SoundState.step2678 = 0;
                    SoundState.timer26A8 = 0;
                    if (r == 2) CdAudioSetVolume(SoundState.volume26CE);
                } else {
                    if (SoundState.reserved26B0 < 0x20) {
                        SoundState.request26B4 = 2;
                    } else {
                        SoundState.action2670 = 4;
                        SoundState.step2678 = 0;
                        SoundState.timer26A8 = 0;
                    }
                }
            }
        }
    }
}

void CdAudioRequestPause(void)
{
    switch (SoundState.state269C) {
    case 0:
    case 1:
        if (SoundState.action2670 != 2) {
            SoundState.action2670 = 2;
            SoundState.step2678 = 0;
            SoundState.volume26CE = SoundState.currentVol26C4;
        }
        if (SoundState.state269C == 1) {
            SoundState.pausedState26B8 = SoundState.state269C;
        } else {
            SoundState.pausedState26B8 = 0;
            SoundState.reserved2694 = SoundState.sector2664;
        }
        break;
    case 2:
        if (SoundState.action2670 != 3) {
            SoundState.action2670 = 3;
            SoundState.timer26A4 = 0;
            SoundState.step2678 = 0;
        }
        break;
    }
    if (SoundState.action2670 == 1) {
        if (SoundState.reserved26B0 < 0x20) {
            SoundState.request26B4 = SoundState.action2670;
            return;
        }
        SoundState.action2670 = 2;
        SoundState.step2678 = 0;
        SoundState.volume26CE = SoundState.currentVol26C4;
    }
}

void CdAudioSetFilter(int a0) {
    SoundState.filter266C = a0;
    SoundState.action2670 = 6;
    SoundState.step2678 = 0;
}

int CdAudioFilterPending(void) {
    if (SoundState.phase2674 != 0 || SoundState.step2678 != 0) return 1;
    return SoundState.flag2680;
}

void CdAudioFilterStep(void) {
    char sp[8];
    switch (SoundState.step2678) {
    case 0:
        sp[0] = 1;
        sp[1] = SoundState.filter266C;
        if (CdControl(0xD, sp, 0) != 0) {
            SoundState.step2678++;
            return;
        }
        if (CdDiskReady(1) == 0x10) {
            SoundState.flag267C = 1;
            SoundState.flag2680 = -3;
            SoundState.action2670 = -1;
            SoundState.step2678 = 0;
            return;
        }
        return;
    case 1:
        SoundState.action2670 = -1;
        SoundState.step2678 = 0;
        SoundState.flag2680 = 0;
        SoundState.index2698 = SoundState.filter266C;
        break;
    }
}
