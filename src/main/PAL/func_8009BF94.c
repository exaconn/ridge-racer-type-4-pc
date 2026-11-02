/* GCC_VERSION: 2.7.2 */

extern volatile int D_800ABDE4;
extern int D_800ABDE8;
extern unsigned char D_80114741;
extern int D_80114870;
int func_8009BF94(a0, a1, a2) int a0; int *a1; int *a2; {
    if (D_80114870 == 0 || D_800ABDE4 == 1) {
        *a2 = 0;
        *a1 = 0;
        return -1;
    }
    if (a0 == 0) {
        while (D_800ABDE4 < 5);
    }
    *a1 = D_80114870;
    switch (D_800ABDE4) {
    case 4:
    case 2:
        *a2 = 0;
        return 0;
    case 17:
        if (D_800ABDE8 == 1) {
            *a2 = D_800ABDE8;
        } else {
    case 33:
            *a2 = 2;
        }
        goto fin;
    default:
        if (D_80114741 == 8) {
            *a2 = 3;
        } else {
            *a2 = 0;
        }
    fin:
        D_80114870 = 0;
        return 1;
    }
}
