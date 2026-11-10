#include "rr/menu_ui_bank.h"
void ovl662_func_80116808(void)
{
    D_800F3826=D_800F4D99;
    {
        UiRow24 *ui=UiRows;
        s32 selected=D_800F3826;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    do {
        if ((D_800F3A42 & 0x2000) && D_800F3826 < 3) {
            D_800F3826++;
            MenuStepDirection = 2;
        } else if ((D_800F3A42 & 0x8000) && D_800F3826 > 0) {
            D_800F3826--;
            MenuStepDirection = 1;
        } else {
            MenuStepDirection = 0;
            break;
        }
        PlaySfx(0);
        if (D_800F3826<0)D_800F3826=0;
        else if (D_800F3826>=4)D_800F3826=3;
        D_800F4D99=(u8)D_800F3826;
        GlyphRows[D_800F3826].control02=-6;
    } while (0);
    UiRows->byte16=(u8)D_800F3826+4;
    {
        UiRow24 *ui=UiRows;
        s32 selected=D_800F3826;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
