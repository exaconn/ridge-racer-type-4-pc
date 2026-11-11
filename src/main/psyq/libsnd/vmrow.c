#include "rr/channels.h"
#include "psyq/libsnd/vmrow.h"
#include "game/data/globals.h"

void vmNoiseOff(u8 index) {
    _svm_voice[index].flags1D = 0;
    _svm_voice[index].value00 = 0;
    _svm_voice[index].value04 = 0;
}

void _SsVmKeyOffNow(s32 unused_mode) {
    s32 selected = (u16)_svm_cur.f18;
    u32 index = selected & 0xFFFF;
    u32 high_mask, low_mask;
    u32 low, high;
    if (index < 16) {
        low_mask = 1U << index;
        high_mask = 0;
    } else {
        low_mask = 0;
        index -= 16;
        high_mask = 1U << index;
        index = selected & 0xFFFF;
    }
    _svm_voice[index].flags1D = 0;
    low = _svm_okof1;
    high = _svm_okof2;
    _svm_voice[index].value04 = 0;
    _svm_voice[index].value00 = 0;
    low |= low_mask;
    _svm_okof1 = low;
    _svm_okon1 &= ~low;
    high |= high_mask;
    _svm_okof2 = high;
    _svm_okon2 &= ~high;
}
