extern int func_80013514();
extern int func_800135BC();
extern int func_8001E854();
extern unsigned char D_8009F8FC;
extern struct { short a; short b; } D_800ACBD0[];
extern char *D_800F3B24;
extern char D_800F3B28[];
extern short D_800F4D44;
extern unsigned char D_800F4D7C;
extern unsigned char D_800F4D98[][4];

int func_8003AE4C(int arg0, int arg1) {
    int r;
    int q4 = 0;
    int a1;
    char *q3;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
        break;
    case 1:
        arg0 += D_8009F8FC;
        if (func_80013514(D_800F4D98[arg0][1], D_800F4D98[arg0][0], D_800F4D98[arg0][2] % 20, D_800ACBD0[arg0].a, arg0) == 1) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        r = func_800135BC();
        if (r != 1) break;
        arg0 += D_8009F8FC;
        a1 = D_800ACBD0[arg0].b;
        if (a1 >= 2) {
            q3 = D_800F3B24;
            a1 -= 2;
        } else {
            if (a1 == r) {
                q3 = D_800F3B28 + (arg0 & 1) * 0x840;
                a1 = 0;
            } else {
                a1 = -1;
            }
        }
        if (D_800F4D44 != 1) {
            func_8001E854(q3, a1, arg0);
        }
        D_8009F8FC++;
        if (D_8009F8FC == arg1) {
            D_800F4D7C = 3;
        } else {
            D_800F4D7C = 1;
        }
        break;
    case 3:
        q4 = 1;
        break;
    default:
        break;
    }
    return q4;
}
