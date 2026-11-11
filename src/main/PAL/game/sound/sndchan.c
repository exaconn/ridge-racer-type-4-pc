#include "types.h"
#include "rr/audio_bank_setup.h"
#include "rr/runtime_settings.h"
#include "game/sound/sndchan.h"
#include "rr/channel_pitch.h"
#include "rr/audio_loop.h"
#include "rr/audio_pan.h"

extern u16 MenuCarTextureFileOffsets[];

void UpdateSoundSlot(int slotIndex) {
    AudioSlot620 *slot = &SoundState.slots38[slotIndex];
    u16 spread = 0;
    int voice, programIndex;
    u16 pitchOffset;
    int activeLevel;
    int curvePosition;
    int filterIndex;

    if (slot->program14 != slot->previous18) {
        voice = slot->voice08;
        for (programIndex = 0; programIndex < slot->count0C; programIndex++, voice++) {
            if (slot->programData3C.programs[0][programIndex] != slot->programData3C.programs[1][programIndex]) {
                SsUtKeyOnV((s16)voice, slot->bank04.half.value, (s16)slot->programData3C.programs[slot->program14][programIndex], 0, slot->group28, 0, 0, 0);
            }
        }
        slot->previous18 = slot->program14;
    }

    if ((slot->flags00 & 4) == 0) {
        activeLevel = slot->levels20.halves.level;
    } else {
        activeLevel = 0;
    }

    curvePosition = (slot->level10 * 0x2800) / slot->programData3C.duration;
    voice = slot->voice08;
    if (curvePosition > 0x2800) {
        curvePosition = 0x2800;
    }

    filterIndex = 0x11A800 / (0x2350 - slot->filter30);
    filterIndex -= 0x4D;
    if (filterIndex < 0) {
        filterIndex = 0;
    }
    if (filterIndex >= 0x120) {
        filterIndex = 0x11F;
    }
    pitchOffset = MenuCarTextureFileOffsets[filterIndex];

    for (programIndex = 0; programIndex < slot->count0C; programIndex++, voice++) {
        int pitchCurve = SlotInterpCurve(programIndex * 2, curvePosition, slot->program14, slotIndex);
        int leftVolume = SlotInterpCurve(programIndex * 2 + 1, curvePosition, slot->program14, slotIndex) * activeLevel;
        int rightVolume;
        if (leftVolume < 0) {
            leftVolume += 0x7F;
        }
        leftVolume >>= 7;
        rightVolume = (PanGainTable[slot->pan24] * leftVolume) / 16129;
        leftVolume = (PanGainTable[(s16)(0x7F - slot->pan24)] * leftVolume) / 16129;

        if (slotIndex >= 2 && programIndex == 2) {
            int spatialGain = PanGainTable[slot->spatial2C.halves.value];
            spread = slot->programData3C.spread;
            rightVolume = (spatialGain * rightVolume) / 16129;
            leftVolume = (spatialGain * leftVolume) / 16129;
            rightVolume = -rightVolume;
            leftVolume = -leftVolume;
        }

        rightVolume = (rightVolume * slot->levels20.halves.level * slot->levels20.halves.gain) / 16384;
        leftVolume = (leftVolume * slot->levels20.halves.level * slot->levels20.halves.gain) / 16384;
        rightVolume = (rightVolume * slot->programData3C.gain) / 128;
        leftVolume = (leftVolume * slot->programData3C.gain) / 128;

        if (Options.audio.mono32 != 0) {
            rightVolume = (rightVolume + leftVolume) / 2;
            leftVolume = rightVolume;
        }

        leftVolume = (leftVolume * SoundState.gain18) / 128;
        rightVolume = (rightVolume * SoundState.gain18) / 128;
        rightVolume = (rightVolume * Options.audio.volume31) / 16;
        leftVolume = (leftVolume * Options.audio.volume31) / 16;

        SetVoiceVolumeScaled(voice, leftVolume, rightVolume);

        {
            s16 pitch;
            int coarseOffset;
            s16 fine;
            int coarse;
            pitch = (slot->pitch5F8[slot->program14][programIndex].fine * pitchCurve) / 128;
            pitch += pitchOffset;
            pitch += spread;
            fine = pitch % 128;
            coarseOffset = pitch / 128;
            coarse = slot->pitch5F8[slot->program14][programIndex].coarse + coarseOffset;
            SsUtChangePitch((s16)voice, slot->bank04.half.value, (s16)slot->programData3C.programs[slot->program14][programIndex], slot->group28, 0, (s16)coarse, fine);
        }
    }
}

int SlotInterpCurve(int curveIndex, int position, int programGroup, int slotIndex) {
    int upperIndex, lowerIndex, lowerValue, upperValue, lowerKey, upperKey;
    for (upperIndex = 1; upperIndex < 9; upperIndex++) {
        if (position < SoundState.slots38[slotIndex].programData3C.curves[programGroup][curveIndex].keys[upperIndex]) break;
    }
    lowerIndex = upperIndex - 1;
    lowerValue = SoundState.slots38[slotIndex].programData3C.curves[programGroup][curveIndex].values[lowerIndex];
    upperValue = SoundState.slots38[slotIndex].programData3C.curves[programGroup][curveIndex].values[upperIndex];
    lowerKey = SoundState.slots38[slotIndex].programData3C.curves[programGroup][curveIndex].keys[lowerIndex];
    upperKey = SoundState.slots38[slotIndex].programData3C.curves[programGroup][curveIndex].keys[upperIndex];
    return (upperValue - lowerValue) * (position - lowerKey) / (upperKey - lowerKey) + lowerValue;
}
