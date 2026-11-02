#include "rr/menu_text_rows.h"

s32 func_800402F4(RrMenuTextRow60 *a) {
    u32 sz = (u32)D_800F4D30 * 60U;
    D_800FFC14 = a;
    func_80096434(a, sz);
    return (s32)sz;
}
