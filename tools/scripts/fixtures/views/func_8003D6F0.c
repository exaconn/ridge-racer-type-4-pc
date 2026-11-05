#define EP ((E_2 *)((a0 * 16) + D_800ACC00))

/* legacy typing of shared globals in this part of the unit */
#define D_800ACC00 ((E_1 *)D_800ACC00)

#undef D_800ACC00

/* legacy typing of shared globals in this part of the unit */
#define D_800F4D8C ((char *)D_800F4D8C)

#undef D_800F4D8C

typedef struct {
    short p0, f2, f4, f6, f8, fA, fC, fE, f10, f12, f14, f16, f18, f1A;
} E;

extern int func_8003D474(), rsin(), rcos();

extern E *D_800F3AFC;

extern int D_800F2ECC;

void func_8003D6F0(a0) int a0; {
    int s2;
    if (a0 < D_800F2ECC) {
        s2 = D_800F3AFC[a0].f12;
        switch (D_800F3AFC[a0].f10) {
        case 0:
            D_800F3AFC[a0].f1A = 2;
            D_800F3AFC[a0].f10 = 0xA;
            D_800F3AFC[a0].f18 = 0;
            D_800F3AFC[a0].f12 = 0;
            D_800F3AFC[a0].f4 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f6 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - 2;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - 2;
            break;
        case 1:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 2;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 2:
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f16 = (D_800F3AFC[a0].fA + D_800F3AFC[a0].fE) - D_800F3AFC[a0].f1A;
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].f16;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 3:
            D_800F3AFC[a0].f4 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].fA;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA;
            D_800F3AFC[a0].f18 = D_800F3AFC[a0].fC + 2;
            D_800F3AFC[a0].f1A = D_800F3AFC[a0].fE;
            break;
        case 4:
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rcos((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA + (D_800F3AFC[a0].fE - D_800F3AFC[a0].f1A);
            D_800F3AFC[a0].f6 = D_800F3AFC[a0].f16;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 5;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 5:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rcos((s2 << 10) / 15)) / 4096;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 0xB;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 10:
            if (D_800F3AFC[a0].f2 != 0) {
                D_800F3AFC[a0].f10 = 1;
            }
            break;
        case 7:
            D_800F3AFC[a0].f18 = ((D_800F3AFC[a0].fC + 2) * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f1A = D_800F3AFC[a0].fE;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA;
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        case 6:
        case 9:
        case 11:
            break;
        case 8:
            D_800F3AFC[a0].f18 = D_800F3AFC[a0].fC + 2;
            D_800F3AFC[a0].f1A = (D_800F3AFC[a0].fE * rsin((s2 << 10) / 15)) / 4096;
            D_800F3AFC[a0].f14 = D_800F3AFC[a0].f8;
            D_800F3AFC[a0].f16 = D_800F3AFC[a0].fA + ((D_800F3AFC[a0].fE / 2) - (D_800F3AFC[a0].f1A / 2));
            if (s2 < 0xF) {
                D_800F3AFC[a0].f12++;
            } else {
                D_800F3AFC[a0].f10 = 3;
                D_800F3AFC[a0].f12 = 0;
            }
            break;
        }
        func_8003D474(a0);
    }
}
