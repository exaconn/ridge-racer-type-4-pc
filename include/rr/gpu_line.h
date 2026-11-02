#ifndef RR_GPU_LINE_H
#define RR_GPU_LINE_H
#include "rr/gpu.h"
typedef struct {
    RrGpuDmaTag tag;
    u8 r,g,b,code;
    s16 x0,y0,x1,y1;
} RrGpuLineF2Packet;
RR_ASSERT_SIZE(RrGpuLineF2Packet,16);
RR_ASSERT_OFFSET(RrGpuLineF2Packet,x0,8);
RR_ASSERT_OFFSET(RrGpuLineF2Packet,x1,12);
#endif
