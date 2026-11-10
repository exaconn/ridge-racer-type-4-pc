#include "rr/overlay662_action_loop.h"

void ovl662_dispatch_editor_actions(s32 mode)
{
    s32 i;
    s16 selected;
    GarageActionRecord16 *a;
    GarageButton *p4, *p9, *p11, *p12, *p14;

    if (mode) {
        for (i = 0; i < 53; i++) {
            a = &ovl662_editor_action_records[i];
            ovl662_action_lookup[a->key] = &a->prefix;
        }
        for (i = 0; i < 46; i++) {
            ovl662_button_rect_lookup[ovl662_editor_buttons[i].key0C] = &ovl662_editor_buttons[i].rectangle;
        }
    } else {
        if (!(ovl662_editor_cursor_state.words[0] & 2)) {
            if (!((ovl662_editor_cursor_state.words[0] & 4) &&
                  (s32)ovl662_editor_cursor_state.words[10] < 4096)) {
                if (ovl662_editor_cursor_state.words[0] & 8) {
                    ovl662_action_lookup[ovl662_editor_cursor_state.words[4]]->callback(0);
                } else {
                    for (i = 0; i < 53; i++) {
                        ovl662_action_lookup[i]->callback(0);
                        if (ovl662_editor_cursor_state.words[0] & 8) {
                            break;
                        }
                    }
                }
            }
        }
        switch (ovl662_editor_workspace.config1E) {
        case 0: selected = 9; break;
        case 1: selected = 11; break;
        case 2: selected = 12; break;
        case 3: selected = 14; break;
        case 4: selected = 4; break;
        }
        p4 = (GarageButton *)ovl662_button_rect_lookup[4];
        p9 = (GarageButton *)ovl662_button_rect_lookup[9];
        p11 = (GarageButton *)ovl662_button_rect_lookup[11];
        p12 = (GarageButton *)ovl662_button_rect_lookup[12];
        p14 = (GarageButton *)ovl662_button_rect_lookup[14];
        p4->timer0E = 0;
        p14->timer0E = 0;
        p12->timer0E = 0;
        p11->timer0E = 0;
        p9->timer0E = 0;
        ((GarageButton *)ovl662_button_rect_lookup[selected])->timer0E = -1;
        if (!ovl662_editor_workspace.flagsD1) {
            for (i = 0; i < 53; i++) {
                /* Recover the complete owner from its initial prefix member. */
                a = (GarageActionRecord16 *)ovl662_action_lookup[i];
                a->render(a->argument);
            }
            ovl662_draw_rgb_sliders();
        }
    }
}
