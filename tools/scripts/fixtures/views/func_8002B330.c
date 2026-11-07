#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define MenuFrameState (*(short (*))&MenuFrameState)

#undef MenuFrameState

/* legacy typing of shared globals in this part of the unit */
#define ScreenFadeState (*(M2C_UNK (*))&ScreenFadeState)

#define MenuFrameState (*(M2C_UNK (*))&MenuFrameState)

#undef ScreenFadeState

#undef MenuFrameState

/* legacy typing of shared globals in this part of the unit */
#define ScreenFadeState (*(S_ACC68 (*))&ScreenFadeState)

#define MenuFrameState (*(S_F4CB0 (*))&MenuFrameState)

#undef ScreenFadeState

#undef MenuFrameState

/* legacy typing of shared globals in this part of the unit */
#define CurrentDrawBuf (*(s32 (*))&CurrentDrawBuf)

#undef CurrentDrawBuf

/* legacy typing of shared globals in this part of the unit */
#define CurrentDrawBuf (*(s32 (*))&CurrentDrawBuf)

#define AddTilePrim ((s32 (*)())AddTilePrim)

#undef CurrentDrawBuf

#undef AddTilePrim

/* legacy typing of shared globals in this part of the unit */
#define PlayerControllers (*(E (*)[])&PlayerControllers)

#undef PlayerControllers

/* legacy typing of shared globals in this part of the unit */
#define PlayerCars (*(S_800ABEF8 (*)[])&PlayerCars)

#undef PlayerCars

typedef struct {
    char pad[0x10];
    int w10;
    char pad14[0x48];
} S_F3A38;

extern S_F3A38 PlayerControllers[];

extern s32 MenuStepDirection;

extern int PlaySfx();

void StepMenuValue(a0, a1, a2, a3, a4) int a0; int a1; char *a2; int a3; int a4; {
    int s0;
    int a;
    unsigned short b;
    switch (a1) {
    case 2:
        s0 = *(short *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 4:
        s0 = *(int *)a2;
        a = (a0 >> 2) & 1;
        break;
    case 1:
    case -1:
        s0 = *(unsigned char *)a2;
        a = (a0 >> 2) & 1;
        break;
    default:
        a = (a0 >> 2) & 1;
        break;
    }
    if ((a0 & 1) == 0) {
        b = PlayerControllers[a].b;
        if ((b & 0x8000) == 0) {
            if ((b & 0x2000) == 0) {
                if (b & 0x1000) {
                    s0 -= 1;
                    goto L;
                }
                if (b & 0x4000) {
                    s0 += 1;
                    goto L;
                }
            }
        }
    } else {
        b = PlayerControllers[a].b;
        if ((b & 0x1000) == 0) {
          if ((b & 0x4000) == 0) {
            if (b & 0x8000) {
                MenuStepDirection = 1;
                s0 -= 1;
                goto L;
            }
            if (b & 0x2000) {
                MenuStepDirection = 2;
                s0 += 1;
                goto L;
            }
          }
        }
        MenuStepDirection = 0;
        goto M;
    }
    goto M;
L:
    if ((a0 & 2) == 0) {
        if (s0 < a3) {
            if (s0 >= a4) {
                PlaySfx(0, a);
            } else {
                s0 = a4;
            }
        } else {
            s0 = a3 - 1;
        }
    } else {
        PlaySfx(0, a);
        if (s0 >= a3) {
            s0 = a4;
        } else if (s0 < a4) {
            s0 = a3 - 1;
        }
    }
M:
    switch (a1) {
    case 2: *(short *)a2 = s0; break;
    case 4: *(int *)a2 = s0; break;
    case 1:
    case -1: *(char *)a2 = s0; break;
    }
}
