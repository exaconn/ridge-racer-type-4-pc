#ifndef RR_ANIMATED_GLYPH_H
#define RR_ANIMATED_GLYPH_H
#include "rr/icon_sprites.h"
#include "psyq/libgpu/gpudisp.h"
void SetSprt(void *packet);
/* PsyZ libgpu: link packet p at the head of ordering entry ot (OT_TYPE). */
void AddPrim(void *ot, void *p);
void DrawAnimatedGlyph(s32 x,s32 y,s32 glyph,s32 clut,s16 *animation);
/* Emits one animated 8x24 foreground sprite and its draw-mode packet.
 * The original retains a shadow branch but exits after pass zero. Animation is
 * a readable/writable signed16 cell; the four-byte caller cell's spare halfword
 * is untouched. All four scalar inputs retain32 bits, clut publishes low16.
 * Cursor and frame owners must be live and disjoint from the animation cell.
 * Packet links are host GpuDmaTag pointers. */
#endif
