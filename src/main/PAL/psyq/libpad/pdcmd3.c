#include "rr/channels.h"

extern int func_80098AC4();

void func_80097C58(s32 selector, s32 first, s32 second);
void func_80097CA0();

void func_80097C58(s32 selector, s32 first, s32 second) {
    func_800994CC(D_800ABC7C(selector), first & 0xFF, second & 0xFF);
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ABC7C (*(int (*(*))())&D_800ABC7C)

void func_80097CA0(a0, a1, a2) int a0, a1, a2; { func_80098AC4(D_800ABC7C(), a1, a2); }

#undef D_800ABC7C
