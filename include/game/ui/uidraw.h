#ifndef RR_GAME_UI_UIDRAW_H
#define RR_GAME_UI_UIDRAW_H

#include "types.h"
#include "rr/menu_rect_rows.h"
#include "rr/menu_glyph_types.h"

extern MenuGlyphRow1C *GlyphRows;
extern MenuGlyphPreset14 AnimatedGlyphPresets[];

extern char SizeLdFmt[];
extern int BadgeRowCount;
int AllocRect16Rows(void *a0);
int AllocGlyphRows(void *buffer);
void UpdateBadgeRowAnim(int index);
void InitRect16Row(s32 index, s16 x, s16 y, s16 width, s16 height, s16 phase, s16 parameter);
int UpdateRect16Row(int index);
void InitGlyphRow(int index, int control, int glyph, int presetIndex);


#endif /* RR_GAME_UI_UIDRAW_H */
