#include "types.h"

/* Car / course setup controller (one player). */

typedef struct {
    s16 x00, y02;
    s32 words04[4];
} Ovl665View14;

typedef struct {
    s32 scene00, reserved04, selection08;
    s32 step0C, detail10, next14, kind18;
    s32 angle1C, phase20, fade24;
    u16 buttons28;
    s16 course2A;
    s32 ready2C;
    u8 pad30[8];
    Ovl665View14 view38;
    u8 pad4C[0x3A];
    u16 rotation86;
    u8 pad88[4];
    s16 offset8C, offset8E, offset90;
    u8 pad92[2];
    s16 offset94, offset96, offset98;
    u8 pad9A[6];
} Ovl665CarState;

typedef struct {
    u8 byte00, course01;
    u8 pad02[6];
    u16 buttons08;
    u8 pad0A[0x12];
    s16 option1C;
    u8 pad1E[2];
    s16 balance20;
    u8 pad22[6];
    s32 confirm28;
    u8 pad2C[0x14];
    u8 ready40;
    s8 tilt41;
    u8 pad42[0x1A];
} Ovl665Pad5C;

typedef struct {
    struct { s16 car00, colour02; } a[2];
    struct { s16 h[8]; } b[2];
    struct { s8 c[4]; } c[2];
} Ovl665Settings;

typedef struct {
    u8 pad00[4];
    u8 b4, b5, b6, b7;
    u8 pad08[0x20];
} Ovl665Entry28;

typedef struct {
    u8 byte00, byte01;
    s16 h02;
    s16 x04, y06;
    u8 pad08[8];
    s16 state10;
} Ovl665Panel;

typedef struct {
    u8 pad00[0xC];
    u8 cells[8];
    u8 byte14, byte15, byte16;
    u8 pad17[0xD];
} Ovl665Row24;

typedef struct {
    u8 pad00[4];
    u8 b4;
} Ovl665Text;

typedef struct {
    u8 pad00[0x70];
    u32 ordering70;
} Ovl665Frame;

typedef struct {
    u32 tag;
    u32 command;
} Ovl665Packet;

typedef struct {
    u8 a, pad1, b, pad3;
} Ovl665Colour4;

typedef struct {
    s16 x0, y0, x1, y1;
} Ovl665Line;

extern Ovl665CarState ovl665_D_80119768;
extern s32 ovl665_D_80119760;
extern Ovl665Pad5C D_800F3A38[2];
extern Ovl665Settings D_800AD528;
extern Ovl665Entry28 D_8010C1A0[];
extern Ovl665Panel *D_800F3AFC;
extern Ovl665Row24 *D_800F4D80;
extern Ovl665Text *D_800AC590;
extern Ovl665Frame *D_800ACC14;
extern s16 D_800AC5D0;
extern s32 D_800AC5D4;
extern s32 D_800F4CB8;
extern u8 D_800ACC68;
extern Ovl665View14 ovl665_D_80119698[];
extern Ovl665Colour4 ovl665_D_8011902C[][5];
extern Ovl665Colour4 ovl665_D_801190CC[][5];
extern Ovl665Colour4 ovl665_D_8011916C[][5];
extern void *ovl665_D_80119290[][5];
extern void *ovl665_D_801193E8[][5];
extern void *ovl665_D_80119558[][5];
extern void *ovl665_D_801195F8[][5];
extern s32 ovl665_D_80119740[];
extern s32 ovl665_D_80119724[];
extern u8 ovl665_D_8011973C[];
extern u8 ovl665_D_80119020[], ovl665_D_80119024[];
extern s16 D_8009FFB8[];
extern s16 D_8009FFC0[];
extern s32 D_8009FFCC[];

extern s32 func_800978E0(s32);
extern void func_80097C58(s32, s32, s32);
extern s32 func_8004E8CC(void);
extern s32 ovl665_func_8011629C(s32);
extern void func_8005B2D4(s32);
extern void func_8002B330(s32, s32, void *, s32, s32);
extern void func_8004A5C0(s32, s32, s32, s32);
/* Sound cue; several call sites pass the freshly computed offset as a
 * second (ignored) argument, so it is declared without a prototype. */
