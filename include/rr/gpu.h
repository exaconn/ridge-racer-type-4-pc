#ifndef RR_GPU_H
#define RR_GPU_H

#include "rr/layout.h"

/* Configured GPU data and status/control register addresses. */
extern volatile u32 *GPU_DATA, *GPU_STATUS;
s32 GPU_CheckVersion(s32 mode);

/* GPU DMA channel and queue timeout state. Queue tail is interrupt-visible. */
extern volatile u32 *DMA2_MADR, *DMA2_BCR, *DMA2_CHCR;
extern s32 GpuResetSavedIntrMask, GpuTimeoutDeadline, GpuAlarmCount;
extern volatile s32 _qin, _qout;

/* Drawing limits and packed E3/E4 drawing-area coordinate commands. */
extern s16 D_800ABAC4, D_800ABAC6;
u32 get_cs(s32 x, s32 y);
u32 get_ce(s32 x, s32 y);

/* Ordering-table DMA on channel 6. Address/count arithmetic wraps at 32 bits on PAL.
 * The supplied address is recorded after adding signed32(count*4-4). */
extern u8 * volatile *DMA6_MADR;
extern volatile u32 *DMA6_BCR, *DMA6_CHCR, *DPCR;
s32 _otc(u8 *buffer, s32 count);
void set_alarm(void);
s32 get_alarm(void);

/* RECT has both signed-coordinate and packed-word views on little-endian PAL. */
typedef struct { s16 x,y,w,h; } GpuRectangleFields;
typedef union GpuRectangle {
    GpuRectangleFields rect;
    u32 words[2];
} GpuRectangle;

typedef struct {
    u32 position, destination, size;
} GpuMovePayload12;

/* The DMA packet header occupies the eight bytes preceding this payload. */
extern GpuMovePayload12 GpuMovePayload;
void checkRECT(const char *name, GpuRectangle *rectangle);
void GPU_DmaDoneRequeue();
void _cwc(unsigned long address);
s32 MoveImage2(GpuRectangle *rectangle, s32 x, s32 y);
/* Uploads ceil(width*height/2) packed pixel words, using a CPU tail and DMA
 * blocks of16 words. Dimensions clamp against the signed configured limits
 * (inclusive), storing their low16 bits. Returns-1 for a nonpositive word
 * count or timeout, otherwise0. The source must cover the complete transfer. */
s32 _dws(GpuRectangle *rectangle, const u32 *pixels);
/* Reverse transfer with the same dimension and buffer-capacity contract.
 * Polls command readiness, requests image data, then polls receive readiness
 * before reading the CPU tail and starting DMA. Returns -1 on either timeout
 * or a nonpositive word count, otherwise 0. */
s32 _drs(GpuRectangle *rectangle, u32 *pixels);
/* In-place dimensions clamp against signed limit-1. Aligned X/width use the
 * fast GPU fill; other rectangles use a linked packet that restores draw state.
 * The color's high bit contributes bit10 of the drawing-mode command. */
s32 _clr(GpuRectangle *rectangle, u32 color);
/* Query a GPU state selector and return its low24 bits. */
s32 _param(s32 selector);
RR_ASSERT_SIZE(GpuRectangle, 8);
RR_ASSERT_OFFSET(GpuRectangleFields, w, 4);
RR_ASSERT_OFFSET(GpuRectangleFields, h, 6);
RR_ASSERT_SIZE(GpuMovePayload12, 12);
RR_ASSERT_OFFSET(GpuMovePayload12, destination, 4);
RR_ASSERT_OFFSET(GpuMovePayload12, size, 8);

/* GPU sprite packets with a 24-bit link and 8-bit command length.
 * CLUT signedness differs between original units; preserve both views. */

typedef struct SpritePacket {
    u32 addr:24;
    u32 len:8;
    u8 r, g, b, code;
    s16 x, y;
    u8 u, v;
    u16 clut;
    s16 w, h;
} SpritePacket;

typedef struct SpritePacketSignedClut {
    u32 addr:24;
    u32 len:8;
    u8 r, g, b, code;
    s16 x, y;
    u8 u, v;
    s16 clut, w, h;
} SpritePacketSignedClut;

RR_ASSERT_SIZE(SpritePacket, 0x14);
RR_ASSERT_OFFSET(SpritePacket, r, 0x4);
RR_ASSERT_OFFSET(SpritePacket, x, 0x8);
RR_ASSERT_OFFSET(SpritePacket, clut, 0xe);
RR_ASSERT_OFFSET(SpritePacket, w, 0x10);
RR_ASSERT_SIZE(SpritePacketSignedClut, 0x14);
RR_ASSERT_OFFSET(SpritePacketSignedClut, r, 0x4);
RR_ASSERT_OFFSET(SpritePacketSignedClut, x, 0x8);
RR_ASSERT_OFFSET(SpritePacketSignedClut, clut, 0xe);
RR_ASSERT_OFFSET(SpritePacketSignedClut, w, 0x10);

