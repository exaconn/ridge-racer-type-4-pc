#include "rr/diagnostics.h"
#include "rr/overlay662_garage.h"
#include "rr/menu_setup.h"
extern const char ovl662_str_enter_editor_clean[];
extern s16 ModeGroup,ModeIndex,SubmodeState;
void InitUiRow(s32,s32,s32,s32,s32,s32,s32,s32,s32,s32,...);
void ovl662_enter_editor_clean(void) {
    DebugPrintStub(ovl662_str_enter_editor_clean);
    ovl662_setup_editor_buffers(0);
    ovl662_editor_workspace.dirtyD3 = 0;
    ClearScreenOverlayState();
    InitBadgeRow(0, 12, 304, 296, 88, 0, 0);
    InitUiRow(0, 140, 136, 28, 32, 0, 0, 1, 0, 104);
    InitUiRow(1, 140, 176, 28, 32, 0, 0, 1, 0, 105);
    ModeGroup = 3;
    ModeIndex = 4;
    SubmodeState = 0;
}
