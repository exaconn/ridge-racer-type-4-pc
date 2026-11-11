#ifndef RR_MENU_CHOICE_TYPES_H
#define RR_MENU_CHOICE_TYPES_H
#include "types.h"
/* Byte-aligned four-byte selection prefix. */
typedef struct {u8 bytes[4];} MenuChoice4;
/* Team/maker/car selection bytes as PackChoiceGlyph reads them. */
typedef struct {
    u8 team;
    u8 maker;
    u8 car;
    u8 reserved03;
} MenuChoiceIds4;
/* Current menu selection (0x800AD4F4). The PS1 map names +0
 * MenuSelectedTeamId and +1 MenuSelectedMakerId. */
extern MenuChoiceIds4 MenuSelectedChoice;
#endif
