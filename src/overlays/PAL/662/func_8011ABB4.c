#include "rr/overlay662_draw_area.h"
u8 *ovl662_func_8011ABB4(u32 *ordering, u32 *pkt, u32 x, u32 y, u16 w, u16 h) {
    GpuRectangleFields rect;
    s16 py;
    rect.x = ovl662_D_801276A8.halfC8 + x;
    py = ovl662_D_801276A8.halfCA + y;
    if (DrawBuffers[0].environment.display5C.display.y == DrawBuffers[1].environment.display5C.display.y) {
        rect.y = py;
    } else {
        rect.y = py + ((u32)*(volatile s32 *)&DrawBufIndex << 8);
    }
    rect.w = w;
    rect.h = h;
    SetDrawArea(pkt, &rect);
    *pkt = (*pkt & 0xFF000000) | (*ordering & 0xFFFFFF);
    *ordering = (*ordering & 0xFF000000) | ((unsigned long)pkt & 0xFFFFFF);
    return (u8 *)(pkt + 3);
}
