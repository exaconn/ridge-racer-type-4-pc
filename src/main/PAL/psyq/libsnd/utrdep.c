/* GCC_VERSION: 970318 */
#include "rr/library.h"
#include "psyq/libspu/srevattr.h"
extern int _svm_rattr[];
void SsUtSetReverbDepth(int raw_left, int raw_right) {
 short a0 = raw_left;
 short a1 = raw_right;
 int *p = _svm_rattr;
 p[0] = 6;
 ((short *)p)[4] = (a0 * 0x7FFF) / 127;
 ((short *)p)[5] = (a1 * 0x7FFF) / 127;
 SpuSetReverbModeParam(p);
}
