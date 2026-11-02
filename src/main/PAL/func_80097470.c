/* CC1_FLAGS: -mno-split-addresses */
extern int func_80096F04();
extern int func_80096F14();
extern int func_80096F84();
extern int func_80097568();
extern int func_800976C4();
extern int func_8009774C();
extern int func_800977C4();
extern int D_800ABC50;
void func_80097470(a0, a1, a2, a3) int a0; int a1; int a2; int a3; {
    func_800977C4();
    func_80096F04();
    func_8009774C();
    func_80096F14();
    func_80096F84(0);
    func_80097568();
    func_800976C4(a0, a1, a2, a3);
    D_800ABC50 = 1;
    /* ASM_BARRIER: keeps epilogue stack loads (lw ra/s*) after the final global stores; sched2 otherwise hoists/sinks them */
    __asm__ volatile ("" : : : "memory");
}
