/* RODATA 0x800107e4 0x20 */
extern int func_80053BB4();
extern int func_80032914();
extern int func_8001223C();
extern int func_800129BC();
extern int func_80012A58();
extern int func_80029358();
extern int func_8001292C();
extern int func_80043608();
extern int func_800436E8();
extern unsigned char D_800F4D7C;
extern unsigned char D_8009F8FC;
extern int D_800102F0;
extern int D_800AC5A0;
extern struct { char pad[4]; signed char f4; char pad2[0x1B]; int f20; } *D_800FF68C;
extern int D_800F4CA8;
extern struct { char pad[0x17]; signed char f17; char pad2[2]; short f1A; unsigned char f1C; } D_800AD4D8;
extern int D_800AD564;
extern unsigned char D_800AD4F4;
extern signed char *D_800ACBFC;

int func_8003B588(void) {
    int s3 = 0;
    int s0, s1, s2, a0, v0, k;

    switch (D_800F4D7C) {
    case 0:
        k = 0xAF000;
        *(unsigned char *)&D_8009F8FC = 0;
        D_800AC5A0 = D_800102F0 + k;
        v0 = D_800AC5A0;
        D_800AC5A0 = v0;
        D_800FF68C = (void *)v0;
        v0 += 0x348;
        D_800AC5A0 = v0;
        if (D_800F4CA8 == 1 && D_800AD4D8.f17 == 7 && D_800AD4D8.f1A == 4) {
            func_80053BB4(1);
        }
        func_80032914(D_800FF68C);
        D_800F4D7C = 1;
        break;
    case 1:
        if (D_800F4CA8 == 1) {
            s2 = 0x41;
            s1 = 0x14;
            s2 = D_800AD4D8.f1C * 9 + (D_800AD4D8.f17 + s1);
        } else {
            s2 = 0x41;
            s1 = 4;
        }
        if (func_800129BC(func_8001223C(1, s2), s1 * D_800AD564, s1, D_800AC5A0)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        s2 = func_80012A58();
        if (s2) {
            func_80029358(D_800AC5A0);
            D_800F4D7C = 3;
            D_800AC5A0 += s2;
            D_800FF68C->f20 = D_800AC5A0;
        }
        break;
    case 3:
        if (func_8001292C(func_8001223C(1, 0x42), D_800AC5A0)) {
            D_800F4D7C = 4;
        }
        break;
    case 4:
        if (func_80012A58()) {
            func_80043608(D_800AC5A0);
            D_800F4D7C = 5;
        }
        break;
    case 5:
        s2 = D_800AD4F4 * 9 + 0x1C;
        a0 = D_8009F8FC;
        s0 = D_800ACBFC[D_800FF68C->f4 * 8 + a0];
        if (s0 != -1) {
            if (D_800F4CA8 == 0) {
                s1 = 8;
                s0 <<= 3;
            } else if (D_800F4CA8 == 1) {
                s1 = 8;
                if (a0 == 0) {
                    s1 = 0x2E;
                    s0 = s0 * s1;
                } else {
                    s0 <<= 3;
                    s0 += 0xE6;
                }
            } else {
                s1 = 8;
                s0 <<= 3;
            }
            if (func_800129BC(func_8001223C(1, s2), s0, s1, D_800AC5A0)) {
                D_800F4D7C = 6;
            }
        } else {
            D_800F4D7C = 7;
        }
        break;
    case 6:
        s2 = func_80012A58();
        if (s2) {
            if (D_8009F8FC == 0) {
                func_800436E8(D_800AC5A0);
            } else {
                D_800AC5A0 = D_800AC5A0 + s2;
            }
            D_8009F8FC = D_8009F8FC + 1;
            s0 = D_800ACBFC[D_800FF68C->f4 * 8 + D_8009F8FC];
            if (D_8009F8FC == 8 || s0 == -1) {
                D_800F4D7C = 7;
            } else {
                D_800F4D7C = 5;
            }
        }
        break;
    case 7:
        s3 = 1;
        break;
    }
    return s3;
}
