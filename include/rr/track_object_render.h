#ifndef RR_TRACK_OBJECT_RENDER_H
#define RR_TRACK_OBJECT_RENDER_H

#include "rr/rro_model.h"
#include "rr/car_render_types.h"

/* The textured track-object stream has a four-byte command/count header,
 * followed by these thirty-two-byte quads. A following record supplies the
 * final pipeline prefetch even when it does not produce another quad. */
typedef struct {
    union {u16 indices[4]; s32 words[2];} vertices;
    u32 reserved08;
    s32 uv0C, uv10, uv14;
    s8 flashPhase18, orderingBias19;
    u16 translationZ1A;
    s32 translationXY1C;
} TrackObjectQuad;

/* The GTE's first input is reused for the next quad before the current
 * projection is emitted. Its unused high Z halfword is preserved. */
typedef struct {
    Vec3s8 input00;
    u8 reserved08[0x48];
    s32 depth50, flashing54;
    RroScreenPosition projected58[4];
} TrackObjectRenderWork;

typedef struct {
    RroSceneScratchPrefix scene;
    TrackObjectRenderWork work7C;
} TrackObjectScratch;

/* Untextured Gouraud triangles: packed indices and three GPU color words. */
typedef struct {
    union {u16 indices[4]; s32 words[2];} vertices;
    s32 colors08[3];
} TrackObjectTriangle;

/* Instance translations are a separate counted stream. Its last byte biases
 * the ordering bucket; the preceding byte is unused by this renderer. */
typedef struct {
    s32 xy00;
    u16 z04;
    u8 reserved06;
    s8 orderingBias07;
} TrackObjectPlacement;

typedef struct {
    MatrixWords32 rotation00;
    u32 reserved20;
    Vec3s8 input24;
    u32 slot2C, slot30; /* RroScene.workVertices / workCommands */
    u8 reserved34[0x20];
    s32 depth54;
} TrackObjectLitWork;

typedef struct {
    RroSceneScratchPrefix scene;
    TrackObjectLitWork work7C;
} TrackObjectLitScratch;

typedef struct {
    MatrixWords32 rotation00;
    u32 reserved20[2];
    s32 depth28;
    Vec3s8 input2C;
    RroScreenPosition projected34[4];
} TrackObjectBillboardWork;

typedef struct {
    RroSceneScratchPrefix scene;
    TrackObjectBillboardWork work7C;
} TrackObjectBillboardScratch;

typedef struct {
    u32 reserved00[7];
    s32 depth1C, area20;
    RroScreenPosition projected24[4];
} TrackObjectFlashWork;

typedef struct {
    RroSceneScratchPrefix scene;
    TrackObjectFlashWork work7C;
} TrackObjectFlashScratch;

RR_ASSERT_SIZE(TrackObjectQuad, 0x20);
RR_ASSERT_OFFSET(TrackObjectQuad, translationXY1C, 0x1C);
RR_ASSERT_OFFSET(TrackObjectQuad, translationZ1A, 0x1A);
RR_ASSERT_SIZE(TrackObjectRenderWork, 0x68);
RR_ASSERT_OFFSET(TrackObjectRenderWork, depth50, 0x50);
RR_ASSERT_OFFSET(TrackObjectRenderWork, projected58, 0x58);
RR_ASSERT_SIZE(TrackObjectTriangle, 20);
RR_ASSERT_OFFSET(TrackObjectTriangle, colors08, 8);
RR_ASSERT_SIZE(TrackObjectPlacement, 8);
RR_ASSERT_OFFSET(TrackObjectPlacement, orderingBias07, 7);
RR_ASSERT_SIZE(TrackObjectBillboardWork, 0x44);
RR_ASSERT_OFFSET(TrackObjectBillboardWork, input2C, 0x2C);
RR_ASSERT_OFFSET(TrackObjectBillboardWork, projected34, 0x34);
RR_ASSERT_SIZE(TrackObjectFlashWork, 0x34);
RR_ASSERT_OFFSET(TrackObjectFlashWork, projected24, 0x24);
#if defined(mips)
RR_ASSERT_OFFSET(TrackObjectScratch, work7C, 0x7C);
RR_ASSERT_SIZE(TrackObjectScratch, 0xE4);
RR_ASSERT_SIZE(TrackObjectLitWork, 0x58);
RR_ASSERT_OFFSET(TrackObjectLitWork, input24, 0x24);
RR_ASSERT_OFFSET(TrackObjectLitWork, vertices2C, 0x2C);
RR_ASSERT_OFFSET(TrackObjectLitWork, commands30, 0x30);
RR_ASSERT_OFFSET(TrackObjectLitWork, depth54, 0x54);
RR_ASSERT_OFFSET(TrackObjectLitScratch, work7C, 0x7C);
RR_ASSERT_SIZE(TrackObjectLitScratch, 0xD4);
RR_ASSERT_OFFSET(TrackObjectBillboardScratch, work7C, 0x7C);
RR_ASSERT_SIZE(TrackObjectBillboardScratch, 0xC0);
RR_ASSERT_OFFSET(TrackObjectFlashScratch, work7C, 0x7C);
RR_ASSERT_SIZE(TrackObjectFlashScratch, 0xB0);
#endif

#endif
