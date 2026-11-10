#include "types.h"
#include "rr/runtime_settings.h"
#include "game/system/pad.h"
#include "rr/race_setup.h"
#include "rr/audio_cd_controller.h"
#include "game/menu/mnpulse.h"
#include "game/system/pad3.h"

#ifndef NULL
#define NULL 0
#endif

extern s32 D_800FFC08[2];
extern s32 D_800AD590[2];
extern s32 D_800F4CD8[2];
extern s16 D_8009FFB8[4];
extern s16 PadRangeLimits[6];
extern s32 WheelDeadzoneTable[8];
extern s32 PadDeadzoneTable[8];
extern s16 D_800F3824;
int abs(int);
extern char StrControllerNoErrorFmt[];
extern char StrActuatorNoErrorFmt[];

void PadUpdate(void) {
    PadPort28 *ports[2];
    ControllerState5C *states[2];
    s32 i;
    s32 v;
    s16 val;

    /* ASM_BARRIER: keep prologue register saves before ports array initialization */
    __asm__ ("" ::: "memory");
    ports[0] = &PadPorts[0];
    ports[1] = &PadPorts[1];
    __asm__ ("" ::: "memory"); /* ASM_BARRIER: schedule ports array initialization before loop preheader */
    states[0] = &PlayerControllers[0];
    states[1] = &PlayerControllers[1];

    for (i = 0; i < 2; i++) {
        if (ports[i]->type01 != states[i]->input00.fields.type) {
            states[i]->changed1E = 1;
            states[i]->protocol10 = -1;
        }
        states[i]->input00.fields.status = ports[i]->status00;
        states[i]->input00.fields.type = ports[i]->type01;
        if (ports[i]->status00 != 0) {
            D_800FFC08[i] = 1;
            D_800F4CD8[i] = 0x22;
            D_800AD590[i] |= 0x10;
        } else if (D_800F4CD8 != NULL) {
            D_800F4CD8[i]--;
            if (ports[i]->type01 == 0x23) {
                u16 btn = ~((ports[i]->data02 << 8) | ports[i]->data03);
                if ((btn & 0x5000) == 0x5000 || (btn & 0xA000) == 0xA000 || (btn & 0x1C4)) {
                    D_800FFC08[i] = 2;
                    D_800F4CD8[i] = 0x22;
                    D_800AD590[i] |= 0x10;
                }
            }
        }
        D_800AD590[i] >>= 1;
        if (D_800AD590[i] != 0) {
            PadPort28 *port = ports[i];
            ControllerState5C *pad = states[i];
            port->type01 = 0;
            pad->input00.fields.type = 0;
        } else {
            D_800FFC08[i] = 0;
        }
        if (states[i]->input00.fields.type != 0 && states[i]->protocol10 < 0) {
            if (states[i]->input00.fields.type == 0x41) {
                states[i]->protocol10 = 0;
            } else if (states[i]->input00.fields.type == 0x23) {
                states[i]->protocol10 = 1;
            } else if (states[i]->input00.fields.type == 0x73) {
                states[i]->protocol10 = 0;
            } else {
                states[i]->protocol10 = -1;
            }
        }
        switch (ports[i]->type01) {
        case 0x41:
            states[i]->previous04 = states[i]->input00.fields.buttons;
            states[i]->input00.fields.buttons = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->edges08.fields.pressed = states[i]->input00.fields.buttons & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->input00.fields.buttons ^ states[i]->previous04);
            states[i]->steering20 = (((states[i]->input00.fields.buttons >> 13) & 1) - ((states[i]->input00.fields.buttons & 0x8000) ? 1 : 0)) * D_8009FFB8[D_800F3824] + 0x80;
            states[i]->pressure22 = (states[i]->input00.fields.buttons & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->input00.fields.buttons & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->input00.fields.buttons & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->edges08.fields.pressed & 0x820;
            states[i]->action2C = states[i]->edges08.fields.pressed & 0x40;
            states[i]->action30 = states[i]->edges08.fields.pressed & 0x20;
            states[i]->action34 = states[i]->edges08.fields.pressed & 0x40;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0x23:
            states[i]->previous04 = states[i]->input00.fields.buttons;
            states[i]->input00.fields.buttons = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->released06 = states[i]->previous04 & (states[i]->input00.fields.buttons ^ states[i]->previous04);
            states[i]->steering20 = ports[i]->analog04.bytes[0];
            states[i]->pressure22 = ports[i]->analog04.bytes[1] - Options.config.p[i].offset02;
            states[i]->pressure24 = ports[i]->analog04.bytes[2] - Options.config.p[i].offset04;
            states[i]->pressure26 = ports[i]->analog04.bytes[3] - Options.config.p[i].offset06;
            if (states[i]->pressure22 < 0) {
                states[i]->pressure22 = 0;
            }
            if (states[i]->pressure24 < 0) {
                states[i]->pressure24 = 0;
            }
            if (states[i]->pressure26 < 0) {
                states[i]->pressure26 = 0;
            }
            if (states[i]->steering20 >= 0xA3) {
                states[i]->input00.fields.buttons |= 0x2000;
            }
            if (states[i]->steering20 < 0x5E) {
                states[i]->input00.fields.buttons |= 0x8000;
            }
            if (states[i]->pressure22 >= 0x36) {
                states[i]->input00.fields.buttons |= 0x40;
            }
            if (states[i]->pressure24 >= 0x36) {
                states[i]->input00.fields.buttons |= 0x80;
            }
            if (states[i]->pressure26 >= 0x36) {
                states[i]->input00.fields.buttons |= 4;
            }
            states[i]->edges08.fields.pressed = states[i]->input00.fields.buttons & ~states[i]->previous04;
            states[i]->action28 = states[i]->edges08.fields.pressed & 0x840;
            states[i]->action2C = states[i]->edges08.fields.pressed & 0x80;
            states[i]->action30 = states[i]->edges08.fields.pressed & 0x40;
            states[i]->action34 = states[i]->edges08.fields.pressed & 0x80;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0x73:
            states[i]->previous04 = states[i]->input00.fields.buttons;
            states[i]->input00.fields.buttons = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->edges08.fields.pressed = states[i]->input00.fields.buttons & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->input00.fields.buttons ^ states[i]->previous04);
            states[i]->steering20 = ports[i]->analog04.bytes[2];
            states[i]->pressure22 = (states[i]->input00.fields.buttons & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->input00.fields.buttons & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->input00.fields.buttons & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->edges08.fields.pressed & 0x820;
            states[i]->action2C = states[i]->edges08.fields.pressed & 0x40;
            states[i]->action30 = states[i]->edges08.fields.pressed & 0x20;
            states[i]->action34 = states[i]->edges08.fields.pressed & 0x40;
            states[i]->axes14.word = ports[i]->analog04.word;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0xE3:
            states[i]->previous04 = states[i]->input00.fields.buttons;
            states[i]->input00.fields.buttons = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->edges08.fields.pressed = states[i]->input00.fields.buttons & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->input00.fields.buttons ^ states[i]->previous04);
            states[i]->position41 = ports[i]->analog04.bytes[0];
            states[i]->position42 = ports[i]->analog04.bytes[1];
            states[i]->position43 = ports[i]->analog04.bytes[2];
            states[i]->position45 = states[i]->position42;
            states[i]->previous4E = states[i]->position4C;
            states[i]->steering20 = states[i]->position4C = states[i]->position41 + ((s8)states[i]->position45 << 8);
            states[i]->pressure22 = (states[i]->input00.fields.buttons & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->input00.fields.buttons & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->input00.fields.buttons & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->edges08.fields.pressed & 0x820;
            states[i]->action2C = states[i]->edges08.fields.pressed & 0x40;
            states[i]->action30 = states[i]->edges08.fields.pressed & 0x20;
            states[i]->action34 = states[i]->edges08.fields.pressed & 0x40;
            states[i]->idleStage54 = 0;
            if (states[i]->previous04 == states[i]->input00.fields.buttons) {
                if (VSyncThreshold == 0x80) {
                    states[i]->idleTick58 += 1;
                } else {
                    states[i]->idleTick58 += 2;
                }
                if (states[i]->idleTick58 >= 0xBB9) {
                    states[i]->idleTick58 = 0xBB8;
                }
            } else {
                states[i]->idleTick58 = 0;
            }
            if (states[i]->idleTick58 >= 0xBB8) {
                states[i]->idleStage54 = 2;
            } else if (states[i]->idleTick58 >= 0xABF) {
                states[i]->idleStage54 = 1;
            }
            break;
        case 0x12:
            states[i]->previous04 = states[i]->input00.fields.buttons;
            states[i]->input00.fields.buttons = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->edges08.fields.pressed = states[i]->input00.fields.buttons & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->input00.fields.buttons ^ states[i]->previous04);
            states[i]->axes14.word = ports[i]->analog04.word;
            states[i]->action28 = 0;
            states[i]->action2C = 0;
            states[i]->action30 = 0;
            states[i]->action34 = 0;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        default:
            if (D_800FFC08[i] == 0) {
                D_800FFC08[i] = 2;
            }
            states[i]->input00.fields.status = 1;
            states[i]->input00.fields.buttons = 0;
            states[i]->edges08.fields.pressed = 0;
            states[i]->edges08.fields.repeat = 0;
            states[i]->steering20 = 0x80;
            states[i]->pressure22 = 0;
            states[i]->pressure24 = 0;
            states[i]->pressure26 = 0;
            states[i]->action28 = 0;
            states[i]->action2C = 0;
            states[i]->action30 = 0;
            states[i]->action34 = 0;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        }
        states[i]->edges08.fields.repeat = states[i]->edges08.fields.pressed;
        if (states[i]->input00.fields.buttons == states[i]->previous04) {
            states[i]->repeatTick0C++;
            if (states[i]->repeatTick0C >= 0x22) {
                states[i]->edges08.fields.repeat = states[i]->input00.fields.buttons;
                states[i]->repeatTick0C = 0x1E;
            }
        } else {
            states[i]->repeatTick0C = 0;
        }
        switch (ports[i]->type01) {
        case 0x23:
            val = states[i]->steering20 - (Options.config.p[i].center00 + 0x80);
            if (val > 0) {
                val -= PadDeadzoneTable[Options.config.p[i].deadzone08];
                if (val < 0) {
                    val = 0;
                }
                if (val > D_8009FFB8[Options.config.p[i].range0A]) {
                    val = D_8009FFB8[Options.config.p[i].range0A];
                }
            } else {
                val += PadDeadzoneTable[Options.config.p[i].deadzone08];
                if (val > 0) {
                    val = 0;
                }
                if (val < -D_8009FFB8[Options.config.p[i].range0A]) {
                    val = -D_8009FFB8[Options.config.p[i].range0A];
                }
            }
            states[i]->steering3C = val;
            break;
        case 0xE3:
            val = states[i]->steering20;
            if (val > 0) {
                val -= WheelDeadzoneTable[Options.config.w[i].deadzone00];
                if (val < 0) {
                    val = 0;
                }
                if (val > PadRangeLimits[Options.config.w[i].range01]) {
                    val = PadRangeLimits[Options.config.w[i].range01];
                }
            } else {
                val += WheelDeadzoneTable[Options.config.w[i].deadzone00];
                if (val > 0) {
                    val = 0;
                }
                if (val < -PadRangeLimits[Options.config.w[i].range01]) {
                    val = -PadRangeLimits[Options.config.w[i].range01];
                }
            }
            states[i]->steering3C = (val << 7) / PadRangeLimits[Options.config.w[i].range01];
            break;
        case 0x73:
            val = states[i]->steering20 - 0x80;
            if (abs(val) < 0x18) {
                val = 0;
            }
            if (val > 0) {
                val -= 0x10;
            }
            if (val < 0) {
                val += 0x10;
            }
            states[i]->steering3C = (val * 0x7F) / 111;
            break;
        }
        if (states[i]->pressure22 >= 0x6B) {
            states[i]->pressure22 = 0x6A;
        }
        if (states[i]->pressure24 >= 0x6B) {
            states[i]->pressure24 = 0x6A;
        }
        if (states[i]->pressure26 >= 0x6B) {
            states[i]->pressure26 = 0x6A;
        }
    }
    PadStepPulse();
    PadUpdateRumble();
    for (i = 0; i < 2; i++) {
        PadPollPort(i);
    }
}

