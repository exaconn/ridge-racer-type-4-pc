#include "rr/menu_ui_bank.h"
void ovl662_step_selected_team(void)
{
    SelectedTeamIndex=SelectionRows.bytes[0];
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedTeamIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    if ((MenuSelectedCarSlot & 0x2000) && SelectedTeamIndex < 3) {
        SelectedTeamIndex++;
        MenuStepDirection = 2;
    } else if ((MenuSelectedCarSlot & 0x8000) && SelectedTeamIndex > 0) {
        SelectedTeamIndex--;
        MenuStepDirection = 1;
    } else {
        MenuStepDirection = 0;
        goto skip_step;
    }
    PlaySfx(0);
    if (SelectedTeamIndex<0)SelectedTeamIndex=0;
    else if (SelectedTeamIndex>=4)SelectedTeamIndex=3;
    SelectionRows.bytes[0]=(u8)SelectedTeamIndex;
    GlyphRows[SelectedTeamIndex].control02=-6;
    GlyphRows[SelectedTeamIndex].flags00=0;
skip_step:;
    UiRows->byte16=(u8)SelectedTeamIndex;
    {
        UiRow24 *ui=UiRows;
        s32 selected=SelectedTeamIndex;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
