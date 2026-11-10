#ifndef RR_GAME_UI_UIDRAW_H
#define RR_GAME_UI_UIDRAW_H

#include "types.h"

extern char SizeLdFmt[];
extern int BadgeRowCount;
int AllocRect16Rows(int a0);
int AllocGlyphRows(int a);
void UpdateBadgeRowAnim(int a0);
void InitRect16Row(int i, short a, short b, short c, short d, short e, short f);
int UpdateRect16Row(int a0);
void InitGlyphRow(int a0, int a1, int a2, int a3);


#endif /* RR_GAME_UI_UIDRAW_H */
