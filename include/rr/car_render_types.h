#ifndef RR_CAR_RENDER_TYPES_H
#define RR_CAR_RENDER_TYPES_H

#include "rr/layout.h"
#include "rr/geometry.h"

typedef struct {
    s32 sxy, sz;
} RroProjectedVertex;

/* A selected model block: packed short vertices, count and primitive stream. */
typedef struct {
    const Vec3s8 *vertices;
    u32 vertexCount;
    const u32 *commands;
} RroEffectMesh;

typedef union {s32 words[2];u16 indices[4];} RroQuadIndices;

/* Consecutive UV/CLUT and UV/texture-page words shared by textured model
 * commands. The patcher changes only the signed attribute halves, preserving
 * the packed UV coordinates and every surrounding geometry word. */
typedef struct {
    u16 firstUV;
    s16 clut;
    u16 secondUV;
    s16 texturePage;
} RroTextureAttributes;

RR_ASSERT_SIZE(RroTextureAttributes, 8);
RR_ASSERT_OFFSET(RroTextureAttributes, clut, 2);
RR_ASSERT_OFFSET(RroTextureAttributes, texturePage, 6);

/* Texture tail in an environment-map command. A negative page selects the
 * slot-atlas rewrite; the normal branch only patches CLUT/page attributes. */
typedef struct {
    RroTextureAttributes texture;
    s16 thirdUV, fourthUV;
    u8 reserved1C;
    s8 orderingBias1D;
    u16 control;
} RroEnvironmentTextureAttributes;
RR_ASSERT_SIZE(RroEnvironmentTextureAttributes, 16);
RR_ASSERT_OFFSET(RroEnvironmentTextureAttributes, thirdUV, 8);
RR_ASSERT_OFFSET(RroEnvironmentTextureAttributes, control, 14);

/* Raw-texture FT4 command stream: four packed vertex indices, three UV words
 * and a signed ordering-table bias. The fourth UV is the signed high half of
 * uv10. The emitter prefetches one extra record after the counted commands. */
typedef struct {
    RroQuadIndices vertices;
    s32 uv08, uv0C, uv10;
    u8 reserved14;
    s8 orderingBias15;
    u16 reserved16;
} RroRawTexturedQuad;

RR_ASSERT_SIZE(RroRawTexturedQuad, 24);
RR_ASSERT_OFFSET(RroRawTexturedQuad, uv08, 8);
RR_ASSERT_OFFSET(RroRawTexturedQuad, orderingBias15, 21);

/* Lit Gouraud G4 command stream. Geometry is prefetched one command ahead;
 * normal indices, source RGB and ordering bias belong to the current command. */
typedef struct {
    RroQuadIndices vertices, normals;
    u32 color10;
    u8 reserved14;
    s8 orderingBias15;
    u16 reserved16;
} RroLitGouraudQuad;

RR_ASSERT_SIZE(RroQuadIndices, 8);
RR_ASSERT_SIZE(RroLitGouraudQuad, 24);
RR_ASSERT_OFFSET(RroLitGouraudQuad, normals, 8);
RR_ASSERT_OFFSET(RroLitGouraudQuad, color10, 16);
RR_ASSERT_OFFSET(RroLitGouraudQuad, orderingBias15, 21);

/* Environment-mapped command: the primary UVs are stored in the stream;
 * a second pass derives UVs from transformed normals when control bits0..1
 * are nonzero. The fourth primary UV sign-extends the high half of uv18.
 * Geometry is still prefetched one record ahead. */
typedef struct {
    RroQuadIndices vertices, normals;
    s32 uv10, uv14, uv18;
    u8 reserved1C;
    s8 orderingBias1D;
    u16 control1E;
} RroEnvMapQuad;

RR_ASSERT_SIZE(RroEnvMapQuad, 32);
RR_ASSERT_OFFSET(RroEnvMapQuad, normals, 8);
RR_ASSERT_OFFSET(RroEnvMapQuad, uv10, 16);
RR_ASSERT_OFFSET(RroEnvMapQuad, orderingBias1D, 29);
RR_ASSERT_OFFSET(RroEnvMapQuad, control1E, 30);

/* Shared textured-quad prefix. The high half of uv14 supplies the fourth UV
 * word, with signed extension. Flash quads advance by this complete prefix;
 * the shadow renderer reads just one prefix and its command tail is unknown. */
typedef struct {
    RroQuadIndices vertices;
    u32 reserved08;
    s32 uv0C, uv10, uv14;
    s8 flashPhase18;
    s8 orderingBias19;
    u16 reserved1A;
} RroTexturedQuadPrefix;

/* Three radial panels join matching outer/center points in two projected slices.
 * The ordering bias belongs to the resulting four-entry DMA chain. */
