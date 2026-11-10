#ifndef RR_GAME_MENU_MENUMODE_H
#define RR_GAME_MENU_MENUMODE_H

#include "types.h"
#include "rr/layout.h"

typedef struct {
    u16 textId00;
    s8 kind02;
    u8 handlerIdx03;
    u8 tableIdx04;
    u8 flags05;
    u8 promptIdx06;
    s8 defaultChoice07;
} MenuStateEntry8;
RR_ASSERT_SIZE(MenuStateEntry8, 0x8);

extern MenuStateEntry8 MenuStateTable[];
extern unsigned char D_800A44A8[];
extern int CardTargetFileNames[];
extern unsigned short D_80109458;
extern short D_80109490;
extern short D_801094A0;
void StepCardFileSlotSelection(void);
void StepCardPortSelection(void);
void UpdateCardMenuRowStates(void);


#endif /* RR_GAME_MENU_MENUMODE_H */
