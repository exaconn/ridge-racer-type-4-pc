#ifndef RR_ANIMATED_TRACK_TYPES_H
#define RR_ANIMATED_TRACK_TYPES_H

#include "rr/rro_model.h"

/* GPU links remain encoded words; CPU references to packets are full pointers. */
typedef struct {
    GpuDmaTag tag;
    GpuColorWord color0;
    s32 xy0;
    GpuColorWord color1;
    s32 xy1;
} AnimatedLinePacket;

typedef struct {
    Vec3sFields8 position00, position08;
    Vec3sFields8 step10, step18;
    s32 colorPhase20;
} AnimatedTrackParticle;

typedef struct {
    GpuColorCommand color;
    s32 duration;
} AnimatedTrackColorKey;

/* Initialization writes command words; interpolation reads/copies RGBA bytes. */
typedef union {
    u32 words[16];
    GpuColorCommand values[16];
} AnimatedTrackPalette;

typedef struct {
    AnimatedTrackParticle *particles00;
    s32 activeCount04, capacity08, field0C, initialized10, frame14;
    s32 colorTick18, colorTick1C;
    s32 scale20, scale24, startFrame28, endFrame2C, advance30;
    s32 keyIndex34, keyIndex38, style3C;
    const AnimatedTrackColorKey *keys40, *keys44;
    AnimatedTrackPalette palette48, palette88;
    Vec3i positionC8;
    u32 reservedD4;
    Vec3i anglesD8;
    u32 reservedE4;
    AnimatedLinePacket *headsE8[2], *tailsF0[2];
} AnimatedTrackObject;

/* Original course7 data reserves these pools before its relative key tables.
 * Native allocations use this owner's actual pointer alignment and sizes. */
typedef struct {
    AnimatedTrackParticle particles[5][255];
    AnimatedTrackObject objects[5];
    AnimatedLinePacket packets[2 * 5 * 255];
    u32 colorOffsets[9][2];
#if defined(__GNUC__) && __GNUC__ < 3
    u8 colorKeys[0];
#else
    u8 colorKeys[];
#endif
} AnimatedTrackArena;

/* Handwritten emission saves CPU registers and stages vectors after the scene. */
typedef struct {
    RroSceneScratchPrefix scene;
    u32 work7C[26];
} AnimatedTrackScratch;

RR_ASSERT_SIZE(AnimatedLinePacket, 20);
RR_ASSERT_OFFSET(AnimatedLinePacket, color1, 12);
RR_ASSERT_SIZE(AnimatedTrackParticle, 36);
RR_ASSERT_OFFSET(AnimatedTrackParticle, step10, 16);
RR_ASSERT_OFFSET(AnimatedTrackParticle, colorPhase20, 32);
RR_ASSERT_SIZE(AnimatedTrackColorKey, 8);
RR_ASSERT_OFFSET(AnimatedTrackColorKey, duration, 4);
RR_ASSERT_SIZE(AnimatedTrackPalette, 64);
#if defined(mips)
RR_ASSERT_SIZE(AnimatedTrackObject, 0xF8);
RR_ASSERT_OFFSET(AnimatedTrackObject, keys40, 0x40);
RR_ASSERT_OFFSET(AnimatedTrackObject, palette48, 0x48);
RR_ASSERT_OFFSET(AnimatedTrackObject, positionC8, 0xC8);
RR_ASSERT_OFFSET(AnimatedTrackObject, headsE8, 0xE8);
RR_ASSERT_OFFSET(AnimatedTrackObject, tailsF0, 0xF0);
RR_ASSERT_OFFSET(AnimatedTrackArena, objects, 0xB34C);
RR_ASSERT_OFFSET(AnimatedTrackArena, packets, 0xB824);
RR_ASSERT_OFFSET(AnimatedTrackArena, colorOffsets, 0x17F5C);
RR_ASSERT_OFFSET(AnimatedTrackScratch, work7C, 0x7C);
RR_ASSERT_SIZE(AnimatedTrackScratch, 0xE4);
#endif

/* Handwritten emitter returns the first packet after those emitted. */
AnimatedLinePacket *EmitAnimatedTrackLines(AnimatedTrackScratch *scratch,
                               AnimatedLinePacket *cursor,
                               AnimatedTrackObject *object, s32 colorOffset);

#endif
