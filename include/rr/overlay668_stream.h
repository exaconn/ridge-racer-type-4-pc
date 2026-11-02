#ifndef RR_OVERLAY668_STREAM_H
#define RR_OVERLAY668_STREAM_H
#include "rr/overlay668_display.h"
#include "rr/overlay668_control.h"
/* The shared live allocation, including the display configuration at0x24. */
typedef struct {
    s32 width00,height04;
    u8 reserved08[8];
    RrStrCommandHeader *commands10[2];
    void *buffers18[2];
    u32 commandIndex20;
    RrStrDisplayConfig display24;
} RrStrPlaybackState;
extern s32 D_801102DC,D_800AC5DC,D_800F2DE4;
extern u8 D_800AD558;
void func_80092CB8(s32);
void func_800825B4(s8,s8,s8);
void func_80083CD4(s8,s32,s32);
s32 func_8008B154(s32);
s32 ovl668_func_801150D8(RrStrDisplayConfig *);
void ovl668_func_801153F0(u8 *location);
void ovl668_func_80114B2C(u8 *resource);
void func_8008F934(void);
void func_80092FFC(const RrGpuRectangleFields *rectangle,const void *pixels);
void ovl668_func_80114F48(void);
/* Both selectors are0..1. The full playback state remains live across callbacks.
 * Coordinates narrow to signed16, DMA output covers width*height/2 words.
 * Captured rectangle is passed by address; its pixel buffer uses the old index. */
RR_ASSERT_SIZE(RrStrPlaybackState,0x4C);
RR_ASSERT_OFFSET(RrStrPlaybackState,display24,0x24);
RR_ASSERT_OFFSET(RrStrPlaybackState,buffers18,0x18);
#endif
