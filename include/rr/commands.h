#ifndef RR_COMMANDS_H
#define RR_COMMANDS_H

#include "rr/layout.h"

typedef s32 (*RrCommandReadCallback)(void);
typedef s32 (*RrCommandWriteCallback)(s32);

/* Command 2 retains a buffer here; other commands use an integer parameter. */
typedef union {
    s32 value;
    u8 *buffer;
} RrCommandParameter;

/* Prefix beginning at D_80114718. Callback signatures follow their call sites. */
typedef struct {
    u8 *buffer;
    void *first_result; /* Byte buffer or word result, depending on the command. */
    u32 *second_result;
    s32 parameter24;
    s32 count28;
    RrCommandParameter parameter2C;
    s32 result30;
    RrCommandReadCallback read;
    RrCommandWriteCallback write;
} RrCommandContextPrefix;

extern s32 D_800ABDF0;
extern u8 D_800ABDC0[];
/* Received and transmitted command bytes, indexed by D_800ABDF8/F4. */
extern u8 D_80114740[], D_801147D8[];
s32 func_8009BC3C(void);
s32 func_8009C0F4(void);
s32 func_8009C0A4(s32 request);

extern s32 D_800ABDE4, D_800ABDE8, D_800ABDEC, D_800ABDF4, D_800ABDF8;
extern u8 *D_80114718;
extern void *D_8011471C;
extern u32 *D_80114720;
extern s32 D_80114724, D_80114728, D_80114730, D_80114870;
extern RrCommandParameter D_8011472C;
extern RrCommandReadCallback D_80114734;
extern RrCommandWriteCallback D_80114738;

/* Command 11 block transfer and its checksum-producing byte reader. */
extern s32 D_80114710;
extern u8 D_80114714;
s32 func_8009D8B0(s32 request, s32 block, s32 mode);
s32 func_8009D978(void);
s32 func_8009DB18(s32 value);

/* Command 101 response: handshake, then two words assembled high byte first. */
s32 func_8009DE54(s32 value);

static __inline__ s32 rr_command_begin(s32 request) {
    if (!(D_800ABDE4 & 1)) return 0;
    D_800ABDF4 = 0;
    D_800ABDF8 = 0;
    D_80114730 = 0;
    D_800ABDE8 = 0;
    D_800ABDEC = request;
    return 1;
}

RR_ASSERT_SIZE(RrCommandParameter, 4);
RR_ASSERT_SIZE(RrCommandContextPrefix, 0x24);
RR_ASSERT_OFFSET(RrCommandContextPrefix, first_result, 4);
RR_ASSERT_OFFSET(RrCommandContextPrefix, second_result, 8);
RR_ASSERT_OFFSET(RrCommandContextPrefix, parameter24, 0xC);
RR_ASSERT_OFFSET(RrCommandContextPrefix, count28, 0x10);
RR_ASSERT_OFFSET(RrCommandContextPrefix, parameter2C, 0x14);
RR_ASSERT_OFFSET(RrCommandContextPrefix, result30, 0x18);
RR_ASSERT_OFFSET(RrCommandContextPrefix, read, 0x1C);
RR_ASSERT_OFFSET(RrCommandContextPrefix, write, 0x20);

#endif
