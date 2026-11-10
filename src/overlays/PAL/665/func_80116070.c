#include "rr/overlay665_menu.h"
void ovl665_init_controller_options(void) {
    ClearScreenOverlayState();
    InitUiRow(0,56,160,64,32,0,0,0,1,22);
    InitUiRow(1,128,160,64,32,0,0,0,0,109);
    InitUiRow(2,200,160,64,32,0,0,0,1,23);
    InitUiRow(3,92,360,64,32,0,0,0,0,110);
    InitUiRow(4,164,360,64,32,0,0,0,0,111);
    InitUiRow(5,128,80,52,32,0,0,1,0,106);
    InitUiRow(6,200,80,28,32,0,0,1,0,107);
    InitUiRow(7,200,120,28,32,0,0,1,0,108);
    ScreenFadeState[0]=2;
    InitBadgeRow(0,16,96,76,40,0,0xD01);
    ovl665_saved_stereo_mode=0;
    BadgeRows[0].half02=1;
    ovl665_options_state.view74.position00.words[2]=0;
    ovl665_options_state.view74.position00.words[1]=0;
    ovl665_options_state.view74.position00.words[0]=0;
    ovl665_options_state.view74.rotation14=0;
    ovl665_options_state.view74.rotation12=0;
    ovl665_options_state.view74.rotation10=0;
    ovl665_options_state.reset2C=0;
    SetMenuFrameOptions(1,1,0);
}
