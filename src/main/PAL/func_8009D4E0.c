/* GCC_VERSION: 2.7.2 */
extern int D_800ABDE4, D_800ABDE8, D_800ABDEC, D_800ABDF4, D_800ABDF8, D_80114724, D_80114728, D_80114730, D_80114870;
extern char D_8011472C;
extern char *D_80114718;
extern int *D_80114734, *D_80114738;
extern int func_8009CA8C(), func_8009CB50();
int func_8009D4E0(a0, a1) int a0; int a1; {
    int r;
    char *p;
    if (!(D_800ABDE4 & 1)) {
        r = 0;
    } else {
        r = 1;
        D_800ABDF4 = 0;
        D_800ABDF8 = 0;
        D_80114730 = 0;
        D_800ABDE8 = 0;
        D_800ABDEC = a0;
    }
    if (r == 0) return 0;
    if (a1 != 0) {
        D_80114724 = 0xD800008;
    } else {
        D_80114724 = 0xD800004;
    }
    p = &D_8011472C;
    D_80114718 = p;
    *p = 2;
    D_80114728 = 1;
    D_80114870 = 9;
    D_80114734 = (int *)func_8009CA8C;
    D_80114738 = (int *)func_8009CB50;
    D_800ABDE4 = 2;
    return 1;
}
