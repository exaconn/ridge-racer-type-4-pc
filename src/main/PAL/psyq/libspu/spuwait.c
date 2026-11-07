/* CC1_FLAGS: -mno-split-addresses */
#include "psyq/libspu/spuwait.h"
#include "psyq/libspu/s_crwa.h"
#include "psyq/libspu/spuregs2.h"
#include "rr/bios_events.h"
extern int _spu_inTransfer;
int SpuIsTransferCompleted(int a0) {
  int r;
  if (_spu_trans_mode == 1 || _spu_inTransfer == 1) return 1;
  r = TestEvent(_spu_EVdma);
  if (a0 == 1) {
    if (r == 0) {
      while (TestEvent(_spu_EVdma) == 0) ;
    }
    r = 1;
    _spu_inTransfer = r;
  } else if (r == 1) {
    _spu_inTransfer = r;
  }
  return r;
}
