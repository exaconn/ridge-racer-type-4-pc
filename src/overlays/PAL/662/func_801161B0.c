#include "rr/frame_loop.h"
void ovl662_setup_history_arenas(void);
void UpdateScreenFade(s32 command);
void ovl662_enter_history_screen(void)
{
    ovl662_setup_history_arenas();
    ModeGroup=1;
    ModeIndex=66;
    SubmodeState=0;
    UpdateScreenFade(0x10002);
}
