#ifndef RR_MENU_ARENAS_H
#define RR_MENU_ARENAS_H
#include "rr/menu_choice.h"
#include "rr/menu_arena_types.h"

/* Two resident pointer cells; the second is refreshed by menu resource loading. */
#include "rr/menu_arena_state.h"
#include "rr/cd_directory_buffer.h"
void InitMenuArenas(void);
void ovl662_init_history_overview(void);
void ovl662_setup_history_arenas(void);
RR_ASSERT_SIZE(MenuArenas8,2*sizeof(void *));
RR_ASSERT_OFFSET(MenuArenas8,second,sizeof(void *));
#endif
