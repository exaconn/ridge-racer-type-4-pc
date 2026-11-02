extern short D_800A22E4[];

int func_8006E368(int a0, int a1)
{
    int r;
    if (a0 == 0) {
        r = 0;
        if (a1 != 0) {
            r = -0x400;
            if (a1 > 0) {
                return 0x400;
            }
            goto ret;
        }
        goto ret;
    }
    if (a0 > 0) {
        if (a1 >= 0) {
            if (a0 < a1) {
                return 0x400 - D_800A22E4[(a0 << 10) / a1];
            }
            r = D_800A22E4[(a1 << 10) / a0];
ret:
            return r;
        }
        a1 = -a1;
        if (a0 < a1) {
            return D_800A22E4[(a0 << 10) / a1] - 0x400;
        }
        return -D_800A22E4[(a1 << 10) / a0];
    }
    a0 = -a0;
    if (a1 >= 0) {
        if (a0 < a1) {
            return D_800A22E4[(a0 << 10) / a1] + 0x400;
        }
        return 0x800 - D_800A22E4[(a1 << 10) / a0];
    }
    a1 = -a1;
    if (a0 < a1) {
        return 0xC00 - D_800A22E4[(a0 << 10) / a1];
    }
    return D_800A22E4[(a1 << 10) / a0] + 0x800;
}
