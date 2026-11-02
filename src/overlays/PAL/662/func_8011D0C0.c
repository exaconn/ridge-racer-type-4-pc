#include "rr/overlay662_garage.h"
s32 ovl662_func_8011D0C0(void)
{
    u32 mode=ovl662_D_801276A8.layer23;
    u32 selected=ovl662_D_801276A8.active21;
    u32 *cursor;
    s32 i;
    if(mode==0)
        cursor=ovl662_D_801276A8.image38[selected];
    else
        cursor=ovl662_D_801276A8.image40[selected];
    for(i=0;i<512;cursor++){i++;if(*cursor)return 0;}
    return 1;
}
