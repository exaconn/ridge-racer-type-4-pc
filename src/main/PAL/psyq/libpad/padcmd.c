/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
s32 _padSetMainMode(PadChannel *raw,s32 first_raw,s32 second_raw) {
 /* ASM_BARRIER: Retain the channel and both original words across the readiness callback. */
 register PadChannel *channel __asm__("$16")=raw;
 register s32 first __asm__("$17")=first_raw;
 register s32 second __asm__("$18")=second_raw;
 register s32 copy __asm__("$19");
 register ChannelStatusCallback ready;
 s32 result;

 /* ASM_BARRIER: Save arguments before loading readiness, leaving the saved comparison copy late. */
 __asm__("" : "+r"(channel) : "r"(first),"r"(second) : "$2", "$19");
 ready=PadChannelBusyFn;

 copy=first;
 if(ready(raw))return 0;
 result=1;
 /* ASM_BARRIER: Separate success from byte constants and capture argument only after readiness. */
 __asm__("" : "+r"(result) : : "$4", "memory");
 {
  register u32 wanted=channel->argumentE4;
  channel->phase=1;
  channel->update14=_padMainModeUpdate;
  channel->complete18=_padMainModeComplete;
  {
   u32 masked;
   masked=(u8)copy;
   channel->modeArgs51[0]=first;
   channel->modeArgs51[1]=second;
   channel->modeEqual53=masked==wanted;
   /* ASM_BARRIER: Preserve the unmasked saved word until the comparison has been written. */
   __asm__("" : : "r"(copy));
  }
 }
 return result;
}
