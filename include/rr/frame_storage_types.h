#ifndef RR_FRAME_STORAGE_TYPES_H
#define RR_FRAME_STORAGE_TYPES_H
#include <stddef.h>
#include "rr/frame_environment_types.h"
/* Two RAM arenas, each holding the drawing environments, ordering tables and
 * packet storage for one bank. Offsets used by the outer loop:
 * display environment 0x5C, ordering tables 0x70 and 0xB70 (704 entries),
 * draw packets 0xB6C and optional 0x166C, initial packet cursor 0x1670. */
typedef struct {
    FrameEnvironment environment00;
    FrameOrderingTable ordering70, secondaryOrderingB70;
    u8 packets1670[0x60000]; /* PS1: 0x21108; host tags are 16 bytes */
    /* Persistent pre-linked race HUD sprites. PS1: the last 12 0x14-byte
     * packets of the arena (frame offset 0x22688, MenuCoursePreviewArena+0x18
     * for bank 0); built by InitRaceHudPackets, linked by SubmitRaceHudPackets. */
    SpritePacket hudSprites[12];
} FrameBuffer;

typedef union {
    FrameBuffer buffer;
    FrameEnvironment environment;
    FrameOrderingPrefix ordering;
    u32 alignment;
} FrameStorage;

/* Current bank owner. Callers select its environments, ordering words or
 * packet storage; narrower views do not declare separate global objects. */
extern FrameStorage *CurrentDrawBuf;

RR_ASSERT_SIZE(FrameBuffer, 0x22778);
RR_ASSERT_OFFSET(FrameBuffer, ordering70, 0x70);
RR_ASSERT_OFFSET(FrameBuffer, secondaryOrderingB70, 0xB70);
RR_ASSERT_OFFSET(FrameBuffer, packets1670, 0x1670);
RR_ASSERT_SIZE(FrameStorage, 0x22778);
/* Both views must name the same ordering table on every host. */
_Static_assert(offsetof(FrameStorage, ordering.ordering70) ==
                   offsetof(FrameStorage, buffer.ordering70),
               "FrameStorage ordering views disagree");
#endif
