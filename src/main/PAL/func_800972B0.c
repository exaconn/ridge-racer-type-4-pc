/* CC1_FLAGS: -mno-split-addresses */
#include "rr/bios_devices.h"
extern s32 func_80096404(const char *,const char *);
void func_800972B0(s32 *a0,s32 a1,s32 a2) {
    /* ASM_BARRIER: Keep the captured handler in its original saved register across name comparisons. */
    register RrBiosDeviceHandler handler __asm__("$19");
    unsigned long cursor,end,limit;
    if(*a0==0) *a0=1;
    cursor=(unsigned long)*(RrBiosDevice **)0x150;
    handler=D_801143B8;
    end=cursor+(*(u32 *)0x154/sizeof(RrBiosDevice))*sizeof(RrBiosDevice);
    if(cursor<end) {
        /* ASM_BARRIER: Keep the loop bound assignment after the initial table-range gate. */
        __asm__ volatile("" : : : "$17");
        limit=end;
        do {
            RrBiosDevice *device=(RrBiosDevice *)cursor;
            if(device->name) {
                /* ASM_BARRIER: Materialize the comparison name in its call argument each iteration. */
                register char *page __asm__("$5");
                /* ASM_BARRIER: Keep page materialization inside the nonnull-name branch. */
                __asm__ volatile("" : : : "$5");
                page=(char *)0x80110000U;
                /* ASM_BARRIER: Materialize the name page before its call-delay offset. */
                __asm__("" : "+r"(page));
                if(func_80096404(device->name,page+0x43C0)==0) {
                    device->handler=handler;
                    break;
                }
            }
            cursor+=sizeof(RrBiosDevice);
        } while(cursor<limit);
    }
    D_801143B8(a0,a1,a2);
}
