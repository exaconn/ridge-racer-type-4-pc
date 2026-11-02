#ifndef RR_BIOS_DEVICES_H
#define RR_BIOS_DEVICES_H

#include "rr/layout.h"

/* Raw card-sector BIOS entries: 128 bytes per sector. Status is indexed by
 * slot (port >> 4); bit0 signals a completed transfer, including error statuses. */
s32 func_80095CE4(s32 port, s32 sector, const u8 *buffer);
void func_80095CF4(void);
s32 func_800963A4(s32 port, s32 sector, u8 *buffer);
s32 func_800963B4(s32 slot);
void func_80096434(void *buffer, s32 size);
/* Adds XOR of bytes 0..126 at byte127, then makes up to eight write/read attempts.
 * Verification compares the readback XOR to the current caller checksum byte. */
s32 func_80096054(s32 port, s32 sector, u8 *buffer);

typedef s32 (*RrBiosDeviceHandler)(s32 *, s32, s32);

/* Device-table entry used by the handler replacement routine. */
typedef struct {
    const char *name;
    u8 pad04[0x30];
    RrBiosDeviceHandler handler;
    u8 pad38[0x18];
} RrBiosDevice;

extern RrBiosDeviceHandler D_801143B8;
extern char D_801143C0[];
void func_800972B0(s32 *context, s32 second, s32 third);

RR_ASSERT_SIZE(RrBiosDevice, 0x50);
RR_ASSERT_OFFSET(RrBiosDevice, handler, 0x34);

#endif
