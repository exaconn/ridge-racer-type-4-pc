#ifndef RR_BIOS_FILE_TYPES_H
#define RR_BIOS_FILE_TYPES_H
#include "rr/layout.h"

struct BiosDevice;
/* BIOS file-control block, complete PAL extent 0x2C. Native buffers and device
 * pointers retain their full width. Source: PSX-SPX BIOS Control Blocks. */
typedef struct {
    u32 status;
    u32 disk_or_port;
    u8 *transfer_address;
    s32 transfer_length;
    u32 position;
    u32 device_flags;
    s32 error;
    struct BiosDevice *device;
    u32 size;
    u32 logical_block;
    u32 number;
} BiosFileControlBlock;

#if defined(__mips__)
RR_ASSERT_SIZE(BiosFileControlBlock, 0x2C);
RR_ASSERT_OFFSET(BiosFileControlBlock, transfer_address, 8);
RR_ASSERT_OFFSET(BiosFileControlBlock, transfer_length, 0xC);
RR_ASSERT_OFFSET(BiosFileControlBlock, error, 0x18);
RR_ASSERT_OFFSET(BiosFileControlBlock, device, 0x1C);
RR_ASSERT_OFFSET(BiosFileControlBlock, number, 0x28);
#endif
#endif
