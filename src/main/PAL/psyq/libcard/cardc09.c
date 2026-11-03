/* GCC_VERSION: 2.7.2 */
#include "psyq/libcard/cardc09.h"
extern volatile int D_800ABDE8;
extern int D_80114724, D_8011472C, D_80114728;
extern unsigned char *D_80114718;
int func_8009CC44() {
    switch (D_800ABDE8) {
    case 2: return 0x5B;
    case 3: return D_80114724;
    }
    if (D_800ABDE8 < 5) return 0;
    if (D_8011472C != 0) return 0;
    if (D_800ABDE8 >= D_80114728 + 5) return 0;
    return D_80114718[D_800ABDE8 - 5];
}
