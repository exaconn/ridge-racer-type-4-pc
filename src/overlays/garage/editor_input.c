#include "game/system/pad.h"
#include "rr/overlay662_input.h"
#include "rr/overlay662_garage.h"
#include "rr/tables.h"
#include "rr/overlay662_garage_fade.h"
#include "rr/overlay662_distance.h"
#include "rr/overlay662_actions.h"
#include "rr/overlay662_cursor_control.h"
#include "rr/overlay662_action_loop.h"

extern void *PsyqMemset(void *, s32, u32);
s32 ovl662_map_controller_buttons(s32 mode);

void ovl662_read_analog_cursor_delta(GaragePoint *output)
{
    GaragePoint point;
    s32 step,delta;
    u32 axis;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[0].input00.fields.type == 0x73 && PlayerControllers[0].kind1C != 0 && PlayerControllers[0].flag40 == 0) {
        step=1;
        if (PlayerControllers[0].input00.fields.buttons & 8) step=4;
        axis=PlayerControllers[0].axes14.bytes[2];
        if (axis <112) {
            delta=(axis-112)*step;
            if (ovl662_analog_cursor_accum[0] <0) ovl662_analog_cursor_accum[0]+=delta;
            else ovl662_analog_cursor_accum[0]=delta;
            point.x=ovl662_analog_cursor_accum[0]/112;
            ovl662_analog_cursor_accum[0]-=point.x*112;
        } else if (axis>=145) {
            delta=(axis-143)*step;
            if (ovl662_analog_cursor_accum[0] >0) ovl662_analog_cursor_accum[0]+=delta;
            else ovl662_analog_cursor_accum[0]=delta;
            point.x=ovl662_analog_cursor_accum[0]/112;
            ovl662_analog_cursor_accum[0]-=point.x*112;
        } else ovl662_analog_cursor_accum[0]=0;
        axis=PlayerControllers[0].axes14.bytes[3];
        if (axis <112) {
            delta=(axis-112)*step;
            if (ovl662_analog_cursor_accum[1] <0) ovl662_analog_cursor_accum[1]+=delta;
            else ovl662_analog_cursor_accum[1]=delta;
            point.y=(s16)(ovl662_analog_cursor_accum[1]/112)*2;
            ovl662_analog_cursor_accum[1]-=point.y*112;
        } else if (axis>=145) {
            delta=(axis-143)*step;
            if (ovl662_analog_cursor_accum[1] >0) ovl662_analog_cursor_accum[1]+=delta;
            else ovl662_analog_cursor_accum[1]=delta;
            point.y=(s16)(ovl662_analog_cursor_accum[1]/112)*2;
            ovl662_analog_cursor_accum[1]-=point.y*112;
        } else ovl662_analog_cursor_accum[1]=0;
    }
    if ((u16)point.x | (u16)point.y) {output->x=point.x;output->y=point.y;}
}

void ovl662_read_mouse_cursor_delta(GaragePoint *output)
{
    GaragePoint point;
    s32 value;
    s32 y;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[1].input00.fields.type == 0x12) {
        value = (s32)(PlayerControllers[1].axes14.bytes[0] * 0x1000000U) >> 25;
        if (value < -3 || value > 3) value += value / 2;
        if (value) point.x=value;
        y = PlayerControllers[1].axes14.signedBytes[1];
        if (y < -7 || y > 7) y += (y + (s32)((u32)y >> 31)) >> 1;
        if (y) point.y=y;
    }
    if ((u16)point.x | (u16)point.y) {
        output->x=point.x;
        output->y=point.y;
    }
}

void ovl662_read_dpad_cursor_delta(GaragePoint *output)
{
    GaragePoint point;
    u32 buttons;
    s32 step;
    PsyqMemset(&point,0,4);
    if (PlayerControllers[0].input00.fields.type != 0x12) {
        buttons = PlayerControllers[0].input00.fields.buttons;
        step=1;
        if (buttons & 8) step=4;
        if (buttons & 0x1000) point.y=-(step*2);
        if (buttons & 0x4000) point.y=step*2;
        if (buttons & 0x8000) point.x=-step;
        if (buttons & 0x2000) point.x=step;
    }
    if ((u16)point.x | (u16)point.y) {
        output->x=point.x;
        output->y=point.y;
    }
}

