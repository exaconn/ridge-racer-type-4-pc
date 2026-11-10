#include "game/system/pad.h"
#include "rr/overlay662_cursor_control.h"

void ovl662_func_8011E070(void) {
    GaragePoint movement;
    GaragePoint destination;
    u32 events;
    s32 flags;
    u16 ySum;
    u16 xSum;

    PsyqMemset(&movement, 0, 4);
    events = 0;
    if (!(ovl662_D_80127668.words[0] & 1)) {
        if (ovl662_D_80127668.words[0] & 2) {
            ovl662_func_8011DA90();
            if ((s32)ovl662_D_80127668.points.word28 < 0x1000) {
                return;
            }
            ovl662_D_80127668.points.cursor18 = ovl662_D_80127668.points.cursor14;
            ovl662_D_80127668.words[0] &= ~2U;
            ovl662_D_80127668.words[0] &= ~4U;
        }
        if (ovl662_D_80127668.words[0] & 4) {
            if (!(ovl662_D_80127668.words[0] & 8)) {
                ovl662_func_8011DA90();
                if ((s32)ovl662_D_80127668.points.word28 < 0x1000) {
                    return;
                }
                ovl662_func_8011D730(&events);
                ovl662_func_8011D8E4(&events);
                ovl662_func_8011DCDC(events);
            }
        } else {
            ovl662_func_8011D338(&movement);
            ovl662_func_8011D58C(&movement);
            ovl662_func_8011D664(&movement);
            xSum = ovl662_D_80127668.points.cursor18.x + movement.x;
            ySum = ovl662_D_80127668.points.cursor18.y + movement.y;
            ovl662_D_80127668.points.cursor18.x = xSum;
            ovl662_D_80127668.points.cursor18.y = ySum;
            if ((s16) xSum < ovl662_D_80127668.halves[24]) {
                ovl662_D_80127668.points.cursor18.x = (u16) ovl662_D_80127668.halves[24];
            }
            if (((ovl662_D_80127668.halves[24] + ovl662_D_80127668.halves[26]) - 1) < (s16) ovl662_D_80127668.points.cursor18.x) {
                ovl662_D_80127668.points.cursor18.x = (u16) (((u16) ovl662_D_80127668.halves[24] + (u16) ovl662_D_80127668.halves[26]) - 1);
            }
            if ((s16) ySum < ovl662_D_80127668.halves[25]) {
                ovl662_D_80127668.points.cursor18.y = (u16) ovl662_D_80127668.halves[25];
            }
            if (((ovl662_D_80127668.halves[25] + ovl662_D_80127668.halves[27]) - 1) < (s16) ovl662_D_80127668.points.cursor18.y) {
                ovl662_D_80127668.points.cursor18.y = (u16) (((u16) ovl662_D_80127668.halves[25] + (u16) ovl662_D_80127668.halves[27]) - 1);
            }
            ovl662_D_80127668.points.cursor14 = ovl662_D_80127668.points.cursor18;
            flags = ovl662_D_80127668.words[0];
            if (!(flags & 8) && (PlayerControllers[0].input00.fields.type != 0x12) && (PlayerControllers[0].edges08.fields.pressed & 4)) {
                ovl662_D_80127668.words[0] = (flags | 4);
                ovl662_D_80127668.words[1] = 0x30;
                ovl662_D_80127668.points.first20 = ovl662_D_80127668.points.cursor18;
                destination=ovl662_func_8011DC74(ovl662_D_801276A8.saved64[0]);
                ovl662_D_80127668.points.second24 = destination;
                ovl662_D_80127668.points.word28 = 0;
                ovl662_D_80127668.points.distance2C = ovl662_func_8011DBD8(&ovl662_D_80127668.points.first20,&ovl662_D_80127668.points.second24);
                ovl662_D_801276A8.current60 = ovl662_D_801276A8.saved64[0];
                ovl662_D_801276A8.selectorsB8[0] = ovl662_D_801276A8.saved64[1];
                ovl662_D_801276A8.selectorsB8[1] = ovl662_D_801276A8.saved64[2];
                ovl662_D_801276A8.selectorsB8[2] = ovl662_D_801276A8.saved64[3];
                ovl662_D_801276A8.selectorsB8[3] = ovl662_D_801276A8.saved64[4];
                ovl662_D_80127470[((GarageButton *)ovl662_D_801273B8[ovl662_D_801276A8.saved64[0]])->action0A]->callback(-1);
                PlaySfx(3);
                return;
            }
        }
        ovl662_func_8011DE70();
    }
}
