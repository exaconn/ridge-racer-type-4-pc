/* RODATA 0x800113f0 0x1c */
extern int func_80095C84();
extern int func_800780B8();
extern int func_8009D700();
extern int func_8009BF94();
extern int func_800782F4();
extern int func_80095CA4();
extern int func_8007825C();
extern int func_80078204();
extern int func_80095C94();
extern int D_8010ECB8[], D_8010ECC0[], D_8010ECC8[], D_8010EF80[], D_800A4094[];
extern char D_8010EF28[];

int func_800787C4(int p, char *a1) {
    char *s2;
    int a0;
    int sp[2];
    int x;
    s2 = (char *)((p << 4) + (int)a1);
    switch (D_8010ECB8[p]) {
    case 0:
        func_80095C84(s2);
        D_8010ECB8[p] = 1;
        D_8010ECC0[p] = 0;
        D_8010ECC8[p] = 0;
        break;
    case 1:
        a0 = func_800780B8(p);
        if (a0 == 0) break;
        switch (a0) {
        case 1:
            D_8010EF80[p] = a0;
            D_8010ECB8[p] = 2;
            D_8010ECC0[p] = 0;
            break;
        case 3:
            D_8010EF80[p] = -1;
            D_8010ECB8[p] = 6;
            D_800A4094[p] = 0;
            break;
        case 4:
            D_8010EF80[p] = 2;
            D_8010ECB8[p] = 2;
            D_8010ECC0[p] = 0;
            break;
        case 2:
        default:
            D_8010EF80[p] = -4;
            D_8010ECB8[p] = 6;
            D_800A4094[p] = 0;
            break;
        }
        break;
    case 2:
        if (func_8009D700(s2, p * 17 + (int)D_8010EF28)) {
            D_8010ECB8[p] = 3;
            D_8010ECC0[p] = 0;
        } else {
            if (D_8010ECC0[p]++ >= 0x78) {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        }
        break;
    case 3:
        a0 = func_8009BF94(1, &sp[0], &sp[1]);
        if (a0 != 0) {
            if (a0 > 0) {
                a0 = sp[1];
                switch (a0) {
                case 0:
                    D_8010EF80[p] = 3;
                    if (D_800A4094[p] == 1) {
                        D_8010ECB8[p] = 6;
                    } else {
                        D_8010ECB8[p] = 4;
                    }
                    break;
                case 2:
                    if (D_8010EF80[p] == 1) {
                        if (D_800A4094[p] == 1) {
                            D_8010ECB8[p] = 6;
                        } else {
                            D_8010ECB8[p] = 4;
                        }
                    } else {
                        D_8010EF80[p] = 2;
                        func_800782F4();
                        func_80095CA4(s2);
                        func_8007825C();
                        D_8010ECB8[p] = 4;
                        D_800A4094[p] = 0;
                    }
                    break;
                case 1:
                    D_8010EF80[p] = -1;
                    D_8010ECB8[p] = 6;
                    D_800A4094[p] = 0;
                    break;
                case 3:
                    D_8010EF80[p] = 4;
                    func_800782F4();
                    func_80095CA4(s2);
                    func_8007825C();
                    D_8010ECB8[p] = 4;
                    D_800A4094[p] = 0;
                    break;
                }
            } else {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        } else {
            if (D_8010ECC0[p]++ >= 0x78) {
                D_8010EF80[p] = -4;
                D_8010ECB8[p] = 6;
                D_800A4094[p] = 0;
            }
        }
        break;
    case 4:
        func_80078204();
        func_80095C94(s2);
        D_8010ECB8[p] = 5;
        D_8010ECC0[p] = 0;
        break;
    case 5:
        a0 = func_800780B8(p);
        if (a0 == 0) break;
        D_8010ECB8[p] = 6;
        switch (a0) {
        case 1:
            D_800A4094[p] = a0;
            break;
        case 3:
            D_8010EF80[p] = -1;
            D_800A4094[p] = 0;
            break;
        case 4:
            x = D_8010EF80[p];
            if ((unsigned)(x - 1) < 2) {
                D_8010EF80[p] = -2;
            } else if ((unsigned)(x - 3) < 2) {
                D_8010EF80[p] = -3;
            }
            D_800A4094[p] = 0;
            break;
        case 2:
        default:
            D_8010EF80[p] = -4;
            D_800A4094[p] = 0;
            break;
        }
        break;
    case 6:
        D_8010ECC8[p] = D_8010EF80[p];
        D_8010ECB8[p] = 0;
        break;
    }
    return D_8010ECC8[p];
}
