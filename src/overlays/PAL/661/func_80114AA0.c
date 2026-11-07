#include "rr/menu_screen_exit.h"
void ovl661_func_80114AA0(void)
{
    StopRaceAudioAndRumble();
    func_8003CAB4();
    ModeGroup = 1;
    D_800F3A26 = 38;
    D_800F4D8A = 0;
    AddScreenFadeTiles(((RrFrameOrderingPrefix *)CurrentDrawBuf)->ordering70, 255, 69);
}
