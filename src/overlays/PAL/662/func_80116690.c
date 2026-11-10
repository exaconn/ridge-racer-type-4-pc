#include "rr/menu_ui_bank.h"
void ovl662_func_80116690(void)
{
    D_800AD520=SelectionRows.bytes[0];
    {
        UiRow24 *ui=UiRows;
        s32 selected=D_800AD520;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    do {
        if ((D_800F3A42 & 0x2000) && D_800AD520 < 3) {
            D_800AD520++;
            MenuStepDirection = 2;
        } else if ((D_800F3A42 & 0x8000) && D_800AD520 > 0) {
            D_800AD520--;
            MenuStepDirection = 1;
        } else {
            MenuStepDirection = 0;
            break;
        }
        PlaySfx(0);
        if (D_800AD520<0)D_800AD520=0;
        else if (D_800AD520>=4)D_800AD520=3;
        SelectionRows.bytes[0]=(u8)D_800AD520;
        GlyphRows[D_800AD520].control02=-6;
        GlyphRows[D_800AD520].flags00=0;
    } while (0);
    UiRows->byte16=(u8)D_800AD520;
    {
        UiRow24 *ui=UiRows;
        s32 selected=D_800AD520;
        ((MenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
