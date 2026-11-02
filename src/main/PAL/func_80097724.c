/* CC1_FLAGS: -mno-split-addresses */
extern void *jtbl_80114400;

/* Dispatch stub: tail-jumps through the function pointer stored at 0x80114400. */
void func_80097724(void) {
    /* ASM_BARRIER: the original stub loads and jumps through $t1. */
    register void *target asm("$9") = jtbl_80114400;
    goto *target;
}
