/* CC1_FLAGS: -mno-split-addresses */

struct S { char pad[0x1AA]; unsigned short f; };

extern struct S *_spu_RXX;

extern int _spu_rev_flag, _spu_rev_reserve_wa, _spu_rev_offsetaddr;

extern int _SpuIsInAllocateArea_();

int SpuSetReverb(int a0) {
    switch (a0) {
    case 0:
        _spu_rev_flag = 0;
        _spu_RXX->f &= 0xFF7F;
        break;
    case 1:
        if (_spu_rev_reserve_wa != a0 && _SpuIsInAllocateArea_(_spu_rev_offsetaddr)) {
            _spu_rev_flag = 0;
            _spu_RXX->f &= 0xFF7F;
        } else {
            _spu_rev_flag = a0;
            _spu_RXX->f |= 0x80;
        }
        break;
    }
    return _spu_rev_flag;
}
