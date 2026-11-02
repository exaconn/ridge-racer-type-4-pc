#include "types.h"
#include "m2c_macros.h"

extern int func_80095DA8();
extern int func_80096E84();
extern int func_80096F04();
extern int func_80096F14();
extern int func_8009BA94();
extern s32 D_8010EF58;
extern s32 D_8010EF5C;
extern s32 D_8010EF60;
extern s32 D_8010EF64;
extern s32 D_8010EF68;
extern s32 D_8010EF6C;
extern s32 D_8010EF70;
extern s32 D_8010EF74;

void func_80078000(void) {
    func_80096F04();
    func_80096E84(D_8010EF58);
    func_80096E84(D_8010EF5C);
    func_80096E84(D_8010EF60);
    func_80096E84(D_8010EF64);
    func_80096E84(D_8010EF68);
    func_80096E84(D_8010EF6C);
    func_80096E84(D_8010EF70);
    func_80096E84(D_8010EF74);
    func_80096F14();
    func_8009BA94();
    func_80095DA8();
}
