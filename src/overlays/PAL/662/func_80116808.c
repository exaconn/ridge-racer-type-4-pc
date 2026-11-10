#include "rr/menu_ui_bank.h"
void ovl662_step_selected_maker(void)
{
    SelectedMakerIndex=ActiveSelectionRowValue;
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedMakerIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    if ((MenuSelectedCarSlot & 0x2000) && SelectedMakerIndex < 3) {
        SelectedMakerIndex++;
        MenuStepDirection = 2;
    } else if ((MenuSelectedCarSlot & 0x8000) && SelectedMakerIndex > 0) {
        SelectedMakerIndex--;
        MenuStepDirection = 1;
    } else {
        MenuStepDirection = 0;
        goto skip_step;
    }
    PlaySfx(0);
    if (SelectedMakerIndex<0)SelectedMakerIndex=0;
    else if (SelectedMakerIndex>=4)SelectedMakerIndex=3;
    ActiveSelectionRowValue=(u8)SelectedMakerIndex;
    GlyphRows[SelectedMakerIndex].control02=-6;
skip_step:;
    UiRows->byte16=(u8)SelectedMakerIndex+4;
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedMakerIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
