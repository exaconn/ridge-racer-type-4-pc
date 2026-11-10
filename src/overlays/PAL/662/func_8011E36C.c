#include "rr/overlay662_action_loop.h"

void ovl662_func_8011E36C(s32 mode)
{
    s32 i;
    s16 selected;
    GarageActionRecord16 *a;
    GarageButton *p4, *p9, *p11, *p12, *p14;

    if (mode) {
        for (i = 0; i < 53; i++) {
            a = &ovl662_D_80126C38[i];
            ovl662_D_80127470[a->key] = &a->prefix;
        }
        for (i = 0; i < 46; i++) {
            ovl662_D_801273B8[ovl662_D_80126F88[i].key0C] = &ovl662_D_80126F88[i].rectangle;
        }
    } else {
        if (!(ovl662_D_80127668.words[0] & 2)) {
            if (!((ovl662_D_80127668.words[0] & 4) &&
                  (s32)ovl662_D_80127668.words[10] < 4096)) {
                if (ovl662_D_80127668.words[0] & 8) {
                    ovl662_D_80127470[ovl662_D_80127668.words[4]]->callback(0);
                } else {
                    for (i = 0; i < 53; i++) {
                        ovl662_D_80127470[i]->callback(0);
                        if (ovl662_D_80127668.words[0] & 8) {
                            break;
                        }
                    }
                }
            }
        }
        switch (ovl662_D_801276A8.config1E) {
        case 0: selected = 9; break;
        case 1: selected = 11; break;
        case 2: selected = 12; break;
        case 3: selected = 14; break;
        case 4: selected = 4; break;
        }
        p4 = (GarageButton *)ovl662_D_801273B8[4];
        p9 = (GarageButton *)ovl662_D_801273B8[9];
        p11 = (GarageButton *)ovl662_D_801273B8[11];
        p12 = (GarageButton *)ovl662_D_801273B8[12];
        p14 = (GarageButton *)ovl662_D_801273B8[14];
        p4->timer0E = 0;
        p14->timer0E = 0;
        p12->timer0E = 0;
        p11->timer0E = 0;
        p9->timer0E = 0;
        ((GarageButton *)ovl662_D_801273B8[selected])->timer0E = -1;
        if (!ovl662_D_801276A8.flagsD1) {
            for (i = 0; i < 53; i++) {
                /* Recover the complete owner from its initial prefix member. */
                a = (GarageActionRecord16 *)ovl662_D_80127470[i];
                a->render(a->argument);
            }
            ovl662_func_8011BF04();
        }
    }
}
