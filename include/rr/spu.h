#ifndef RR_SPU_H
#define RR_SPU_H

#include "rr/layout.h"
#include "psyq/libsnd/vab_format.h"
#include "psyq/libspu/spumode.h"

typedef struct {
    u8 bank, pad01[4], program, value;
} SpuSelectionPrefix;

/* Resource cursors also support byte/halfword aliases of live SDK state.
 * The parser's complete ProgAtr view is used only for word-aligned buffers. */
extern ProgAtrBuffer *_svm_vab_pg[], *_svm_pg;
extern u8 *_svm_vab_vh[], *_svm_vh;
extern VagAtrBuffer *_svm_vab_tn[], *_svm_tn;
extern s16 kMaxPrograms;
extern SpuSelectionPrefix D_80110101;
/* Signed low16 program is checked only against the upper bound; a negative
 * accepted program requires an accessible prefix in the selected allocation. */
s32 _SsVmVSetUp(s32 bank, s32 program);
s32 _SsVmSetProgVol(s16 bank, s16 program, s32 value);
s32 _SsVmGetProgVol(s16 bank, s16 program);
s32 _SsVmSetProgPan(s16 bank, s16 program, s32 value);
s32 _SsVmGetProgPan(s16 bank, s16 program);
/* Flag storage must cover the signed low16 bank. The selected table must
 * cover program even if selection fails. Output has8 bytes, aligned for u16;
 * byte5 is preserved. Selector failure does not cancel the copy. */
s32 SsUtGetProgAtr(s32 bank, s16 program, u8 output[8]);

/* A zero mask updates the entire bank; otherwise each bit selects a halfword. */
typedef struct {
    s32 mask;
    u16 values[32];
} SpuRegisterPatch;

/* Actual MMIO channel registers: every halfword access must reach hardware. */
typedef struct {
    volatile u16 left, right;
    volatile u16 pitch, start_address;
    volatile u16 adsr1, adsr2;
    volatile u16 envx, repeat_address;
} SpuVoiceRegisters;

typedef struct {
    u32 address_flags;
    u32 size;
} SpuAllocation;

extern SpuAllocation *_spu_memList;
/* A null table returns zero; other tables need a bit-30 marker without bit 31 set.
 * Bit 31 skips an entry even when bit 30 is also set. */
s32 _SpuIsInAllocateArea(u32 address);

/* Known registers in the complete bank addressed by _spu_RXX.
 * KON/KOF and PMON/NON/EON each split the 24 voice bits into two halfwords.
 * Register map: https://psx-spx.consoledev.net/soundprocessingunitspu/ */
typedef struct {
    SpuVoiceRegisters voices[24];
    volatile u16 main_vol_left, main_vol_right;
    volatile u16 reverb_vol_left, reverb_vol_right;
    volatile u16 key_on[2], key_off[2];
    volatile u16 pitch_modulation[2], noise_enable[2], reverb_enable[2];
    u8 reserved19C[0xA];
    volatile u16 transfer_address;
    volatile u16 transfer_data;
    volatile u16 spu_control;
    volatile u16 transfer_control;
    volatile u16 spu_status;
    volatile u16 cd_vol_left, cd_vol_right;
    volatile u16 ext_vol_left, ext_vol_right;
    volatile u16 curr_vol_left, curr_vol_right;
    volatile u16 voice_on1, voice_on2;
    volatile u16 control[32];
} SpuRegisterBank;

/* The hardware bank is one complete512-byte owner. This union also exposes
 * its indexed halfword interface without stepping across struct members. */
typedef union {
    SpuRegisterBank registers;
    volatile u16 halves[256];
} SpuRegisterStorage;
extern s32 _spu_mem_mode;
extern u32 _spu_mem_mode_unit, _spu_mem_mode_unitM;
/* Shift is0..31; enabled rounding needs a nonzero divisor. Index is-2 (byte
 * address),-1 (low16 units), or0..255 (write units and return byte address).
 * _spu_RXX points at registers in a complete SpuRegisterStorage owner. */
s32 _spu_FsetRXXa(s32 index, s32 address);
RR_ASSERT_SIZE(SpuRegisterStorage,0x200);

/* Software volume cache: dirty flags precede the 24 voice entries. */
typedef struct {
    s16 left, right;
    u8 pad04[2];
    u16 program06, tone08, tone0A;
    u8 pad0C[4];
} SpuCachedVolumeEntry;

typedef struct {
    u8 flags[0x20];
    SpuCachedVolumeEntry voices[24];
} SpuVolumeCache;

