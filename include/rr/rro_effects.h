#ifndef RR_RRO_EFFECTS_H
#define RR_RRO_EFFECTS_H

#include "rr/car_render_types.h"
#include "game/car/bounds.h"
#include "rr/race_types.h"
#include "rr/rro_model.h"

/* One projected glow point: its adjusted GTE translation, screen centre and
 * actual ordering-table cell. The pointer retains its native width. */
typedef struct {
    Vec3i translation;
    s32 centerXY;
    u32 *ordering;
} RroGlowProjection;

typedef union {
    u8 bytes[0x28];
    struct {
        u32 reserved44;
        MatrixWords32 rotation48;
        u32 reserved68;
    } glow;
} RroEffectMatrixArea;

/* The fan cursor begins two words before its centre. It advances by three
 * before each GTE store, producing eighteen XY outputs after that centre.
 * The circle has seventeen points (including the repeated closing point);
 * the final group also prefetches the following data word, whose projection
 * is stored but not emitted. Trails use XY/depth pairs at the centre's address.
 * The body cursor starts three XY/depth pairs before the first result and
 * advances before each triple store. All cursors stay in their actual arrays.
 * Sixty pairs fit after PAL scratchpad+0x7C+0x1A0. */
typedef union {
    RroProjectedVertex pipeline[63];
    struct {
        u32 reserved188[4];
        s32 xy[21];
    } fan;
    struct {
        u32 reserved188[6];
        s32 xy[4];
    } shadow;
    struct {
        u32 reserved188[6];
        RroProjectedVertex vertices[60];
    } mesh;
    struct {
        u32 reserved188[6];
        RroGlowProjection points[24];
    } glow;
} RroProjectionWorkspace;

/* The color cursor advances before each of the six stores. Its initial word
 * belongs to the same scratch array and remains untouched. */
typedef union {
    s32 words[7];
    struct {u32 reserved15C; s32 rgb160[6];} palette;
} RroEffectColors;

typedef struct {
    u32 input[6];
    u8 farColor[6][4];
} RroEffectColorTable;

/* Effect workspace at PAL scratchpad+0x7C. Pointer cells name the actual
 * selected owners; native layouts keep their complete pointer widths. */
typedef struct {
    MatrixWords32 object00;
    u8 reserved20[0x18];
    Vec3i worldPosition38;
    RroEffectMatrixArea matrix44;
    u32 *ordering6C;
    s32 depth70, firstArea74, secondArea78;
    const Vec3s8 *vertices7C;
    u32 reserved80;
    const u32 *commands84;
    s32 vertexCount88, flags8C;
    RaceVehicle *vehicle90;
    CarBounds *bounds94;
    Vec3i facing98;
    u32 reservedA4;
    Vec3sFields8 *trailVerticesA8;
    Vec3i jitterTranslationAC;
    u32 reservedB8;
    s32 windingBC, blendC0, temporaryC4;
    Vec3s8 inputC8;
    RroProjectedVertex *projectedD0, *projectedD4;
    s32 *colorsD8;
    s32 interpolationDC;
    Vec3s8 verticesE0[15];
    u32 reserved158;
    RroEffectColors colors15C;
    u8 reserved178[0x10];
    RroProjectionWorkspace projection188;
} RroEffectWork;

typedef struct {
    RroSceneScratchPrefix scene;
    RroEffectWork work7C;
} RroEffectScratch;

/* One E1 draw-mode word followed by a semi-transparent POLY_G3 triangle. */
typedef struct {
    GpuDmaTag tag;
    u32 drawMode04, centerColor08;
    s32 centerXY0C;
    u32 firstColor10;
    s32 firstXY14;
    u32 secondColor18;
    s32 secondXY1C;
} RroGlowPacket;

/* One E1 draw-mode word followed by the four color/XY pairs of POLY_G4. */
typedef struct {
    GpuDmaTag tag;
    u32 drawMode04;
    struct {s32 color; s32 xy;} vertices[4];
} RroBodyPacket;

/* One E1 draw-mode word followed by a flat-color LINE_F2. */
typedef struct {
    GpuDmaTag tag;
    u32 drawMode04, color08;
    s32 firstXY0C, secondXY10;
} RroSparkPacket;

