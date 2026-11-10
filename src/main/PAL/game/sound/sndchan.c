#include "types.h"
#include "rr/audio_bank_setup.h"
#include "rr/runtime_settings.h"
#include "game/sound/sndchan.h"
#include "rr/channel_pitch.h"
#include "rr/audio_loop.h"
#include "rr/audio_pan.h"

extern u16 D_800A053C[];

int SlotInterpCurve(int a0, int x, int a2, int a3);

void UpdateSoundSlot(int a0) {
    AudioSlot620 *s0 = &SoundSlots[a0];
    u16 fp = 0;
    int s3, i;
    u16 sp20;
    int sp28;
    int s5;
    int v1;

    if (s0->program14 != s0->previous18) {
        s3 = s0->voice08;
        for (i = 0; i < s0->count0C; i++, s3++) {
            if (s0->programs5DC[0][i] != s0->programs5DC[1][i]) {
                SsUtKeyOnV((s16)s3, s0->bank04, (s16)s0->programs5DC[s0->program14][i], 0, s0->group28, 0, 0, 0);
            }
        }
        s0->previous18 = s0->program14;
    }

    if ((s0->flags00 & 4) == 0) {
        sp28 = s0->levels20.halves.level;
    } else {
        sp28 = 0;
    }

    s5 = (s0->level10 * 0x2800) / s0->duration5F0;
    s3 = s0->voice08;
    if (s5 > 0x2800) {
        s5 = 0x2800;
    }

    v1 = 0x11A800 / (0x2350 - s0->filter30);
    v1 -= 0x4D;
    if (v1 < 0) {
        v1 = 0;
    }
    if (v1 >= 0x120) {
        v1 = 0x11F;
    }
    sp20 = D_800A053C[v1];

    for (i = 0; i < s0->count0C; i++, s3++) {
        int s6 = SlotInterpCurve(i * 2, s5, s0->program14, a0);
        int a1 = SlotInterpCurve(i * 2 + 1, s5, s0->program14, a0) * sp28;
        int a2;
        if (a1 < 0) {
            a1 += 0x7F;
        }
        a1 >>= 7;
        a2 = (PanGainTable[s0->pan24] * a1) / 16129;
        a1 = (PanGainTable[(s16)(0x7F - s0->pan24)] * a1) / 16129;

        if (a0 >= 2 && i == 2) {
            int t = PanGainTable[s0->spatial2C.halves.value];
            fp = s0->spread5F4;
            a2 = (t * a2) / 16129;
            a1 = (t * a1) / 16129;
            a2 = -a2;
            a1 = -a1;
        }

        a2 = (a2 * s0->levels20.halves.level * s0->levels20.halves.gain) / 16384;
        a1 = (a1 * s0->levels20.halves.level * s0->levels20.halves.gain) / 16384;
        a2 = (a2 * s0->gain5F2) / 128;
        a1 = (a1 * s0->gain5F2) / 128;

        if (Options.audio.mono32 != 0) {
            a2 = (a2 + a1) / 2;
            a1 = a2;
        }

        a1 = (a1 * D_800F4DD0) / 128;
        a2 = (a2 * D_800F4DD0) / 128;
        a2 = (a2 * Options.audio.volume31) / 16;
        a1 = (a1 * Options.audio.volume31) / 16;

        SetVoiceVolumeScaled(s3, a1, a2);

        {
            int off = (s0->program14 * 5 + i) * 4;
            AudioSlot620 *p = (AudioSlot620 *)((char *)s0 + off);
            s16 v;
            int t1;
            s16 t0;
            int c;
            v = (p->pitch5F8[0][0].fine * s6) / 128;
            v += sp20;
            v += fp;
            t0 = v % 128;
            t1 = v / 128;
            c = p->pitch5F8[0][0].coarse + t1;
            SsUtChangePitch((s16)s3, s0->bank04, (s16)s0->programs5DC[s0->program14][i], s0->group28, 0, (s16)c, t0);
        }
    }
}

int SlotInterpCurve(int a0, int x, int a2, int a3) {
    int i, lo, vlo, vhi, klo, khi;
    for (i = 1; i < 9; i++) {
        if (x < SoundState.slots38[a3].curves3C[a2][a0].keys[i]) break;
    }
    lo = i - 1;
    vlo = SoundState.slots38[a3].curves3C[a2][a0].values[lo];
    vhi = SoundState.slots38[a3].curves3C[a2][a0].values[i];
    klo = SoundState.slots38[a3].curves3C[a2][a0].keys[lo];
    khi = SoundState.slots38[a3].curves3C[a2][a0].keys[i];
    return (vhi - vlo) * (x - klo) / (khi - klo) + vlo;
}
