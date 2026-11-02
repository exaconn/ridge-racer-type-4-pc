extern int func_8004E008();
extern int func_8004E5E8();
extern char D_8009F8FC;
extern short D_800AC650[];
extern unsigned char D_800F4D7C;
int func_8003A79C(void) {
    int r = 0;
    int i;
    switch (D_800F4D7C) {
    case 0:
        D_800F4D7C = 1;
        D_8009F8FC = 0;
        func_8004E008();
    case 1:
        if (func_8004E5E8() == 1) {
            D_800F4D7C = 2;
    case 2:
            i = 0;
            do {
                D_800AC650[i] = i + 1;
                i++;
            } while (i < 7);
            D_800AC650[7] = 0;
            D_800F4D7C = 3;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
