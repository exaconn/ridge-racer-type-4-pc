/* GCC_VERSION: 970318 */
#include "psyq/libsnd/utrtype.h"
#include "psyq/libspu/srevattr.h"
extern int _svm_rattr[];
void SsUtSetReverbDelay(short a0) {
 int *p = _svm_rattr;  p[0] = 8; p[3] = a0; SpuSetReverbModeParam(p);
}
