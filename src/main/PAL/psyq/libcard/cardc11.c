/* GCC_VERSION: 2.7.2 */
#include "psyq/libcard/cardc11.h"
extern volatile int D_800ABDE8;
extern int D_80114724;
extern int D_80114728;
extern int D_8011472C;
extern unsigned char *D_80114718;

int func_8009CEBC() {
    switch (D_800ABDE8) {
    case 2: return 0x5C;
    case 3: return D_80114724;
    }
    if (D_800ABDE8 < 5) return 0;
    if (D_8011472C == 0) {
        if (D_800ABDE8 >= D_80114728 + 5) return 0;
        return D_80114718[D_800ABDE8 - 5];
    }
    return D_80114718[D_800ABDE8 - D_80114728 - 6];
}
