int func_80095A44(unsigned *ot, unsigned *p) {
    register unsigned lo asm("$6") = 0xFFFFFF;
    unsigned hi = 0xFF000000;
    *p = (*p & hi) | (*ot & lo);
    *ot = (*ot & hi) | ((unsigned)p & lo);
}
