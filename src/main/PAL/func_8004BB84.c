typedef struct { char p0; unsigned char f1; char p2[6]; unsigned short f8; char pa[0x38]; unsigned char f42; char p43; unsigned char f44; char p45; char f46; unsigned char f47; unsigned char f48; char f49; char p4a[6]; short f50; char p52[10]; } T;
extern T D_800F3A38[];
void func_8004BB84(int i, int flags) {
    T *t = &D_800F3A38[i];
    if (flags & 1) {
        if (!(t->f44 & 0x40)) {
            t->f48 |= 0x40;
        }
    }
    if (flags & 2) {
        if (!(t->f44 & 0x80)) {
            t->f48 |= 0x80;
        }
    }
    if ((flags & 3) == 3) {
        t->f46 = 0;
        t->f47 = 0;
        t->f50 = 0;
        t->f49 = 0;
    }
    if (flags == 4) {
        if (!(t->f48 & 4)) {
            t->f47 = t->f42;
            t->f48 |= 0x84;
        }
    }
}
