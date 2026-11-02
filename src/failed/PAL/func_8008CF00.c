/* CC1_FLAGS: -mno-split-addresses -fno-schedule-insns */
extern int func_8008B154(), func_8008B4B8(), func_8008C724(), func_8008D65C(), func_800963DC(), func_8001E84C();
extern char D_80011A14[], D_80011A24[], D_80011AA4[];
extern void (*D_800A6488)();
extern void (*D_800A648C)();
extern unsigned char D_800A64A5, D_800A6764[];
extern int D_800A64AC[], D_800A652C[];
extern volatile unsigned char *D_800A674C;
extern char D_80110318[], D_80110320[], D_80110328[];
extern int D_80110330, D_80110334;
extern char *D_80110338;
int func_8008CF00(a0, a1) int a0; unsigned char *a1; {
    int c, r, v, s1, a2, n;
    char *p, *q;
    D_80110330 = func_8008B154(-1) + 0x3C0;
    D_80110334 = 0;
    D_80110338 = D_80011AA4;
    for (;;) {
        if (D_80110330 >= func_8008B154(-1)) {
            c = D_80110334;
            D_80110334 = c + 1;
            if (c > 0x3C0000) goto to;
            r = 0;
        } else {
        to:
            func_800963DC(D_80011A14);
            func_8001E84C(D_80011A24, D_80110338, D_800A64AC[D_800A64A5], D_800A652C[D_800A6764[0]], D_800A652C[D_800A6764[1]]);
            func_8008D65C();
            r = -1;
        }
        if (r != 0) return -1;
        if (func_8008B4B8() != 0) {
            s1 = *D_800A674C & 3;
            while ((v = func_8008C724()) != 0) {
                if (v & 4) {
                    if (D_800A648C != 0) D_800A648C(D_800A6764[1], D_80110320);
                }
                if (v & 2) {
                    if (D_800A6488 != 0) D_800A6488(D_800A6764[0], D_80110318);
                }
            }
            *D_800A674C = s1;
        }
        a2 = D_800A6764[2];
        if (a2 != 0) {
            D_800A6764[2] = 0;
            p = D_80110328;
            q = (char *)a1;
            if (a1 != 0) {
                n = 7;
                do { *q++ = *p++; n--; } while (n != -1);
            }
            return a2;
        }
        a2 = D_800A6764[1];
        if (a2 != 0) {
            D_800A6764[1] = 0;
            q = (char *)a1;
            p = D_80110320;
            if (q != 0) {
                n = 7;
                do { *q++ = *p++; n--; } while (n != -1);
            }
            return a2;
        }
        if (a0 != 0) return 0;
    }
}
