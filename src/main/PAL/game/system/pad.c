#define NULL 0

typedef signed char s8;

typedef unsigned char u8;

typedef signed short s16;

typedef unsigned short u16;

typedef signed int s32;

typedef unsigned int u32;

typedef struct {
    u8 status00, type01;
    u16 buttons02, previous04, released06, pressed08, repeat0A, repeatTick0C, reserved0E;
    s32 protocol10;
    u32 axes14;
    u8 reserved18, output19, output1A, flag1B;
    s16 kind1C, changed1E;
    s16 steering20, pressure22, pressure24, pressure26;
    s32 action28, action2C, action30, action34;
    u32 reserved38;
    s16 steering3C, reserved3E;
    u8 flag40, position41, position42, position43, reserved44, position45, reserved46[6];
    u16 position4C, previous4E;
    u32 reserved50;
    s32 idleStage54, idleTick58;
} Pad;

typedef struct {
    u8 status00, type01, data02, data03, data04, data05, data06, data07, reserved08[32];
} Port;

typedef struct {
    s16 pad00[4];
    struct {
        u16 center00;
        u16 offset02;
        u16 offset04;
        u16 offset06;
        s16 deadzone08;
        s16 range0A;
        s16 pad0C;
        s16 pad0E;
    } p[2];
    struct {
        s8 deadzone00;
        s8 range01;
        s8 pad02[2];
    } w[2];
    u8 rem[12];
} Settings;

typedef struct { char pad[0x54]; int f54; char pad2[4]; } T;

typedef struct { short s[8]; short t[2]; short u[2]; } T_1;

typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16; } T_2;

typedef struct { short s0, s2, s4, s6, s8, sA, sC, sE, s10, s12, s14, s16; } T_3;

extern Settings D_800AD528;
extern Pad D_800F3A38[2];
extern Port D_8010C1A0[2];
extern s32 D_800FFC08[2];
extern s32 D_800AD590[2];
extern s32 D_800F4CD8[2];
extern s16 D_8009FFB8[4];
extern s16 D_8009FFC0[6];
extern s32 D_8009FFCC[8];
extern s32 D_800A016C[8];
extern s32 D_800AC5DC;
extern s16 D_800F3824;
int abs(int);
void func_8004B8A8(void);
void func_8004BC5C(void);
void func_8004BE4C(s32);
extern T_1 D_8010C170[];
extern char D_80011014[];
extern char D_80011030[];
extern int func_8001E84C();

void func_8004A6C4(void);
int func_8004B70C(int i);
void func_8004B738(unsigned a, unsigned b, short c, short d);
void func_8004B7E0(int i, int k, int v);
void func_8004B864(int i);

