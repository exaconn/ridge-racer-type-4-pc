/* GCC_VERSION: 2.7.2 */
extern int func_8007F54C(), func_8007F574();
struct H { char p[0x1A6]; volatile unsigned short f1A6; char q[2]; volatile unsigned short f1AA; };
extern struct H *D_800A47D8;
extern int *D_800A47DC, *D_800A47E0, *D_800A47E4;
extern unsigned short D_800A47F0;
extern int D_800A4800, D_800A4828, D_800A482C, D_800A4830;
int func_8007F068(int a0, ...) {
    unsigned *s0;
    unsigned v1;
    unsigned t;
    int x;
    int r;
    s0 = (unsigned *)__builtin_next_arg(a0);
    if (a0 != 1) {
        if (a0 < 2) {
            if (a0 != 0) return 0;
            D_800A4828 = 1;
            if (D_800A47D8->f1A6 != D_800A47F0) {
                v1 = 1;
            l1:
                r = -2;
                if (v1 < 0xF01) {
                    v1++;
                    if (D_800A47D8->f1A6 == D_800A47F0) goto b17;
                    goto l1;
                }
                return r;
            }
        b17:
            D_800A47D8->f1AA |= 0x30;
            return 0;
        }
        switch (a0) {
        case 2:
            t = s0[0] >> D_800A4800;
            D_800A47F0 = t;
            D_800A47D8->f1A6 = t;
            return 0;
        case 3:
            x = 0x20;
            if (D_800A4828 == 1) x = 0x30;
            x &= 0xFFFF;
            v1 = 1;
            if ((D_800A47D8->f1AA & 0x30) != x) {
            l2:
                r = -2;
                if (v1 < 0xF01) {
                    v1++;
                    if ((D_800A47D8->f1AA & 0x30) == x) goto b23;
                    goto l2;
                }
            } else {
            b23:
                if (D_800A4828 == 1) {
                    s0 = s0 + 1;
                    func_8007F574(x, D_800A47D8, 1);
                } else {
                    s0 = s0 + 1;
                    func_8007F54C(x, D_800A47D8, 1);
                }
                D_800A482C = s0[-1];
                t = s0[0];
                D_800A4830 = (t >> 6) + ((t & 0x3F) != 0);
                *D_800A47DC = D_800A482C;
                *D_800A47E0 = (D_800A4830 << 16) | 0x10;
                x = 0x01000201;
                if (D_800A4828 == 1) x = 0x01000200;
                *D_800A47E4 = x;
                r = 0;
            }
            return r;
        default:
            return 0;
        }
    } else {
        D_800A4828 = 0;
        if (D_800A47D8->f1A6 != D_800A47F0) {
            v1 = 1;
        l3:
            r = -2;
            if (v1 < 0xF01) {
                v1++;
                if (D_800A47D8->f1A6 == D_800A47F0) goto b12;
                goto l3;
            }
            return r;
        }
    b12:
        D_800A47D8->f1AA = (D_800A47D8->f1AA & 0xFFCF) | 0x20;
        return 0;
    }
}
