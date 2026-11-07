/* GCC_VERSION: 2.7.2 */

#include "types.h"

extern int DMACallback();

void CdAltDataCallback(s32 arg0) {
    DMACallback(3, arg0);
}
