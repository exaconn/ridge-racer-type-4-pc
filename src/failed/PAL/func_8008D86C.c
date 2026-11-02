/* CC1_FLAGS: -mno-split-addresses */
extern int func_800963DC(), func_8001E84C(), func_8008B364(), func_8008B394(), func_8008CC80(), func_8008D1C8(), func_8008DDAC();
extern char D_80011B08[], D_80011B14[], D_800A6768[];
extern int D_800A6488, D_800A648C, D_800A6494, D_800A6498;
extern signed char D_800A64A4, D_800A64A5;
extern volatile signed char *D_800A674C;
extern volatile signed char *D_800A6754;
extern volatile unsigned char *D_800A6758;
extern volatile int *D_800A675C;
extern volatile unsigned char D_800A6764[];

int func_8008D86C() {
    func_800963DC(D_80011B08);
    func_8001E84C(D_80011B14, D_800A6768);
    D_800A64A5 = 0;
    D_800A64A4 = 0;
    D_800A648C = 0;
    D_800A6488 = 0;
    D_800A6498 = 0;
    D_800A6494 = 0;
    func_8008B364();
    func_8008B394(2, func_8008DDAC);
    *D_800A674C = 1;
    if (*D_800A6758 & 7) {
        do {
            *D_800A674C = 1;
            *D_800A6758 = 7;
            *D_800A6754 = 7;
        } while (*D_800A6758 & 7);
    }
    D_800A6764[2] = 0;
    D_800A6764[1] = D_800A6764[2];
    D_800A6764[0] = 2;
    *D_800A674C = 0;
    *D_800A6758 = 0;
    *D_800A675C = 0x1325;
    func_8008D1C8(1, 0, 0, 0);
    if (D_800A6494 & 0x10) {
        func_8008D1C8(1, 0, 0, 0);
    }
    if (func_8008D1C8(0xA, 0, 0, 0) == 0) {
        if (func_8008D1C8(0xC, 0, 0, 0) == 0) {
            if (func_8008CC80(0, 0) == 2) {
                return 0;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}
