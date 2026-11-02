/* GCC_VERSION: 2.7.2 */
extern int func_80085BA4();
extern int D_80110008[];
void func_80085B64(short a0) {
 /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
 int *p = D_80110008; __asm__("" : "=r"(p) : "0"(p)); p[0] = 8; p[3] = a0; func_80085BA4(p);
}
