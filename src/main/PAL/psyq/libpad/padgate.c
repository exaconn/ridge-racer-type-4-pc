/* CC1_FLAGS: -mno-split-addresses */
#include "rr/channel_init.h"
#include "psyq/libpad/padgate.h"
void PadInitDirect(u8 *first,u8 *second) {
    register PadChannel *record;
    /* ASM_BARRIER: preserve the first input pointer in S1 across setup helpers. */
    register u8 *input_first __asm__("$17")=first;
    /* ASM_BARRIER: preserve the second input pointer in S2 across setup helpers. */
    register u8 *input_second __asm__("$18")=second;
    u8 *buffer_a,*buffer_b;
    PadChannel *metadata;
    /* ASM_BARRIER: form the metadata end address in A0 before the buffer addresses. */
    register u8 **slots __asm__("$4");
    s32 i,j;
    /* ASM_BARRIER: retain the shared byte sentinel in T1 for both loops. */
    register s32 sentinel __asm__("$9");
    u8 *payload;
    register void (*callback)(PadChannel *);
    PadComActive=0;
    PadMultitapEnabled=0;
    _padInitDirSeq();
    record=PadChannelRecordArray;
    PadAdvanceChannelFn=_padAdvanceChannel;
    PadResetChannelFn=_padResetChannel;
    PadTxByteCallback=_padGetTxByte;
    PadAllocGatesFn=_padAllocGates;
    PadChannelToPortFn=_padChannelToPort;
    PadPortToChannelFn=_padPortToChannel;
    callback=_padSaveCommand;
    /* ASM_BARRIER: defer the clear-call A0 argument until the callback address is ready. */
    __asm__("" : : "r"(callback) : "$4");
    PadChannelRecords=PadChannelRecordArray;
    PadSaveCommandCallback=callback;
    PsyqBzero(PadChannelRecordArray,2*sizeof(PadChannel));
    i=0;
    sentinel=255;
    metadata=record;
    slots=&metadata->buffer40;
    /* ASM_BARRIER: materialize the sentinel and A0 metadata address before the buffers. */
    __asm__("" : "=r"(sentinel) : "0"(sentinel), "r"(slots));
    buffer_b=PadTxBuffers;
    buffer_a=D_80114448;
    record[0].input30=input_first;
    record[1].input30=input_second;
    do {
        payload=record->output54.fields.payload;
        metadata->children0C=0;
        metadata->owner10=record;
        metadata->input30[0]=sentinel;
        metadata->input30[1]=0;
        metadata->packet=buffer_a;
        metadata->buffer40=buffer_b;
        j=5;
        do { *payload=sentinel;j--;payload++; } while(j>=0);
        buffer_b+=0x23;
        buffer_a+=0x23;
        i++;
        metadata=(PadChannel *)((u8*)metadata+sizeof(PadChannel));
        record++;
    } while(i<2);
    _padSetVsyncParam();
    PadComActive=1;
    /* ASM_BARRIER: finish ready publication before the return-register restore. */
    __asm__("" : : : "$31");
}