int GetControllerWarning(int i) {
    return PlayerControllers[i].idleStage54;
}

void PadRangeCheckSet(unsigned a, unsigned b, short c, short d) {
    PadPulseState18 *t;
    if (a >= 2) {
        DebugPrintStub(StrControllerNoErrorFmt, a);
    }
    if (b >= 2) {
        DebugPrintStub(StrActuatorNoErrorFmt, b);
    }
    t = &PadPulseState[a];
    t->delay10[b] = d;
    t->limit00[b] = c;
    t->active14 = 0;
    t->reserved16 = 0;
}

void PadStartPulse(int i, int k, int v) {
    PadPulseState18 *t = &PadPulseState[i];
    switch (k) {
    case 0:
        t->repeat04 = 0;
        return;
    case 1:
        t->repeat04 = v + 1;
        t->repeatTick08 = 0;
        t->repeatStep0C = 0;
        t->active14 = 0;
        return;
    case 2:
        t->once06 = v + 1;
        t->onceTick0A = 0;
        t->onceStep0E = 0;
        t->active14 = 0;
        return;
    }
}

void PadClearPulse(int i) {
    PadPulseState18 *t = &PadPulseState[i];
    t->delay10[0] = 0;
    t->limit00[0] = 0;
    t->delay10[1] = 0;
    t->limit00[1] = 0;
    t->repeat04 = 0;
    t->once06 = 0;
    t->repeatTick08 = 0;
    t->onceTick0A = 0;
    t->repeatStep0C = 0;
    t->onceStep0E = 0;
}

