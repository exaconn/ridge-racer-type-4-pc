/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/library.h"

extern int D_800A63C4;

extern char D_80011878[];

extern char D_80011894[];

extern void func_8001E84C();

void trapIntrDMA(void) {
    unsigned int s1;
    int i;
    RrLibInterruptCallback *bp;
    unsigned int one, m;
    s1 = (*D_800A63A0 >> 24) & 0x7F;
    if (s1 != 0) {
        one = 1;
        m = 0xFFFFFF;
        bp = D_800A63A4;
        do {
            i = 0;
            if (s1 != 0) {
                for (; s1 != 0; i++, s1 >>= 1) {
                    if (i >= 7) break;
                    if (s1 & 1) {
                        *D_800A63A0 &= (one << (i + 24)) | m;
                        if (bp[i] != 0) bp[i]();
                    }
                }
            }
            s1 = (*D_800A63A0 >> 24) & 0x7F;
        } while (s1 != 0);
    }
    if ((*D_800A63A0 & 0xFF000000) == 0x80000000 || (*D_800A63A0 & 0x8000)) {
        func_8001E84C(D_80011878, *D_800A63A0);
        i = 0;
        do {
            func_8001E84C(D_80011894, i, *(int *)(i * 16 + D_800A63C4));
            i++;
        } while (i < 7);
    }
}
