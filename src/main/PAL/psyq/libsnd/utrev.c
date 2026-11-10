/* CC1_FLAGS: -mno-split-addresses */
#include "rr/spu.h"
#include "psyq/libspu/smutil.h"
#include "psyq/libspu/spuregs2.h"
#include "psyq/libsnd/utrev.h"
int SpuSetReverb(int a0) {
    switch (a0) {
    case 0:
        _spu_rev_flag = 0;
        _spu_RXX->spu_control &= 0xFF7F;
        break;
    case 1:
        if (_spu_rev_reserve_wa != a0 && _SpuIsInAllocateArea_(_spu_rev_offsetaddr)) {
            _spu_rev_flag = 0;
            _spu_RXX->spu_control &= 0xFF7F;
        } else {
            _spu_rev_flag = a0;
            _spu_RXX->spu_control |= 0x80;
        }
        break;
    }
    return _spu_rev_flag;
}
