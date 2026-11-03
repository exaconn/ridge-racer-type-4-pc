/* CC1_FLAGS: -mno-split-addresses */

#include "rr/spu.h"

u32 func_8008AF84(const u16 *a0, u32 a1) {
  if (a1 > 0x7EFF0) a1 = 0x7EFF0;
  func_8007F2E8(a0, a1);
  if (D_800A4810 == 0) D_800A480C = 0;
  return a1;
}
