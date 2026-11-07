#include "rr/overlay662_actions.h"
void ovl662_func_8011DCDC(u32 flags){
 if(flags)PlaySfx(0);
 if(flags&1)ovl662_D_80127470[ovl662_D_801276A8.current60]->callback(1);
 else if(flags&2)ovl662_D_80127470[ovl662_D_801276A8.current60]->callback(2);
 if(flags&4)ovl662_D_80127470[ovl662_D_801276A8.current60]->callback(4);
 else if(flags&8)ovl662_D_80127470[ovl662_D_801276A8.current60]->callback(8);
}
