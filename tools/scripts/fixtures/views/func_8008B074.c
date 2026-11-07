/* GCC_VERSION: 2.7.2 */

#include "types.h"

extern int SpuIsTransferCompleted();

short SsVabTransCompleted(short a0) { return SpuIsTransferCompleted(a0); }
