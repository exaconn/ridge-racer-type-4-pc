#include "types.h"
#include "m2c_macros.h"

extern int func_80096EA4();
extern s32 D_8010EF58;
extern s32 D_8010EF5C;
extern s32 D_8010EF60;
extern s32 D_8010EF64;

void func_80078204(void) {
    func_80096EA4(D_8010EF58);
    func_80096EA4(D_8010EF5C);
    func_80096EA4(D_8010EF60);
    func_80096EA4(D_8010EF64);
}