/* Serialized GPU DMA tags retain only the low24 address bits. The tag is
 * an encoded hardware word, never a C pointer used for memory access. */
typedef union {
    u32 word;
    struct {u8 link[3],length;} bytes;
    struct {u32 address:24,length:8;} bits;
} GpuDmaTag;
/* RGB bytes and command byte shared by CPU templates and GTE output words. */
typedef struct {u8 r,g,b,code;} GpuColorCommand;
typedef union {u32 word;GpuColorCommand bytes;} GpuColorWord;
RR_ASSERT_SIZE(GpuColorCommand, 4);
RR_ASSERT_OFFSET(GpuColorCommand, code, 3);
RR_ASSERT_SIZE(GpuColorWord, 4);

typedef struct {
    GpuColorWord color0; s32 xy0; u32 uv0;
    GpuColorWord color1; s32 xy1; u32 uv1;
    GpuColorWord color2; s32 xy2; u32 uv2;
    GpuColorWord color3; s32 xy3; u32 uv3;
} GpuGT4Body;
/* A direct packet starts at command04; a masked packet starts at tag00.
 * Both layouts share the complete GT4 body at08. */
typedef struct {
    GpuDmaTag tag00;
    union {u32 word;GpuDmaTag directTag;} command04;
    GpuGT4Body body08;
} GpuEnvMapBlock;
/* The secondary coordinates are staged even for direct rendering. A caller
 * needs the whole lookahead footprint although direct emission advances56. */
typedef struct {
    GpuEnvMapBlock primary00, reflected38;
    struct {GpuDmaTag tag;u32 command;} restore70;
} GpuEnvMapEmission;
RR_ASSERT_SIZE(GpuGT4Body, 48);
RR_ASSERT_SIZE(GpuEnvMapBlock, 56);
RR_ASSERT_OFFSET(GpuEnvMapBlock, body08, 8);
RR_ASSERT_SIZE(GpuEnvMapEmission, 120);
RR_ASSERT_OFFSET(GpuEnvMapEmission, reflected38, 56);
RR_ASSERT_OFFSET(GpuEnvMapEmission, restore70, 112);

typedef struct {GpuDmaTag tag;u32 command[2];} GpuDrawModePacket;
typedef struct {GpuDmaTag tag;u32 command;} GpuCommandPacket;
typedef struct {
    GpuDmaTag tag;
    s32 color0, xy0, color1, xy1, color2, xy2;
} GpuPolyG3;
/* One DMA chain entry: E6 mask command, untextured Gouraud quad, E6 restore.
 * Color words include the primitive command byte produced by the GTE. */
typedef struct {
    GpuDmaTag tag;
    u32 setMask04;
    s32 color08, xy0C, color10, xy14, color18, xy1C, color20, xy24;
    u32 restoreMask28;
} GpuMaskedG4Packet;
RR_ASSERT_SIZE(GpuMaskedG4Packet, 44);
RR_ASSERT_OFFSET(GpuMaskedG4Packet, color08, 8);
RR_ASSERT_OFFSET(GpuMaskedG4Packet, color20, 32);
RR_ASSERT_OFFSET(GpuMaskedG4Packet, restoreMask28, 40);
RR_ASSERT_SIZE(GpuCommandPacket, 8);
RR_ASSERT_SIZE(GpuPolyG3, 28);
RR_ASSERT_OFFSET(GpuPolyG3, xy0, 8);
RR_ASSERT_OFFSET(GpuPolyG3, xy2, 24);
RR_ASSERT_SIZE(GpuDrawModePacket,12);
RR_ASSERT_OFFSET(GpuDrawModePacket,command,4);
typedef struct {
    GpuDmaTag tag;
    u8 r,g,b,code;
    s16 x,y,w,h;
} GpuTilePacket;
RR_ASSERT_SIZE(GpuDmaTag,4);
RR_ASSERT_SIZE(GpuTilePacket,16);
RR_ASSERT_OFFSET(GpuTilePacket,r,4);
RR_ASSERT_OFFSET(GpuTilePacket,x,8);
RR_ASSERT_OFFSET(GpuTilePacket,w,12);

/* Flat triangle: three packed signed16 X/Y pairs in wire words. */
typedef struct {
    GpuDmaTag tag;
    u8 r, g, b, code;
    s32 xy0, xy1, xy2;
} PolyF3Packet;
RR_ASSERT_SIZE(PolyF3Packet,20);
RR_ASSERT_OFFSET(PolyF3Packet,xy0,8);

