#include "rr/menu_screen_exit.h"
#include "rr/frame_storage_types.h"
void ovl661_exit_race(void)
{
    StopRaceAudioAndRumble();
    SetupRaceAndSceneId();
    ModeGroup = 1;
    ModeIndex = 38;
    SubmodeState = 0;
    AddScreenFadeTiles(CurrentDrawBuf->buffer.ordering70.words, 255, 69);
}
