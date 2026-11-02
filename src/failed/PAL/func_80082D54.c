extern int func_80087064(), func_80082FC4(), func_800831D4(), func_80083274(), func_80083A94(), func_80083AF4(), func_80084314();
extern volatile int D_8010F05C; extern int D_8010F060;
extern int *D_8010F068[];
extern short D_8010F8E8, D_8010F8EA;
void func_80082D54() {
    int i;
    int j;
    int k;
    int m;
    int **pp;
    int *base;
    if (D_8010F05C != 1) {
        D_8010F05C = 1;
        i = 0;
        func_80087064();
        if (D_8010F8E8 > 0) {
            pp = &D_8010F068[0];
            do {
                if (D_8010F060 & (1 << i)) {
                    j = 0;
                    if (D_8010F8EA > 0) {
                        k = 0;
                        m = 0;
                        do {
                            if (*(int *)((char *)*pp + m + 0x98) & 1) {
                                func_80083274((short)i, k >> 16);
                                if (*(int *)((char *)*pp + m + 0x98) & 0x10) func_80082FC4((short)i, k >> 16);
                                if (*(int *)((char *)*pp + m + 0x98) & 0x20) func_80082FC4((short)i, k >> 16);
                                if (*(int *)((char *)*pp + m + 0x98) & 0x40) func_80084314((short)i, k >> 16);
                                if (*(int *)((char *)*pp + m + 0x98) & 0x80) func_80084314((short)i, k >> 16);
                            }
                            if (*(int *)((char *)*pp + m + 0x98) & 2) func_800831D4((short)i, k >> 16);
                            if (*(int *)((char *)*pp + m + 0x98) & 8) func_80083A94((short)i, k >> 16);
                            if (*(int *)((char *)*pp + m + 0x98) & 4) {
                                func_80083AF4((short)i, k >> 16);
                                *(int *)((char *)*pp + m + 0x98) = 0;
                            }
                            k += 0x10000;
                            j++;
                            m += 0xB0;
                        } while (j < D_8010F8EA);
                    }
                }
                i++;
                pp += 4;
            } while (i < D_8010F8E8);
        }
        D_8010F05C = 0;
    }
}
