#include "rr/spu.h"
#include "rr/spu_init.h"

extern char D_800116B8[];
extern char StrSpuDmaClearWaitMsg[];

void _spu_FwriteByIO(const u16 *source, u32 size) {
    u32 remaining = size;
    const u16 *p;
    u16 previous;
    s32 chunk;
    u32 count;
    u16 ctrl;

    previous = _spu_RXX->spu_status & 0x7FF;
    p = source;
    _spu_RXX->transfer_address = _spu_tsa;
    _spu_Fw1ts();
    while (remaining) {
        s32 written;
        if (remaining < 65) {
            chunk = remaining;
        } else {
            chunk = 64;
        }
        for (written = 0; written < chunk; written += 2) {
            _spu_RXX->transfer_data = *p++;
        }
        _spu_RXX->spu_control = ((ctrl = _spu_RXX->spu_control) & 0xFFCF) | 0x10;
        _spu_Fw1ts();
        count = 0;
        while (_spu_RXX->spu_status & 0x400) {
            if (++count > 0xF00) {
                DebugPrintStub(SpuTimeoutFmt, D_800116B8);
                break;
            }
        }
        _spu_Fw1ts();
        remaining -= chunk;
        _spu_Fw1ts();
    }
    _spu_RXX->spu_control = (ctrl = _spu_RXX->spu_control) & 0xFFCF;
    count = 0;
    while ((_spu_RXX->spu_status & 0x7FF) != previous) {
        if (++count > 0xF00) {
            DebugPrintStub(SpuTimeoutFmt, StrSpuDmaClearWaitMsg);
            break;
        }
    }
}
