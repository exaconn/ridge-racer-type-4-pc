typedef struct { char p[0xF8]; int f0, f4; unsigned short a[6]; unsigned short b[6]; short c, d; char pad[0x320 - 0x11C]; } T;
extern T D_8010738C[];
void func_80049430(int x, int y, int z) {
    int i;
    T *tx = &D_8010738C[x];
    T *ty = &D_8010738C[y];
    T *tz = &D_8010738C[z];
    tx->f0 = (ty->f0 + tz->f0) / 2;
    for (i = 0; i < 6; i++) {
        tx->a[i] = (ty->a[i] + tz->a[i]) >> 1;
    }
    for (i = 0; i < 6; i++) {
        tx->b[i] = (ty->b[i] + tz->b[i]) >> 1;
    }
    tx->c = (ty->c + tz->c) / 2;
    tx->d = (ty->d + tz->d) / 2;
}
