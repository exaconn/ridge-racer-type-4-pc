#ifndef RR_FRAME_ENVIRONMENT_TYPES_H
#define RR_FRAME_ENVIRONMENT_TYPES_H
#include "rr/gpu_environment_types.h"
/* The frame prefix owns distinct SDK drawing and display descriptors. */
typedef struct {
    DrawEnvironment draw00;
    DisplayEnvironment display5C;
} FrameEnvironment;
/* 704 ordering entries, each a host packet tag (OT_TYPE). */
typedef struct {
    GpuDmaTag tags[704];
} FrameOrderingTable;
/* Prefix shared by screen transitions and frame storage. The first rectangle
 * is also exposed by the screen-tile interface; ordering storage follows70. */
typedef struct {
    GpuRectangle rectangle;
    u8 pad08[0x68];
    FrameOrderingTable ordering70;
} FrameOrderingPrefix;
RR_ASSERT_SIZE(FrameOrderingTable,0xB00);
RR_ASSERT_SIZE(FrameOrderingPrefix,0xB70);
RR_ASSERT_OFFSET(FrameOrderingPrefix,ordering70,0x70);

RR_ASSERT_SIZE(FrameEnvironment,0x70);
RR_ASSERT_OFFSET(FrameEnvironment,draw00,0);
RR_ASSERT_OFFSET(FrameEnvironment,display5C,0x5C);
#endif
