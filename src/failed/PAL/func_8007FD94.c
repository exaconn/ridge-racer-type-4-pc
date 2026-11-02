/* GCC_VERSION: 2.7.2 */
extern int func_8001E84C(), func_80081EB4();
extern char D_800116E8[];
extern int D_8010EFC8[];
extern int D_8010F060;
extern int func_80080034(), func_800800B4(), func_80080434(), func_80080504(), func_800805D4(), func_800806C4(), func_80080774(), func_80080804(), func_80080904(), func_80080A44(), func_80080AB4(), func_80080B24(), func_80080BE4(), func_80080C74(), func_80080D34(), func_80080DC4(), func_80080E54(), func_80081004(), func_800810C4(), func_80081174(), func_80081224(), func_800812D4(), func_80081394(), func_80081444(), func_800814F4(), func_800815D4(), func_80081674(), func_80081714(), func_80081744(), func_80081774(), func_800817A4(), func_800817D4(), func_80081804(), func_800818B4(), func_80081AE4(), func_80081CB4(), func_80081D94();
short func_8007FD94(a0, a1) int a0; int a1; {
    int t0;
    int v1;
    int a3;
    int a;
    int x;
    int bit;
    int *tab;
    t0 = 0;
    a3 = D_8010F060;
    if (a3 == -1) {
    err:
        func_8001E84C(D_800116E8, a1, a0, a3);
        return -1;
    } else {
        v1 = 0;
        a = 0;
        for (;;) {
            bit = 1 << v1;
            if (!(a3 & bit)) { t0 = v1; a = 1; }
            v1++;
            if (v1 >= 0x20) goto err;
            if (a != 0) {
                D_8010F060 |= 1 << t0;
                x = func_80081EB4((short)t0, (short)a1, a0, a3) << 16;
                tab = D_8010EFC8;
                tab[0] = (int)func_80081CB4;
                tab[1] = (int)func_80081D94;
                tab[3] = (int)func_80081AE4;
                tab[2] = (int)func_80081804;
                tab[4] = (int)func_800818B4;
                tab[5] = (int)func_80080034;
                tab[7] = (int)func_80080434;
                tab[8] = (int)func_80080504;
                tab[9] = (int)func_800805D4;
                tab[10] = (int)func_800806C4;
                tab[11] = (int)func_80080804;
                tab[12] = (int)func_80080904;
                tab[13] = (int)func_80080A44;
                tab[14] = (int)func_80080AB4;
                tab[15] = (int)func_80080774;
                tab[16] = (int)func_80080B24;
                tab[6] = (int)func_800800B4;
                tab[17] = (int)func_80080BE4;
                tab[18] = (int)func_80080C74;
                tab[19] = (int)func_80080D34;
                tab[20] = (int)func_80080DC4;
                tab[21] = (int)func_80080E54;
                tab[22] = (int)func_80081004;
                tab[23] = (int)func_800810C4;
                tab[24] = (int)func_80081174;
                tab[25] = (int)func_80081224;
                tab[26] = (int)func_800812D4;
                tab[27] = (int)func_80081394;
                tab[28] = (int)func_80081444;
                tab[29] = (int)func_800814F4;
                tab[30] = (int)func_800815D4;
                tab[31] = (int)func_80081674;
                tab[32] = (int)func_80081714;
                tab[33] = (int)func_80081744;
                tab[34] = (int)func_80081774;
                tab[35] = (int)func_800817A4;
                tab[36] = (int)func_800817D4;
                if ((x >> 16) != -1) return (short)t0;
                return -1;
            }
        }
    }
}
