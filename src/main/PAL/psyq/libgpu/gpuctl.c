#include "types.h"
#include "m2c_macros.h"
#include "psyq/libgpu/gpuctl.h"
#include "rr/bios_devices.h"

void _card_clear(s32 arg0) {
    _new_card();
    _card_write(arg0, 0x3F, 0);
}
