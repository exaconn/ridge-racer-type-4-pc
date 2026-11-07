#ifndef RR_RING_RENDERER_H
#define RR_RING_RENDERER_H

#include "rr/layout.h"
#include "rr/matrix_words.h"

/* The transform service reads three word coordinates and three halfword angles. */
typedef struct {
    s32 x, y, z, reserved0C;
    s16 rx, ry, rz, reserved16;
} RrRingTransform24;
typedef struct { s16 x, y, z, pad; } RrRingVertex8;
typedef struct { s16 x, y; } __attribute__((packed)) RrRingScreen4;

/* Geometry view of the scratch tail after the transform service has run. */
typedef struct {
    s16 origin[4];
    RrRingScreen4 screen[4];
    u8 reserved18[12];
    RrRingVertex8 vectors[4];
    u8 red, green, blue, reserved47;
} RrRingScratch48;

typedef union {
    u32 raw;
    struct { u32 addr:24; u32 len:8; } bits;
} RrRingGpuTag;
/* Pointer slots retain their native width; on PAL each slot is one word. */
typedef union { u32 word; void *pointer; } RrRingGpuAddress;
typedef struct { RrRingGpuAddress cursor, order; } RrRingGpuSlots;
typedef struct {
    RrRingGpuTag tag;
    u8 red, green, blue, code;
    s16 xy[4][2];
} RrRingPacket24;

void DrawRing(s32 *position, s32 radius);

RR_ASSERT_SIZE(RrRingTransform24, 24);
RR_ASSERT_OFFSET(RrRingTransform24, rx, 0x10);
RR_ASSERT_SIZE(RrRingVertex8, 8);
RR_ASSERT_SIZE(RrRingScreen4, 4);
RR_ASSERT_SIZE(RrRingScratch48, 0x48);
RR_ASSERT_OFFSET(RrRingScratch48, screen, 8);
RR_ASSERT_OFFSET(RrRingScratch48, vectors, 0x24);
RR_ASSERT_OFFSET(RrRingScratch48, red, 0x44);
RR_ASSERT_SIZE(RrRingGpuTag, 4);
RR_ASSERT_SIZE(RrRingPacket24, 24);
RR_ASSERT_OFFSET(RrRingPacket24, xy, 8);
#if defined(mips)
RR_ASSERT_SIZE(RrRingGpuSlots, 8);
RR_ASSERT_OFFSET(RrRingGpuSlots, order, 4);
#endif

#endif