extern void func_8004F24C();
extern void ovl665_func_80116AA4(void *, s32);
extern void *func_8001FF84(void *, void *, s32, s32, void *, s32, s32);
extern void ovl665_func_8011643C(void);
extern void func_8003EC88(s32);
extern void func_800207D4(void *, s32, s32);
extern void ovl665_func_80114A24(void);
extern void func_8002A604();
extern void func_8003D228(s32, s32, s32, s32, s32, s32, s32);
extern void func_8002AF8C(s32, s32, s32);
extern s32 func_80090424(s32);
extern void func_8004D290(void *, void *, s32, s32);
extern void func_8004BB84(s32, s32);
extern void ovl665_func_801169BC(s32);
extern void func_8003D6F0(s32);
extern void func_80032A14(s32, s32, void *);
extern void func_8002A344(s32);

#define ST ovl665_D_80119768

typedef struct {
    void *cursor;
} Ovl665Scratch;

#define SCRATCH ((Ovl665Scratch *)0x1F800000)
#define CURSOR (*(void **)0x1F800000)

#define CLOSE_ROWS(n)                                   \
    D_800F3AFC->state10 = 4;                            \
    for (i = 0; i < (n); i++) {                         \
        if (D_800F4D80[i].byte14 == 2) {                \
            D_800F4D80[i].byte14 = 3;                   \
        }                                               \
    }

#define COPY_COLOURS(src)                               \
    for (i = 0; i < 5; i++) {                           \
        D_800F4D80[i].byte15 = (src)->a;                \
        D_800F4D80[i].byte16 = (src)->b;                \
        (src)++;                                        \
    }

/* Links a texture-page command packet into the frame ordering list. */
#define LINK_PACKET()                                                       \
    {                                                                       \
        Ovl665Frame *frame;                                                 \
        packet = SCRATCH->cursor;                                           \
        ((u8 *)&packet->tag)[3] = 1;                                        \
        frame = D_800ACC14;                                                 \
        packet->command = 0xE1000259;                                       \
        packet->tag = (packet->tag & 0xFF000000) | (frame->ordering70 & 0xFFFFFF); \
        frame->ordering70 = (frame->ordering70 & 0xFF000000) | ((u32)packet & 0xFFFFFF); \
        packet++;                                                           \
        SCRATCH->cursor = packet;                                           \
    }

