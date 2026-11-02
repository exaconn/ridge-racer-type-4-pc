/* RODATA 0x80010550 0x40 */
extern int func_80039F98();
extern signed char D_800AD4D8[];
extern unsigned char D_800AD4EC;
extern signed char D_800AD4EF;
extern short D_800AD4F2;
extern unsigned char D_800AD4F5;
extern short D_800F4C5C;
extern signed char *D_800FF68C;
extern short D_800FFC18;

void func_8003159C(int a0, int a1) {
    int sw;
    int idx, a3, v, t, r;
    signed char *P;
    int u;
    int i, sum;

    sw = D_800AD4F2;
    D_800FF68C[7] = 1;
    *(short *)(D_800FF68C + 0x16) = 0;
    D_800FF68C[0x1E] = -2;
    switch (sw) {
    case 0:
        D_800FF68C[7] = 0;
    case 1:
        D_800FF68C[4] = 0;
        D_800F4C5C = 0x109;
        D_800AD4F2 = 1;
        break;
    case 2:
        D_800FF68C[4] = D_800AD4F5 + 0x18;
        D_800F4C5C = 0x10F;
        break;
    case 4:
        idx = D_800AD4D8[0x17];
        if (idx < 4) {
            a3 = D_800AD4D8[idx];
        } else {
            a3 = 3 - D_800AD4D8[idx + 8];
            if (idx == 7) a3 = 0;
        }
        switch (idx) {
        case 0:
            a1 = D_800AD4D8[idx + 8] < 3;
            break;
        case 1:
        case 3:
            a1 = 4;
            if (D_800AD4D8[idx + 8] >= 3) a1 = D_800AD4D8[idx - 1];
            break;
        case 2:
            a1 = 4;
            if (D_800AD4D8[idx + 8] >= 3) a1 = (unsigned char)D_800AD4D8[0x14];
            break;
        case 4:
            a1 = D_800AD4EC;
            break;
        case 5:
            a1 = 3 - D_800AD4D8[idx + 7];
            break;
        case 6:
        case 7:
            a1 = 0;
            i = 0;
            sw = (int)D_800AD4D8;
            do {
                a1 += *(signed char *)(i + sw + 0xC);
                i++;
            } while (i < 4);
            if (a1 == 0xC) {
                a1 = 0;
            } else if (idx == 7) {
                a1 = 1;
            } else {
                a1 = 4 - D_800AD4D8[idx + 7];
            }
            break;
        }
        P = D_800FF68C;
        u = a3 * 5;
        t = a1 + 4;
        P[4] = u + t;
        v = D_800AD4EF;
        if (v == 3) goto L814;
        if (v < 4) {
            if (v == 1) goto L814;
            D_800F4C5C = 0x10F;
        } else {
            if (v == 6) {
L814:
                D_800AD4F2 = 6;
                func_80039F98(P, a1, sw, a3);
                D_800F4C5C = 0x12A;
            } else D_800F4C5C = 0x10F;
        }
        if (D_800AD4EF == 7) D_800F4C5C = 0x205;
        break;
    case 6:
        D_800FF68C[4] = 0x25 - D_800AD4EC;
        D_800F4C5C = 0x111;
        D_800FFC18 = 3;
        break;
    }
}
