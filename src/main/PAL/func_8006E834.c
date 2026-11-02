extern unsigned int *D_800ADAD0;
extern int D_800F2EB4;
extern unsigned int D_8010E290[];
extern unsigned int *D_8010E690;
extern unsigned int *func_8006F288();

void func_8006E834(void) {
    unsigned int *t;
    unsigned int i;
    D_8010E690 = D_8010E290;
    t = func_8006F288();
    D_800ADAD0 = t;
    D_8010E290[0] = *t;
    if (D_8010E290[0] >= 0x100) {
        D_8010E290[0] = 0xFF;
    }
    t++;
    i = 0;
    if (D_8010E290[0] != 0) {
        do {
            D_8010E290[i + 1] = D_800F2EB4 + ((unsigned short *)t)[i] * 0x50;
            i++;
        } while (i < D_8010E290[0]);
    }
}
