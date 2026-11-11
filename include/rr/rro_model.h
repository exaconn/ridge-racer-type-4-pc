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

/* Host pointer state the PS1 kept in scratch: the selected model bank
 * (scratch 0x48..0x58), the dispatcher primitive (0x74) and the work-area
 * pointers of the RRO and lit track-object emitters (0x7C onwards). The
 * scratch views keep scalar placeholders at those offsets. Defined in
 * gfx/model.c. */
typedef struct {
    const RroEffectMesh *modelEntries;  /* 0x48: entries of the relocated model table */
    const RroBankCommand *commands;     /* 0x50 */
    const Vec3s8 *vertices;             /* 0x54 */
    const Vec3s8 *normals;              /* 0x58 */
    const u32 *drawPrimitive;           /* 0x74 */
    const Vec3s8 *workVertices;         /* RroRenderWork 0x00 / TrackObjectLitWork 0x2C */
    const Vec3s8 *workNormals;          /* RroRenderWork 0x04 */
    struct GpuDmaTag *workOrdering;     /* RroRenderWork 0x14 */
    const u32 *workCommands;            /* TrackObjectLitWork 0x30 */
} RroScenePointers;
extern RroScenePointers RroScene;

/* Pointer words (slot00/04/14) live in RroScene.work*. */
typedef struct {
    u32 slot00, slot04;
    s32 flags08;
    u32 envClut0C, envTpage10;
    u32 slot14;
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
 * slot/counter while the preview dispatcher owns them; the primitive pointer
 * itself is RroScene.drawPrimitive. */
typedef union {
    u32 words[2];
    struct {u32 primitiveSlot; s32 counter;} draw;
} RroDrawCommandState;

/* Scratchpad layout while a model bank is selected for RRO rendering. It is
 * scalar-only with the PS1 offsets; the bank pointers that occupied
 * 0x48..0x58 are RroScene. Other render paths reuse this storage. */
typedef struct {
    ViewMatrixScratch48 view;
    u32 slot48;
    u32 reserved4C;
    u32 slot50, slot54, slot58;
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
 * 32-bit integer address. Bank selection (SelectModelBank) initializes
 * RroScene.commands/vertices/normals. */
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
 * prefetched quad after positive counts. Each quad stages a complete
 * GpuEnvMapEmission at the packet cursor, even when it emits the shorter
 * direct GpuEnvMapDirect packet. */
const u32 *EmitRroEnvMapQuads(const u32 *commands, RroRenderWork *work, s32 count);

RR_ASSERT_SIZE(RroScreenPosition, 4);
RR_ASSERT_SIZE(RroModelSetup, 0x48);
RR_ASSERT_OFFSET(RroModelSetup, rotation, 8);
RR_ASSERT_OFFSET(RroModelSetup, light, 0x28);
/* Scalar-only scratch views: PS1 offsets hold on every host. */
_Static_assert(sizeof(RroRenderWork) == 0x60, "RroRenderWork");
_Static_assert(RR_FIELD_OFFSET(RroRenderWork, vertices28) == 0x28, "vertices28");
_Static_assert(RR_FIELD_OFFSET(RroRenderWork, projected50) == 0x50, "projected50");
_Static_assert(sizeof(RroSceneScratchPrefix) == 0x7C, "RroSceneScratchPrefix");
_Static_assert(RR_FIELD_OFFSET(RroSceneScratchPrefix, depthShift60) == 0x60, "depthShift60");
_Static_assert(RR_FIELD_OFFSET(RroSceneScratchPrefix, viewport6C) == 0x6C, "viewport6C");
_Static_assert(RR_FIELD_OFFSET(RroSceneScratchPrefix, draw74) == 0x74, "draw74");
_Static_assert(RR_FIELD_OFFSET(RroModelScratch, work7C) == 0x7C, "work7C");
_Static_assert(sizeof(RroModelScratch) == 0xDC, "RroModelScratch");

#endif
