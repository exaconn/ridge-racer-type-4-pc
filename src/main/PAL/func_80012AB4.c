void func_80012AB4(int *a0, int a1) {
    int n = a0[0];
    char **list = (char **)(a0 + 3);
    int x = (a1 << 6) + 0x180;
    int tp_x = (x & 0x3FF) >> 6;
    int clut_x = (x >> 4) & 0x3F;
    int u_add = a1 << 6;
    int u_mask = ~0xC0;
    int clut2 = a1 + 0x1028;
    int t, z;

    while (z = 0, ((t = n--) + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z) != 0) {
        char *p = *list++;
        unsigned int cmd;
        while (cmd = *(unsigned int *)p, p += 4, cmd != 0) {
            int count = cmd >> 16;
            switch (cmd & 0xFFFF) {
            case 0:
                p += count * 16;
                break;
            case 2:
                p += count * 24;
                break;
            case 1:
                while (count-- > 0) {
                    *(short *)(p + 0xE) = (*(short *)(p + 0xE) & ~0x1F) | tp_x;
                    *(short *)(p + 0xA) = (*(short *)(p + 0xA) & ~0x403C) | clut_x;
                    p += 0x18;
                }
                break;
            case 3:
                while (count-- > 0) {
                    if (*(short *)(p + 0x16) < 0) {
                        *(short *)(p + 0x16) = 0xA;
                        *(short *)(p + 0x10) = (*(short *)(p + 0x10) & u_mask) | u_add;
                        *(short *)(p + 0x14) = (*(short *)(p + 0x14) & u_mask) | u_add;
                        *(short *)(p + 0x18) = (*(short *)(p + 0x18) & u_mask) | u_add;
                        *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & u_mask) | u_add;
                        *(short *)(p + 0x12) = (*(short *)(p + 0x12) & ~0x403C) | clut2;
                        *(short *)(p + 0x1E) |= 0x78;
                    } else {
                        *(short *)(p + 0x16) = (*(short *)(p + 0x16) & ~0x1F) | tp_x;
                        *(short *)(p + 0x12) = (*(short *)(p + 0x12) & ~0x403C) | clut_x;
                        *(short *)(p + 0x1E) |= 0x78;
                    }
                    p += 0x20;
                }
                break;
            }
        }
    }
}
