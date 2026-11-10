#ifndef RR_GAME_MOTION_H
#define RR_GAME_MOTION_H
#include "rr/presentation.h"
#include "game/camera/campath.h"
#include "game/gfx/motion4.h"
#if defined(mips)
#define RR_SCROLL_CHANNEL_OFFSET 24
#else
#include <stddef.h>
#define RR_SCROLL_CHANNEL_OFFSET offsetof(ScrollTrack40,channel)
#endif
typedef struct {u8 prefix[RR_SCROLL_CHANNEL_OFFSET];ScrollChannel4 channel;} __attribute__((__may_alias__)) ScrollChannelCursor;

RR_ASSERT_SIZE(ScrollKey4,4);
RR_ASSERT_SIZE(ScrollChannel4,4);
RR_ASSERT_SIZE(MotionVector8,8);
RR_ASSERT_SIZE(MotionKey32,32);
RR_ASSERT_OFFSET(MotionKey32,to,12);
RR_ASSERT_OFFSET(MotionKey32,timeCode,24);
#if defined(mips)
RR_ASSERT_SIZE(ScrollTrack40,40);
RR_ASSERT_OFFSET(ScrollTrack40,channel,0x18);
RR_ASSERT_SIZE(MotionTrack44,44);
RR_ASSERT_OFFSET(MotionTrack44,elapsed,16);
RR_ASSERT_OFFSET(MotionTrack44,position,20);
RR_ASSERT_OFFSET(MotionTrack44,velocity,40);
#endif
#endif
