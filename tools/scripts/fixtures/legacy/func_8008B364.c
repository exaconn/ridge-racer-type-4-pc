/* GCC_VERSION: 2.7.2 */
#include "rr/library.h"

void ResetCallback(void) {
    IntrDriverTablePtr->reset();
}
