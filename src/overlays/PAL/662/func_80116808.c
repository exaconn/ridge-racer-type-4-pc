#include "rr/menu_ui_bank.h"
void ovl662_func_80116808(void)
{
    SelectedMakerIndex=D_800F4D99;
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedMakerIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    do {
        if ((D_800F3A42 & 0x2000) && SelectedMakerIndex < 3) {
            SelectedMakerIndex++;
            MenuStepDirection = 2;
        } else if ((D_800F3A42 & 0x8000) && SelectedMakerIndex > 0) {
            SelectedMakerIndex--;
            MenuStepDirection = 1;
        } else {
            MenuStepDirection = 0;
            break;
        }
        PlaySfx(0);
        if (SelectedMakerIndex<0)SelectedMakerIndex=0;
        else if (SelectedMakerIndex>=4)SelectedMakerIndex=3;
        D_800F4D99=(u8)SelectedMakerIndex;
        GlyphRows[SelectedMakerIndex].control02=-6;
    } while (0);
    UiRows->byte16=(u8)SelectedMakerIndex+4;
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedMakerIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
