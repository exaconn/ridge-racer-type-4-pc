#include "types.h"
#include "m2c_macros.h"

extern int func_80096EA4();
extern s32 D_8010EF68;
extern s32 D_8010EF6C;
extern s32 D_8010EF70;
extern s32 D_8010EF74;

void func_800782F4(void) {
    func_80096EA4(D_8010EF68);
    func_80096EA4(D_8010EF6C);
    func_80096EA4(D_8010EF70);
    func_80096EA4(D_8010EF74);
}
