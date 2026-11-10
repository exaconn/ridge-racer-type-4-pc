#ifndef RR_MENU_ARENA_BASE_H
#define RR_MENU_ARENA_BASE_H

/* Root of the active menu resource/workspace allocation. Each consumer selects
 * its own complete view; the shared CPU pointer retains native address width. */
extern void *MenuArenaBase;
#endif
