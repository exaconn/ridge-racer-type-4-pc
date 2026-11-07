#include "rr/overlay668_playback_init.h"

void ovl668_func_801149E8(void)
{
    RrStrCdFileInfo file;
    u32 sectors;
    RrStrPlaybackState *state;

    SetDispMask(0);
    func_8001ECE8(0, 0, 0, 5);
    state = D_800F4D40;
    LoadCursor = state;
    RaceContext.activeB8 = state;
    LoadCursor = (u8 *)state + sizeof(*state);
    func_80096434(state, sizeof(*state));
    ResetGraph(3);
    CdSync(0, 0);
    CdControl(9, 0, 0);
    RaceContext.phaseB4 = 0;
    RaceContext.stopD0 = 0;
    ovl668_func_80114910(RaceContext.resourceB0, &file, &sectors);
    D_800F4C6A = 2;
    D_800F3A26 = 4;
    D_800F4D8A = 0;
}
