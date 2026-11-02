#include "rr/menu_ui_bank.h"
void ovl662_func_80116690(void)
{
    D_800AD520=D_800F4D98.bytes[0];
    {
        RrUiRow24 *ui=D_800F4D80;
        s32 selected=D_800AD520;
        ((RrMenuUiBank *)ui)->cell_view.cells0C[selected]=0;
    }
    if (D_800F3A42&0x2000) {
        if (D_800AD520<3) {
            D_800AD520++;
            D_8009FDA4=2;
            goto changed;
        }
    }
    if (D_800F3A42&0x8000) {
        if (D_800AD520>0) {
            D_800AD520--;
            D_8009FDA4=1;
            goto changed;
        }
    }
    D_8009FDA4=0;
    goto active;
changed:
    func_8004F24C(0);
    if (D_800AD520<0)D_800AD520=0;
    else if (D_800AD520>=4)D_800AD520=3;
    D_800F4D98.bytes[0]=(u8)D_800AD520;
    D_800F4D8C[D_800AD520].control02=-6;
    D_800F4D8C[D_800AD520].flags00=0;
active:
    D_800F4D80->byte16=(u8)D_800AD520;
    {
        RrUiRow24 *ui=D_800F4D80;
        s32 selected=D_800AD520;
        ((RrMenuUiBank *)ui)->cell_view.cells0C[selected]=2;
    }
}
