/* CC1_FLAGS: -mno-split-addresses */

#include "rr/channels.h"

#include "rr_asm.h"

extern void func_8009A7F4(s32);

extern s32 func_80098A00(void);

void func_80098488(RrChannelRecordF0 *channel) {
    s32 *stage=&D_800ABCAC;
    RrChannelStageCallback callback=D_800ABCEC[*stage];
    s32 result,value;
    *stage+=1;
    result=callback(channel);
    if(result<0)goto error;
    value=D_800ABCAC;
    if(value) {
        if(value==3 && channel->status.packet[0]==0x80)goto reload;
        func_8009A7F4(60);
        if(!func_80098A00())D_800ABC68(-3);
reload:
        value=D_800ABCAC;
        /* ASM_BARRIER: Keep the reload gap before the shared saturation label. */
        RR_NOP_DEP_ON(value);
    }
    if(value>=5) {
        s32 decremented=value-1;
        /* ASM_BARRIER: Form the decremented stage before constructing its PAL page. */
        __asm__("" : "+r"(decremented) : : "$1");
        {
            /* ASM_BARRIER: Retain the real PAL stage page in the assembler temporary. */
            register s32 *page __asm__("$1")=(s32 *)0x800B0000U;
            /* ASM_BARRIER: Expose the stage store for the final jump's delay slot. */
            __asm__("" : "+r"(page));
            page[-0x4354/4]=decremented;
        }
    }
    goto out;
error:
    D_800ABC68(result);
out:;
}
