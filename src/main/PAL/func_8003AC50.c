extern int func_80013514();
extern int func_800135BC();
extern int func_8001E854();
extern unsigned char D_8009F8FC;
extern struct { short a; short b; } D_800ACBD0[];
extern int D_800ACC18;
extern char *D_800F3B24;
extern char D_800F3B28[];
extern short D_800F4D44;
extern int D_800F4D70;
extern unsigned char D_800F4D7C;
extern unsigned char D_800F4D98[][4];

int func_8003AC50(void) {
    int s0;
    int s1 = 0;
    int r;
    int a1;
    int t;
    char *s2;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 3;
        D_8009F8FC = 0;
    case 3:
        s0 = D_8009F8FC;
        if (s0 < 0) goto l5;
        if (func_80013514(D_800F4D98[s0][1], D_800F4D98[s0][0], D_800F4D98[s0][2] % 20, D_800ACBD0[s0].a, s0) == 1) {
            D_800F4D7C = 4;
        }
        break;
    case 4:
        s0 = D_8009F8FC;
        r = func_800135BC();
        if (r != 1) break;
        a1 = D_800ACBD0[s0].b;
        if (a1 >= 2) {
            s2 = D_800F3B24;
            a1 -= 2;
        } else {
            if (a1 == r) {
                s2 = D_800F3B28 + (s0 & 1) * 0x840;
                a1 = 0;
            } else {
                a1 = -1;
            }
        }
        func_8001E854(s2, a1, s0);
        D_8009F8FC++;
        s1 = -D_8009F8FC;
        if (D_8009F8FC != 4) {
            D_800F4D7C = 3;
        } else {
l5:
            D_800F4D7C = 5;
        }
        break;
    case 5:
        s1 = 1;
        if (D_800F4D44 == 4) D_800F4D70 = D_800ACC18;
        break;
    default:
        break;
    }
    return s1;
}
