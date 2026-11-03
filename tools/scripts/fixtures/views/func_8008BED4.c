/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

extern int func_8008D86C();

extern int func_8008D730();

int func_8008BED4() {
    int r;
    if (func_8008D86C() == 0) { r = func_8008D730() == 0; } else r = 0;
    return r;
}
