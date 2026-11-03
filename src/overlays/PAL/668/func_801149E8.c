#include "rr/overlay668_playback_init.h"

void ovl668_func_801149E8(void)
{
    RrStrCdFileInfo file;
    u32 sectors;
    RrStrPlaybackState *state;

    SetDispMask(0);
    func_8001ECE8(0, 0, 0, 5);
    state = D_800F4D40;
    D_800ACC18 = state;
    D_800AC650.activeB8 = state;
    D_800ACC18 = (u8 *)state + sizeof(*state);
    func_80096434(state, sizeof(*state));
    func_800929D4(3);
    func_8008C0D8(0, 0);
    CdControl(9, 0, 0);
    D_800AC650.phaseB4 = 0;
    D_800AC650.stopD0 = 0;
    ovl668_func_80114910(D_800AC650.resourceB0, &file, &sectors);
    D_800F4C6A = 2;
    D_800F3A26 = 4;
    D_800F4D8A = 0;
}
