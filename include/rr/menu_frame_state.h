#ifndef RR_MENU_FRAME_STATE_H
#define RR_MENU_FRAME_STATE_H

#include "types.h"
#include "rr/layout.h"

/* Screen fade controller (0x800ACC68, twelve bytes, cleared as a block). */
typedef struct {
    u8 phase;      /* +0: 0 start, 1 fading, 2 complete */
    u8 tick;       /* +1: frames into the 30-frame fade */
    u8 target;     /* +2: ScreenFadeColors index faded towards */
    u8 prevTarget; /* +3: target of the previous call */
    u8 reserved04[8];
} ScreenFadeState0C;
extern ScreenFadeState0C ScreenFadeState;

/* Menu frame and title-bar animation (0x800F4CB0, 0x20 bytes, cleared as a
 * block). +8 is named MenuAvailableCourseMask in the PS1 map; it holds the
 * 1-based player number drawn after the title (0 hides it), and the menu
 * text code takes the active controller from it. */
typedef struct {
    short s0; /* frame phase */
    short s2; /* phase tick */
    unsigned char b4;
    unsigned char b5;
    unsigned char b6;
    unsigned char b7;
    s32 titlePlayer08;
    unsigned char bC;
    unsigned char bD;
    short sE;
    short s10;
    short s12;
    short s14;
    short s16;
    short s18;
    short s1A;
    short s1C;
    short s1E;
} MenuFrameState20;
extern MenuFrameState20 MenuFrameState;

RR_ASSERT_SIZE(ScreenFadeState0C, 0xC);
RR_ASSERT_SIZE(MenuFrameState20, 0x20);
RR_ASSERT_OFFSET(MenuFrameState20, titlePlayer08, 8);

#endif
