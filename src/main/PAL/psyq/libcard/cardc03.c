/* GCC_VERSION: 2.7.2 */
#include "psyq/libcard/cardc03.h"
extern volatile int D_800ABDE8;
extern unsigned char D_80114724;
extern unsigned char *D_80114718;
extern int D_800ABDF8;
extern int D_80114730;

int func_8009C55C();
int func_8009C610();

int func_8009C55C() {
    switch (D_800ABDE8) {
    case 2: return 0x59;
    case 4: return D_80114725;
    case 5: return D_80114724;
    }
    if (D_800ABDE8 < 6) return 0;
    if (D_800ABDE8 >= 10) return 0;
    return D_80114718[D_800ABDE8 - 6];
}

int func_8009C610(a0) int a0; { if (D_800ABDF8 == 3) D_80114730 = a0 & 0xFF; return 0; }
