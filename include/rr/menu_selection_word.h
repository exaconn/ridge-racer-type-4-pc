#ifndef RR_MENU_SELECTION_WORD_H
#define RR_MENU_SELECTION_WORD_H
#include "rr/layout.h"
typedef union {s32 word;s16 halves[2];} RrMenuSelectionWord32;
RR_ASSERT_SIZE(RrMenuSelectionWord32,4);
RR_ASSERT_OFFSET(RrMenuSelectionWord32,halves,0);
#endif
