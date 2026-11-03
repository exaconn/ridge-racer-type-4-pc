/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

void func_8008BDBC(RrLibInterruptCallback *callbacks, s32 count) {
    u32 remaining=(u32)count-1;
    if(count) {
        do {
            *callbacks=0;
            remaining--;
            callbacks++;
        } while(remaining!=0xFFFFFFFFU);
    }
}
