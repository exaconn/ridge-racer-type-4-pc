/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"
extern int func_80085BA4();
extern int D_80110008[];
void func_80086554(int raw_left, int raw_right) {
 short a0 = raw_left;
 short a1 = raw_right;
 int *p = D_80110008;
 /* ASM_BARRIER: legacy, reason not recorded (forces reg copy/CSE/scheduling); retire & retest with fd.py */
 __asm__("" : "=r"(p) : "0"(p));
 p[0] = 6;
 ((short *)p)[4] = (a0 * 0x7FFF) / 127;
 ((short *)p)[5] = (a1 * 0x7FFF) / 127;
 func_80085BA4(p);
}
