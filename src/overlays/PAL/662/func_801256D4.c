#include "rr/overlay662_garage.h"
void PlaySfx(s32 mode);
void ovl662_func_801256D4(void)
{
    if(ovl662_D_80127668.words[1]&2){
        PlaySfx(1);
        ovl662_D_801276A8.active21=!ovl662_D_801276A8.active21;
    }
}
