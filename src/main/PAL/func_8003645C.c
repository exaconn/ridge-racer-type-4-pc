extern int func_80036330();
typedef struct { int k; short f; } E;
extern E *D_800F2DE8[];
extern short D_800F4D44;
void func_8003645C(void) {
    int i, j;
    int t[2];
    for (i = 0; i < 7; i++) {
        for (j = 7; i < j; j--) {
            t[0] = D_800F2DE8[j - 1]->k;
            t[1] = D_800F2DE8[j]->k;
            if (t[0] < t[1] || (t[0] == t[1] && D_800F2DE8[j]->f == 0)) {
                E *x = D_800F2DE8[j - 1];
                E *y = D_800F2DE8[j];
                D_800F2DE8[j] = x;
                D_800F2DE8[j - 1] = y;
            }
        }
    }
    if (D_800F4D44 == 1) {
        func_80036330();
    }
}
