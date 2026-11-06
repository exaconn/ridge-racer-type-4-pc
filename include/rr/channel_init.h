#ifndef RR_CHANNEL_INIT_H
#define RR_CHANNEL_INIT_H
#include "rr/channels.h"
#include "rr/channel_advance.h"
#include "rr/bios_devices.h"
#include "psyq/libpad/padchk.h"
extern u8 D_80114448[],D_80114490[];
extern s32 D_800ABCB4;
extern u32 (*D_800ABC70)(const RrChannelTransferView *);
extern RrChannelUpdateCallback D_800ABC74;
extern s32 (*D_800ABC78)(void *);
extern void (*D_800ABC8C)(RrChannelTransferView *);
void func_80099E34(void *);
s32 func_8009A2D8(void *);
void *func_8009A310(s32);
void func_8009A334(void),_padSetVsyncParam(void);
/* Two input buffers must each permit two byte stores. They may overlap each
 * other or byte fields in the record arena; pointer slots must remain valid.
 * Clears both canonical records, installs callbacks, and publishes ready last. */
void PadInitDirect(u8 *first,u8 *second);
#endif
