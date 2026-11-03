#include "rr/menu_text_rows.h"

s32 func_800402F4(RrMenuTextRow60 *rows) {
    u32 size = (u32)D_800F4D30 * 60U;
    D_800FFC14 = rows;
    func_80096434(rows, size);
    return (s32)size;
}
