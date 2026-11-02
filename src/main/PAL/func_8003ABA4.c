extern int func_800131D0();
extern int func_80013200();
extern char D_8009F8FC;
extern short D_800F3914;
extern unsigned char D_800F4D7C;
int func_8003ABA4(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
    case 1:
        if (func_800131D0(D_800F3914)) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (func_80013200()) r = 1;
        break;
    case 3:
        r = 1;
        break;
    }
    return r;
}
