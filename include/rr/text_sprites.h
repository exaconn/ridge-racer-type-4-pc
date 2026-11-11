#ifndef RR_TEXT_SPRITES_H
#define RR_TEXT_SPRITES_H
#include "game/course/trktext.h"

#include "rr/icon_sprites.h"

/* Glyph widths (0x800A01DC, 60 entries); the renderer reaches glyph values
 * past the table through FontDataWindow in trktext.c. */
extern u8 FontGlyphWidths[60];

/* Text must terminate and each emitted glyph needs an aligned 20-byte sprite
 * packet in the scratch cursor's arena. The biased field cursor also requires
 * 14 bytes of accessible arena after the final packet. The context must contain
 * the selected aligned ordering head at 0x70+bank*4; bank*4 must fit signed32.
 * Mapping 255 advances X by four without emitting. Color exactly 255 selects
 * raw textured mode and preserves RGB; other values publish their low byte.
 * Widths and text are read again after packet stores/list insertion. Coordinates
 * and X increments wrap at 16/32 bits respectively. Height is 12 and width comes
 * from the first width-table read. */

#endif
