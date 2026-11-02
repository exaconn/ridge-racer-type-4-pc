/* CC1_FLAGS: -mno-split-addresses */
extern int func_80097EA0(), func_80097E38();
extern int D_8011441C[];
void func_80097E0C() {
    int *p = D_8011441C;
    p[0] = (int)func_80097EA0;
    p[1] = (int)func_80097E38;
    p[-1] = 0;
    p[2] = 0;
}