/* Textured quad and four-vertex polyline DMA packets. */
typedef struct {
    GpuDmaTag tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad1;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad2;
} GpuPolyFT4Packet;
/* Packed word writes and byte command/tag updates address the same packet. */
typedef union {
    GpuPolyFT4Packet fields;
    struct {
        GpuDmaTag tag;
        s32 color04;
        s32 xy0, uv0, xy1, uv1, xy2, uv2, xy3, uv3;
    } words;
} GpuPolyFT4;
typedef struct {
    GpuDmaTag tag;
    u8 r, g, b, code;
    s16 x0, y0, x1, y1, x2, y2, x3, y3;
    u32 pad;
} GpuLineF4Packet;
RR_ASSERT_SIZE(GpuPolyFT4Packet,40);
RR_ASSERT_SIZE(GpuPolyFT4,40);
RR_ASSERT_OFFSET(GpuPolyFT4Packet,x0,8);
RR_ASSERT_OFFSET(GpuPolyFT4Packet,u0,12);
RR_ASSERT_OFFSET(GpuPolyFT4Packet,tpage,22);
RR_ASSERT_OFFSET(GpuPolyFT4Packet,x3,32);
RR_ASSERT_SIZE(GpuLineF4Packet,28);
RR_ASSERT_OFFSET(GpuLineF4Packet,x0,8);
RR_ASSERT_OFFSET(GpuLineF4Packet,pad,24);

/* Quad primitive GPU packets (POLY_F4, POLY_G4, POLY_GT4) and mesh face records. */
typedef struct {
    u8 link[3], len;
    s32 colorCode04;
    s32 xy0, xy1, xy2, xy3;
} PolyF4Packet;

typedef struct {
    u8 link[3], len;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad0F;
    s16 x1, y1;
    u8 r2, g2, b2, pad17;
    s16 x2, y2;
    u8 r3, g3, b3, pad1F;
    s16 x3, y3;
} PolyG4Packet;

typedef struct {
    u8 link[3], len;
    s32 color04, xy0, color0C, xy1, color14, xy2, color1C, xy3;
} PolyG4WordPacket;

/* The byte fields, packed color/XY writes and DMA header share one G4 packet. */
typedef union {
    PolyG4Packet fields;
    PolyG4WordPacket words;
    GpuDmaTag tag;
} GpuPolyG4;
RR_ASSERT_SIZE(GpuPolyG4, 36);
RR_ASSERT_OFFSET(PolyG4WordPacket, xy3, 32);

typedef struct {
    u8 link[3], len;
    s32 color04, xy0, uvClut0C, color10, xy1, uvTpage18, color1C, xy2;
    u16 uv2;
    u8 pad26[2];
    s32 color28, xy3;
    u16 uv3;
    u8 pad32[2];
} PolyGT4WordPacket;

typedef struct {
    u16 v0, v1, v2, v3;
    s32 color08;
    u8 pad0C, otBias0D, pad0E[2];
} MeshQuadF4;

typedef struct {
    u16 v0, v1, v2, v3;
    s32 color08, color0C, color10, color14;
    u8 pad18, otBias19, pad1A[2];
} MeshQuadG4;

typedef struct {
    u16 v0, v1, v2, v3;
    s32 color08, color0C, color10, color14, uvClut18, uvTpage1C;
    u16 uv2, uv3;
    u8 pad24, otBias25, pad26[2];
} MeshQuadGT4;

RR_ASSERT_SIZE(PolyF4Packet, 0x18);
RR_ASSERT_SIZE(PolyG4Packet, 0x24);
RR_ASSERT_OFFSET(PolyG4Packet, x0, 0x8);
RR_ASSERT_OFFSET(PolyG4Packet, x1, 0x10);
RR_ASSERT_OFFSET(PolyG4Packet, x2, 0x18);
RR_ASSERT_OFFSET(PolyG4Packet, x3, 0x20);
RR_ASSERT_SIZE(PolyG4WordPacket, 0x24);
RR_ASSERT_SIZE(PolyGT4WordPacket, 0x34);
RR_ASSERT_OFFSET(PolyGT4WordPacket, uv2, 0x24);
RR_ASSERT_OFFSET(PolyGT4WordPacket, uv3, 0x30);
RR_ASSERT_SIZE(MeshQuadF4, 0x10);
RR_ASSERT_SIZE(MeshQuadG4, 0x1C);
RR_ASSERT_SIZE(MeshQuadGT4, 0x28);

#endif
