/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

extern int CD_init();

extern int CD_initvol();

int func_8008BED4() {
    int r;
    if (CD_init() == 0) { r = CD_initvol() == 0; } else r = 0;
    return r;
}
