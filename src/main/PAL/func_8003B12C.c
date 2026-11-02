/* RODATA 0x800107c4 0x20 */
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800137B0();
extern int func_800137D4();
extern int func_800436E8();
extern int func_8004E008();
extern int func_8004E70C();
extern int func_8004F600();
extern int func_8005AF5C();
extern int func_8005B2D4();
extern char D_8009F8FC;
extern int D_800ACC18, D_800ACC1C;
extern short D_800F4D44;
extern unsigned char D_800F4D7C;
int func_8003B12C(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        func_8005B2D4(0);
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, 0xF), D_800ACC18)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58()) {
            func_800436E8(D_800ACC18);
            D_800F4D7C = 3;
            D_800ACC18 = D_800ACC1C;
            func_8004E008();
        }
        break;
    case 3:
        if (func_8004E70C()) {
            func_8005AF5C();
            func_8004F600();
            if (D_800F4D44 == 1) {
                goto s7;
            }
            D_800F4D7C = 5;
        }
        break;
    case 5:
        if (func_800137B0()) {
            D_800F4D7C = 6;
        }
        break;
    case 6:
        if (func_800137D4()) {
s7:
            D_800F4D7C = 7;
        }
        break;
    case 7:
        r = 1;
        break;
    }
    return r;
}
