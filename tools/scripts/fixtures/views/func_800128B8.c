/* Candidate module: CD read state machine, 80012578..80012A68.
 * Adjacent matching functions share one directory and location definition.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/cd.h"

#define B (*(int *)&D_800ACC18)

/* legacy typing of shared globals in this part of the unit */
#define func_80070108 ((void (*)(void *, s32))func_80070108)

#define D_800ACC18 ((Resource *)D_800ACC18)

#define D_800F3A08 (*(void *(*)[])&D_800F3A08)

#undef func_80070108

#undef D_800ACC18

#undef D_800F3A08

/* legacy typing of shared globals in this part of the unit */
#define DrawSync ((int (*)())DrawSync)

#define func_80092FFC ((int (*)())func_80092FFC)

#undef DrawSync

#undef func_80092FFC

/* legacy typing of shared globals in this part of the unit */
#define D_8010BF60 ((short *)D_8010BF60)

#undef D_8010BF60

extern s32 D_8009DF90;

extern s32 (*D_8009DFA8[])();

extern s32 D_80109488;

s32 func_800128B8(void) {
    if (D_8009DF90 > 0) {
        D_80109488 = func_8008C0D8(1, 0);
        if (D_80109488 != 0) {
            return D_8009DFA8[D_8009DF90]();
        }
    }
    return 0;
}
