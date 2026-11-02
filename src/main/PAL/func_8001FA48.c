#include "rr/animated_glyph.h"

void func_8001FA48(arg0, arg1, p, arg3, arg4)
int arg0; int arg1; char *p; int arg3; int arg4;
{
    int s7 = 0, s4 = 0, s2 = 0, s6, s3;
    unsigned s0;
    unsigned short *s1;
    unsigned a2, v0; int v1;
    if (arg3 == 1) {
        *(short *)(p + 0x12) = (unsigned char)p[0xA] - (unsigned char)p[0xB];
        s6 = (unsigned char)p[0xA] - (unsigned char)p[0xB];
    } else {
        s6 = *(short *)(p + 0x12);
    }
    s1 = *(unsigned short **)p;
    s0 = 0;
    if (s6 != 0) {
        do {
            a2 = *s1++;
            s3 = 0;
            if (a2 & 0x1000) {
                v0 = a2 & 0xFFF;
                if (v0 == 0) {
                    s2 = s3;
                    s4 += 0x18;
                    goto L_BE8;
                }
                a2 = v0;
                s3 = 1;
                goto L_call;
            }
            v1 = a2;
            if ((a2 & 0xFE00) == 0x6000) {
                s0 -= 1;
                a2 = *s1++;
                switch (v1 & 0xFFFF) {
                default:
                    s0 += 1;
                    goto L_BEC;
                case 0x6005:
                    *(short *)(p + 0xE) = a2 & 0xFF;
                    goto L_BE8;
                case 0x6003:
                    *(char *)(p + 0x1C) = a2;
                    goto L_BE8;
                case 0x6004:
                    *(short *)(p + 0x14) = (signed char)a2;
                    if (s0 != s6 - 2) {
                        s0++;
                        goto L_BEC;
                    }
                    s0 = s6 - 1;
                    goto L_BE8;
                }
            }
            /* ASM_BARRIER: launder a2 so the second (a2 & 0xFE00) is not CSE'd with the 0x6000 test */
            __asm__("" : "+r"(a2));
            if ((int)(a2 & 0xFE00) > 0) goto L_BE8;
            if (a2 == 0) {
                s2 += 7;
                goto L_BE8;
            }
        L_call:
            func_8001F808(arg0 + (short)s2, arg1 + (short)s4, a2 - 1, arg4, (s16 *)(p + (s7 * 4 + 0x28)));
            s2 += 7;
            if (s3 != 0) {
                s2 = 0;
                s4 += 0x1A;
            }
            s7 += 1;
        L_BE8:
            s0 += 1;
        L_BEC:;
        } while (s0 < s6);
    }
}
