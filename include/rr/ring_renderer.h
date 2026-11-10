#ifndef RR_RING_RENDERER_H
#define RR_RING_RENDERER_H

#include "rr/layout.h"
#include "rr/matrix_words.h"
#include "rr/object_transform_types.h"

/* The transform service reads three word coordinates and three halfword angles. */
typedef struct {
    s32 x, y, z, reserved0C;
    s16 rx, ry, rz, reserved16;
} RingTransform24;
typedef struct { s16 x, y, z, pad; } RingVertex8;
typedef struct { s16 x, y; } __attribute__((packed)) RingScreen4;

/* Geometry view of the scratch tail after the transform service has run. */
typedef struct {
    s16 origin[4];
    RingScreen4 screen[4];
    u8 reserved18[12];
    RingVertex8 vectors[4];
    u8 red, green, blue, reserved47;
} RingGeometry48;
/* The transform is prepared in this same tail before ring geometry reuses it. */
typedef union {
    ObjectTransform38 transform;
    RingGeometry48 geometry;
} RingScratch48;

typedef union {
    u32 raw;
    struct { u32 addr:24; u32 len:8; } bits;
} RingGpuTag;
/* Pointer slots retain their native width; on PAL each slot is one word. */
typedef union { u32 word; void *pointer; } RingGpuAddress;
typedef struct { RingGpuAddress cursor, order; } RingGpuSlots;
typedef struct {
    RingGpuTag tag;
    u8 red, green, blue, code;
    s16 xy[4][2];
} RingPacket24;

void DrawRing(s32 *position, s32 radius);

RR_ASSERT_SIZE(RingTransform24, 24);
RR_ASSERT_OFFSET(RingTransform24, rx, 0x10);
RR_ASSERT_SIZE(RingVertex8, 8);
RR_ASSERT_SIZE(RingScreen4, 4);
RR_ASSERT_SIZE(RingScratch48, 0x48);
RR_ASSERT_OFFSET(RingGeometry48, screen, 8);
RR_ASSERT_OFFSET(RingGeometry48, vectors, 0x24);
RR_ASSERT_OFFSET(RingGeometry48, red, 0x44);
RR_ASSERT_SIZE(RingGpuTag, 4);
RR_ASSERT_SIZE(RingPacket24, 24);
RR_ASSERT_OFFSET(RingPacket24, xy, 8);
#if defined(mips)
RR_ASSERT_SIZE(RingGpuSlots, 8);
RR_ASSERT_OFFSET(RingGpuSlots, order, 4);
#endif

#endif
