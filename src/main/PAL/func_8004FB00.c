#include "types.h"
extern int func_8004FFA4();
extern int func_80050E94();
extern int func_800512C4();
extern int func_80051538();
extern int func_800517F8();
extern int func_80052AB8();
extern int func_8004F9C8();
extern int func_80051B38();
extern int D_800F4DBC;
struct E { char pad[0x38]; u32 flags; char r[1568 - 0x3C]; };
extern struct E D_800F4DB8[];
void func_8004FB00(void) {
    int i;
    struct E *p;
    for (i = 0, p = D_800F4DB8; i < 4; i++, p++) {
        if (p->flags & 1) func_8004FFA4(i);
    }
    for (i = 0; i < 2; i++) func_80050E94(0, 0, 0, i);
    for (i = 0; i < 2; i++) func_800512C4(i, 0, 0);
    func_80051538(0, 0, 0);
    func_800517F8(5, 0x1E00, 0, 0x40);
    func_80052AB8();
    func_8004F9C8();
    func_80051B38();
    D_800F4DBC = 1;
}
