/* CC1_FLAGS: -mno-split-addresses */
#include "rr/library.h"
extern int VSync();
extern void checkRECT();
extern void func_80095990();
extern int get_alarm();
extern char D_80011FCC[];
extern char *D_800ABAB8;
extern volatile int *D_800ABBC8;
extern volatile int *D_800ABBD4;
extern int D_800ABBFC;
extern int D_800ABC00;
int LoadImage2(int a0, int a1) {
    checkRECT(D_80011FCC, a0);
    D_800ABBFC = VSync(-1) + 0xF0;
    D_800ABC00 = 0;
    while ((*D_800ABBD4 & 0x01000000) || !(*D_800ABBC8 & 0x04000000)) {
        if (get_alarm() != 0) return -1;
    }
    DMACallback(2, func_80095990);
    (*(void (**)())(D_800ABAB8 + 0x20))(a0, a1);
    return 0;
}
