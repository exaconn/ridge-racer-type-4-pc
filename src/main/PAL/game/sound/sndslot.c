#include "types.h"
#include "rr/audio_loop.h"
#include "game/sound/fxramp1.h"
#include "game/sound/sndbgm3.h"
#include "game/sound/sndchan.h"
#include "game/sound/sndcmd.h"
#include "game/sound/sndctrl.h"
#include "game/sound/sndmain.h"
#include "game/sound/sndrst2.h"
#include "game/sound/sndvol2.h"
#include "game/data/trkld6.h"
#include "game/sound/sndslot.h"
#include "rr/memory.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/audio_cd_controller.h"

void SoundMasterFadeStep(void);
void SetVoiceVolumeScaled(s32 a0, int a1, int a2);
void SlotStart(int a0);
void SlotSetPause(int a0, int a1);
void SlotSetLevelProgram(int a0, int a1, int a2);
void SlotKeyOn();

void SoundMasterFadeStep(void) {
    AudioState *p = &SoundState;
    int n = p->fadeLength1A;
    int v = 0x80;
    int c;
    short c1;
    if (n > 0) {
        c = (unsigned short)p->fadeTick1C;
        if (p->fadeTick1C < n) {
            n = ((p->fadeTarget1E - p->fadeStart20) << 12) / n;
            c1 = c + 1;
            p->fadeTick1C = c1;
            v = (unsigned short)p->fadeStart20 + n * c1 / 4096;
        } else {
            p->fadeLength1A = 0;
            SoundStopAll(n, p, c);
            return;
        }
    }
    p->gain18 = v;
}

void SoundUpdate(void) {
    int i;
    u32 *p;
    int a;
    if (SoundState.seqPrevBeat2878 != SoundState.seqBeat2874) {
        a = SoundState.seqBeat2874 - 1;
        SoundState.seqTick286C = 1;
        if ((a & 1) == 0) SoundState.seqHalf286E = 1;
        if ((a & 3) == 0) SoundState.seqQuarter2870 = 1;
    } else {
        SoundState.seqTick286C = 0;
        SoundState.seqHalf286E = 0;
        SoundState.seqQuarter2870 = 0;
    }
    i = 0;
    SoundState.seqPrevBeat2878 = SoundState.seqBeat2874;
    CdAudioUpdate();
    SeqFadeStep();
    SoundMasterFadeStep();
    SoundListenerUpdate();
    for (p = &SoundState.slots38[0].flags00; i < 4; i++, p += 0x188) {
        if (*p & 1) UpdateSoundSlot(i);
    }
    UpdatePlayerVoices();
    UpdateLoopVoices();
    UpdateMasterSoundVolumes();
    SoundQueueUpdate();
    SoundLoopUpdate();
}

void SetVoiceVolumeScaled(s32 voice, s32 left, s32 right) {
    /* Hardware volume products wrap at 32 bits before the logical shift. */
    u32 scale = VoiceVolumeScale;
    u32 leftProduct = left * scale;
    u32 rightProduct = right * scale;
    SsUtSetVVol((s16)voice, (s16)(leftProduct >> 7), (s16)(rightProduct >> 7));
}

void SlotsStartForRace(void) {
    s32 var_a0;

    SlotStart(0);
    if (DualPlayerSoundEnabled == 0) {
        if (ComSoundBankIndex >= 0) {
            SlotStart(2);
            SlotStart(3);
        }
    } else {
        SlotStart(1);
    }
}

void SlotStart(int a0) {
    AudioState *base = &SoundState;
    AudioState *e = (AudioState *)((u8 *)base + a0 * 0x620);
    e->slots38[0].flags00 |= 1;
    SlotKeyOn(a0);
    e->slots38[0].previous18 = e->slots38[0].program14;
    SlotSetPause(a0, 0);
}

void SlotKeyOffAll(int arg0) {
    int i;
    int s0;
    AudioSlot620 *t = &SoundSlots[arg0];
    s0 = t->voice08;
    i = 0;
    while (i < t->count0C) {
        SsUtKeyOffV((short)s0);
        i++;
        s0++;
    }
    t->flags00 = 0;
}

void SlotSetPause(int a0, int a1) {
    u32 *p = &SoundSlots[a0].flags00;
    if (a1 == 1) *p |= 4;
    else *p &= ~4;
}

void SlotSetLevelProgram(int a0, int a1, int a2) {
    SoundState.slots38[a2].level10 = a0;
    SoundState.slots38[a2].program14 = a1;
}

void SlotKeyOn(int a0) {
    int s[4];
    AudioSlot620 *w = &SoundSlots[a0];
    unsigned short *q;
    int i;
    int k;
    int t;
    PsyqMemset(s, 0, 0x10);
    s[3] = 2;
    k = w->voice08;
    i = 0;
    if (w->count0C > 0) {
        q = (unsigned short *)((int)s + a0 * 4);
        do {
            SsUtKeyOnV((short)k, w->bank04, (s16)w->programs5DC[0][w->program14 * 5 + i], 0, (short)(*q + 0x3C), 0, 0, 0);
            k++;
            i++;
        } while (i < w->count0C);
    }
    t = *(unsigned short *)&s[a0];
    w->reserved2A = 0;
    w->group28 = t + 0x3C;
}

