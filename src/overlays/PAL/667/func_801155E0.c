#include "rr/overlay_screen_exit.h"
#include "rr/frame_storage_types.h"
void ovl667_func_801155E0(void)
{
    StopRaceAudioAndRumble();
    SetupRaceAndSceneId();
    ModeGroup=1;
    D_800F3A26=26;
    D_800F4D8A=0;
    AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words,255,69);
}
