extern short D_800F3A26;
extern short D_800F4C6A;
extern short D_800F4D8A;

void func_8007AB8C(int arg0) {
    if (arg0 == -1) {
        D_800F4C6A = 1;
        D_800F3A26 = 3;
    } else {
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
    }
    D_800F4D8A = 0;
}
