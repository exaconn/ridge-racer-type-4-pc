#include "rr/overlay668_decode.h"
#include "rr/cd.h"
void StUnSetRing(void);
#include "rr/frame_loop.h"
void ovl668_func_80114DF4(void){
 ovl668_func_80116534(0);
 StUnSetRing();
 CdControlB(9,0,0);
 D_800F4C6A=1;D_800F3A26=3;D_800F4D8A=0;
}
