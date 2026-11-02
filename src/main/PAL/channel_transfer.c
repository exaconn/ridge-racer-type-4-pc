/* GCC_VERSION: 2.7.2 */
#include "rr/channels.h"
void func_80099F90(RrChannelTransferView *channel) {
    u8 command=channel->command37;
    channel->command37=0;
    channel->previous38=command;
}

u32 func_80099FA0(const RrChannelTransferView *channel) {
    u32 phase=channel->phase45;
    s32 index;
    u32 command=channel->command37;
    const u8 *p;
    u32 result;
    index=(s32)phase-3;
    switch(command) {
    case 0:
        if(index<6 && channel->gates54[index+3]==0) {result=0;goto out;}
        if(index>=channel->length34) {result=0;goto out;}
        p=channel->data28+index;
load:
        result=*p;
        /* ASM_BARRIER: Retain the separate shared-load return for command zero and M. */
        __asm__ volatile("" : : "r"(result) : "memory");
        goto out;
    case 0x4D:
        if(index>=channel->length36) {result=255;goto out;}
        p=channel->data2C+index;
        goto load;
    default:
        if(index>=channel->length36) {result=0;goto out;}
        result=channel->data2C[index];
    }
out:
    return result;
}
