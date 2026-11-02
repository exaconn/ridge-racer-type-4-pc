#ifndef RR_SPU_H
#define RR_SPU_H

#include "rr/layout.h"

/* Program attributes selected from a bank's 16-byte record table. */
typedef struct {
    u8 value00, value01, value02, value03, value04, pad05;
    u16 value06;
    u8 value08, pad09[3];
    u16 value0C, value0E;
} RrSpuProgram;

typedef struct {
    u8 bank, pad01[4], program, value;
} RrSpuSelectionPrefix;

extern RrSpuProgram *D_80110020[], *D_801100EC;
extern u8 *D_80110060[], *D_801100A8[];
extern u8 *D_801100F4, *D_801100F8;
extern s16 D_801100EA;
extern RrSpuSelectionPrefix D_80110101;
/* Signed low16 program is checked only against the upper bound; a negative
 * accepted program requires an accessible prefix in the selected allocation. */
s32 func_8008A564(s32 bank, s32 program);
s32 func_80089624(s16 bank, s16 program, s32 value);
s32 func_80089694(s16 bank, s16 program);
s32 func_800896E4(s16 bank, s16 program, s32 value);
s32 func_80089754(s16 bank, s16 program);
/* Flag storage must cover the signed low16 bank. The selected table must
 * cover program even if selection fails. Output has8 bytes, aligned for u16;
 * byte5 is preserved. Selector failure does not cancel the copy. */
s32 func_80084FB4(s32 bank, s16 program, u8 output[8]);

/* A zero mask updates the entire bank; otherwise each bit selects a halfword. */
typedef struct {
    s32 mask;
    u16 values[32];
} RrSpuRegisterPatch;

typedef struct {
    volatile u16 left, right;
    u8 pad04[12];
} RrSpuVoiceRegisters;

typedef struct {
    u32 address_flags;
    u32 size;
} RrSpuAllocation;

extern RrSpuAllocation *D_800A48C0;
/* A null table returns zero; other tables need a bit-30 marker without bit 31 set.
 * Bit 31 skips an entry even when bit 30 is also set. */
s32 func_8007FC54(u32 address);

/* Partial view of the SPU register bank addressed by D_800A47D8. */
typedef struct {
    RrSpuVoiceRegisters voices[24];
    u8 pad180[0x26];
    volatile u16 transfer_address;
    volatile u16 transfer_data;
    volatile u16 spu_control;
    volatile u16 transfer_control;
    volatile u16 spu_status;
    u8 pad1B0[0x10];
    volatile u16 control[32];
} RrSpuRegisterBank;

/* The hardware bank is one complete512-byte owner. This union also exposes
 * its indexed halfword interface without stepping across struct members. */
typedef union {
    RrSpuRegisterBank registers;
    volatile u16 halves[256];
} RrSpuRegisterStorage;
extern s32 D_800A47FC;
extern u32 D_800A4804, D_800A4808;
/* Shift is0..31; enabled rounding needs a nonzero divisor. Index is-2 (byte
 * address),-1 (low16 units), or0..255 (write units and return byte address).
 * D_800A47D8 points at registers in a complete RrSpuRegisterStorage owner. */
s32 func_8007F414(s32 index, s32 address);
RR_ASSERT_SIZE(RrSpuRegisterStorage,0x200);

/* Software volume cache: dirty flags precede the 24 voice entries. */
typedef struct {
    s16 left, right;
    u8 pad04[2];
    u16 program06, tone08, tone0A;
    u8 pad0C[4];
} RrSpuCachedVolumeEntry;

typedef struct {
    u8 flags[0x20];
    RrSpuCachedVolumeEntry voices[24];
} RrSpuVolumeCache;

/* Bank allocation state; only active banks (flag 1) are released. */
extern u8 D_80110120[];
extern u16 D_80110178;
extern u32 D_80110180[];
extern void func_8008A6F4(u32 address);

extern RrSpuVolumeCache D_8010FE18;
extern RrSpuRegisterBank *D_800A47D8;
/* Programmed I/O writes in chunks of at most64 bytes, rounding an odd final
 * byte count up to one input halfword. A zero count still restores status. */
extern volatile u16 D_800A47F0;
extern s32 D_800A47F4, D_800A4800, D_800A480C;
extern void (*volatile D_800A4810)(void);
void func_8007ED44(const u16 *source, u32 bytes);
void func_8007F59C(void);
s32 func_8007F068(s32 command, ...);
u32 func_8007F2E8(const u16 *source, u32 bytes);
u32 func_8007F36C(u16 *destination, u32 bytes);
u32 func_8008AF84(const u16 *source, u32 bytes);
extern u32 D_80110138[];
/* Low16 bank values 0..16 are accepted; state2 is transferred and becomes1. */
s32 func_8008AEC4(const u16 *source, s32 bank);
u32 func_8008AFE4(s32 address);
void func_8008B044(s32 mode);
void func_80087DA4(s32 mode);

extern u16 D_800A4D58[12], D_800A4D70[128];
/* Requires nonzero low16 pitch and the fixed PAL tuning tables. Values at
 * least0x4000 saturate to0x3FFF before conversion; caller words wrap in32bits. */
u32 func_80084D44(s32 first, s32 second, u32 pitch);
void func_8007EF04(void);
void func_8007F6EC(void);
void func_8007EFC0(s32 address, s32 transfer_address, s32 length);

RR_ASSERT_SIZE(RrSpuProgram, 0x10);
RR_ASSERT_OFFSET(RrSpuProgram, value00, 0);
RR_ASSERT_OFFSET(RrSpuProgram, value01, 1);
RR_ASSERT_OFFSET(RrSpuProgram, value02, 2);
RR_ASSERT_OFFSET(RrSpuProgram, value03, 3);
RR_ASSERT_OFFSET(RrSpuProgram, value06, 6);
RR_ASSERT_OFFSET(RrSpuProgram, value04, 4);
RR_ASSERT_OFFSET(RrSpuProgram, value08, 8);
RR_ASSERT_SIZE(RrSpuSelectionPrefix, 7);
RR_ASSERT_OFFSET(RrSpuSelectionPrefix, program, 5);
RR_ASSERT_OFFSET(RrSpuSelectionPrefix, value, 6);
RR_ASSERT_SIZE(RrSpuCachedVolumeEntry, 0x10);
RR_ASSERT_OFFSET(RrSpuCachedVolumeEntry, right, 2);
RR_ASSERT_SIZE(RrSpuVolumeCache, 0x1A0);
RR_ASSERT_OFFSET(RrSpuVolumeCache, voices, 0x20);

RR_ASSERT_SIZE(RrSpuRegisterPatch, 0x44);
RR_ASSERT_OFFSET(RrSpuRegisterPatch, values, 4);
RR_ASSERT_SIZE(RrSpuVoiceRegisters, 0x10);
RR_ASSERT_OFFSET(RrSpuVoiceRegisters, right, 2);
RR_ASSERT_SIZE(RrSpuAllocation, 8);
RR_ASSERT_OFFSET(RrSpuAllocation, size, 4);
RR_ASSERT_SIZE(RrSpuRegisterBank, 0x200);
RR_ASSERT_OFFSET(RrSpuRegisterBank, transfer_address, 0x1A6);
RR_ASSERT_OFFSET(RrSpuRegisterBank, spu_control, 0x1AA);
RR_ASSERT_OFFSET(RrSpuRegisterBank, control, 0x1C0);

#endif
