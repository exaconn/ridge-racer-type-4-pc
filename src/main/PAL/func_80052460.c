#include "types.h"
extern int func_80052024();
extern int func_8005007C();
struct A { char p[0x5A]; u16 x5A, x5C; char q[4]; s32 x64, x68; char r[1568 - 0x6C]; };
struct R { char p[0x248]; u16 x248, x24A; char q[800 - 0x24C]; };
extern struct A D_800F4DB8[];
extern struct R D_80107198[];
void func_80052460(void) {
    int i;
    for (i = 0; i < 2; i++) {
        char *c = (char *)D_800F4DB8 + i * 0x1C;
        struct R *r = &D_80107198[*(s16 *)(c + 0x18F0)];
        func_80052024(i);
        if (*(s16 *)(c + 0x18F0) != -1) {
            D_800F4DB8[i + 2].x68 = *(s32 *)(c + 0x18FC);
            D_800F4DB8[i + 2].x5C = *(u16 *)(c + 0x1900);
            D_800F4DB8[i + 2].x5A = *(u16 *)(c + 0x1902);
            D_800F4DB8[i + 2].x64 = *(s32 *)(c + 0x1904);
            func_8005007C(r->x24A, r->x248, i + 2);
        }
    }
}
