#ifndef RR_MENU_STATE_TYPES_H
#define RR_MENU_STATE_TYPES_H

#include "rr/layout.h"
#include "rr/menu_choice_types.h"

/* Persistent menu settings and their four byte-sized selection records. */
typedef struct {
    s8 results00[8];
    s8 retries08[8];
    u8 reserved10[7];
    s8 phase17;
    u8 reserved18;
    s8 result19;
    s16 mode1A;
    MenuChoice4 copies[4];
} MenuSettings2C;
RR_ASSERT_SIZE(MenuSettings2C, 0x2C);
RR_ASSERT_OFFSET(MenuSettings2C, retries08, 8);
RR_ASSERT_OFFSET(MenuSettings2C, phase17, 0x17);
RR_ASSERT_OFFSET(MenuSettings2C, mode1A, 0x1A);
RR_ASSERT_OFFSET(MenuSettings2C, copies, 0x1C);
RR_ASSERT_OFFSET(MenuSettings2C,result19,0x19);

#include "rr/menu_glyph_types.h"

#endif
