#include "types.h"
extern int func_8005534C();
extern int func_80055660();
extern u16 D_800F3A40;
extern struct { char pad[0x3DC]; s32 a; } D_800AC650;
void func_800556B4(void) {
    if (D_800F3A40 & 0x10) {
        D_800AC650.a ^= 1;
        func_8005534C(D_800AC650.a);
    }
    func_80055660();
}
