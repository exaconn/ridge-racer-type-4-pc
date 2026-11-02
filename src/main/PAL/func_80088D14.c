/* CC1_FLAGS: -mno-split-addresses */
extern int D_800A47D8;
int func_80088D14(a0) int a0; {
    /* ASM_BARRIER: pin the clamp result to $a1 to match the original register allocation */
    int v;
    int r;
    int p;
    v = a0; __asm__ volatile("" : "+r"(v)); /* ASM_BARRIER: keep the copy of a0 as a separate pseudo so the clamp tests the copy as the original does */
    if (v < 0) r = 0; else { r = v; if (r >= 0x40) r = 0x3F; }
    p = D_800A47D8;
    *(unsigned short *)(p + 0x1AA) = (*(unsigned short *)(p + 0x1AA) & 0xC0FF) | ((r & 0x3F) << 8);
    return r;
}
