/* RODATA 0x80010700 0x1c */
extern int func_80031DEC();
typedef struct { char x[0x320]; } S;
extern S *D_800FFC20[];
extern S D_800ABEF8[];
extern S D_80106B58[];
extern S D_80106E78[];
extern S D_80107198[];
extern short D_800AC59C;
extern short D_800AD508;

void func_800373DC(int a) {
    int i;
    int n;
    int v;
    S **p;
    S *q;
    switch (a) {
    case 1:
        D_800AC59C = 8;
        i = 0;
        for (; i < D_800AC59C; i++) {
            if (i == 0) {
                D_800FFC20[0] = D_800ABEF8;
            } else {
                D_800FFC20[i] = D_80106E78 + i;
            }
        }
        break;
    case 2:
        D_800AC59C = 2;
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_800ABEF8 + 1;
        break;
    case 3:
        i = 2;
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_800ABEF8 + 1;
        v = D_800AC59C;
        if (i < v) {
            n = v;
            do {
                D_800FFC20[i] = D_80106B58 + i;
                i++;
            } while (i < n);
        }
        break;
    case 4:
        D_800AC59C = 4;
        if (D_800AD508 == 1) {
            D_800FFC20[0] = D_800ABEF8;
            D_800FFC20[1] = D_800ABEF8 + 1;
            D_800FFC20[2] = D_80107198;
            D_800FFC20[3] = D_80107198 + 1;
        } else {
            D_800FFC20[2] = D_800ABEF8;
            D_800FFC20[3] = D_800ABEF8 + 1;
            D_800FFC20[0] = D_80107198;
            D_800FFC20[1] = D_80107198 + 1;
        }
        break;
    case 5:
        D_800AC59C = 7;
        i = 0;
        n = 7;
        for (; i < n; i++) {
            D_800FFC20[i] = D_80107198 + i;
        }
        break;
    default:
    case 0:
        D_800FFC20[0] = D_800ABEF8;
        D_800AC59C = 1;
        break;
    case 6:
        D_800FFC20[0] = D_800ABEF8;
        D_800FFC20[1] = D_80107198;
        D_800AC59C = 2;
        break;
    }
    func_80031DEC();
}
