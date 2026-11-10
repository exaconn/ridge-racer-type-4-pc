#ifndef RR_ANIMATED_GLYPH_H
#define RR_ANIMATED_GLYPH_H
#include "rr/icon_sprites.h"
#include "rr/draw_environment.h"
#ifndef RR_GLYPH_ANIMATION_SCRATCH
#define RR_GLYPH_ANIMATION_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
void SetSprt(void *packet);
void AddPrim(u32 *ordering,u32 *packet);
void DrawAnimatedGlyph(s32 x,s32 y,s32 glyph,s32 clut,s16 *animation);
/* Each preference binds a real initialized value only for the empty constraint;
 * the enclosing C variable remains tracked normally by the allocator. */
#ifdef __mips__
#define RR_GLYPH_PREF(value,reg) ({ register __typeof__(value) prefValue __asm__(reg)=(value); __asm__("" : "+r"(prefValue)); prefValue; })
#else
#define RR_GLYPH_PREF(value,reg) (value)
#endif
/* Emits one animated 8x24 foreground sprite and its draw-mode packet (32 bytes).
 * The original retains a shadow branch but exits after pass zero. Animation is
 * a readable/writable signed16 cell; the four-byte caller cell's spare halfword
 * is untouched. All four scalar inputs retain32 bits, clut publishes low16.
 * Cursor and frame owners must be live and disjoint from the animation cell.
 * Software pointers keep native width; only GPU DMA links serialize low24. */
#endif
