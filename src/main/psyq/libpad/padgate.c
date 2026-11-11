#include "rr/channel_init.h"
#include "psyq/libpad/padgate.h"
void PadInitDirect(u8 *first,u8 *second) {
    PadChannel *record;
    u8 *input_first=first;
    u8 *input_second=second;
    s32 i,j;
    u8 *payload;
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
    PadChannelRecords=record;
    PadSaveCommandCallback=_padSaveCommand;
    PsyqBzero(record,2*sizeof(PadChannel));
    i=0;
    record[0].input30=input_first;
    record[1].input30=input_second;
    do {
        payload=record->output54.fields.payload;
        record->children0C=0;
        record->owner10=record;
        record->input30[0]=255;
        record->input30[1]=0;
        record->packet=D_80114448+i*0x23;
        record->buffer40=PadTxBuffers+i*0x23;
        j=5;
        do { *payload=255;j--;payload++; } while(j>=0);
        i++;
        record++;
    } while(i<2);
    _padSetVsyncParam();
    PadComActive=1;
}
