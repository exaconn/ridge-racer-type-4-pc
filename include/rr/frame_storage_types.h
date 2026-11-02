#ifndef RR_FRAME_STORAGE_TYPES_H
#define RR_FRAME_STORAGE_TYPES_H
#include "rr/frame_environment.h"
/* Two RAM arenas, each holding the drawing environments, ordering tables and
 * packet storage for one bank. Offsets used by the outer loop:
 * display environment 0x5C, ordering tables 0x70 and 0xB70 (704 entries),
 * draw packets 0xB6C and optional 0x166C, initial packet cursor 0x1670. */
typedef union {
    RrFrameEnvironment environment;
    RrFrameOrderingPrefix ordering;
    struct {u8 reserved00[0xB70];u32 secondB70[704];} tables;
    u32 alignment;
    u8 bytes[0x22778];
} RrFrameStorage;
#endif
