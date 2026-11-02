/* RODATA 0x8001162c 0x20 */
extern int func_80053AF4();
extern int func_8003A198();
extern int func_800207D4();
extern int func_8003B934();
extern int func_8002A604();
extern int func_80053CB0();
extern int func_80114920();
extern int func_80115598();
extern int func_801161A8();
extern int func_8011512C();
typedef struct { char b[0x17]; signed char b17; char r[0x2C - 0x18]; } w8c5S;
extern w8c5S D_800F2E30[], D_8010B258[];
extern struct { char pad[0x500]; unsigned char b500; } *D_800F4D80;
extern struct { char pad[0x10]; short h10; char pad2[0x1A]; short h2C; } *D_800F3AFC;
extern short D_800FFC18, D_800AC5D0, D_800F4C6A, D_800F3A26, D_800FF688, D_8010948C;
extern unsigned short D_800F4D8A, D_800F3A1E, D_800F4D44;
extern int D_800ACC14, D_800AC5D4, D_800F4CA8;

void func_8007C308(void) {
    int i;
    char *q;
    if (D_800F4D80->b500 == 2) {
        for (i = 0; i < 4; i++) {
            q = (char *)(i * 0x24 + (int)D_800F4D80) + 0x4C8;
            if (q[0x14] == 2) {
                q[0x14] = 3;
            }
        }
        D_800F3AFC->h10 = 4;
        if (D_800FFC18 == 3 || D_800FFC18 == 6 || D_800FFC18 == 7) {
            D_800F3AFC->h2C = 4;
        }
        D_800AC5D0 = 0x40;
        if (D_800FFC18 == 0) {
            func_80053AF4(0, 0x3C);
        }
        func_8003A198();
        D_800F4CA8 = 2;
        return;
    }
    if (D_800AC5D0 > 0) {
        D_800AC5D0 = D_800AC5D0 - 1;
    }
    func_800207D4(D_800ACC14 + 0x70, (0x40 - D_800AC5D0) << 2, 0x45);
    if (func_8003B934() != 1) return;
    if (D_800AC5D0 > 0) return;
    if (D_800F4D80->b500 != 5) return;
    if (D_800F3AFC->h10 != 0xB) return;
    for (i = 0; i < 3; i++) {
        D_800F2E30[i] = D_8010B258[i];
    }
    func_8002A604();
    switch (D_800FFC18) {
    case 0:
    default:
    case 4:
        func_80053CB0();
        D_800F4C6A = 1;
        D_800F3A26 = 3;
        D_800F4D8A = 0;
        break;
    case 1:
        if (D_800F3A1E == 0x132) {
            func_80114920();
            D_800F4D8A = 0;
            D_800F4C6A = D_800F3A1E >> 8;
            D_800F3A26 = D_800F3A1E & 0xFF;
        }
        break;
    case 2:
        if ((unsigned)(D_800F4D44 - 2) < 2) {
            func_80115598(D_800AC5D4);
            D_800F4C6A = 1;
            D_800F3A26 = 0x38;
            D_800F4D8A = 0;
        } else if ((short)D_800F4D44 == 4) {
            if (D_800FF688 != -1) {
                func_801161A8(D_800AC5D4);
                D_800F4C6A = 1;
                D_800F3A26 = 0x3C;
            } else {
                func_80053CB0();
                D_800F4C6A = 1;
                D_800F3A26 = 0x2D;
            }
            D_800F4D8A = 0;
        }
        break;
    case 3:
        D_800F4C6A = 1;
        D_800F3A26 = 0xF;
        D_800F4D8A = 0;
        break;
    case 5:
        func_8011512C();
        D_800F4C6A = 1;
        D_800F3A26 = 0x40;
        D_800F4D8A = 0;
        break;
    case 6:
        if (D_8010948C != 0) {
            D_800F4C6A = 3;
            D_800F3A26 = 3;
        } else {
            D_800F4C6A = 3;
            D_800F3A26 = 2;
        }
        D_800F4D8A = 0;
        break;
    case 7:
        D_800F4C6A = 1;
        D_800F3A26 = 0x27;
        D_800F4D8A = 0;
        break;
    }
}
