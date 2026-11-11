#include "types.h"
#include "psyq/libsnd/libsnd.h"
#include "rr/audio_bank_setup.h"
#include "rr/spu.h"
#include "game/sound/sndsys.h"

int SlotSetProgram(int a0, int a1, int a2, int a3);

int SlotSetProgram(int a0, int a1, int a2, int a3) {
    AudioState *s = &SoundState;
    int old = s->slots38[a3].programData3C.programs[a1][a0];
    if (a2 >= 0) s->slots38[a3].programData3C.programs[a1][a0] = a2;
    return old;
}

void SlotLoadCurves(const void *data, s32 slot) {
    /* Binary resources need not have the owned record's word alignment. */
    __builtin_memcpy(&SoundState.slots38[slot].programData3C, data, sizeof(AudioProgramData));
}

void ComSlotSetupBank(AudioBankSetup *input) {
    VagAtr tone;
    s32 slotIndex, programIndex, baseNote;

    if (SoundState.banks24.ids[6] >= 0) {
        SsVabClose(SoundState.banks24.ids[6]);
        SoundState.banks24.ids[6] = -1;
    }

    PsyqBzero(&SoundState.slots38[2], sizeof(SoundState.slots38[2]));
    PsyqBzero(&SoundState.slots38[3], sizeof(SoundState.slots38[3]));

    if (input->bank00 != (const u8 *)~0UL) {
        SoundState.banks24.ids[6] = SsVabOpenHeadSticky(input->bank00, -1, 0x5D000);
        SoundState.slots38[2].bank04.word = SoundState.slots38[3].bank04.word = SoundState.banks24.ids[6];
    }

    if (input->data04 != (const u16 *)~0UL) {
        SoundState.banks24.ids[6] = SsVabTransBody(input->data04, SoundState.slots38[2].bank04.half.value);
        SoundState.slots38[2].bank04.word = SoundState.slots38[3].bank04.word = SoundState.banks24.ids[6];
        SsVabTransCompleted(1);
    }

    if (input->group08 != (const u8 *)~0UL) {
        SlotLoadCurves(input->group08, 2);
        SlotLoadCurves(input->group08, 3);
    }

    for (programIndex = 2; programIndex < 4; programIndex++) {
        SoundState.slots38[programIndex].flags00 = 1;
        SoundState.slots38[programIndex].levels20.halves.level = 0x56;
        SoundState.slots38[programIndex].levels20.halves.gain = 0x80;
        SoundState.slots38[programIndex].pan24 = 0x40;
        SoundState.slots38[programIndex].reserved26 = 0;
        SoundState.slots38[programIndex].count0C = 3;
    }

    SoundState.slots38[2].voice08 = 0xE;
    SoundState.slots38[3].voice08 = 0x11;

    for (slotIndex = 2; slotIndex < 4; slotIndex++) {
        for (programIndex = 0; programIndex < 5; programIndex++) {
            SsUtGetVagAtr(
                SoundState.slots38[slotIndex].bank04.half.value,
                (s16)SoundState.slots38[slotIndex].programData3C.programs[0][programIndex],
                0, (u8 *)&tone);
            baseNote = (slotIndex & 1) * 2 + 0x3C;
            SoundState.slots38[slotIndex].pitch5F8[0][programIndex].coarse = baseNote - tone.pbmin;
            SoundState.slots38[slotIndex].pitch5F8[0][programIndex].fine = (tone.pbmax + tone.pbmin) << 7;
            SsUtGetVagAtr(
                SoundState.slots38[slotIndex].bank04.half.value,
                (s16)SoundState.slots38[slotIndex].programData3C.programs[1][programIndex],
                0, (u8 *)&tone);
            SoundState.slots38[slotIndex].pitch5F8[1][programIndex].coarse = baseNote - tone.pbmin;
            SoundState.slots38[slotIndex].pitch5F8[1][programIndex].fine = (tone.pbmax + tone.pbmin) << 7;
        }
    }
}
