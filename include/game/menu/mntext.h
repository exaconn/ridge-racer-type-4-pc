#ifndef RR_GAME_MENU_MNTEXT_H
#define RR_GAME_MENU_MNTEXT_H

#include "types.h"
#include "rr/menu_text_rows.h"

s32 InitMenuTextRows(MenuTextRow60 *rows);
void SetMenuTextRow(s32 row, s32 position, s32 phase, s32 style, s32 label);


#endif /* RR_GAME_MENU_MNTEXT_H */
