/* CC1_FLAGS: -mno-split-addresses */
extern unsigned char *D_800A674C, *D_800A6750, *D_800A6754, *D_800A6758;
extern char *D_800A6760;
#define R(o) (*(unsigned short *)(D_800A6760 + (o)))
int func_8008D730(void) {
    unsigned char b[4];
    if (R(0x1B8) == 0 && R(0x1BA) == 0) {
        R(0x180) = 0x3FFF;
        R(0x182) = 0x3FFF;
    }
    R(0x1B0) = 0x3FFF;
    R(0x1B2) = 0x3FFF;
    R(0x1AA) = 0xC001;
    b[2] = 0x80;
    b[0] = 0x80;
    b[3] = 0;
    b[1] = 0;
    *D_800A674C = 2;
    *D_800A6754 = b[0];
    *D_800A6758 = b[1];
    *D_800A674C = 3;
    *D_800A6750 = b[2];
    *D_800A6754 = b[3];
    *D_800A6758 = 0x20;
    return 0;
}
