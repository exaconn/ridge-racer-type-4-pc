/* GCC_VERSION: 2.7.2 */
#include "psyq/libsnd/utrev3.h"
#include "psyq/libsnd/utrev.h"

void SsUtReverbOff() { SpuSetReverb(0); }

void SsUtReverbOn() { SpuSetReverb(1); }
