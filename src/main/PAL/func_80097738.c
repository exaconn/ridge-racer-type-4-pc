/* CC1_FLAGS: -mno-split-addresses */
extern void *jtbl_80114404;

/* Dispatch stub: tail-jumps through the function pointer stored at 0x80114404. */
void func_80097738(void) {
    /* ASM_BARRIER: the original stub loads and jumps through $t1. */
    register void *target asm("$9") = jtbl_80114404;
    goto *target;
}
