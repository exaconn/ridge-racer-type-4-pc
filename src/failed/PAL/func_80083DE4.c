extern int D_8010F060;
extern char *D_8010F068[];
extern short D_8010F8E8;
extern short D_8010F8EA;
void func_80083DE4(a0, a1, a2) int a0; short a1; short a2; {
    int a3, v1, v0, a0b, a2b;
    short s3;
    char **a2p;
    char **t1;
    char *b;
    D_8010F8E8 = a1;
    D_8010F8EA = a2;
    a3 = 0;
    if (a1 > 0) {
        a2p = D_8010F068;
        v1 = 0;
        do {
            *a2p = (char *)(a0 + v1 * 0xB0);
            a2p += 4;
            a3 += 1;
            v1 += a2;
        } while (a3 < a1);
    }
    s3 = a1;
    if (s3 < 0x20) {
        v0 = 1 << s3;
        do {
            s3 += 1;
            D_8010F060 |= v0;
            v0 = 1 << s3;
        } while (s3 < 0x20);
    }
    a3 = 0;
    if (D_8010F8E8 > 0) {
        t1 = D_8010F068;
        do {
            a2b = 0;
            if (D_8010F8EA > 0) {
                a0b = 0;
                do {
                    b = *t1 + a0b;
                    *(int *)(b + 0x98) = 0;
                    *(signed char *)(*t1 + a0b + 0x22) = -1;
                    *(char *)(*t1 + a0b + 0x23) = 0;
                    *(short *)(*t1 + a0b + 0x48) = 0;
                    *(short *)(*t1 + a0b + 0x4A) = 0;
                    *(int *)(*t1 + a0b + 0x9C) = 0;
                    *(int *)(*t1 + a0b + 0xA0) = 0;
                    *(short *)(*t1 + a0b + 0x4C) = 0;
                    *(int *)(*t1 + a0b + 0xAC) = 0;
                    *(int *)(*t1 + a0b + 0xA8) = 0;
                    *(int *)(*t1 + a0b + 0xA4) = 0;
                    *(short *)(*t1 + a0b + 0x4E) = 0;
                    *(short *)(*t1 + a0b + 0x58) = 0x7F;
                    *(short *)(*t1 + a0b + 0x5A) = 0x7F;
                    a2b += 1;
                    *(short *)(*t1 + a0b + 0x5C) = 0x7F;
                    b = *t1 + a0b;
                    a0b += 0xB0;
                    *(short *)(b + 0x5E) = 0x7F;
                } while (a2b < D_8010F8EA);
            }
            a3 += 1;
            t1 += 4;
        } while (a3 < D_8010F8E8);
    }
}
