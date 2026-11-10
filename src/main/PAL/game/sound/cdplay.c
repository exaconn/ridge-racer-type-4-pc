#include "types.h"
#include "game/sound/cdplay.h"
#include "rr/audio_cd_controller.h"
#include "rr/frame_loop.h"
#include "rr/bios_devices.h"

extern int RaceOverlayFilePath[][3];
extern int StageUnlockMovieIds[];
extern int PsyqSrand();


void CdAudioSelectTrack(int bank, int track) {
    AudioState *state = &SoundState;
    u32 sector;
    u8 *table;
    int filter;
    int offset, end;
    const s32 *timing;
    table = state->streamBanks[bank].table;
    if (table != AUDIO_STREAM_TABLE_UNAVAILABLE) {
        sector = ((AudioStreamEntry *)table)[track].position;
        filter = sector >> 28;
        sector &= 0x0FFFFFFF;
        sector += state->streamBanks[bank].sector;
        CdAudioSetTarget(sector, filter, 0);
        state->end2690 = (((AudioStreamEntry *)state->streamBanks[bank].table)[track].sectors - 2U) * 8;
        PsyqBzero(&state->tick26D0, 0x1C);
        { const s32 *base = RaceOverlayFilePath[0]; timing = base + track * 3; }
        state->reserved26EC = 0;
        state->base26F0 = 0;
        state->reserved26E0 = timing[0];
        state->rate26F4 = timing[1];
        end = state->end2690;
        offset = timing[2];
        state->timer26A8 = 0;
        state->end2690 = end + sector;
        state->offset26E8 = offset;
        state->targetVol26C0.word = StageUnlockMovieIds[track];
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
