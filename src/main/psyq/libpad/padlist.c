#include "rr/channel_advance.h"
#include "rr/channels.h"

void _padResetChannel(void *raw) {
    PadChannel *channel=raw;
    s32 i;
    u8 *payload;
    u8 unassigned;
    if (channel->state49 != 0) {
        payload=channel->output54.fields.payload;
        unassigned=0xFF;
        i=5;
        channel->state49=0;
        channel->phase=0;
        channel->pending=0;
        channel->update14=0;
        channel->complete18=0;
        channel->modeCountE3=0;
        channel->argumentE4=0;
        channel->pending=0;
        channel->entry_countE9=0;
        channel->combCountEA=0;
        channel->modes00=0;
        channel->entries04=0;
        channel->combs08=0;
        do {
            *payload=unassigned;
            i--;
            payload++;
        } while (i >= 0);
    }
}

void _padAdvanceChannel(s32 command)
{
    s32 mode = command;
    enum { SKIP_CLEANUP = -9 };
    s32 current;
    SerialPortRegisters *control;
    s32 status;
    PadChannel *channel;
    do {
        channel = &PadChannelRecordArray[PadActiveChannel];
        if (mode != SKIP_CLEANUP) {
            if (mode == 0) {
                s32 *slot = &PadMultitapSlotIndex[PadActiveChannel];
                *slot = 0;
            } else {
                _dirFailAuto((u8 *)channel);
                _padSaveCommand(channel);
            }
        }
        control = D_800ABD00;
        current = PadActiveChannel;
        PadRecvStage = 0;
        control->control = 0;
        {
            s32 bound = PadLastChannel;
            current++;
            PadActiveChannel = current;
            if (bound >= current) {
                status = _padInitSioMode(&PadChannelRecordArray[current]);
                mode = 0xFFFF;
            } else {
                status = 1;
                mode = 0xFFFF;
            }
        }
    } while (!status);
}

void _padSaveCommand(PadChannel *channel) {
    u8 command=channel->command37;
    channel->command37=0;
    channel->previous38=command;
}

u32 _padGetTxByte(const PadChannel *channel, s32 transferMode) {
    u32 phase=channel->b45;
    s32 index;
    u32 command=channel->command37;
    const u8 *p;
    u32 result;
    index=phase-3;
    switch(command) {
    case 0:
        if(index<6 && channel->output54.fields.gates[index+3]==0) {result=0;goto out;}
        if(index>=channel->length34) {result=0;goto out;}
        p=channel->data28+index;
load:
        result=*p;
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