void func_8004A6C4(void) {
    Port *ports[2];
    Pad *states[2];
    s32 i;
    s32 v;
    s16 val;

    __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: keep prologue register saves before ports array initialization */
    ports[0] = &D_8010C1A0[0];
    ports[1] = &D_8010C1A0[1];
    __asm__ volatile("" ::: "memory"); /* ASM_BARRIER: schedule ports array initialization before loop preheader */
    states[0] = &D_800F3A38[0];
    states[1] = &D_800F3A38[1];

    for (i = 0; i < 2; i++) {
        if (ports[i]->type01 != states[i]->type01) {
            states[i]->changed1E = 1;
            states[i]->protocol10 = -1;
        }
        states[i]->status00 = ports[i]->status00;
        states[i]->type01 = ports[i]->type01;
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
            Port *port = ports[i];
            Pad *pad = states[i];
            port->type01 = 0;
            pad->type01 = 0;
        } else {
            D_800FFC08[i] = 0;
        }
        if (states[i]->type01 != 0 && states[i]->protocol10 < 0) {
            if (states[i]->type01 == 0x41) {
                states[i]->protocol10 = 0;
            } else if (states[i]->type01 == 0x23) {
                states[i]->protocol10 = 1;
            } else if (states[i]->type01 == 0x73) {
                states[i]->protocol10 = 0;
            } else {
                states[i]->protocol10 = -1;
            }
        }
        switch (ports[i]->type01) {
        case 0x41:
            states[i]->previous04 = states[i]->buttons02;
            states[i]->buttons02 = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->pressed08 = states[i]->buttons02 & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->buttons02 ^ states[i]->previous04);
            states[i]->steering20 = (((states[i]->buttons02 >> 13) & 1) - ((states[i]->buttons02 & 0x8000) ? 1 : 0)) * D_8009FFB8[D_800F3824] + 0x80;
            states[i]->pressure22 = (states[i]->buttons02 & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->buttons02 & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->buttons02 & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->pressed08 & 0x820;
            states[i]->action2C = states[i]->pressed08 & 0x40;
            states[i]->action30 = states[i]->pressed08 & 0x20;
            states[i]->action34 = states[i]->pressed08 & 0x40;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0x23:
            states[i]->previous04 = states[i]->buttons02;
            states[i]->buttons02 = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->released06 = states[i]->previous04 & (states[i]->buttons02 ^ states[i]->previous04);
            states[i]->steering20 = ports[i]->data04;
            states[i]->pressure22 = ports[i]->data05 - D_800AD528.p[i].offset02;
            states[i]->pressure24 = ports[i]->data06 - D_800AD528.p[i].offset04;
            states[i]->pressure26 = ports[i]->data07 - D_800AD528.p[i].offset06;
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
                states[i]->buttons02 |= 0x2000;
            }
            if (states[i]->steering20 < 0x5E) {
                states[i]->buttons02 |= 0x8000;
            }
            if (states[i]->pressure22 >= 0x36) {
                states[i]->buttons02 |= 0x40;
            }
            if (states[i]->pressure24 >= 0x36) {
                states[i]->buttons02 |= 0x80;
            }
            if (states[i]->pressure26 >= 0x36) {
                states[i]->buttons02 |= 4;
            }
            states[i]->pressed08 = states[i]->buttons02 & ~states[i]->previous04;
            states[i]->action28 = states[i]->pressed08 & 0x840;
            states[i]->action2C = states[i]->pressed08 & 0x80;
            states[i]->action30 = states[i]->pressed08 & 0x40;
            states[i]->action34 = states[i]->pressed08 & 0x80;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0x73:
            states[i]->previous04 = states[i]->buttons02;
            states[i]->buttons02 = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->pressed08 = states[i]->buttons02 & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->buttons02 ^ states[i]->previous04);
            states[i]->steering20 = ports[i]->data06;
            states[i]->pressure22 = (states[i]->buttons02 & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->buttons02 & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->buttons02 & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->pressed08 & 0x820;
            states[i]->action2C = states[i]->pressed08 & 0x40;
            states[i]->action30 = states[i]->pressed08 & 0x20;
            states[i]->action34 = states[i]->pressed08 & 0x40;
            states[i]->axes14 = *(u32 *)&ports[i]->data04;
            states[i]->idleStage54 = 0;
            states[i]->idleTick58 = 0;
            break;
        case 0xE3:
            states[i]->previous04 = states[i]->buttons02;
            states[i]->buttons02 = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->pressed08 = states[i]->buttons02 & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->buttons02 ^ states[i]->previous04);
            states[i]->position41 = ports[i]->data04;
            states[i]->position42 = ports[i]->data05;
            states[i]->position43 = ports[i]->data06;
            states[i]->position45 = states[i]->position42;
            states[i]->previous4E = states[i]->position4C;
            states[i]->steering20 = states[i]->position4C = states[i]->position41 + ((s8)states[i]->position45 << 8);
            states[i]->pressure22 = (states[i]->buttons02 & 0x40) ? 0x6A : 0;
            states[i]->pressure24 = (states[i]->buttons02 & 0x80) ? 0x6A : 0;
            states[i]->pressure26 = (states[i]->buttons02 & 4) ? 0x6A : 0;
            states[i]->action28 = states[i]->pressed08 & 0x820;
            states[i]->action2C = states[i]->pressed08 & 0x40;
            states[i]->action30 = states[i]->pressed08 & 0x20;
            states[i]->action34 = states[i]->pressed08 & 0x40;
            states[i]->idleStage54 = 0;
            if (states[i]->previous04 == states[i]->buttons02) {
                if (D_800AC5DC == 0x80) {
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
            states[i]->previous04 = states[i]->buttons02;
            states[i]->buttons02 = ~((ports[i]->data02 << 8) | ports[i]->data03);
            states[i]->pressed08 = states[i]->buttons02 & ~states[i]->previous04;
            states[i]->released06 = states[i]->previous04 & (states[i]->buttons02 ^ states[i]->previous04);
            states[i]->axes14 = *(u32 *)&ports[i]->data04;
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
            states[i]->status00 = 1;
            states[i]->buttons02 = 0;
            states[i]->pressed08 = 0;
            states[i]->repeat0A = 0;
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
        states[i]->repeat0A = states[i]->pressed08;
        if (states[i]->buttons02 == states[i]->previous04) {
            states[i]->repeatTick0C++;
            if (states[i]->repeatTick0C >= 0x22) {
                states[i]->repeat0A = states[i]->buttons02;
                states[i]->repeatTick0C = 0x1E;
            }
        } else {
            states[i]->repeatTick0C = 0;
        }
        switch (ports[i]->type01) {
        case 0x23:
            val = states[i]->steering20 - (D_800AD528.p[i].center00 + 0x80);
            if (val > 0) {
                val -= D_800A016C[D_800AD528.p[i].deadzone08];
                if (val < 0) {
                    val = 0;
                }
                if (val > D_8009FFB8[D_800AD528.p[i].range0A]) {
                    val = D_8009FFB8[D_800AD528.p[i].range0A];
                }
            } else {
                val += D_800A016C[D_800AD528.p[i].deadzone08];
                if (val > 0) {
                    val = 0;
                }
                if (val < -D_8009FFB8[D_800AD528.p[i].range0A]) {
                    val = -D_8009FFB8[D_800AD528.p[i].range0A];
                }
            }
            states[i]->steering3C = val;
            break;
        case 0xE3:
            val = states[i]->steering20;
            if (val > 0) {
                val -= D_8009FFCC[D_800AD528.w[i].deadzone00];
                if (val < 0) {
                    val = 0;
                }
                if (val > D_8009FFC0[D_800AD528.w[i].range01]) {
                    val = D_8009FFC0[D_800AD528.w[i].range01];
                }
            } else {
                val += D_8009FFCC[D_800AD528.w[i].deadzone00];
                if (val > 0) {
                    val = 0;
                }
                if (val < -D_8009FFC0[D_800AD528.w[i].range01]) {
                    val = -D_8009FFC0[D_800AD528.w[i].range01];
                }
            }
            states[i]->steering3C = (val << 7) / D_8009FFC0[D_800AD528.w[i].range01];
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
    func_8004B8A8();
    func_8004BC5C();
    for (i = 0; i < 2; i++) {
        func_8004BE4C(i);
    }
}

/* legacy typing of shared globals in this part of the unit */
#define D_800F3A38 (*(T (*)[])&D_800F3A38)

int func_8004B70C(int i) {
    return D_800F3A38[i].f54;
}

#undef D_800F3A38

void func_8004B738(unsigned a, unsigned b, short c, short d) {
    T_1 *t;
    if (a >= 2) {
        func_8001E84C(D_80011014, a);
    }
    if (b >= 2) {
        func_8001E84C(D_80011030, b);
    }
    t = &D_8010C170[a];
    t->t[b] = d;
    t->s[b] = c;
    t->u[0] = 0;
    t->u[1] = 0;
}

/* legacy typing of shared globals in this part of the unit */
#define D_8010C170 (*(T_2 (*)[])&D_8010C170)

void func_8004B7E0(int i, int k, int v) {
    T_2 *t = &D_8010C170[i];
    switch (k) {
    case 0:
        t->s4 = 0;
        return;
    case 1:
        t->s4 = v + 1;
        t->s8 = 0;
        t->sC = 0;
        t->s14 = 0;
        return;
    case 2:
        t->s6 = v + 1;
        t->sA = 0;
        t->sE = 0;
        t->s14 = 0;
        return;
    }
}

#undef D_8010C170

/* legacy typing of shared globals in this part of the unit */
#define D_8010C170 (*(T_3 (*)[])&D_8010C170)

void func_8004B864(int i) {
    T_3 *t = &D_8010C170[i];
    t->s10 = 0;
    t->s0 = 0;
    t->s12 = 0;
    t->s2 = 0;
    t->s4 = 0;
    t->s6 = 0;
    t->s8 = 0;
    t->sA = 0;
    t->sC = 0;
    t->sE = 0;
}

#undef D_8010C170
