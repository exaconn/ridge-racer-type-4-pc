/* RODATA 0x80010804 0x18 */
extern int func_8001223C();
extern int func_8001292C();
extern int func_80012A58();
extern int func_800129BC();
extern int func_80029358();
extern int func_80032914();
extern int func_80043608();
extern char D_8009F8FC;
extern char D_800FFC48[];
extern char D_80104DB0[];
typedef struct { char p[7]; char b; } Z;
extern Z *D_800AC590;
extern int D_800AD564, D_800F4CA8;
extern unsigned char D_800F4D7C;
int func_8003B934(void) {
    int r = 0;
    char *s;
    int a1;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_8001292C(func_8001223C(1, 0x42), D_800FFC48)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80012A58()) {
            s = D_800FFC48;
            func_80043608(s);
            s += 0x4E20;
            D_800F4D7C = 3;
            D_800AC590 = (Z *)s;
            func_80032914(s);
    case 3:
            if (D_800F4CA8 == 2) {
                a1 = 0x44;
            } else if (D_800F4CA8 == 4) {
                a1 = 0x46;
                D_800AC590->b = 2;
            } else {
                a1 = 0x45;
            }
            if (func_800129BC(func_8001223C(1, a1), D_800AD564 << 2, 4, D_80104DB0)) {
                D_800F4D7C = 4;
            }
        }
        break;
    case 4:
        if (func_80012A58()) {
            func_80029358(D_80104DB0);
            D_800F4D7C = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
