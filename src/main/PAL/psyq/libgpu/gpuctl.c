#include "types.h"
#include "m2c_macros.h"
#include "psyq/libgpu/gpuctl.h"
#include "rr/bios_devices.h"

void _card_clear(s32 arg0) {
    func_80095CF4();
    func_80095CE4(arg0, 0x3F, 0);
}
