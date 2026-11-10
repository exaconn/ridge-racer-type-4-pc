/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_control.h"
s32 ovl668_mdec_in_sync(s32 mode){s32 result;if(mode==0)result=ovl668_mdec_wait_in_dma();else result=(ovl668_mdec_read_status()>>29)&1;return result;}
