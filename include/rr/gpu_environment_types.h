#ifndef RR_GPU_ENVIRONMENT_TYPES_H
#define RR_GPU_ENVIRONMENT_TYPES_H

#include "rr/gpu.h"

typedef void (*GpuCompletionCallback)(void);

/* SDK DRAWENV: drawing state and its sixteen-word command backing. It does
 * not own a display descriptor; standalone users such as STR allocate92 bytes.
 * background enables a clear/fill command; it does not select RGB24 output. */
typedef struct {
    GpuRectangleFields clip;
    s16 offset_x,offset_y;
    GpuRectangleFields window;
    u16 page;
    u8 dither,draw_display,background,r,g,b;
    u32 draw_packet1C[16];
} DrawEnvironment;

/* Submission compares all four trailing bytes as one cached word. mode is
 * the video-mode byte;8 marks a mode change before timing is rebuilt. */
typedef union {
    u32 word;
    struct {u8 interlace,rgb24,mode,reserved;} bytes;
} DisplayFlags;

/* SDK DISPENV is a separate twenty-byte descriptor. The packed-word view also
 * gives standalone descriptors the alignment needed by submission's loads. */
typedef struct {
    GpuRectangleFields display,screen;
    DisplayFlags flags10;
} DisplayEnvironment;

RR_ASSERT_SIZE(DrawEnvironment,0x5C);
RR_ASSERT_OFFSET(DrawEnvironment,window,0xC);
RR_ASSERT_OFFSET(DrawEnvironment,offset_x,8);
RR_ASSERT_OFFSET(DrawEnvironment,page,0x14);
RR_ASSERT_OFFSET(DrawEnvironment,background,0x18);
RR_ASSERT_OFFSET(DrawEnvironment,r,0x19);
RR_ASSERT_OFFSET(DrawEnvironment,draw_packet1C,0x1C);
RR_ASSERT_SIZE(DisplayEnvironment,0x14);
RR_ASSERT_OFFSET(DisplayEnvironment,screen,8);
RR_ASSERT_SIZE(DisplayFlags,4);
RR_ASSERT_OFFSET(DisplayFlags,bytes,0);
RR_ASSERT_OFFSET(DisplayEnvironment,flags10,0x10);

#endif
