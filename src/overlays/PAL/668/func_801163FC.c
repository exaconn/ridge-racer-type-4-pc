/* GCC_VERSION: 2.7.2 */
#include "rr/overlay668_control.h"
void ovl668_mdec_in(StrCommandHeader *header,u32 flags){
 if(flags&1)header->word&=~0x08000000U;else header->word|=0x08000000U;
 if(flags&2)header->word|=0x02000000U;else header->word&=~0x02000000U;
 ovl668_mdec_start_in_dma(header,header->halves[0]);
}
