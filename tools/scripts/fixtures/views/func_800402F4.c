#include "rr/menu_text_rows.h"

s32 InitMenuTextRows(RrMenuTextRow60 *rows) {
    u32 size = (u32)MenuTextRowCount * 60U;
    MenuTextRows = rows;
    func_80096434(rows, size);
    return (s32)size;
}
