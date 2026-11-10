#ifndef RR_UI_BAR_H
#define RR_UI_BAR_H
#include "rr/gpu.h"

typedef struct {u8 u,v,w,h;} UiUvRect;
typedef struct {s16 glyph,count;} UiBarDescriptor;
extern UiUvRect SelectionBarGlyphUvs[];
extern UiBarDescriptor SelectionBarDescriptors[];

/* Emits a glyph, left cap, count-dependent middle pieces, right cap and white
 * underline through the existing GPU helpers. Style and glyph indices must
 * name accessible records. Count is signed and re-read after each helper.
 * Coordinates use wrapping32 arithmetic; helper stores narrow to wire16.
 * The supplied cursor is threaded through helper return values; the final
 * helper result is returned to the caller. */
SpritePacket *DrawBadgeTitleBar(u32 *ordering,SpritePacket *cursor,s32 x,s32 y,s32 style);
RR_ASSERT_SIZE(UiUvRect, 4);
RR_ASSERT_SIZE(UiBarDescriptor, 4);
RR_ASSERT_OFFSET(UiBarDescriptor, count, 2);
#endif
