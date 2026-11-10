#ifndef RR_OVERLAY668_DISPLAY_H
#define RR_OVERLAY668_DISPLAY_H
#include "rr/gpu.h"
#include "rr/cd.h"

typedef struct {
    u32 word00, alternate04;
    u32 word08;
    volatile u32 ready0C;
    GpuRectangleFields rectangles[3];
} StrDisplayConfig;
typedef struct {u8 pad00[0x10];void *buffers[5];} StrBufferSet;
typedef struct {
    u8 pad00[0xB0];s32 resourceB0,phaseB4;void *activeB8;
    CdLocation locationBC,currentC0;s32 counterC4;
    u32 thresholdC8,budgetCC;s32 stopD0;
} StrContextPrefix;
extern s32 DrawBufIndex;
extern StrContextPrefix RaceContext;

/* Preserve the other configuration fields and store low16 coordinate bits. */
void ovl668_func_80114E90(StrDisplayConfig *config,s32 x,s32 y,s32 second_x,s32 second_y);
/* Base covers the complete partitioned allocation through offset0x19A00.
 * Reload activeB8 for every publication: a destination slot may overlap it. */
void ovl668_func_80114E48(u8 *base);

/* Consume readiness; on timeout toggle the buffer and select its coordinates. */
/* The playback caller supplies mode0. This implementation ignores the mode. */
void ovl668_func_8011535C(StrDisplayConfig *config,s32 mode);

RR_ASSERT_SIZE(StrDisplayConfig,0x28);
RR_ASSERT_OFFSET(StrDisplayConfig,ready0C,0x0C);
RR_ASSERT_OFFSET(StrDisplayConfig,rectangles,0x10);
RR_ASSERT_SIZE(StrBufferSet,0x24);
RR_ASSERT_OFFSET(StrBufferSet,buffers,0x10);
RR_ASSERT_SIZE(StrContextPrefix,0xD4);
RR_ASSERT_OFFSET(StrContextPrefix,phaseB4,0xB4);
RR_ASSERT_OFFSET(StrContextPrefix,currentC0,0xC0);
RR_ASSERT_OFFSET(StrContextPrefix,activeB8,0xB8);
#endif
