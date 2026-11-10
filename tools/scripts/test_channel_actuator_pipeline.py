"""Actual game-to-SDK actuator flow through one complete native channel owner."""
from pathlib import Path
import subprocess
from tempfile import TemporaryDirectory
import unittest
from c_functions import source_owners, definition_source

ROOT = Path(__file__).resolve().parents[2]
FUNCTIONS = ['_padChannelToPort', '_padPortToChannel', 'PadGetState', 'PadInfoMode', 'PadInfoAct', 'PadSetAct', 'PadSetActAlign', '_padSetAct', '_padSetActAlign', '_padActAlignUpdate', '_padActAlignComplete', '_padChannelBusy', '_padGetTxByte', 'PadSetRumbleFlags', 'PadPollPort']
SETUP = r'''
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "rr/channel_init.h"
#include "game/system/pad.h"
#include "game/system/pad3.h"
#include "psyq/libpad/padctl.h"
#include "psyq/libpad/padstat2.h"
#include "psyq/libpad/padpatch.h"

PadChannel PadChannelRecordArray[2];
PadChannel *PadChannelRecords=PadChannelRecordArray;
ChannelRecordCallback PadPortToChannelFn=_padPortToChannel;
ChannelStatusCallback PadChannelBusyFn=_padChannelBusy;
s32 PadMultitapEnabled;
ControllerState5C PlayerControllers[2];
u8 D_8009FFDC[6]={0,0,255,255,255,255};
u8 PadActAlignBytes[6]={0,255,255,255,255,255};
static u8 inputs[2][8];
static ChannelGateEntry5 descriptions[2][2];
static s32 clears;
void PsyqBzero(void *p,s32 count) {
    assert(p==&PlayerControllers[0].flag40 || p==&PlayerControllers[1].flag40);
    assert(count==0x1C);clears++;memset(p,0,count);
}
'''
CHECKS = r'''
int main(void) {
    int bank,wheel,busy,minimum,slot,phase,states;
    unsigned cases=0;
    for(bank=0;bank<2;bank++)for(wheel=0;wheel<2;wheel++)
    for(busy=0;busy<2;busy++)for(minimum=0;minimum<7;minimum++) {
        PadChannel expected[2];ControllerState5C expectedPads[2];
        PadChannel *channel=&PadChannelRecordArray[bank];
        ControllerState5C *pad=&PlayerControllers[bank],*want=&expectedPads[bank];
        memset(PadChannelRecordArray,0,sizeof(PadChannelRecordArray));
        memset(PlayerControllers,0xA5,sizeof(PlayerControllers));
        for(slot=0;slot<2;slot++) {
            PadChannelRecordArray[slot].owner10=&PadChannelRecordArray[slot];
            PadChannelRecordArray[slot].input30=inputs[slot];
            PadChannelRecordArray[slot].entries04=descriptions[slot];
            PadChannelRecordArray[slot].state49=6;
            PadChannelRecordArray[slot].pending=wheel?0x300:1;
            PadChannelRecordArray[slot].responseE8=wheel?14:7;
            PadChannelRecordArray[slot].phase=busy?1:255;
            PadChannelRecordArray[slot].entry_countE9=1;
            memset(PadChannelRecordArray[slot].output54.fields.gates,1,9);
            descriptions[slot][0].opaque00[0]=4;
            descriptions[slot][0].opaque00[1]=3;
            descriptions[slot][0].all_bits02=minimum;
            PlayerControllers[slot].flag1B=0;
            PlayerControllers[slot].changed1E=0;
        }
        memcpy(expected,PadChannelRecordArray,sizeof(expected));
        memcpy(expectedPads,PlayerControllers,sizeof(expectedPads));
        want->kind1C=1;
        if(wheel) {
            memset(&want->flag40,0,0x1C);want->flag40=1;want->protocol10=2;
        } else {want->flag40=1;want->protocol10=0;}
        expected[bank].data28=wheel?&pad->actuator44:pad->output19;
        expected[bank].length34=wheel?1:2;
        if(!busy) {
            expected[bank].phase=1;
            expected[bank].update14=_padActAlignUpdate;
            expected[bank].complete18=_padActAlignComplete;
            expected[bank].layout20=wheel?PadActAlignBytes:D_8009FFDC;
            want->flag1B=1;want->changed1E=0;
            if(wheel)want->rumbleFlags48=0xC0;
        }
        clears=0;PadPollPort(bank);
        assert(clears==wheel);
        assert(!memcmp(expected,PadChannelRecordArray,sizeof(expected)));
        assert(!memcmp(expectedPads,PlayerControllers,sizeof(expectedPads)));
        /* The actual byte transmitter consumes the actual game-owned output. */
        for(phase=3;phase<10;phase++) {
            channel->b45=phase;
            assert(_padGetTxByte(channel,0)==(phase-3<channel->length34?channel->data28[phase-3]:0));
        }
        if(!busy) {
            assert(channel->update14 && channel->complete18);
            channel->update14(channel);
            assert(channel->data2C==channel->layout20 && channel->length36==6 && channel->command37==0x4D);
            for(phase=3;phase<11;phase++) {
                channel->b45=phase;
                assert(_padGetTxByte(channel,0)==(phase<9?channel->layout20[phase-3]:255));
            }
            assert(channel->complete18(channel)==0 && channel->phase==254);
            for(slot=0;slot<6;slot++) {
                u8 group=channel->layout20[slot];
                int count=wheel?1:2;
                if(group==0)assert(channel->output54.fields.payload[slot]==(minimum>count?255:0));
                else assert(channel->output54.fields.payload[slot]==0);
            }
        }
        cases++;
    }
    /* Forwarding is observable through distinct bank state bytes. */
    for(states=0;states<256;states++) {
        for(bank=0;bank<2;bank++) {
            PadChannel *channel=&PadChannelRecordArray[bank];
            channel->state49=states;channel->command37=0;channel->previous38=0;
            channel->owner10=channel;channel->input30=inputs[bank];inputs[bank][0]=0;
            assert(PadGetState(bank*16)==states);
            channel->command37=1;
            assert(PadGetState(bank*16)==(states==2||states==3?1:states==6?4:states));
        }
    }
    printf("%u complete game/SDK actuator flows and 1024 state-forwarding cases passed\n",cases);
    return 0;
}
'''

class ChannelActuatorPipelineTest(unittest.TestCase):
    def test_real_game_caller_sdk_setters_callbacks_and_byte_transmitter(self):
        owners = source_owners(ROOT/'src/main/PAL')
        source = SETUP + '\n'.join(definition_source(owners[name].read_text(), name)
                                   for name in FUNCTIONS) + CHECKS
        with TemporaryDirectory() as directory:
            c = Path(directory)/'pipeline.c'
            executable = Path(directory)/'pipeline'
            c.write_text(source)
            result = subprocess.run([
                'cc', '-std=gnu99', '-O2', '-Wall', '-Wextra', '-Werror',
                '-Wno-deprecated-non-prototype', '-Wno-unused-parameter',
                '-fsanitize=address,undefined', '-fno-sanitize-recover=all',
                '-I', str(ROOT/'include'), str(c), '-o', str(executable)
            ], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            result = subprocess.run([str(executable)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            self.assertIn('56 complete game/SDK actuator flows', result.stdout)
