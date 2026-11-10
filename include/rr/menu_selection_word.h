#ifndef RR_MENU_SELECTION_WORD_H
#define RR_MENU_SELECTION_WORD_H
#include "rr/layout.h"
typedef union {s32 word;s16 halves[2];} MenuSelectionWord32;
RR_ASSERT_SIZE(MenuSelectionWord32,4);
RR_ASSERT_OFFSET(MenuSelectionWord32,halves,0);
#endif
