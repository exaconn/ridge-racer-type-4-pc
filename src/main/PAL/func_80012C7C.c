/* RODATA 0x8001007c 0x3c */

void func_80012C7C(int *a0, int a1) {
    int n = a0[0];
    char *list = (char *)(a0 + 1);
    int x = (a1 << 6) + 0x180;
    int tp_x = (x & 0x3FF) >> 6;
    int clut_x = (x >> 4) & 0x3F;
    int m1 = ~0x1F;
    int m2 = ~0x403C;

    while (n-- > 0) {
        char *p = *(char **)(list + 8);
        unsigned int cmd;
        list += 0xC;
        while (cmd = *(unsigned int *)p, p += 4, cmd != 0) {
            int count = cmd >> 16;
            switch (cmd & 0xFFFF) {
            case 7:
                p += count * 20;
                break;
            case 8:
                p += count * 28;
                break;
            case 1:
                while (count-- > 0) {
                    *(short *)(p + 0x12) = (*(short *)(p + 0x12) & m1) | tp_x;
                    *(short *)(p + 0xE) = (*(short *)(p + 0xE) & m2) | clut_x;
                    p += 0x1C;
                }
                break;
            case 9:
                p += count * 20;
                break;
            case 5:
            case 10:
                p += count * 24;
                break;
            case 3:
                p += count * 20;
                break;
            case 4:
                p += count * 12;
                break;
            case 6:
                p += count * 8;
                break;
            case 11:
                while (count-- > 0) {
                    *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & m1) | tp_x;
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m2) | clut_x;
                    p += 0x20;
                }
                break;
            case 14:
                while (count-- > 0) {
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m1) | tp_x;
                    *(short *)(p + 0x12) = (*(short *)(p + 0x12) & m2) | clut_x;
                    p += 0x1C;
                }
                break;
            case 15:
                while (count-- > 0) {
                    *(short *)(p + 0x1A) = (*(short *)(p + 0x1A) & m1) | tp_x;
                    *(short *)(p + 0x16) = (*(short *)(p + 0x16) & m2) | clut_x;
                    p += 0x24;
                }
                break;
            }
        }
    }
}
