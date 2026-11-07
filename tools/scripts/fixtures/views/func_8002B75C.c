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

void CopyStringClearTail(char *d, char *s)
{
    int i = 0;
    char *p;
    if (*s) {
        do {
            d[i] = *s++;
            i++;
        } while (*s);
    }
    while (*(p = d + i++)) {
        *p = 0;
    }
}
