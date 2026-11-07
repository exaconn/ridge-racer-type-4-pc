/* GCC_VERSION: 2.7.2 */
#include "rr/cd.h"
s32 CdSync(s32 mode,u8 *status) {
    return CD_sync(mode,status);
}
