#include "types.h"
#include "rr/audio_bank_setup.h"
#include "rr/spu.h"
#include "game/sound/sndsys.h"

typedef struct {
    s32 bank00;
    s32 data04;
    s32 group08;
} AudioSetupArg;

int SlotSetProgram(int a0, int a1, int a2, int a3);
void SlotLoadCurves(AudioProgramData *a0, s32 a1);

int SlotSetProgram(int a0, int a1, int a2, int a3) {
    AudioState *s = &SoundState;
    int old = s->slots38[a3].programs5DC[a1][a0];
    if (a2 >= 0) s->slots38[a3].programs5DC[a1][a0] = a2;
    return old;
}

void SlotLoadCurves(AudioProgramData *a0, s32 a1) {
    *(AudioProgramData *)SoundState.slots38[a1].curves3C = *a0;
}

void ComSlotSetupBank(AudioSetupArg *arg0) {
    u8 sp10[32];
    s32 i, j, k;

    if (SoundState.bank24[6] >= 0) {
        SsVabClose(SoundState.bank24[6]);
        SoundState.bank24[6] = -1;
    }

    PsyqBzero(&SoundState.slots38[2], 0x620);
    PsyqBzero(&SoundState.slots38[3], 0x620);

    if (arg0->bank00 != -1) {
        SoundState.bank24[6] = SsVabOpenHeadSticky((const u8 *)arg0->bank00, -1, 0x5D000);
        *(AudioBankStorage32 *)&SoundState.slots38[2].bank04 = *(AudioBankStorage32 *)&SoundState.slots38[3].bank04 = SoundState.bank24[6];
    }

    if (arg0->data04 != -1) {
        SoundState.bank24[6] = SsVabTransBody((const u16 *)arg0->data04, SoundState.slots38[2].bank04);
        *(AudioBankStorage32 *)&SoundState.slots38[2].bank04 = *(AudioBankStorage32 *)&SoundState.slots38[3].bank04 = SoundState.bank24[6];
        SsVabTransCompleted(1);
    }

    if (arg0->group08 != -1) {
        SlotLoadCurves((AudioProgramData *)arg0->group08, 2);
        SlotLoadCurves((AudioProgramData *)arg0->group08, 3);
    }

    for (j = 2; j < 4; j++) {
        SoundState.slots38[j].flags00 = 1;
        SoundState.slots38[j].levels20.halves.level = 0x56;
        SoundState.slots38[j].levels20.halves.gain = 0x80;
        SoundState.slots38[j].pan24 = 0x40;
        SoundState.slots38[j].reserved26 = 0;
        SoundState.slots38[j].count0C = 3;
    }

    SoundState.slots38[2].voice08 = 0xE;
    SoundState.slots38[3].voice08 = 0x11;

    for (i = 2; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            SsUtGetVagAtr(SoundState.slots38[i].bank04, (s16)SoundState.slots38[i].programs5DC[0][j], 0, (SpuTone32 *)sp10);
            k = (i & 1) * 2 + 0x3C;
            SoundState.slots38[i].pitch5F8[0][j].coarse = k - sp10[12];
            SoundState.slots38[i].pitch5F8[0][j].fine = (sp10[13] + sp10[12]) << 7;
            SsUtGetVagAtr(SoundState.slots38[i].bank04, (s16)SoundState.slots38[i].programs5DC[1][j], 0, (SpuTone32 *)sp10);
            SoundState.slots38[i].pitch5F8[1][j].coarse = k - sp10[12];
            SoundState.slots38[i].pitch5F8[1][j].fine = (sp10[13] + sp10[12]) << 7;
        }
    }
}
