/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
extern volatile int *D_800A47DC, *D_800A47E0, *D_800A47E4;
extern int D_800A4828;
extern void func_8007F59C(void), func_8007F574(void);
void func_8007EFC0(int a0, int a1, int a2) {
    D_800A47D8->transfer_address = a1;
    func_8007F59C();
    D_800A47D8->spu_control |= 0x30;
    func_8007F59C();
    func_8007F574();
    *D_800A47DC = a0;
    *D_800A47E0 = ((u32)a2 << 16) | 0x10;
    D_800A4828 = 1;
    *D_800A47E4 = 0x1000200;
}
