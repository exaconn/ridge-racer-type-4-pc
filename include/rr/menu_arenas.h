#ifndef RR_MENU_ARENAS_H
#define RR_MENU_ARENAS_H
#include "rr/menu_choice.h"
#include "rr/menu_arena_types.h"

/* Two resident pointer cells; the second is refreshed by menu resource loading. */
extern RrMenuArenas D_800F4D70;
extern u8 *D_800102F0;
void func_8003A1B4(void);
void ovl662_func_80116020(void);
void ovl662_func_80116148(void);
RR_ASSERT_SIZE(RrMenuArenas,8);
RR_ASSERT_OFFSET(RrMenuArenas,second,4);
#endif
