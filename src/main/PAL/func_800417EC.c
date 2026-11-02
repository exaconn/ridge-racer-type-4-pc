extern int func_8003A198();
extern int func_8004F24C();
extern int D_8009FDA4;
extern char D_800AD4D8[];
extern short D_800AD520;
extern int D_800F2DE4;
extern short D_800F3A26;
extern unsigned short D_800F3A42;
extern int D_800F3A60;
typedef struct { char p[0x10]; short s10; } T3;
extern T3 *D_800F3AFC;
extern short D_800F4C6A;
extern char *D_800F4D80;
extern short D_800F4D8A;
extern signed char *D_800F4D8C;
extern unsigned char D_800F4D98;
extern char *D_800FF68C;
void func_800417EC()
{
    int s0;
    char *q; char *r;
    int t = D_800F4D98;
    D_800AD520 = t;
    D_800F4D80[t + 0xC] = 0;
    if (D_800F3A42 & 0x2000) {
        if (D_800AD520 < 3) {
            D_800AD520 = (unsigned short)D_800AD520 + 1;
            D_8009FDA4 = 2;
            goto L_a8;
        }
    }
    if (D_800F3A42 & 0x8000) {
        if (D_800AD520 > 0) {
            D_800AD520 = (unsigned short)D_800AD520 - 1;
            D_8009FDA4 = 1;
            goto L_a8;
        }
    }
    D_8009FDA4 = 0;
    goto L_54;
L_a8:
    func_8004F24C(0);
    if (D_800AD520 < 0) {
        D_800AD520 = 0;
    } else if (D_800AD520 >= 4) {
        D_800AD520 = 3;
    }
    D_800F4D98 = D_800AD520;
    D_800FF68C[4] = (unsigned char)D_800AD520 + 0x22;
    D_800F4D8C[D_800AD520 * 0x1C + 2] = -6;
    D_800F4D8C[D_800AD520 * 0x1C] = 0;
    D_800F4D80[0x16] = D_800AD520;
L_54:
    q = D_800F4D80 + 0xC;
    q[D_800AD520] = 2;
    if (D_800F3A60 != 0) {
        s0 = D_800F3AFC->s10;
        if (s0 == 3) {
            func_8004F24C(1);
            D_800F2DE4 = 0;
            func_8003A198();
            r = D_800AD4D8;
            *(short *)(r + 0x1A) = 1;
            r[0x1C] = D_800F4D98;
            D_800F3AFC->s10 = 4;
            D_800F4D80[0x14] = s0;
            D_800F4C6A = 1;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }
}