void ovl662_poll_stick_direction_repeat(u32 *output)
{
    u32 result=0;
    s32 axis;
    if (PlayerControllers[0].input00.fields.type == 0x73 && PlayerControllers[0].kind1C != 0 && PlayerControllers[0].flag40 == 0) {
        axis=PlayerControllers[0].axes14.bytes[3];
        if (axis < 32) {
            ovl662_stick_repeat_up++;
            if ((s32)ovl662_stick_repeat_up >= 47) ovl662_stick_repeat_up=40;
            if (ovl662_stick_repeat_up == 1 || ovl662_stick_repeat_up == 40) result |= 1;
            *output=result;
        } else {
            ovl662_stick_repeat_up=0;
        }
        if (axis >= 225) {
            ovl662_stick_repeat_down++;
            if ((s32)ovl662_stick_repeat_down >= 47) ovl662_stick_repeat_down=40;
            if (ovl662_stick_repeat_down == 1 || ovl662_stick_repeat_down == 40) result |= 2;
            *output=result;
        } else {
            ovl662_stick_repeat_down=0;
        }
        axis=PlayerControllers[0].axes14.bytes[2];
        if (axis < 32) {
            ovl662_stick_repeat_left++;
            if ((s32)ovl662_stick_repeat_left >= 47) ovl662_stick_repeat_left=40;
            if (ovl662_stick_repeat_left == 1 || ovl662_stick_repeat_left == 40) result |= 4;
            *output=result;
        } else {
            ovl662_stick_repeat_left=0;
        }
        if (axis >= 225) {
            ovl662_stick_repeat_right++;
            if ((s32)ovl662_stick_repeat_right >= 47) ovl662_stick_repeat_right=40;
            if (ovl662_stick_repeat_right == 1 || ovl662_stick_repeat_right == 40) result |= 8;
            *output=result;
        } else {
            ovl662_stick_repeat_right=0;
        }
    }
}

void ovl662_poll_dpad_direction_repeat(u32 *output)
{
    u32 result=0;
    if (PlayerControllers[0].input00.fields.type != 0x12) {
        if (PlayerControllers[0].input00.fields.buttons & 0x1000) {
            ovl662_dpad_repeat_up++;
            if ((s32)ovl662_dpad_repeat_up >= 47) ovl662_dpad_repeat_up=40;
            if (ovl662_dpad_repeat_up == 1 || ovl662_dpad_repeat_up == 40) result |= 1;
            *output=result;
        } else {
            ovl662_dpad_repeat_up=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x4000) {
            ovl662_dpad_repeat_down++;
            if ((s32)ovl662_dpad_repeat_down >= 47) ovl662_dpad_repeat_down=40;
            if (ovl662_dpad_repeat_down == 1 || ovl662_dpad_repeat_down == 40) result |= 2;
            *output=result;
        } else {
            ovl662_dpad_repeat_down=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x8000) {
            ovl662_dpad_repeat_left++;
            if ((s32)ovl662_dpad_repeat_left >= 47) ovl662_dpad_repeat_left=40;
            if (ovl662_dpad_repeat_left == 1 || ovl662_dpad_repeat_left == 40) result |= 4;
            *output=result;
        } else {
            ovl662_dpad_repeat_left=0;
        }
        if (PlayerControllers[0].input00.fields.buttons & 0x2000) {
            ovl662_dpad_repeat_right++;
            if ((s32)ovl662_dpad_repeat_right >= 47) ovl662_dpad_repeat_right=40;
            if (ovl662_dpad_repeat_right == 1 || ovl662_dpad_repeat_right == 40) result |= 8;
            *output=result;
        } else {
            ovl662_dpad_repeat_right=0;
        }
    }
}

void ovl662_step_panel_fade_timers(void)
{
    if ((ovl662_editor_cursor_state.words[0] & 2) ||
        (PlayerControllers[0].input00.fields.type != 0x12 && (PlayerControllers[0].input00.fields.buttons & 4))) {
        ovl662_editor_cursor_state.words[10] += ovl662_editor_cursor_state.words[11];
        if ((s32)ovl662_editor_cursor_state.words[10] > 4096) {
            do {
                do {
                    ovl662_editor_cursor_state.words[10] = 4096;
                } while (0);
            } while (0);
        }
    } else {
        if (ovl662_editor_cursor_state.words[10] == 4096) {
            PlaySfx(2);
        }
        ovl662_editor_cursor_state.words[10] -= ovl662_editor_cursor_state.words[11];
        if ((s32)ovl662_editor_cursor_state.words[10] < 0) {
            ovl662_editor_cursor_state.words[10] = 0;
            ovl662_editor_cursor_state.words[1] = 48;
            ovl662_editor_cursor_state.words[0] &= ~4;
        }
    }
    RR_GARAGE_GTE_LERP(ovl662_editor_cursor_state.words[10],
                       &ovl662_editor_cursor_state.words[8],
                       &ovl662_editor_cursor_state.words[5]);
}

