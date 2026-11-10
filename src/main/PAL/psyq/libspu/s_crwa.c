/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libspu/smutil.h"
#include "psyq/libspu/spuregs2.h"
#include "psyq/libspu/s_crwa.h"
#include "rr/spu.h"

extern s32 _spu_t(s32 mode, ...);
extern s32 WaitEvent(s32 event);
extern u8 SpuZeroBlock[];

void SpuInit(void) { _SpuInit(0); }

s32 SpuClearReverbWorkArea(u32 preset) {
    volatile s32 saved_callback = 0;
    u32 *preset_address;
    unsigned long base;
    u32 remaining, destination;
    s32 saved_mode, more;
    s32 restore_mode = 0;

    if (preset >= 10) return -1;
    base = (unsigned long)_spu_rev_startaddr;
    preset_address = (u32 *)(preset * 4 + base);
    if (_SpuIsInAllocateArea_(*preset_address) != 0) return -1;

    /* The data initializer and _spu_init both set the shift to 3. */
    if (preset == 0) {
        remaining = 0x10U << _spu_mem_mode_plus;
        destination = 0xFFF0U << _spu_mem_mode_plus;
    } else {
        u32 address = *preset_address;
        remaining = (0x10000U - address) << _spu_mem_mode_plus;
        destination = address << _spu_mem_mode_plus;
    }
    saved_mode = _spu_transMode;
    if (saved_mode == 1) {
        _spu_transMode = 0;
        /* ASM_BARRIER: reload 1 instead of reusing the comparison's v0.
         * This emits no instructions and has no effect on the algorithm. */
        restore_mode = 1;
    }
    more = 1;
    if (_spu_transferCallback != 0) {
        saved_callback = _spu_transferCallback;
        _spu_transferCallback = 0;
    }
    do {
        preset = 0x400;
        if (remaining <= 0x400U) {
            preset = remaining;
            more = 0;
        }
        _spu_t(2, destination);
        _spu_t(1);
        _spu_t(3, SpuZeroBlock, preset);
        WaitEvent(_spu_EVdma);
        remaining -= 0x400;
        destination += 0x400;
    } while (more != 0);
    if (restore_mode != 0) _spu_transMode = saved_mode;
    if (saved_callback != 0) _spu_transferCallback = saved_callback;
    return 0;
}
