#include "rr/track_lateral.h"

int ScanCurveAhead(a0, a1, a2, a3, a4) char *a0; int a1; int *a2; int *a3; int *a4;
{
    int v0, v1, s0, s1, s2, s3, s4, s5, r0;
    int r1;
    int t;
    v0 = *(short *)(a0 + 0x1D8) * 0xC;
    if (v0 < 0) v0 += 0xF;
    v1 = v0 >> 4;
    if (v1 < 0x200) v1 = 0x200;
    s0 = *(int *)(a0 + 0xF0);
    s1 = -*(short *)(a0 + 0x1D4) + 0x400;
    s1 &= 0xFFF;
    s2 = v1;
    if (s1 & 0x800) s1 |= ~0xFFF;
    s4 = 0;
    s3 = 0;
    s5 = 0;
    if (s2 > 0) {
        do {
            r0 = GetTrackNodeHeading(s0, a1);
            r0 &= 0xFFF;
            v1 = s1 - r0;
            if (r0 & 0x800) {
                r0 |= ~0xFFF;
                v1 = s1 - r0;
            }
            v1 = v1 & 0xFFF;
            if (v1 & 0x800) v1 |= ~0xFFF;
            if (v1 > 0) s4 += v1; else s3 += v1;
            if (v1 < 0) v1 = -v1;
            /* ASM_BARRIER: stop reorg copying the slt into the bgez delay slot */
            __asm__("" : "=r"(v1) : "0"(v1));
            if (s5 < v1) s5 = v1;
            s0 -= 1;
            s1 = r0;
            if (s0 < 0) s0 += TrackNodeCount;
            s2 -= TrackNodes[s0].segmentLength1C;
        } while (s2 > 0);
    }
    *a2 = s5 << 12;
    *a3 = s3;
    *a4 = s4;
    t = s4 - s3;
    r0 = t;
    r1 = r0 << 12;
    return r1 / 12;
}
