#ifndef RR_GLYPH_LIST_H
#define RR_GLYPH_LIST_H
#include "game/ui/hudmap2.h"
#include "rr/icon_sprites.h"
#include "rr/ui_bar.h"
typedef GpuCommandPacket GlyphCommand8;
typedef struct {SpritePacketSignedClut sprite;GlyphCommand8 mode;} GlyphPacket28;
extern UiUvRect CarHandlingResponseTable[50],HudGlyphUvRects[40];
/* Readable signed16 x/y, then glyph/gap pairs and a negative terminator.
 * Bank0 selects primary glyph0..49; every nonzero bank selects0..39.
 * Glyph0x400..0x427 always selects the secondary40-entry font. Each glyph
 * needs28 writable aligned bytes in a live packet arena, disjoint from input
 * and font owners. Software cursors retain native width; only DMA wire tags
 * use low24 address bits. X arithmetic wraps32 before wire16 publication. */
#endif
