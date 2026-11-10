#ifndef RR_FRAME_STORAGE_TYPES_H
#define RR_FRAME_STORAGE_TYPES_H
#include "rr/frame_environment_types.h"
/* Two RAM arenas, each holding the drawing environments, ordering tables and
 * packet storage for one bank. Offsets used by the outer loop:
 * display environment 0x5C, ordering tables 0x70 and 0xB70 (704 entries),
 * draw packets 0xB6C and optional 0x166C, initial packet cursor 0x1670. */
typedef struct {
    FrameEnvironment environment00;
    FrameOrderingTable ordering70, secondaryOrderingB70;
    u8 packets1670[0x21108];
} FrameBuffer;

typedef union {
    FrameBuffer buffer;
    FrameEnvironment environment;
    FrameOrderingPrefix ordering;
    u32 alignment;
    u8 bytes[0x22778];
} FrameStorage;

/* Current bank owner. Callers select its environments, ordering words or
 * packet storage; narrower views do not declare separate global objects. */
extern FrameStorage *CurrentDrawBuf;

RR_ASSERT_SIZE(FrameBuffer, 0x22778);
RR_ASSERT_OFFSET(FrameBuffer, ordering70, 0x70);
RR_ASSERT_OFFSET(FrameBuffer, secondaryOrderingB70, 0xB70);
RR_ASSERT_OFFSET(FrameBuffer, packets1670, 0x1670);
RR_ASSERT_SIZE(FrameStorage, 0x22778);
#endif