typedef struct {
    u16 outer[3], center;
    u8 reserved08;
    s8 orderingBias09;
    u16 reserved0A;
} RroTrailPanelCommand;
RR_ASSERT_SIZE(RroTrailPanelCommand, 12);
RR_ASSERT_OFFSET(RroTrailPanelCommand, center, 6);
RR_ASSERT_OFFSET(RroTrailPanelCommand, orderingBias09, 9);

/* Packed short position with a signed bias in the high byte of its padding word. */
typedef union {
    Vec3s8 position;
    struct {
        s16 x,y,z;
        u8 reserved06;
        s8 orderingBias07;
    } fields;
} RroGlowPointCommand;
RR_ASSERT_SIZE(RroGlowPointCommand, 8);
typedef char rr_glow_command_bias_offset[
    RR_FIELD_OFFSET(RroGlowPointCommand, fields.orderingBias07) == 7 ? 1 : -1];

/* The body stream advances by twenty bytes per quad. */
typedef union {
    u32 word;
    struct {u16 command, count;} fields;
} RroBodyCommandHeader;

typedef struct {
    u16 vertices[4];
    u16 colors[4];
    u32 reserved10;
} RroBodyQuad;

/* Header fields consumed by the car effect renderer and its buffer allocator.
 * Names describe the observed rendering roles, not recovered source names. */
/* Host view of a car's decoded mesh table (RroModelTable returned by
 * RelocateOffsetTable): count, then twelve meshes. The PS1 wire offsets in
 * the field names refer to the 12-byte entries of the serialized table. */
typedef struct {
    s32 count;
    union {
        RroEffectMesh litQuads04;
        struct {const Vec3s8 *litVertices; u32 litVertexCount; const u32 *litQuadCommands0C;};
    };
    RroEffectMesh primary10, secondary1C, trailing28, history34, trailing40;
    RroEffectMesh reserved4C;
    RroEffectMesh shadow58;
    RroEffectMesh body64[4];
} CarModelHeader;

/* Packed motion words are XY, Z/velocity-Z and velocity-X/velocity-Y. */
typedef union {
    struct {s16 x, y, z, velocityZ, velocityX, velocityY;} fields;
    s32 words[3];
} RroSparkMotion;

typedef struct {
    s32 timer;
    RroSparkMotion motion;
} RroSpark;

/* The first two rings hold XY/depth pairs; history holds short-vector copies.
 * Their backing allocations use the same pointer slot and scalar state word. */
typedef struct {
    u32 state;
    void *vertices;
} CarTrailBuffer;

typedef struct {
    const CarModelHeader *model;
    s32 reserved04;
    s32 color08, mainCount0C, mainIndex10;
    CarTrailBuffer main14[4];
    s32 secondCount34, secondIndex38;
    CarTrailBuffer second3C[4];
    s32 phase5C, intensity60, intensity64, historyCount68, historyIndex6C, overlay70;
    Vec3i storedPosition74;
    u32 reserved80;
    CarTrailBuffer history84[8];
    RroSpark *sparksC4;
} CarSlotBufferC8;

RR_ASSERT_SIZE(RroProjectedVertex, 8);
RR_ASSERT_SIZE(RroTexturedQuadPrefix, 28);
RR_ASSERT_OFFSET(RroTexturedQuadPrefix, uv0C, 12);
RR_ASSERT_OFFSET(RroTexturedQuadPrefix, orderingBias19, 25);
RR_ASSERT_OFFSET(RroTexturedQuadPrefix, reserved1A, 26);
RR_ASSERT_SIZE(RroBodyQuad, 20);
RR_ASSERT_SIZE(RroBodyCommandHeader, 4);
RR_ASSERT_OFFSET(RroBodyQuad, colors, 8);
RR_ASSERT_SIZE(RroSpark, 16);
RR_ASSERT_SIZE(RroSparkMotion, 12);
RR_ASSERT_OFFSET(RroSpark, motion, 4);
#if defined(mips)
RR_ASSERT_SIZE(RroEffectMesh, 12);
RR_ASSERT_OFFSET(CarModelHeader, primary10, 0x10);
RR_ASSERT_OFFSET(CarModelHeader, history34, 0x34);
RR_ASSERT_OFFSET(CarModelHeader, shadow58, 0x58);
RR_ASSERT_OFFSET(CarModelHeader, body64, 0x64);
RR_ASSERT_SIZE(CarModelHeader, 0x94);
RR_ASSERT_SIZE(CarTrailBuffer, 8);
RR_ASSERT_SIZE(CarSlotBufferC8, 0xC8);
RR_ASSERT_OFFSET(CarSlotBufferC8, main14, 0x14);
RR_ASSERT_OFFSET(CarSlotBufferC8, second3C, 0x3C);
RR_ASSERT_OFFSET(CarSlotBufferC8, intensity64, 0x64);
RR_ASSERT_OFFSET(CarSlotBufferC8, history84, 0x84);
RR_ASSERT_OFFSET(CarSlotBufferC8, sparksC4, 0xC4);
#endif

#endif
