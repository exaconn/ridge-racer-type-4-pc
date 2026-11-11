#ifndef RR_OVERLAY668_STREAM_H
#define RR_OVERLAY668_STREAM_H
#include "psyq/libgpu/gpuenv.h"
#include "rr/overlay668_display.h"
#include "rr/library.h"
#include "rr/overlay668_control.h"
#include "psyq/libcd/dsstrm32.h"
/* The shared live allocation at MenuArenaBase. ovl668_init_stream_buffers
 * carves the buffers that follow it: two MDEC command buffers (VLC output),
 * two 16-pixel-wide RGB24 slice buffers and the LIBCD stream ring. */
typedef struct {
    s32 width00,height04;
    u8 reserved08[8];
    StrCommandHeader *commands10[2];
    void *buffers18[2];
    LibQueueRecord32 *queue20;
    StrDisplayConfig display24;
} StrPlaybackState;
extern s32 VSyncThreshold,ModeFrame;
/* LIBCD: set when StCdInterrupt deferred a sector until the MDEC output
 * transfer finished; the slice callback then runs it. */
extern s32 StCdIntrFlag;
#include "rr/runtime_settings.h"
void SetDispMask(s32);
void SsSetSerialAttr(s8,s8,s8);
void SsSetSerialVol(s8,s32,s32);
s32 VSync(s32);
s32 ovl668_decode_next_frame(StrDisplayConfig *);
void ovl668_seek_and_read_stream(u8 *location);
void ovl668_start_stream_playback(u8 *resource);
void ovl668_on_mdec_slice_ready(void);
/* Both selectors are0..1. The full playback state remains live across callbacks.
 * Coordinates narrow to signed16, DMA output covers width*height/2 words.
 * Captured rectangle is passed by address; its pixel buffer uses the old index. */
RR_ASSERT_SIZE(StrPlaybackState,0x4C);
RR_ASSERT_OFFSET(StrPlaybackState,display24,0x24);
RR_ASSERT_OFFSET(StrPlaybackState,buffers18,0x18);
#endif
