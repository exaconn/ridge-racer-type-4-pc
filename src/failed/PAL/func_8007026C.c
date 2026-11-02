extern int D_800F2EBC, D_800F39A4, D_800F391C, D_800F2EB4, D_800F29E0, D_800F4D34, D_800ADAE8;
extern int D_800F4D28, D_800F4C60, D_800ABEA8, D_800F4C20, D_800ABE9C, D_800FFC40, D_800F4C30;
extern int D_800ABEA0, D_800F4C78, D_800ABEB8, D_800F4C6C, D_800ABEB4, D_800F3820, D_800ACC04;
void func_8007026C(arg) int *arg; {
    int *a0;
    int *a2;
    unsigned a3;
    int n;
    int t;
    a0 = arg;
    a2 = a0 + 0x14;
    n = a0[0];
    a3 = 0;
    D_800F2EBC = (int)a0;
    D_800F39A4 = n;
    t = a0[2] + (int)a0;
    a0[1] = a0[1] + (int)a0;
    a0[2] = t;
    D_800F391C = t;
    t = a0[3] + (int)a0;
    a0[3] = t;
    D_800F2EB4 = t;
    D_800F29E0 = a0[4] + (int)a0;
    D_800F4D34 = a0[5] + (int)a0;
    D_800ADAE8 = a0[6] + (int)a0;
    D_800F4D28 = a0[7] + (int)a0;
    D_800F4C60 = a0[8] + (int)a0;
    D_800ABEA8 = a0[9] + (int)a0;
    D_800F4C20 = a0[10] + (int)a0;
    D_800ABE9C = a0[11] + (int)a0;
    D_800FFC40 = a0[12] + (int)a0;
    D_800F4C30 = a0[13] + (int)a0;
    D_800ABEA0 = a0[14] + (int)a0;
    D_800F4C78 = a0[15] + (int)a0;
    D_800ABEB8 = a0[16] + (int)a0;
    D_800F4C6C = a0[17] + (int)a0;
    D_800ABEB4 = a0[18] + (int)a0;
    D_800F3820 = (int)a2;
    D_800ACC04 = a0[19] + (int)a0;
    if (n * 8 != 0) {
        do {
            a3++;
            *a2 += (int)a0;
            a2 += 1;
        } while (a3 < D_800F39A4 * 8);
    }
}