s32 ovl662_calc_point_distance(const GaragePoint *a, const GaragePoint *b) {
    u32 dx = (b->x - a->x) * 2U;
    u32 dy = (b->y - a->y);

    return 4096 / (((CordicSqrt(((dx * dx) + (dy * dy))) >> 6) / 64) + 1);
}

GaragePoint ovl662_get_rect_center(s32 index)
{
    GaragePoint point;
    GarageRectangle *rectangle = ovl662_button_rect_lookup[index];
    u16 width = (*rectangle)[2];
    s32 x = (s16)(*rectangle)[0] + ((s16)width >> 1);
    s32 y = (s16)(*rectangle)[1] + ((s16)(*rectangle)[3] >> 1);
    point.x = x;
    point.y = y;
    return point;
}

void ovl662_handle_shortcut_buttons(u32 flags){
 if(flags)PlaySfx(0);
 if(flags&1)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(1);
 else if(flags&2)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(2);
 if(flags&4)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(4);
 else if(flags&8)ovl662_action_lookup[ovl662_editor_workspace.current60]->callback(8);
}

s32 ovl662_map_controller_buttons(s32 mode)
{
    if(mode){
        ovl662_editor_cursor_state.words[2]++;
        if(ovl662_editor_cursor_state.words[2]>=47)ovl662_editor_cursor_state.words[2]=40;
        if(ovl662_editor_cursor_state.words[2]==1 || ovl662_editor_cursor_state.words[2]==40)return 1;return 0;
    }else{
        ovl662_editor_cursor_state.words[3]++;
        if(ovl662_editor_cursor_state.words[3]>=47)ovl662_editor_cursor_state.words[3]=40;
        if(ovl662_editor_cursor_state.words[3]==1 || ovl662_editor_cursor_state.words[3]==40)return 1;return 0;
    }
}

/* Repeat-key handling for the two garage scroll directions. */
void ovl662_update_button_states(void)
{
    s32 first;
    s32 second;

    if (PlayerControllers[0].input00.fields.type == 0x23) {
        first = PlayerControllers[0].input00.fields.buttons & 0x840;
        second = PlayerControllers[0].input00.fields.buttons & 0x80;
    } else if (PlayerControllers[0].input00.fields.type != 0x12) {
        first = PlayerControllers[0].input00.fields.buttons & 0x820;
        second = PlayerControllers[0].input00.fields.buttons & 0x40;
    }
    if (PlayerControllers[1].input00.fields.type == 0x12) {
        first |= PlayerControllers[1].input00.fields.buttons & 8;
        second |= PlayerControllers[1].input00.fields.buttons & 4;
    }

    if (ovl662_editor_cursor_state.words[1] & 0x10) {
        if (first) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x10) & ~5;
            if (ovl662_map_controller_buttons(1)) {
                ovl662_editor_cursor_state.words[1] |= 0x40;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x40;
            }
        } else {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] & ~0x51) | 4;
        }
    } else {
        if (first) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x11) & ~4;
            ovl662_editor_cursor_state.words[2] = 0;
            if (ovl662_map_controller_buttons(1)) {
                ovl662_editor_cursor_state.words[1] |= 0x40;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x40;
            }
        } else {
            ovl662_editor_cursor_state.words[1] &= ~0x55;
        }
    }

    if (ovl662_editor_cursor_state.words[1] & 0x20) {
        if (second) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x20) & ~0xA;
            if (ovl662_map_controller_buttons(0)) {
                ovl662_editor_cursor_state.words[1] |= 0x80;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x80;
            }
        } else {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] & ~0xA2) | 8;
        }
    } else {
        if (second) {
            ovl662_editor_cursor_state.words[1] = (ovl662_editor_cursor_state.words[1] | 0x22) & ~8;
            ovl662_editor_cursor_state.words[3] = 0;
            if (ovl662_map_controller_buttons(0)) {
                ovl662_editor_cursor_state.words[1] |= 0x80;
            } else {
                ovl662_editor_cursor_state.words[1] &= ~0x80;
            }
        } else {
            ovl662_editor_cursor_state.words[1] &= ~0xAA;
        }
    }
}

