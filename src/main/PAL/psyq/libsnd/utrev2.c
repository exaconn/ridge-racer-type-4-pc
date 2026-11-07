/* GCC_VERSION: 970318 */
#include "psyq/libsnd/utrev2.h"
#include "psyq/libspu/srevattr.h"
extern int D_80110008[];
void SsUtSetReverbFeedback(short a0) {
 int *p = D_80110008;  p[0] = 0x10; p[4] = a0; SpuSetReverbModeParam(p);
}
