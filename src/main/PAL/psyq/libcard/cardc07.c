/* RODATA 0x80012178 0x24 */
/* GCC_VERSION: 2.7.2 */
#include "psyq/libcard/cardc03.h"
#include "psyq/libcard/cardc04.h"
#include "psyq/libcard/cardc07.h"
extern volatile int D_800ABDE8;
extern int D_80114718;
extern int D_80114728;
extern unsigned char D_80114724;
extern int D_800ABDF8;
extern int D_80114730;

int func_8009CA8C() {
    switch (D_800ABDE8) {
    case 2: return 0x5C;
    case 3: return 1;
    case 5: return D_80114724;
    case 6: return D_80114725;
    case 7: return D_80114726;
    case 8: return D_80114727;
    case 9: return D_80114728;
    case 4:
    case 10: break;
    default: return *(unsigned char *)(D_800ABDE8 + D_80114718 - 0xB);
    }
    return 0;
}

int func_8009CB50() { if (D_800ABDF8 == 9) D_80114730 = D_80114728 + 1; return 0; }
