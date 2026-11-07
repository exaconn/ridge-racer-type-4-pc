/* GCC_VERSION: 970318 */
#include "psyq/libsnd/utrev2.h"
#include "psyq/libspu/srevattr.h"
extern int _svm_rattr[];
void SsUtSetReverbFeedback(short a0) {
 int *p = _svm_rattr;  p[0] = 0x10; p[4] = a0; SpuSetReverbModeParam(p);
}
