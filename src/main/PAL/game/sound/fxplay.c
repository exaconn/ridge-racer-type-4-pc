#include "types.h"
#include "rr/audio_state_types.h"
#include "game/camera/campath.h"
#include "game/sound/sndvab.h"
#include "psyq/libsnd/ssopenq.h"
#include "game/sound/fxramp2.h"
#include "psyq/libsnd/sspause.h"
#include "psyq/libsnd/sspause2.h"
#include "psyq/libspu/smalloc2.h"
#include "rr/bios_devices.h"
#include "rr/overlay665_audio_options.h"
#include "game/sound/cdmain.h"

extern struct { s32 a, b, c; } ReplayCamTargetDistTable[];
extern s16 ActiveSfxSeqHandle;

void SeqStartTrack(int a0);
void SeqMuteVolume(void);
void SeqMusicPause(void);
void SeqMusicResume(void);

void SeqStartTrack(int a0) {
    int arr[2] = {0x6C, 0x7F};
    if (SoundState.seqTrack2868 != a0) {
        SeqMuteVolume();
        SoundState.seqTrack2868 = a0;
        SoundState.seqHandle2864 = (s16)SsSeqOpen(SoundState.seqAddr2858[a0], SoundState.banks24.ids[a0 + 2]);
        SsSetMarkCallback((s16)SoundState.seqHandle2864, 0, SeqMarkCallback);
        ReverbReset();
        SetReverbDepthBoth(0x32);
        SsSeqPlay((s16)SoundState.seqHandle2864, 1, 0);
        SeqSetVolume((s16)arr[a0]);
        PsyqBzero(&SoundState.seqTick286C, 0x1C);
        SoundState.seqTempo287C = ReplayCamTargetDistTable[a0].a;
        RestartMotionSequence();
    }
}

void SeqMuteVolume(void) {
    SeqFadeVolume(0, 0);
}

void SeqMusicPause(void) {
    SsSeqPause(ActiveSfxSeqHandle);
}

void SeqMusicResume(void) {
    SsSeqReplay(ActiveSfxSeqHandle);
}
