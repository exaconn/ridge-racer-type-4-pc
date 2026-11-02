#include "types.h"
/* Original overlay data, separate from resident globals and other RAM overlays. */
u32 ovl659_module_id __attribute__((section(".ovl659.header"))) = 10;
typedef struct {
    u8 thresholds[4];
    s32 commands[34];
    s32 countdown;
} RrOvl659Data;
RrOvl659Data ovl659_data __attribute__((section(".ovl659.data"))) = {
    {2, 1, 0, 0},
    {
        0x1e15, 0x0, 0x111a, 0x3, 0x4, -1,
        0x10d, 0x20d, 0x30d, 0x14, 0x40e, 0x1e15,
        0x0, 0x111a, 0x12, -1, 0x10d, 0x20d,
        0x30d, 0x14, 0x40e, 0x1e15, 0x5, 0x1e15,
        0x11, 0x1e15, -1, 0x6, 0x18, 0x14,
        0x100b, -1, 0x1e15, -1,
    },
    0
};
