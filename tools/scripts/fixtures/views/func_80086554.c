/* GCC_VERSION: 970318 */

#include "rr/library.h"

extern int SpuSetReverbModeParam();

extern int D_80110008[];

void SsUtSetReverbDepth(int raw_left, int raw_right) {
 short a0 = raw_left;
 short a1 = raw_right;
 int *p = D_80110008;
 p[0] = 6;
 ((short *)p)[4] = (a0 * 0x7FFF) / 127;
 ((short *)p)[5] = (a1 * 0x7FFF) / 127;
 SpuSetReverbModeParam(p);
}