/* Bank allocation state; only active banks (flag 1) are released. */
extern u8 _svm_vab_used[];
extern u16 _svm_vab_count;
extern u32 _svm_vab_start[];
extern void SpuFree(u32 address);

extern SpuVolumeCache _svm_sreg_dirty;
extern SpuCachedVolumeEntry _svm_sreg_buf[];
extern SpuRegisterBank *_spu_RXX;
/* Programmed I/O writes in chunks of at most64 bytes, rounding an odd final
 * byte count up to one input halfword. A zero count still restores status. */
#ifndef __mips__
extern volatile u16 _spu_tsa;
#else
extern u16 _spu_tsa;
#endif
extern s32 _spu_transMode, _spu_mem_mode_plus, _spu_inTransfer;
extern void (*volatile _spu_transferCallback)(void);
void _spu_FwriteByIO(const u16 *source, u32 bytes);
void _spu_Fw1ts(void);
s32 _spu_t(s32 command, ...);
u32 _spu_Fw(const u16 *source, u32 bytes);
u32 _spu_Fr(u16 *destination, u32 bytes);
u32 SpuWrite(const u16 *source, u32 bytes);
extern u32 _svm_vab_total[];
/* Low16 bank values 0..16 are accepted; state2 is transferred and becomes1. */
s32 SsVabTransBody(const u16 *source, s32 bank);
u32 SpuSetTransferStartAddr(s32 address);
void _spu_setInTransfer(s32 mode);

extern u16 SvmSemitoneTable[12], SvmPitchFineTable[128];
/* Requires nonzero low16 pitch and the fixed PAL tuning tables. Values at
 * least0x4000 saturate to0x3FFF before conversion; caller words wrap in32bits. */
u32 _spu_pitch2note(s32 first, s32 second, u32 pitch);
void _spu_FiDMA(void);
void SpuStart(void);
void _spu_Fr_(s32 address, s32 transfer_address, s32 length);

RR_ASSERT_SIZE(SpuSelectionPrefix, 7);
RR_ASSERT_OFFSET(SpuSelectionPrefix, program, 5);
RR_ASSERT_OFFSET(SpuSelectionPrefix, value, 6);
RR_ASSERT_SIZE(SpuCachedVolumeEntry, 0x10);
RR_ASSERT_OFFSET(SpuCachedVolumeEntry, right, 2);
RR_ASSERT_SIZE(SpuVolumeCache, 0x1A0);
RR_ASSERT_OFFSET(SpuVolumeCache, voices, 0x20);

RR_ASSERT_SIZE(SpuRegisterPatch, 0x44);
RR_ASSERT_OFFSET(SpuRegisterPatch, values, 4);
RR_ASSERT_SIZE(SpuVoiceRegisters, 0x10);
RR_ASSERT_OFFSET(SpuVoiceRegisters, left, 0);
RR_ASSERT_OFFSET(SpuVoiceRegisters, right, 2);
RR_ASSERT_OFFSET(SpuVoiceRegisters, pitch, 4);
RR_ASSERT_OFFSET(SpuVoiceRegisters, start_address, 6);
RR_ASSERT_OFFSET(SpuVoiceRegisters, adsr1, 8);
RR_ASSERT_OFFSET(SpuVoiceRegisters, adsr2, 10);
RR_ASSERT_OFFSET(SpuVoiceRegisters, envx, 12);
RR_ASSERT_OFFSET(SpuVoiceRegisters, repeat_address, 14);
RR_ASSERT_SIZE(SpuAllocation, 8);
RR_ASSERT_OFFSET(SpuAllocation, size, 4);
RR_ASSERT_SIZE(SpuRegisterBank, 0x200);
RR_ASSERT_OFFSET(SpuRegisterBank, reverb_vol_left, 0x184);
RR_ASSERT_OFFSET(SpuRegisterBank, reverb_vol_right, 0x186);
RR_ASSERT_OFFSET(SpuRegisterBank, key_on, 0x188);
RR_ASSERT_OFFSET(SpuRegisterBank, key_off, 0x18C);
RR_ASSERT_OFFSET(SpuRegisterBank, pitch_modulation, 0x190);
RR_ASSERT_OFFSET(SpuRegisterBank, noise_enable, 0x194);
RR_ASSERT_OFFSET(SpuRegisterBank, reverb_enable, 0x198);
RR_ASSERT_OFFSET(SpuRegisterBank, transfer_address, 0x1A6);
RR_ASSERT_OFFSET(SpuRegisterBank, spu_control, 0x1AA);
RR_ASSERT_OFFSET(SpuRegisterBank, control, 0x1C0);

#endif
