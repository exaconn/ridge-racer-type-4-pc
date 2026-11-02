#include "rr/overlay_menu.h"
void ovl666_func_80115148(void)
{
    {
        RrUiRow24 *ui=D_800F4D80;
        s32 selected=D_800F3826;
        ui[2].cells[selected]=0;
    }
    if (D_800F3A42&0x2000) {
        if (D_800F3826<3) {
            D_800F3826++;
            D_8009FDA4=2;
            goto changed;
        }
    }
    if (D_800F3A42&0x8000) {
        if (D_800F3826>0) {
            D_800F3826--;
            D_8009FDA4=1;
            goto changed;
        }
    }
    D_8009FDA4=0;
    goto active;
changed:
    func_8004F24C(0);
    if (D_800F3826<0)D_800F3826=0;
    else if (D_800F3826>=4)D_800F3826=3;
    D_800F4D8C[D_800F3826].control02=-6;
    D_800F4D8C[D_800F3826].flags00=0;
    D_800F4D80[2].byte16=(u8)D_800F3826+8;
active:
    {
        RrUiRow24 *ui=D_800F4D80;
        s32 selected=D_800F3826;
        ui[2].cells[selected]=2;
    }
}
