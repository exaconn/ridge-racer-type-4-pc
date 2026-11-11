#ifndef RR_GAME_MENU_MNTROW_H
#define RR_GAME_MENU_MNTROW_H

#include "types.h"

void DrawMenuTextRow(s32 a0);
void SetGlyphRowImageBase(u8 *imageBase, s32 firstRow);
void DrawResultsRankIcons(void);
void SetMenuTextRowStyle(int a0, int a1, int a2);
void ModeSelectMenuEnter(void);
void InitModeSelectMenu(void);
void ModeSelectMenuUpdate(void);


#endif /* RR_GAME_MENU_MNTROW_H */
