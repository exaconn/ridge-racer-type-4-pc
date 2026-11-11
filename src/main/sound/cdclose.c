/* RODATA 0x80011160 0x14 */
#include "types.h"
#include "rr/audio_cd_controller.h"
#include "game/sound/cdclose.h"

extern s32 CdCloseStateStep;

void CdAudioPauseStep(void)
{
    int r;
    switch (CdAudioFadeState) {
    case 0:
        SoundState.state269C = 2;
        CdAudioSetVolume(0);
        SoundState.step2678 = SoundState.step2678 + 1;
    case 1:
        if (CdSync(1, 0) == 0) break;
        SoundState.step2678 = SoundState.step2678 + 1;
    case 2:
        if (CdControl(9, 0, 0) == 0) break;
        goto inc;
    case 3:
        r = CdSync(1, 0);
        if (r == 2) {
    inc:
            SoundState.step2678 = SoundState.step2678 + 1;
        } else if (r == 5) {
            CdAudioFadeState = 1;
        }
        break;
    case 4:
        SoundState.action2670 = -1;
        SoundState.step2678 = 0;
        if (SoundState.pausedState26B8 == 1) SoundState.reserved2694 = SoundState.position2688;
        break;
    }
}

void CdAudioResumeStep(void) {
    switch (SoundState.timer26A4) {
    case 0:
        CdAudioSetVolume(SoundState.volume26CE);
        SoundState.sector2664 = SoundState.reserved2694;
        SoundState.phase2674 = 0;
        SoundState.timer26A4++;
        break;
    case 1:
        if (SoundState.phase2674 == 0) {
            if (SoundState.playActive26BC == 1) CdAudioBeginPlay(SoundState.flag26A0);
            else SoundState.action2670 = -1;
            CdCloseStateStep = 0;
        }
        break;
    }
}

void CdAudioLoopStep(void) {
    switch (SoundState.timer26A4) {
    case 0:
        SoundState.phase2674 = 0;
        SoundState.timer26A4 = 1;
        SoundState.sector2664 = SoundState.reserved2684;
        SoundState.filter266C = SoundState.index2698;
        break;
    case 1:
        if (SoundState.phase2674 == 0) {
            CdAudioBeginPlay(1);
            SoundState.timer26A4 = 2;
        }
        break;
    }
}
