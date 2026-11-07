int get_mode(int a0, int a1, int a2) {
    int v1;
    register int v0 asm("$2");
    v1 = 0xE1000000;
    if (a1 != 0) {
        v1 = 0xE1000200;
    }
    v0 = a2 & 0x9FF;
    if (a0 != 0) {
        v0 |= 0x400;
    }
    return v1 | v0;
}
