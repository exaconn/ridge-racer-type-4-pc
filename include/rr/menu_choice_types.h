#ifndef RR_MENU_CHOICE_TYPES_H
#define RR_MENU_CHOICE_TYPES_H
#include "types.h"
/* Byte-aligned four-byte selection prefix. */
typedef struct {u8 bytes[4];} MenuChoice4;
/* The current team/maker/car selection (0x800AD4F4, which the PS1 map names
 * MenuSelectedTeamId and MenuSelectedMakerId) is SelectionState.fields.copies[0]. */
#endif