/* Count is the sign-extended high half of the command header. Positive counts
 * require that many complete command records and accessible projected vertices.
 * The scene supplies the current GPU cursor; depthShift60 + 4 is in 0..31. */
const RroTexturedQuadPrefix *EmitProjectedQuadsFT4(const RroTexturedQuadPrefix *commands,
                                                s32 count, RroEffectWork *work);

/* Legacy name: emits three semi-transparent untextured G4 panels and an E1
 * command, totaling 116 bytes. Count is the signed command-header halfword.
 * Both projected arrays cover every selected outer/center index; the initial
 * shift depthShift60 + 3 is 0..31. Even a clipped face can stage three XY words
 * at the current cursor. Emitted panels and the selected ordering head fit
 * their registered GPU region. */
void EmitProjectedLitTrisGT3(const RroTrailPanelCommand *commands, s32 count,
                           RroEffectWork *work);

/* Nine ring vertices include the repeated closing point. */
extern Vec3s8 SpotlightConeRingOuter[9];

/* Legacy name: each visible point emits eight untextured G3 triangles followed
 * by E1, totaling 232 bytes. Count is the signed command-header halfword; a
 * positive count is at most 24 and requires that many complete input records.
 * The captured depthShift60 + 2 is 0..31. Every staged ordering-table cell and
 * emitted packet fits its registered GPU region. Returns the input end. */
const RroGlowPointCommand *EmitProjectedLitQuadsGT4(const RroGlowPointCommand *points,
                                                s32 count, RroEffectWork *work);

void DrawGlowFan(CarSlotBufferC8 *slot, RroEffectWork *work);
void TransformRroVertices(CarSlotBufferC8 *slot, RroEffectWork *work);
void DrawSparkLines(RroSpark *sparks, RroEffectWork *work, s32 speed);
void EmitCarBodyQuads(const RroEffectMesh *mesh, RroEffectWork *work);
void EmitCarShadowQuads(const RroEffectMesh *mesh, RroEffectWork *work);

RR_ASSERT_SIZE(RroEffectMatrixArea, 0x28);
typedef char rr_glow_rotation_offset[
    RR_FIELD_OFFSET(RroEffectMatrixArea, glow.rotation48) == 4 ? 1 : -1];
RR_ASSERT_OFFSET(RroGlowProjection, centerXY, 12);
RR_ASSERT_OFFSET(RroGlowProjection, ordering, 16);
typedef char rr_glow_projection_offset[
    RR_FIELD_OFFSET(RroProjectionWorkspace, glow.points) == 24 ? 1 : -1];
RR_ASSERT_SIZE(RroEffectColors, 28);
RR_ASSERT_SIZE(RroEffectColorTable, 48);
RR_ASSERT_SIZE(RroGlowPacket, 32);
RR_ASSERT_OFFSET(RroGlowPacket, centerXY0C, 12);
RR_ASSERT_OFFSET(RroGlowPacket, secondXY1C, 28);
RR_ASSERT_SIZE(RroBodyPacket, 40);
RR_ASSERT_OFFSET(RroBodyPacket, vertices, 8);
RR_ASSERT_SIZE(RroSparkPacket, 20);
RR_ASSERT_OFFSET(RroSparkPacket, color08, 8);
#if defined(mips)
RR_ASSERT_SIZE(RroGlowProjection, 20);
RR_ASSERT_SIZE(RroProjectionWorkspace, 0x1F8);
RR_ASSERT_OFFSET(RroEffectWork, ordering6C, 0x6C);
RR_ASSERT_OFFSET(RroEffectWork, vertices7C, 0x7C);
RR_ASSERT_OFFSET(RroEffectWork, vehicle90, 0x90);
RR_ASSERT_OFFSET(RroEffectWork, bounds94, 0x94);
RR_ASSERT_OFFSET(RroEffectWork, inputC8, 0xC8);
RR_ASSERT_OFFSET(RroEffectWork, projectedD0, 0xD0);
RR_ASSERT_OFFSET(RroEffectWork, colors15C, 0x15C);
RR_ASSERT_OFFSET(RroEffectWork, projection188, 0x188);
RR_ASSERT_SIZE(RroEffectWork, 0x380);
RR_ASSERT_OFFSET(RroEffectScratch, work7C, 0x7C);
RR_ASSERT_SIZE(RroEffectScratch, 0x3FC);
#endif

#endif
