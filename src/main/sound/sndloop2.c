#include "types.h"
#include "rr/audio_state_types.h"
#include "rr/audio_loop.h"
#include "game/sound/sndloop2.h"


void SoundLoopUpdate(void) {
    s16 left, right;
    s32 i;

    left = SoundState.left19F8;
    right = SoundState.right19FA;
    if (Options.mono32) {
        left = (left + right) / 2;
        right = left;
    }
    if (SoundState.muted04) {
        left = right = 0;
    }
    if (GameModeType == 5) {
        left = right = 0;
    }
    right = (right * SoundState.gain18) / 128;
    left = (left * SoundState.gain18) / 128;
    right = (right * Options.volume31) / 16;
    left = (left * Options.volume31) / 16;
    if (left + right > 0) {
        s32 first = (s32)(SoundState.pitch19FC << 9) >> 16;
        s32 second = SoundState.pitch19FC & 127;
        for (i = 0; i < 2; i++) {
            SetVoiceVolumeScaled(i + 6, left, right);
            SsUtChangePitch((s16)(i + 6), SoundState.banks24.ids[1], SoundState.program1A04.half.value, 60, 0, first, second);
        }
        if (!SoundState.active1A00) {
            SsUtKeyOnV(6, SoundState.banks24.ids[1], SoundState.program1A04.half.value, 0, 60, 0, left, right);
            SoundState.active1A00 = 1;
        }
    } else if (SoundState.active1A00) {
        SsUtKeyOffV(6);
        SoundState.active1A00 = 0;
    }
}

void EnableEngineSoundUpdates(void) {
    SoundState.active18B8 = 1;
}

void DisableEngineSoundUpdates(void) {
    SoundState.active18B8 = 0;
}

/* Initial values of the globals this module owns. */
