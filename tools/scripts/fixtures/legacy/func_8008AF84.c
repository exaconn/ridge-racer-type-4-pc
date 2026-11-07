/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
u32 SpuWrite(const u16 *a0, u32 a1) {
  if (a1 > 0x7EFF0) a1 = 0x7EFF0;
  _spu_Fw(a0, a1);
  if (_spu_transferCallback == 0) _spu_inTransfer = 0;
  return a1;
}
