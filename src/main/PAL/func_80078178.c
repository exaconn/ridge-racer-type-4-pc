#include "types.h"
#include "m2c_macros.h"

extern int func_80096EA4();
extern s32 D_8010EF58;
extern s32 D_8010EF5C;
extern s32 D_8010EF60;
extern s32 D_8010EF64;

s32 func_80078178(void) {
    if (func_80096EA4(D_8010EF58) == 1) {
        return 1;
    }
    if (func_80096EA4(D_8010EF5C) == 1) {
        return 2;
    }
    if (func_80096EA4(D_8010EF60) != 1) {
        return (func_80096EA4(D_8010EF64) == 1) * 4;
    }
    return 3;
}
