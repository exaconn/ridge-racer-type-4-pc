#ifndef RR_GAME_SYSTEM_PAD_H
#define RR_GAME_SYSTEM_PAD_H

#include "rr/layout.h"

/* These words are consumed both as packed input and as individual fields.
 * Byte views describe protocol payloads; they are not separate controller views. */
typedef struct {u8 status, type;u16 buttons;} ControllerInputFields4;
typedef union {u32 word;ControllerInputFields4 fields;} ControllerInputWord4;
typedef struct {u16 pressed, repeat;} ControllerEdgeFields4;
typedef union {u32 word;ControllerEdgeFields4 fields;} ControllerEdges4;
typedef union {u32 word;u8 bytes[4];s8 signedBytes[4];} ControllerAxes4;

typedef struct {
    ControllerInputWord4 input00;
    u16 previous04, released06;
    ControllerEdges4 edges08;
    u16 repeatTick0C, reserved0E;
    s32 protocol10;
    ControllerAxes4 axes14;
    u8 reserved18, output19[2], flag1B;
    s16 kind1C, changed1E;
    s16 steering20, pressure22, pressure24, pressure26;
    s32 action28, action2C, action30, action34;
    u32 reserved38;
    s16 steering3C, reserved3E;
    u8 flag40, position41, position42, position43, actuator44, position45;
    s8 rumble46;
    u8 rumble47, rumbleFlags48, rumbleLevel49;
    s16 reserved4A;
    u16 position4C, previous4E;
    s16 rumbleActive50, rumbleDir52;
    s32 idleStage54, idleTick58;
} ControllerState5C;

typedef struct {
    u8 status00, type01, data02, data03;
    ControllerAxes4 analog04;
    u8 reserved08[32];
} PadPort28;

typedef struct {
    s16 limit00[2];
    s16 repeat04, once06;
    u16 repeatTick08, onceTick0A;
    s16 repeatStep0C, onceStep0E;
    s16 delay10[2];
    s16 active14, reserved16;
} PadPulseState18;


RR_ASSERT_SIZE(ControllerInputFields4, 4);
RR_ASSERT_SIZE(ControllerInputWord4, 4);
RR_ASSERT_OFFSET(ControllerInputFields4, status, 0);
RR_ASSERT_OFFSET(ControllerInputFields4, type, 1);
RR_ASSERT_OFFSET(ControllerInputFields4, buttons, 2);
RR_ASSERT_SIZE(ControllerEdgeFields4, 4);
RR_ASSERT_SIZE(ControllerEdges4, 4);
RR_ASSERT_OFFSET(ControllerEdgeFields4, pressed, 0);
RR_ASSERT_OFFSET(ControllerEdgeFields4, repeat, 2);
RR_ASSERT_SIZE(ControllerAxes4, 4);
RR_ASSERT_SIZE(PadPort28, 0x28);
RR_ASSERT_OFFSET(PadPort28, analog04, 4);
RR_ASSERT_OFFSET(PadPort28, reserved08, 8);
RR_ASSERT_SIZE(PadPulseState18, 0x18);
RR_ASSERT_OFFSET(PadPulseState18, repeat04, 4);
RR_ASSERT_OFFSET(PadPulseState18, once06, 6);
RR_ASSERT_OFFSET(PadPulseState18, repeatTick08, 8);
RR_ASSERT_OFFSET(PadPulseState18, onceTick0A, 10);
RR_ASSERT_OFFSET(PadPulseState18, repeatStep0C, 12);
RR_ASSERT_OFFSET(PadPulseState18, onceStep0E, 14);
RR_ASSERT_OFFSET(PadPulseState18, delay10, 16);
RR_ASSERT_OFFSET(PadPulseState18, active14, 20);
RR_ASSERT_SIZE(ControllerState5C, 0x5C);
RR_ASSERT_OFFSET(ControllerState5C, input00, 0);
RR_ASSERT_OFFSET(ControllerState5C, edges08, 8);
RR_ASSERT_OFFSET(ControllerState5C, protocol10, 0x10);
RR_ASSERT_OFFSET(ControllerState5C, axes14, 0x14);
RR_ASSERT_OFFSET(ControllerState5C, output19, 0x19);
RR_ASSERT_OFFSET(ControllerState5C, kind1C, 0x1C);
RR_ASSERT_OFFSET(ControllerState5C, steering20, 0x20);
RR_ASSERT_OFFSET(ControllerState5C, action28, 0x28);
RR_ASSERT_OFFSET(ControllerState5C, action2C, 0x2C);
RR_ASSERT_OFFSET(ControllerState5C, action30, 0x30);
RR_ASSERT_OFFSET(ControllerState5C, steering3C, 0x3C);
RR_ASSERT_OFFSET(ControllerState5C, flag40, 0x40);
RR_ASSERT_OFFSET(ControllerState5C, rumbleActive50, 0x50);

extern ControllerState5C PlayerControllers[2];
extern PadPort28 PadPorts[2];
extern PadPulseState18 PadPulseState[2];

void PadUpdate(void);
int GetControllerWarning(int i);
void PadRangeCheckSet(unsigned a, unsigned b, short c, short d);
void PadStartPulse(int i, int k, int v);
void PadClearPulse(int i);

#endif /* RR_GAME_SYSTEM_PAD_H */
