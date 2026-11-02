extern int func_8002E0D8();
extern int func_80036FD4();
extern int func_8006EDA4();
extern int func_8006EF8C();
extern int func_80070464();
extern int func_8007064C();
extern int func_800706B8();
extern int func_800747EC();
extern short D_800AD50C;
extern int D_800F4CE0;
extern int D_8010E730;

void func_80070734(int arg0) {
    if (arg0 >= 0x169) {
        D_8010E730 = 1;
    }
    if (D_8010E730 != 0) {
        if (D_800AD50C != 0) {
            if (D_800F4CE0 < 0x12) {
                D_800F4CE0 += 1;
            }
        } else if (D_800F4CE0 >= -0x23) {
            D_800F4CE0 -= 1;
        }
        if (func_80070464() != 0) {
            *(int *)0x1F800000 = func_800706B8(*(int *)0x1F800000);
            func_8006EF8C(1);
            func_80036FD4(0);
            func_8006EDA4(0);
            func_800747EC(2);
            func_8002E0D8(1);
            func_8007064C();
        }
    }
}
