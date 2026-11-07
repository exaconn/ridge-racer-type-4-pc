/* GCC_VERSION: 2.7.2 */
#include "psyq/libspu/s_crwa.h"
#include "psyq/libspu/sinit.h"
#include "psyq/libspu/sinit2.h"
#include "rr/library.h"
extern void SpuClearReverbWorkArea();
void func_8007FA54(void) {
    ResetCallback();
    SpuInit();
    SpuClearReverbWorkArea(7);
    func_8007F964();
}
