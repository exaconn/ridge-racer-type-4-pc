#include "types.h"
#include "rr/audio_cd_controller.h"
#include "rr/frame_loop.h"
#include "rr/bios_devices.h"

typedef struct {
    u32 start;
    s32 length;
} CdTrackEntry8;

extern int D_800A099C[][3];
extern int D_800A0904[];
extern int PsyqSrand();

void CdAudioSelectTrack();
void CdAudioSetTarget(int a0, int a1, int a2);

void CdAudioSelectTrack(int a0, int a1) {
    register AudioState *g = &SoundState;
    register AudioState *s2;
    register int s1;
    unsigned s0;
    int a;
    int h;
    unsigned t2;
    int v2, tt;
    unsigned m;
    int *v;
    s2 = (AudioState *)((char *)g + a0 * 8);
    a = s2->streamTable2638;
    if (a != -1) {
        m = 0xFFF0000;
        s1 = a1 * 8;
        t2 = s1 + a;
        m |= 0xFFFF;
        s0 = *(unsigned *)t2;
        h = s0 >> 28;
        s0 &= m;
        s0 += s2->streamBase2634;
        CdAudioSetTarget(s0, h, 0);
        g->end2690 = (((CdTrackEntry8 *)(s1 + s2->streamTable2638))->length - 2) * 8;
        PsyqBzero(&g->tick26D0, 0x1C);
        { int b = D_800A099C; v = (int *)(b + a1 * 12); }
        g->reserved26EC = 0;
        g->base26F0 = 0;
        g->reserved26E0 = v[0];
        g->rate26F4 = v[1];
        tt = g->end2690;
        v2 = v[2];
        g->timer26A8 = 0;
        g->end2690 = tt + s0;
        g->offset26E8 = v2;
        g->targetVol26C0.word = D_800A0904[a1];
        CdAudioSetVolumeRaw(0);
    }
    PsyqSrand(FrameCounter);
}

void CdAudioSetTarget(int a0, int a1, int a2) {
    SoundState.sector2664 = a0;
    SoundState.filter266C = a1;
    SoundState.phase2674 = 0;
    if (a2 == 0) SoundState.reserved2684 = a0;
    SoundState.action2670 = -1;
    SoundState.step2678 = 0;
    if (SoundState.state269C == 2) CdAudioSetVolume(SoundState.volume26CE);
}
