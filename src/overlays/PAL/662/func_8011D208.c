#include "rr/overlay662_garage.h"
#include "rr/trig.h"
void ovl662_func_8011D208(void)
{
    ovl662_D_801276A8.shadeD0=rsin(ovl662_D_801276A8.phaseCC)/64-65;
    ovl662_D_801276A8.phaseCC+=96U;
    ovl662_D_801276A8.phaseCC=(s32)ovl662_D_801276A8.phaseCC%4096;
}
