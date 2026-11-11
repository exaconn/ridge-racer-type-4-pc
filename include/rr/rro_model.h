#ifndef RR_RRO_MODEL_H
#define RR_RRO_MODEL_H

#include "rr/object_transform_types.h"
#include "rr/projection_state.h"
#include "rr/model_table.h"

/* Decoded upper indices are signed: sizeof alone would make the stride unsigned. */
enum {RRO_VECTOR_STRIDE = sizeof(Vec3s8)};

/* The RRO emitters stage four vertices, one normal and four projected XY
 * pairs here. SetGteObjectTransform uses the same scratch region first;
 * its output is consumed before the emission fields replace it. */
typedef ScreenPosition RroScreenPosition;

typedef struct {
    const Vec3s8 *vertices00;
    const Vec3s8 *normals04;
    s32 flags08;
    u32 envClut0C, envTpage10;
    u32 *ordering14;
    s32 depth18, firstArea1C, secondArea20;
    u32 reserved24;
    Vec3s8 vertices28[4];
    Vec3s8 normal48;
    RroScreenPosition projected50[4];
} RroRenderWork;

/* Intro model setup precedes transformation in the same work arena. */
typedef struct {
    Vec3s8 angles;
    MatrixWords32 rotation;
    MatrixWords32 light;
} RroModelSetup;

typedef union {
    ObjectTransform38 transform;
    RroRenderWork render;
    RroModelSetup setup;
} RroScratchWork;

/* These cells are raw words while a camera script owns them, and a primitive
 * pointer/counter while the preview dispatcher owns them. The native union
 * retains a complete pointer and its actual alignment. */
typedef union {
    u32 words[2];
    struct {const u32 *primitive; s32 counter;} draw;
} RroDrawCommandState;

/* Scratchpad layout while a model bank is selected for RRO rendering.
 * Pointer cells are actual C pointers, including on a native 64-bit host.
 * Other render paths reuse this storage with different contents. */
typedef struct {
    ViewMatrixScratch48 view;
    void *modelEntries48;
    u32 reserved4C;
    const RroBankCommand *commands50;
    const Vec3s8 *vertices54;
    const Vec3s8 *normals58;
    u32 reserved5C;
    s32 depthShift60, renderFlags64;
    u32 reserved68;
    GpuRectangle viewport6C;
    RroDrawCommandState draw74;
} RroSceneScratchPrefix;

typedef struct {
    RroSceneScratchPrefix scene;
    RroScratchWork work7C;
} RroModelScratch;

/* Handwritten dispatch consumes this prefix and the complete scratch arena
 * following it. The first argument is an actual scene pointer, never a
 * 32-bit integer address. Bank selection must initialize commands50. */
void SubmitModel(RroSceneScratchPrefix *scene, s32 modelIndex,
                 s32 flags, s32 depthBias);

/* Packed vertex and normal words use unsigned lower and signed upper indices.
 * Each array pointer is an origin with valid storage for every selected offset.
 * Counts come from signed 16-bit command headers. All three emitters prefetch
 * the initial quad even for nonpositive counts; positive counts require one
 * extra lookahead quad. Depth shifts are 0..31. */
const u32 *EmitRroRawTexturedQuads(const u32 *commands, RroRenderWork *work, s32 count);
const u32 *EmitRroLitGouraudQuads(const u32 *commands, RroRenderWork *work, s32 count);
/* The stream backs the initial quad even for nonpositive counts and one extra
 * prefetched quad after positive counts. GPU output needs the complete 120-byte
 * envmap lookahead footprint, including when a direct packet advances by 56.
 * Native GPU buffers/previous nodes must be registered with GpuDmaSetRegions. */
const u32 *EmitRroEnvMapQuads(const u32 *commands, RroRenderWork *work, s32 count);

RR_ASSERT_SIZE(RroScreenPosition, 4);
RR_ASSERT_SIZE(RroModelSetup, 0x48);
RR_ASSERT_OFFSET(RroModelSetup, rotation, 8);
RR_ASSERT_OFFSET(RroModelSetup, light, 0x28);
#if defined(mips)
RR_ASSERT_SIZE(RroRenderWork, 0x60);
RR_ASSERT_OFFSET(RroRenderWork, ordering14, 0x14);
RR_ASSERT_OFFSET(RroRenderWork, vertices28, 0x28);
RR_ASSERT_OFFSET(RroRenderWork, normal48, 0x48);
RR_ASSERT_OFFSET(RroRenderWork, projected50, 0x50);
RR_ASSERT_SIZE(RroSceneScratchPrefix, 0x7C);
RR_ASSERT_OFFSET(RroSceneScratchPrefix, commands50, 0x50);
RR_ASSERT_OFFSET(RroSceneScratchPrefix, vertices54, 0x54);
RR_ASSERT_OFFSET(RroSceneScratchPrefix, normals58, 0x58);
RR_ASSERT_OFFSET(RroSceneScratchPrefix, depthShift60, 0x60);
RR_ASSERT_OFFSET(RroModelScratch, scene, 0);
RR_ASSERT_OFFSET(RroModelScratch, work7C, 0x7C);
RR_ASSERT_SIZE(RroModelScratch, 0xDC);
#endif

#endif
