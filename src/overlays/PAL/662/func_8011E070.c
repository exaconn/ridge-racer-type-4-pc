#include "rr/overlay662_cursor_control.h"

void ovl662_func_8011E070(void) {
    RrGaragePoint movement;
    RrGaragePoint destination;
    u32 events;
    s32 flags;
    u16 ySum;
    u16 xSum;

    func_80096454(&movement, 0, 4);
    events = 0;
    if (!(ovl662_D_80127668.words[0] & 1)) {
        if (ovl662_D_80127668.words[0] & 2) {
            ovl662_func_8011DA90();
            if ((s32)ovl662_D_80127668.points.word28 >= 0x1000) {
                ovl662_D_80127668.points.cursor18 = ovl662_D_80127668.points.cursor14;
                ovl662_D_80127668.words[0] &= ~2U;
                ovl662_D_80127668.words[0] &= ~4U;
                goto processInput;
            }
        } else {
processInput:
            if (ovl662_D_80127668.words[0] & 4) {
                if (!(ovl662_D_80127668.words[0] & 8)) {
                    ovl662_func_8011DA90();
                    if ((s32)ovl662_D_80127668.points.word28 >= 0x1000) {
                        ovl662_func_8011D730(&events);
                        ovl662_func_8011D8E4(&events);
                        ovl662_func_8011DCDC(events);
                        goto finishInput;
                    }
                } else {
                    goto finishInput;
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
                if (!(flags & 8) && (D_800F3A38[0].type01 != 0x12) && (D_800F3A38[0].buttons08 & 4)) {
                    ovl662_D_80127668.words[0] = (s32) (flags | 4);
                    ovl662_D_80127668.words[1] = 0x30;
                    ovl662_D_80127668.points.first20 = ovl662_D_80127668.points.cursor18;
                    destination=ovl662_func_8011DC74(ovl662_D_801276A8.saved64[0]);
                    ovl662_D_80127668.points.second24 = destination;
                    ovl662_D_80127668.points.word28 = 0;
                    ovl662_D_80127668.points.distance2C = ovl662_func_8011DBD8(&ovl662_D_80127668.points.first20,&ovl662_D_80127668.points.second24);
                    ovl662_D_801276A8.current60 = (s32) ovl662_D_801276A8.saved64[0];
                    ovl662_D_801276A8.selectorsB8[0] = (s32) ovl662_D_801276A8.saved64[1];
                    ovl662_D_801276A8.selectorsB8[1] = (s32) ovl662_D_801276A8.saved64[2];
                    ovl662_D_801276A8.selectorsB8[2] = (s32) ovl662_D_801276A8.saved64[3];
                    ovl662_D_801276A8.selectorsB8[3] = (s32) ovl662_D_801276A8.saved64[4];
                    ovl662_D_80127470[((RrGarageButton *)ovl662_D_801273B8[ovl662_D_801276A8.saved64[0]])->action0A]->callback(-1);
                    func_8004F24C(3);
                    return;
                }
finishInput:
                ovl662_func_8011DE70();
            }
        }
    }
}
