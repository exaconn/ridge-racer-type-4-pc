#include "rr/spu.h"

s32 SsVabTransBody(const u16 *source, s32 raw) {
    if ((u16)raw <= 16) {
        s32 bank = (s16)raw;
        if (_svm_vab_used[bank] == 2) {
            u32 allocation = _svm_vab_start[bank];
            SpuSetTransferMode(0);
            if (SpuSetTransferStartAddr(allocation)) {
                SpuWrite(source, _svm_vab_total[bank]);
                _svm_vab_used[bank] = 1;
                return bank;
            }
        }
    }
    _spu_setInTransfer(0);
    return -1;
}

u32 SpuWrite(const u16 *source, u32 bytes) {
    if (bytes > 0x7EFF0) bytes = 0x7EFF0;
    _spu_Fw(source, bytes);
    if (_spu_transferCallback == 0) _spu_inTransfer = 0;
    return bytes;
}
