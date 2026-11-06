/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channels.h"
s32 _padSetMainMode(RrChannelPatchCommand *raw,s32 first_raw,s32 second_raw) {
 /* ASM_BARRIER: Retain the channel and both original words across the readiness callback. */
 register RrChannelPatchCommand *channel __asm__("$16")=raw;
 register s32 first __asm__("$17")=first_raw;
 register s32 second __asm__("$18")=second_raw;
 register s32 copy __asm__("$19");
 register RrChannelStatusCallback ready;
 s32 result;

 /* ASM_BARRIER: Save arguments before loading readiness, leaving the saved comparison copy late. */
 __asm__("" : "+r"(channel) : "r"(first),"r"(second) : "$2", "$19");
 ready=D_800ABC84;

 copy=first;
 if(ready(raw))return 0;
 result=1;
 /* ASM_BARRIER: Separate success from byte constants and capture argument only after readiness. */
 __asm__("" : "+r"(result) : : "$4", "memory");
 {
  register u32 wanted=channel->argument;
  channel->phase=1;
  channel->update=func_80099564;
  channel->complete=func_800995B8;
  {
   u32 masked;
   masked=(u8)copy;
   channel->first=first;
   channel->second=second;
   channel->equal=masked==wanted;
   /* ASM_BARRIER: Preserve the unmasked saved word until the comparison has been written. */
   __asm__("" : : "r"(copy));
  }
 }
 return result;
}
