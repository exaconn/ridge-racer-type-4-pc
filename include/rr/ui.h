#ifndef RR_UI_H
#define RR_UI_H

#include "rr/layout.h"
#include "rr/gpu.h"

/* Shared 0x24-byte UI row. Cell clearing may extend into later fields/rows;
 * callers provide the complete writable backing allocation for count bytes. */
typedef struct {
    s16 x, y, w, h;
} RectS16;

typedef struct {
    u8 reserved00, byte01, byte02, reserved03;
    s16 half04, half06, half08, half0A;
    u8 cells[8], byte14, byte15, byte16, byte17;
    RectS16 rect18;
    s16 half20, half22;
} UiRow24;

extern s32 UiRowCount;
extern UiRow24 *UiRows;

typedef struct {
    u8 reserved00[0x4D80];
    UiRow24 *rows4D80;
} UiRowPointerPage;
RR_ASSERT_OFFSET(UiRowPointerPage, rows4D80, 0x4D80);
/* Legacy callers may append ignored words after the ten row arguments.
 * The signed upper-bound check accepts negative indices; these require a
 * readable/writable prefix. Positive count clears bytes beginning at cells. */
void InitUiRow(s32 index, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                   s32 count, s32 g, s32 h,...);

RR_ASSERT_SIZE(UiRow24, 0x24);
RR_ASSERT_OFFSET(UiRow24, byte01, 1);
RR_ASSERT_OFFSET(UiRow24, byte02, 2);
RR_ASSERT_OFFSET(UiRow24, half04, 4);
RR_ASSERT_OFFSET(UiRow24, half06, 6);
RR_ASSERT_OFFSET(UiRow24, half08, 8);
RR_ASSERT_OFFSET(UiRow24, half0A, 0xA);
RR_ASSERT_OFFSET(UiRow24, cells, 0xC);
RR_ASSERT_OFFSET(UiRow24, byte14, 0x14);
RR_ASSERT_OFFSET(UiRow24, byte15, 0x15);
RR_ASSERT_OFFSET(UiRow24, byte16, 0x16);
RR_ASSERT_OFFSET(UiRow24, byte17, 0x17);
RR_ASSERT_OFFSET(UiRow24, half20, 0x20);

/* 0x20-byte UI particle record (ParticleEntries, initialized by SpawnParticlePair). */
typedef union {
    s16 s;
    u16 u;
} ParticleHalf2;

typedef struct {
    s32 active00, state04, timer08;
    s16 x0C, y0E;
    u8 r10, g11, b12, pad13;
    s16 spanX14;
    s16 speed16;
    s16 rot18;
    s16 scale1A;
    s16 velX1C;
    s16 velY1E;
} UiParticle20;

typedef struct {
    s32 unk00;
    s32 spawnTimer04;
    s32 fadeDuration08;
    s32 fadeTimer0C;
    s32 fadeStart10;
    s32 fadeTarget14;
    s32 brightness18;
    s32 unk1C[4];
    UiParticle20 entries2C[24];
} ParticleSystem32C;

extern ParticleSystem32C ParticleSystem;
extern UiParticle20 ParticleEntries[];

RR_ASSERT_SIZE(UiParticle20, 0x20);
RR_ASSERT_OFFSET(UiParticle20, x0C, 0xC);
RR_ASSERT_OFFSET(UiParticle20, r10, 0x10);
RR_ASSERT_OFFSET(UiParticle20, spanX14, 0x14);
RR_ASSERT_OFFSET(UiParticle20, scale1A, 0x1A);
RR_ASSERT_SIZE(ParticleSystem32C, 0x32C);
RR_ASSERT_OFFSET(ParticleSystem32C, entries2C, 0x2C);

#endif
