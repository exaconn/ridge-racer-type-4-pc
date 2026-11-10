#ifndef RR_PRESENTATION_TYPES_H
#define RR_PRESENTATION_TYPES_H

#include "rr/camera_types.h"

typedef struct {s16 glyph; u16 timing;} ScrollKey4;
typedef struct {s16 glyph; u16 offset;} ScrollChannel4;

typedef struct {
    s32 state, disabled, index, hold, wait;
    ScrollKey4 *keys;
    ScrollChannel4 channel[2];
    s32 reserved20, step;
} ScrollTrack40;

typedef struct {s16 x, y, z, pad;} __attribute__((packed)) MotionVector8;

typedef struct {
    MotionVector8 from;
    s32 fromScale;
    MotionVector8 to;
    s32 toScale, timeCode, continueAfter;
} MotionKey32;

typedef struct {
    s32 state, disabled, word08;
    MotionKey32 *key;
    s16 elapsed, duration;
    MotionVector8 position;
    s32 scale, word20, word24, velocity;
} MotionTrack44;

typedef struct {
    s32 state, mode;
    u8 reserved08[0x44 - 8];
    s32 image44;
    u16 elapsed48;
    s16 duration4A;
    u16 reserved4C, reserved4E;
} PanelTrack50;

/* One 0x188-byte presentation owner, selected by script operations 0..3.
 * The camera, panel, scrolling glyphs and moving model have separate tracks. */
typedef struct {
    Vector position, target;
    PreviewLighting40 lighting20;
    s32 viewDistance48, scale4C;
    s32 fullViewport50, drawEnabled54, stencilEnabled58, alternateClock5C;
    s32 flags60;
    CameraScriptEntry8 *script64;
    s16 scriptIndex68, scriptTimer6A;
    s32 scriptOperation6C, scriptArgument70;
    CameraMotion70 motion74;
    PanelTrack50 panel;
    ScrollTrack40 scroll;
    MotionTrack44 track;
} PresentationController;

/* The pointer at +0x3D8 normally selects the inline controller at +0x250.
 * MotionController (0x800ACA28) is an address alias of that pointer slot. */
typedef struct {
    u8 reserved00[0x250];
    PresentationController presentation250;
    PresentationController *controller3D8;
    u8 reserved3DC[0x564 - 0x3DC];
    s32 ringModel564;
} PresentationContextPrefix;

typedef struct {
    s16 active;
    u8 reserved02[14];
    u32 tempo;
} MotionClock20;

RR_ASSERT_SIZE(MotionClock20,0x14);
RR_ASSERT_OFFSET(MotionClock20,tempo,0x10);
RR_ASSERT_SIZE(PanelTrack50,0x50);
RR_ASSERT_OFFSET(PanelTrack50,image44,0x44);
#if defined(mips)
RR_ASSERT_OFFSET(PresentationController,panel,0xE4);
RR_ASSERT_OFFSET(PresentationController,scroll,0x134);
RR_ASSERT_OFFSET(PresentationController,track,0x15C);
RR_ASSERT_SIZE(PresentationController,0x188);
RR_ASSERT_OFFSET(PresentationContextPrefix,presentation250,0x250);
RR_ASSERT_OFFSET(PresentationContextPrefix,controller3D8,0x3D8);
RR_ASSERT_OFFSET(PresentationContextPrefix,ringModel564,0x564);
#endif

#endif