void ovl665_func_80116D10(s32 player) {
    Ovl665Colour4 *src;
    Ovl665Entry28 *entry;
    s32 changed;
    s32 i;
    s32 kind;
    s32 index;
    s32 old;
    s32 value;
    s32 phase;
    s32 wave;
    s32 arg;
    s16 timer;
    void *cursor;
    Ovl665Line line;
    Ovl665Packet *packet;

    ST.buttons28 = D_800F3A38[player].buttons08;
    entry = &D_8010C1A0[player];
    changed = 0;
    switch (ST.step0C) {
    case 0:
        if (ovl665_D_80119760 == 0) {
            if (D_800F3A38[player].ready40 == 1) {
                if (ST.ready2C == 0) {
                    if (D_800F3A38[player].course01 != 0xE3) {
                        if (func_800978E0(player << 4) == 6) {
                            func_80097C58(player << 4, 1, 0);
                        }
                    } else {
                        ST.ready2C = 1;
                        ovl665_D_80119760 = 1;
                    }
                }
            } else {
                ovl665_D_80119760 = 1;
            }
        }
        D_800AC5D4 = player;
        if (func_8004E8CC() == 0) {
            break;
        }
        if (ovl665_D_80119760 == 0) {
            break;
        }
        kind = ovl665_func_8011629C(player);
        ST.kind18 = kind;
        if (kind >= 0) {
            switch (kind) {
            case 0:
            case 1:
                index = D_800AD528.a[player].car00;
                src = ovl665_D_8011902C[index];
                index <<= 12;
                ST.rotation86 = index;
                break;
            case 2:
                index = D_800AD528.b[player].h[6];
                src = ovl665_D_801190CC[index];
                index <<= 12;
                ST.rotation86 = index;
                break;
            case 3:
                if (D_800F3A38[player].course01 != 0xE3) {
                    index = D_800AD528.a[player].car00;
                    src = ovl665_D_8011902C[index];
                    index <<= 12;
                ST.rotation86 = index;
                } else {
                    index = D_800AD528.c[player].c[3];
                    src = ovl665_D_8011916C[index];
                    index <<= 12;
                ST.rotation86 = index;
                }
                ST.course2A = D_800F3A38[player].course01;
                break;
            }
            ST.view38 = ovl665_D_80119698[ST.kind18];
            COPY_COLOURS(src);
        } else {
            CLOSE_ROWS(8);
            D_800AC5D0 = 64;
            ST.step0C = 2;
        }
        ST.detail10 = 0;
        ST.angle1C = 0;
        ST.fade24 = 0;
        ST.offset8C = 0;
        ST.offset8E = 0;
        ST.offset90 = 0;
        ST.offset94 = 0;
        ST.offset96 = 0;
        ST.offset98 = 0;
        func_8005B2D4(0);
        ST.step0C++;
        break;
    case 1:
        if (D_800F3AFC->state10 == 3 && D_800F4D80[0].byte14 == 2) {
            if (ovl665_func_8011629C(player) < 0) {
                CLOSE_ROWS(8);
                D_800AC5D0 = 64;
                ST.step0C = 2;
                ST.next14 = 0;
            }
            switch (ST.kind18) {
            case 1:
                if (D_800F3A38[player].option1C != 0) {
                    D_800F4D80[5].cells[0] = 1;
                    D_800F4D80[D_800AD528.a[player].colour02 + 6].cells[0] = 0;
                    func_8002B330(player * 4, 2, &D_800AD528.a[player].colour02, 2, 0);
                    D_800F4D80[D_800AD528.a[player].colour02 + 6].cells[0] = 2;
                }
            case 0:
                old = D_800AD528.a[player].car00;
                func_8002B330(player * 4 | 1, 2, &D_800AD528.a[player].car00, 8, 0);
                if (old != D_800AD528.a[player].car00) {
                    if (D_800AD528.a[player].car00 < old) {
                        ST.angle1C -= 0x1000;
                    } else {
                        ST.angle1C += 0x1000;
                    }
                    src = ovl665_D_8011902C[D_800AD528.a[player].car00];
                    COPY_COLOURS(src);
                    ST.fade24 = 0;
                    func_8004A5C0(player, D_800AD528.a[player].car00, D_800AD528.b[player].h[6],
                                  D_800AD528.c[player].c[3]);
                }
                if (D_800F3A38[player].confirm28 != 0) {
                    func_8004F24C(1);
                    CLOSE_ROWS(5);
                    D_800AC5D0 = 64;
                    ST.step0C = 2;
                    ST.next14 = 0;
                }
                ovl665_func_80116AA4(ovl665_D_80119290[D_800AD528.a[player].car00], ST.fade24);
                cursor = CURSOR;
                if (D_800AD528.a[player].car00 > 0) {
                    cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                }
                if (D_800AD528.a[player].car00 < 7) {
                    cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                }
                CURSOR = cursor;
                break;
            case 2:
                old = D_800AD528.b[player].h[6];
                func_8002B330(player * 4 | 1, 2, &D_800AD528.b[player].h[6], 8, 0);
                if (old != D_800AD528.b[player].h[6]) {
                    if (D_800AD528.b[player].h[6] < old) {
                        ST.angle1C -= 0x1000;
                    } else {
                        ST.angle1C += 0x1000;
                    }
                    src = ovl665_D_801190CC[D_800AD528.b[player].h[6]];
                    COPY_COLOURS(src);
                    ST.fade24 = 0;
                    func_8004A5C0(player, D_800AD528.a[player].car00, D_800AD528.b[player].h[6],
                                  D_800AD528.c[player].c[3]);
                }
                value = D_800F3A38[player].balance20 * 3800 / 256 - 1900;
                ST.offset8C = value / 2;
                ST.offset94 = -value / 2;
                if (D_800F3A38[player].confirm28 != 0) {
                    func_8004F24C(1, value);
                    CLOSE_ROWS(5);
                    D_800AC5D0 = 64;
                    ST.step0C = 2;
                    ST.next14 = 3;
                }
                ovl665_func_80116AA4(ovl665_D_801193E8[D_800AD528.b[player].h[6]], ST.fade24);
                cursor = CURSOR;
                if (D_800AD528.b[player].h[6] > 0) {
                    cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                }
                if (D_800AD528.b[player].h[6] < 7) {
                    cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                }
                CURSOR = cursor;
                break;
            case 3:
                if (ST.course2A != D_800F3A38[player].course01) {
                    changed = 1;
                    ST.course2A = D_800F3A38[player].course01;
                }
                if (D_800F3A38[player].course01 != 0xE3) {
                    old = D_800AD528.a[player].car00;
                    func_8002B330(player * 4 | 1, 1, &D_800AD528.a[player].car00, 8, 0);
                    if (old != D_800AD528.a[player].car00) {
                        if (D_800AD528.a[player].car00 < old) {
                            ST.angle1C -= 0x1000;
                        } else {
                            ST.angle1C += 0x1000;
                        }
                        changed = 1;
                        func_8004A5C0(player, D_800AD528.a[player].car00, D_800AD528.b[player].h[6],
                                      D_800AD528.c[player].c[3]);
                    }
                    if (changed) {
                        src = ovl665_D_8011902C[D_800AD528.a[player].car00];
                        COPY_COLOURS(src);
                        ST.fade24 = 0;
                    }
                    if (D_800F3A38[player].confirm28 != 0) {
                        func_8004F24C(1);
                        CLOSE_ROWS(5);
                        D_800AC5D0 = 64;
                        ST.step0C = 2;
                        ST.next14 = 0;
                    }
                    ovl665_func_80116AA4(ovl665_D_80119558[D_800AD528.a[player].car00], ST.fade24);
                    cursor = CURSOR;
                    if (D_800AD528.a[player].car00 > 0) {
                        cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                    }
                    if (D_800AD528.a[player].car00 < 7) {
                        cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                    }
                    CURSOR = cursor;
                } else {
                    old = D_800AD528.c[player].c[3];
                    func_8002B330(player * 4 | 1, 1, &D_800AD528.c[player].c[3], 8, 0);
                    if (old != D_800AD528.c[player].c[3]) {
                        if (D_800AD528.c[player].c[3] < old) {
                            ST.angle1C -= 0x1000;
                        } else {
                            ST.angle1C += 0x1000;
                        }
                        changed = 1;
                        func_8004A5C0(player, D_800AD528.a[player].car00, D_800AD528.b[player].h[6],
                                      D_800AD528.c[player].c[3]);
                    }
                    if (changed) {
                        src = ovl665_D_8011916C[D_800AD528.c[player].c[3]];
                        COPY_COLOURS(src);
                        ST.fade24 = 0;
                    }
                    value = D_800F3A38[player].tilt41 * 16;
                    ST.offset90 = value;
                    if (D_800F3A38[player].confirm28 != 0) {
                        func_8004F24C(1, value);
                        CLOSE_ROWS(5);
                        D_800AC5D0 = 64;
                        ST.step0C = 2;
                        ST.next14 = 4;
                    }
                    ovl665_func_80116AA4(ovl665_D_801195F8[D_800AD528.c[player].c[3]], ST.fade24);
                    cursor = CURSOR;
                    if (D_800AD528.c[player].c[3] > 0) {
                        cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 280, ovl665_D_80119020, 0x7F38, 255);
                    }
                    if (D_800AD528.c[player].c[3] < 7) {
                        cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 280, ovl665_D_80119024, 0x7F38, 255);
                    }
                    CURSOR = cursor;
                }
                break;
            }
            ST.fade24++;
            if (ST.fade24 > 16) {
                ST.fade24 = 16;
            }
        }
        ST.angle1C = ST.angle1C * 15 / 16;
        ST.rotation86 = ST.angle1C;
        if ((u32)(ST.angle1C + 128) > 256) {
            ST.fade24 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        for (i = 0; i < 5; i++) {
            func_8003EC88(i);
        }
        if (ST.kind18 == 1 && D_800F3A38[player].option1C != 0) {
            for (i = 5; i < 8; i++) {
                func_8003EC88(i);
            }
        }
        break;
    case 2:
        timer = D_800AC5D0 - 1;
        D_800AC5D0 = timer;
        func_800207D4(&D_800ACC14->ordering70, (64 - timer) * 4, 69);
        if (D_800AC5D0 <= 0) {
            if (ST.next14 == 0) {
                ovl665_func_80114A24();
                ST.step0C = 0;
                D_800AC5D4 = 0;
                ST.scene00--;
                return;
            }
            ST.step0C = ST.next14;
            ST.detail10 = 0;
            ST.next14 = 0;
        }
        ovl665_func_8011643C();
        for (i = 0; i < 5; i++) {
            func_8003EC88(i);
        }
        break;
    case 3:
        switch (ST.detail10) {
        case 0:
            func_8002A604();
            D_800ACC68 = 2;
            func_8003D228(0, 12, 304, 296, 88, 0, 0);
            D_800F3AFC->h02 = 1;
            func_8002AF8C(1, 0, 0);
            ST.view38 = ovl665_D_80119698[4];
            ST.rotation86 = 0;
            ST.angle1C = 0;
            D_800AC590->b4 = 9;
            ST.detail10++;
            break;
        case 1:
            value = D_800F3A38[player].balance20 * 3800 / 256 - 1900;
            ST.offset8C = value / 2;
            ST.offset94 = -value / 2;
            if (D_800F3A38[player].buttons08 & 0x800) {
                func_8004F24C(1, value);
                D_800AD528.b[player].h[0] = entry->b4 - 128;
                D_800AD528.b[player].h[1] = entry->b5;
                D_800AD528.b[player].h[2] = entry->b6;
                D_800AD528.b[player].h[3] = entry->b7;
                D_800AC590->b4 = D_800AD528.b[player].h[4] + 10;
                ST.view38 = ovl665_D_80119698[5];
                ST.phase20 = 0;
                ST.detail10++;
                func_8002AF8C(1, 1, 0);
            }
            break;
        case 2:
            old = D_800AD528.b[player].h[4];
            func_8002B330(player * 4 | 1, 1, &D_800AD528.b[player].h[4], 4, 0);
            if (old != D_800AD528.b[player].h[4]) {
                D_800AC590->b4 = D_800AD528.b[player].h[4] + 10;
            }
            value = func_80090424(ST.phase20) * ovl665_D_80119740[D_800AD528.b[player].h[4]] / 512;
            ST.offset8C = value;
            ST.offset94 = -value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (D_800F3A38[player].buttons08 & 0x840) {
                func_8004F24C(1, value);
                D_800AC590->b4 = D_800AD528.b[player].h[5] + 14;
                ST.view38 = ovl665_D_80119698[5];
                ST.phase20 = 0;
                ST.detail10++;
            }
            line.x0 = 148;
            line.x1 = 172;
            line.y0 = line.y1 = ST.view38.y02;
            func_8004D290(&line, ovl665_D_8011973C, 0, 0);
            if (D_800AD528.b[player].h[4] > 0) {
                line.y0 = line.y1 = ST.view38.y02 + ovl665_D_80119724[D_800AD528.b[player].h[4] - 1];
                func_8004D290(&line, ovl665_D_8011973C, 0, 0);
                line.y0 = line.y1 = ST.view38.y02 - ovl665_D_80119724[D_800AD528.b[player].h[4] - 1];
                func_8004D290(&line, ovl665_D_8011973C, 0, 0);
            }
            cursor = CURSOR;
            if (D_800AD528.b[player].h[4] > 0) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (D_800AD528.b[player].h[4] < 3) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            CURSOR = cursor;
            break;
        case 3:
            old = D_800AD528.b[player].h[5];
            func_8002B330(player * 4 | 1, 1, &D_800AD528.b[player].h[5], 4, 0);
            if (old != D_800AD528.b[player].h[5]) {
                D_800AC590->b4 = D_800AD528.b[player].h[5] + 14;
            }
            value = func_80090424(ST.phase20) * D_8009FFB8[D_800AD528.b[player].h[5]] / 512;
            ST.offset8C = value;
            ST.offset94 = -value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (D_800F3A38[player].buttons08 & 0x840) {
                func_8004F24C(1, value);
                CLOSE_ROWS(5);
                D_800AC5D0 = 64;
                ST.step0C = 2;
            }
            cursor = CURSOR;
            if (D_800AD528.b[player].h[5] > 0) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (D_800AD528.b[player].h[5] < 3) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            CURSOR = cursor;
            break;
        }
        if (ovl665_func_8011629C(player) < 0) {
            CLOSE_ROWS(8);
            D_800AC5D0 = 64;
            ST.step0C = 2;
            ST.next14 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        break;
    case 4:
        switch (ST.detail10) {
        case 0:
            func_8002A604();
            D_800ACC68 = 2;
            func_8003D228(0, 12, 304, 296, 88, 0, 0);
            D_800F3AFC->h02 = 1;
            func_8002AF8C(1, 0, 0);
            ST.view38 = ovl665_D_80119698[6];
            ST.rotation86 = 0;
            ST.angle1C = 0;
            D_800AC590->b4 = 8;
            ST.detail10++;
            break;
        case 1:
            value = D_800F3A38[player].tilt41 * 16;
            ST.offset90 = value;
            if (D_800F3A38[player].buttons08 & 0x800) {
                func_8004F24C(1, value);
                func_8004BB84(player, 3);
                D_800AC590->b4 = D_800AD528.c[player].c[0] + 10;
                ST.phase20 = 0;
                ST.detail10++;
                func_8002AF8C(1, 1, 0);
            }
            break;
        case 2:
            old = D_800AD528.c[player].c[0];
            func_8002B330(player * 4 | 1, 1, &D_800AD528.c[player].c[0], 4, 0);
            if (old != D_800AD528.b[player].h[4]) {
                D_800AC590->b4 = D_800AD528.c[player].c[0] + 10;
            }
            value = func_80090424(ST.phase20) * D_8009FFCC[D_800AD528.c[player].c[0]] / 128;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (D_800F3A38[player].confirm28 != 0) {
                func_8004F24C(1, value);
                D_800AC590->b4 = D_800AD528.c[player].c[1] + 18;
                ST.phase20 = 0;
                ST.detail10++;
            }
            if (D_800AD528.c[player].c[0] > 0) {
                ovl665_func_801169BC(D_8009FFCC[D_800AD528.c[player].c[0]] << 5);
                ovl665_func_801169BC(-D_8009FFCC[D_800AD528.c[player].c[0]] << 5);
            }
            cursor = CURSOR;
            if (D_800AD528.c[player].c[0] > 0) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (D_800AD528.c[player].c[0] < 3) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            CURSOR = cursor;
            break;
        case 3:
            old = D_800AD528.c[player].c[1];
            func_8002B330(player * 4 | 1, 1, &D_800AD528.c[player].c[1], 5, 0);
            if (old != D_800AD528.c[player].c[1]) {
                D_800AC590->b4 = D_800AD528.c[player].c[1] + 18;
            }
            value = func_80090424(ST.phase20) * D_8009FFC0[D_800AD528.c[player].c[1]] / 256;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (D_800F3A38[player].confirm28 != 0) {
                func_8004F24C(1, value);
                D_800AC590->b4 = D_800AD528.c[player].c[2] + 23;
                ST.phase20 = 0;
                ST.detail10++;
            }
            cursor = CURSOR;
            if (D_800AD528.c[player].c[1] > 0) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (D_800AD528.c[player].c[1] < 4) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            CURSOR = cursor;
            break;
        case 4:
            old = D_800AD528.c[player].c[2];
            func_8002B330(player * 4 | 1, 1, &D_800AD528.c[player].c[2], 3, 0);
            if (old != D_800AD528.c[player].c[2]) {
                D_800AC590->b4 = D_800AD528.c[player].c[2] + 23;
            }
            wave = func_80090424(ST.phase20) * 21;
            value = wave * D_800AD528.c[player].c[2] / 256;
            ST.offset90 = value;
            phase = ST.phase20;
            ST.phase20 = phase + 32;
            if (ST.phase20 >= 4096) {
                ST.phase20 = phase - 4064;
            }
            if (D_800F3A38[player].confirm28 != 0) {
                func_8004F24C(1, value);
                CLOSE_ROWS(5);
                D_800AC5D0 = 64;
                ST.step0C = 2;
            }
            cursor = CURSOR;
            if (D_800AD528.c[player].c[2] > 0) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 56, 182, ovl665_D_80119020, 0x7F38, 255);
            }
            if (D_800AD528.c[player].c[2] < 2) {
                cursor = func_8001FF84(&D_800ACC14->ordering70, cursor, 254, 182, ovl665_D_80119024, 0x7F38, 255);
            }
            CURSOR = cursor;
            break;
        }
        if (ovl665_func_8011629C(player) < 0 || D_800F3A38[player].course01 != 0xE3) {
            CLOSE_ROWS(8);
            D_800AC5D0 = 64;
            ST.step0C = 2;
            ST.next14 = 0;
        }
        LINK_PACKET();
        ovl665_func_8011643C();
        break;
    }
    if (ST.step0C < 3) {
        func_8003D6F0(0);
        D_800F4CB8 = player + 1;
        arg = 0x10072;
    } else {
        func_8003D6F0(0);
        if (D_800F3AFC->state10 == 3) {
            func_80032A14(D_800F3AFC->x04 + 10, D_800F3AFC->y06 + 40, D_800AC590);
        }
        arg = 0x83;
    }
    func_8002A344(arg);
}