void ovl662_update_editor_cursor(void) {
    GaragePoint movement;
    GaragePoint destination;
    u32 events;
    s32 flags;
    u16 ySum;
    u16 xSum;

    PsyqMemset(&movement, 0, 4);
    events = 0;
    if (!(ovl662_editor_cursor_state.words[0] & 1)) {
        if (ovl662_editor_cursor_state.words[0] & 2) {
            ovl662_step_panel_fade_timers();
            if ((s32)ovl662_editor_cursor_state.points.word28 < 0x1000) {
                return;
            }
            ovl662_editor_cursor_state.points.cursor18 = ovl662_editor_cursor_state.points.cursor14;
            ovl662_editor_cursor_state.words[0] &= ~2U;
            ovl662_editor_cursor_state.words[0] &= ~4U;
        }
        if (ovl662_editor_cursor_state.words[0] & 4) {
            if (!(ovl662_editor_cursor_state.words[0] & 8)) {
                ovl662_step_panel_fade_timers();
                if ((s32)ovl662_editor_cursor_state.points.word28 < 0x1000) {
                    return;
                }
                ovl662_poll_stick_direction_repeat(&events);
                ovl662_poll_dpad_direction_repeat(&events);
                ovl662_handle_shortcut_buttons(events);
            }
        } else {
            ovl662_read_analog_cursor_delta(&movement);
            ovl662_read_mouse_cursor_delta(&movement);
            ovl662_read_dpad_cursor_delta(&movement);
            xSum = ovl662_editor_cursor_state.points.cursor18.x + movement.x;
            ySum = ovl662_editor_cursor_state.points.cursor18.y + movement.y;
            ovl662_editor_cursor_state.points.cursor18.x = xSum;
            ovl662_editor_cursor_state.points.cursor18.y = ySum;
            if ((s16) xSum < ovl662_editor_cursor_state.halves[24]) {
                ovl662_editor_cursor_state.points.cursor18.x = (u16) ovl662_editor_cursor_state.halves[24];
            }
            if (((ovl662_editor_cursor_state.halves[24] + ovl662_editor_cursor_state.halves[26]) - 1) < (s16) ovl662_editor_cursor_state.points.cursor18.x) {
                ovl662_editor_cursor_state.points.cursor18.x = (u16) (((u16) ovl662_editor_cursor_state.halves[24] + (u16) ovl662_editor_cursor_state.halves[26]) - 1);
            }
            if ((s16) ySum < ovl662_editor_cursor_state.halves[25]) {
                ovl662_editor_cursor_state.points.cursor18.y = (u16) ovl662_editor_cursor_state.halves[25];
            }
            if (((ovl662_editor_cursor_state.halves[25] + ovl662_editor_cursor_state.halves[27]) - 1) < (s16) ovl662_editor_cursor_state.points.cursor18.y) {
                ovl662_editor_cursor_state.points.cursor18.y = (u16) (((u16) ovl662_editor_cursor_state.halves[25] + (u16) ovl662_editor_cursor_state.halves[27]) - 1);
            }
            ovl662_editor_cursor_state.points.cursor14 = ovl662_editor_cursor_state.points.cursor18;
            flags = ovl662_editor_cursor_state.words[0];
            if (!(flags & 8) && (PlayerControllers[0].input00.fields.type != 0x12) && (PlayerControllers[0].edges08.fields.pressed & 4)) {
                ovl662_editor_cursor_state.words[0] = (flags | 4);
                ovl662_editor_cursor_state.words[1] = 0x30;
                ovl662_editor_cursor_state.points.first20 = ovl662_editor_cursor_state.points.cursor18;
                destination=ovl662_get_rect_center(ovl662_editor_workspace.saved64[0]);
                ovl662_editor_cursor_state.points.second24 = destination;
                ovl662_editor_cursor_state.points.word28 = 0;
                ovl662_editor_cursor_state.points.distance2C = ovl662_calc_point_distance(&ovl662_editor_cursor_state.points.first20,&ovl662_editor_cursor_state.points.second24);
                ovl662_editor_workspace.current60 = ovl662_editor_workspace.saved64[0];
                ovl662_editor_workspace.selectorsB8[0] = ovl662_editor_workspace.saved64[1];
                ovl662_editor_workspace.selectorsB8[1] = ovl662_editor_workspace.saved64[2];
                ovl662_editor_workspace.selectorsB8[2] = ovl662_editor_workspace.saved64[3];
                ovl662_editor_workspace.selectorsB8[3] = ovl662_editor_workspace.saved64[4];
                ovl662_action_lookup[((GarageButton *)ovl662_button_rect_lookup[ovl662_editor_workspace.saved64[0]])->action0A]->callback(-1);
                PlaySfx(3);
                return;
            }
        }
        ovl662_update_button_states();
    }
}

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
