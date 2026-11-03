/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libspu/smutil.h"
#include "psyq/libspu/spuregs2.h"
#include "psyq/libspu/s_crwa.h"
#include "psyq/libspu/s_crwa.h"
#include "rr/spu.h"

extern s32 func_8007F068(s32 mode, ...);
extern s32 func_80096E94(s32 event);
extern s32 D_800A4800;
extern u32 D_800A48C8[];
extern u8 D_800A4938[];

s32 func_8007FAB4(u32 preset);

void func_8007FA94(void) { func_8007F604(0); }

s32 func_8007FAB4(u32 preset) {
    volatile s32 saved_callback = 0;
    u32 *preset_address;
    unsigned long base;
    u32 remaining, destination;
    s32 saved_mode, more;
    s32 restore_mode = 0;

    if (preset >= 10) return -1;
    base = (unsigned long)D_800A48C8;
    preset_address = (u32 *)(preset * 4 + base);
    if (_SpuIsInAllocateArea_(*preset_address) != 0) return -1;

    /* The data initializer and func_8007EAC4 both set the shift to 3. */
    if (preset == 0) {
        remaining = 0x10U << D_800A4800;
        destination = 0xFFF0U << D_800A4800;
    } else {
        u32 address = *preset_address;
        remaining = (0x10000U - address) << D_800A4800;
        destination = address << D_800A4800;
    }
    saved_mode = D_800A47F4;
    if (saved_mode == 1) {
        D_800A47F4 = 0;
        /* ASM_BARRIER: reload 1 instead of reusing the comparison's v0.
         * This emits no instructions and has no effect on the algorithm. */
        __asm__ volatile("" : : : "$2");
        restore_mode = 1;
    }
    more = 1;
    if (D_800A4810 != 0) {
        saved_callback = D_800A4810;
        D_800A4810 = 0;
    }
    do {
        preset = 0x400;
        if (remaining <= 0x400U) {
            preset = remaining;
            more = 0;
        }
        func_8007F068(2, destination);
        func_8007F068(1);
        func_8007F068(3, D_800A4938, preset);
        func_80096E94(D_800A4840);
        remaining -= 0x400;
        destination += 0x400;
    } while (more != 0);
    if (restore_mode != 0) D_800A47F4 = saved_mode;
    if (saved_callback != 0) D_800A4810 = saved_callback;
    return 0;
}
