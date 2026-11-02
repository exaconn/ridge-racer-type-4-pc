#ifndef RR_GAME_MOTION_H
#define RR_GAME_MOTION_H
#include "rr/layout.h"
typedef struct {s16 glyph;u16 timing;} RrScrollKey4;
typedef struct {s16 glyph;u16 offset;} RrScrollChannel4;
typedef struct {s32 state,disabled,index,hold,wait;RrScrollKey4 *keys;RrScrollChannel4 channel[2];s32 reserved20,step;} RrScrollTrack40;
typedef struct {s16 x,y,z,pad;} __attribute__((packed)) RrMotionVector8;
typedef struct {RrMotionVector8 from; s32 fromScale; RrMotionVector8 to; s32 toScale; s32 timeCode; s32 continueAfter;} RrMotionKey32;
typedef struct {s32 state,disabled,word08;RrMotionKey32 *key;s16 elapsed,duration;RrMotionVector8 position;s32 scale,word20,word24,velocity;} RrMotionTrack44;
typedef struct {u8 prefix00[0x4C];u32 speed4C;u8 prefix50[0x134-0x50];RrScrollTrack40 scroll;RrMotionTrack44 track;} RrMotionControllerPrefix;
typedef struct {s16 active;u8 pad02[14];u32 tempo;} RrMotionClock20;
#if defined(mips)
#define RR_SCROLL_CHANNEL_OFFSET 24
#else
#include <stddef.h>
#define RR_SCROLL_CHANNEL_OFFSET offsetof(RrScrollTrack40,channel)
#endif
typedef struct {u8 prefix[RR_SCROLL_CHANNEL_OFFSET];RrScrollChannel4 channel;} __attribute__((__may_alias__)) RrScrollChannelCursor;

extern RrMotionControllerPrefix *D_800ACA28;
/* Disabled and unknown states return without using the clock or key.
 * State0 needs an active clock to start. Its wrapped unsigned duration must
 * become nonzero; state1 needs nonzero duration and excludes INT_MIN/-1 in
 * scale interpolation. Key, clock and controller storage are disjoint.
 * Position/elapsed updates wrap16, word arithmetic wraps32. The initial
 * eight-byte vector copy includes pad; completion writes only x/y/z.
 * The caller supplies a clock view within the resident audio owner.
 */
void func_8005AC0C(RrMotionClock20 *);
/* Scroll channels require disjoint clock/controller/key storage and accessible
 * current and following keys. Word products and timers wrap32, offsets wrap16.
 * Clock fields use the same view supplied by the presentation dispatcher. */
void func_8005ED34(RrMotionClock20 *);
RR_ASSERT_SIZE(RrScrollKey4,4);
RR_ASSERT_SIZE(RrScrollChannel4,4);
RR_ASSERT_SIZE(RrMotionVector8,8);
RR_ASSERT_SIZE(RrMotionKey32,32);
RR_ASSERT_SIZE(RrMotionClock20,20);
RR_ASSERT_OFFSET(RrMotionKey32,to,12);
RR_ASSERT_OFFSET(RrMotionKey32,timeCode,24);
RR_ASSERT_OFFSET(RrMotionClock20,tempo,16);
#if defined(mips)
RR_ASSERT_SIZE(RrScrollTrack40,40);
RR_ASSERT_OFFSET(RrMotionControllerPrefix,scroll,0x134);
RR_ASSERT_OFFSET(RrScrollTrack40,channel,0x18);
RR_ASSERT_SIZE(RrMotionTrack44,44);
RR_ASSERT_SIZE(RrMotionControllerPrefix,0x188);
RR_ASSERT_OFFSET(RrMotionControllerPrefix,track,0x15C);
RR_ASSERT_OFFSET(RrMotionTrack44,elapsed,16);
RR_ASSERT_OFFSET(RrMotionTrack44,position,20);
RR_ASSERT_OFFSET(RrMotionTrack44,velocity,40);
#endif
#endif
