/* CC1_FLAGS: -mno-split-addresses */

#include "rr/channel_init.h"

void PadInitDirect(u8 *first,u8 *second) {
    register RrChannelRecordF0 *record;
    /* ASM_BARRIER: preserve the first input pointer in S1 across setup helpers. */
    register u8 *input_first __asm__("$17")=first;
    /* ASM_BARRIER: preserve the second input pointer in S2 across setup helpers. */
    register u8 *input_second __asm__("$18")=second;
    u8 *buffer_a,*buffer_b;
    RrChannelStatusView *metadata;
    /* ASM_BARRIER: form the metadata end address in A0 before the buffer addresses. */
    register u8 **slots __asm__("$4");
    s32 i,j;
    /* ASM_BARRIER: retain the shared byte sentinel in T1 for both loops. */
    register s32 sentinel __asm__("$9");
    u8 *payload;
    register void (*callback)(RrChannelTransferView *);
    D_800ABCA0=0;
    D_800ABCB4=0;
    _padInitDirSeq();
    record=D_801144D8;
    D_800ABC68=func_80099E9C;
    D_800ABC6C=func_80099E34;
    D_800ABC70=func_80099FA0;
    D_800ABC74=func_8009A05C;
    D_800ABC78=func_8009A2D8;
    D_800ABC7C=func_8009A310;
    callback=func_80099F90;
    /* ASM_BARRIER: defer the clear-call A0 argument until the callback address is ready. */
    __asm__("" : : "r"(callback) : "$4");
    D_800ABC9C=D_801144D8;
    D_800ABC8C=callback;
    func_80096434(D_801144D8,2*sizeof(RrChannelRecordF0));
    i=0;
    sentinel=255;
    metadata=&record->status;
    slots=&metadata->buffer40;
    /* ASM_BARRIER: materialize the sentinel and A0 metadata address before the buffers. */
    __asm__("" : "=r"(sentinel) : "0"(sentinel), "r"(slots));
    buffer_b=D_80114490;
    buffer_a=D_80114448;
    record[0].status.input30=input_first;
    record[1].status.input30=input_second;
    do {
        payload=record->status.payload5D;
        metadata->reset0C=0;
        metadata->owner10=&record->status;
        metadata->input30[0]=sentinel;
        metadata->input30[1]=0;
        metadata->packet=buffer_a;
        metadata->buffer40=buffer_b;
        j=5;
        do { *payload=sentinel;j--;payload++; } while(j>=0);
        buffer_b+=0x23;
        buffer_a+=0x23;
        i++;
        metadata=(RrChannelStatusView *)((u8*)metadata+sizeof(RrChannelRecordF0));
        record++;
    } while(i<2);
    _padSetVsyncParam();
    D_800ABCA0=1;
    /* ASM_BARRIER: finish ready publication before the return-register restore. */
    __asm__("" : : : "$31");
}
