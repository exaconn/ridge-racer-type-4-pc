#ifndef RR_MENU_ARENA_STATE_H
#define RR_MENU_ARENA_STATE_H

#include "rr/menu_arena_types.h"

/* Two shared CPU pointer cells, retained at full width on native builds.
 * The PS1 map also names the second one MenuSharedScratchPtr. */
extern MenuArenas8 MenuArenas;

#endif
