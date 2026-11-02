extern int func_8002B574();
extern signed char D_800AD4D8[];
extern signed char D_800AD4EF;
typedef struct { int k; short f; } E;
extern E *D_800F2DE8[];
extern int D_800F39C8, D_800F4C64;
void func_80036330(void) {
    int a2;
    int a0;
    int a1;
    a2 = D_800F39C8 + 0x1E8;
    if (D_800AD4EF >= 0) {
        a1 = 0;
        if (D_800AD4EF >= 2) {
            a1 = 2;
            if (D_800AD4EF >= 4) {
                goto b3;
            }
        }
    } else {
b3:
        a1 = 4;
    }
    a0 = 0;
    do {
        if (D_800F2DE8[a0]->f == 0) {
            D_800AD4D8[D_800AD4D8[0x17]] = a0 + 1;
            if (a0 == 0) {
                if (a1 < 4) {
                    if (func_8002B574(*(int *)(a2 + ((D_800AD4D8[0x17] - a1) << 2)), 0, a2) > D_800F4C64) {
                        D_800AD4D8[D_800AD4D8[0x17]] = 0;
                    }
                }
            }
            break;
        }
        a0 += 1;
    } while (a0 < 8);
    if (D_800AD4D8[0x19] < 9) {
        D_800AD4D8[0x19] = D_800AD4D8[D_800AD4D8[0x17]];
    }
}
