#ifndef RR_CHANNEL_INIT_H
#define RR_CHANNEL_INIT_H
#include "rr/channels.h"
#include "psyq/libpad/padgate.h"
#include "psyq/libpad/padstop.h"
#include "rr/channel_advance.h"
#include "rr/bios_devices.h"
#include "psyq/libpad/padchk.h"
extern u8 D_80114448[],PadTxBuffers[];
extern s32 PadMultitapEnabled;
extern ChannelByteCallback PadTxByteCallback;
extern ChannelUpdateCallback PadAllocGatesFn;
extern s32 (*PadChannelToPortFn)(void *);
extern void (*PadSaveCommandCallback)(PadChannel *);
void _padResetChannel(void *);
void _padInitDirSeq(void),_padSetVsyncParam(void);

#endif
