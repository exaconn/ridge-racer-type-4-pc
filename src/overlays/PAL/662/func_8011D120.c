#include "rr/overlay662_garage.h"
void ovl662_func_8011D120(void)
{
    s32 i;
    u8 selected=ovl662_D_801276A8.active21;
    u8 other;
    u32 *source,*destination;
    ovl662_D_801276A8.dirtyD3=1;
    other=!selected;
    source=ovl662_D_801276A8.image38[selected];
    destination=ovl662_D_801276A8.image38[other];
    for(i=0;i<512;){i++;*destination++=*source++;}
    source=ovl662_D_801276A8.image40[selected];
    destination=ovl662_D_801276A8.image40[other];
    for(i=0;i<512;){i++;*destination++=*source++;}
    source=ovl662_D_801276A8.palette48[selected];
    destination=ovl662_D_801276A8.palette48[other];
    for(i=0;i<8;){i++;*destination++=*source++;}
}
