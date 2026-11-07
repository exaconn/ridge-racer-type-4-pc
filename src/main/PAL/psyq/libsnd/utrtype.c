/* GCC_VERSION: 970318 */
#include "psyq/libsnd/utrtype.h"
#include "psyq/libspu/srevattr.h"
extern int D_80110008[];
void SsUtSetReverbDelay(short a0) {
 int *p = D_80110008;  p[0] = 8; p[3] = a0; SpuSetReverbModeParam(p);
}
