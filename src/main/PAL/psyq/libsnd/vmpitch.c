/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/spu.h"
#include "rr/channel_pitch.h"
#include "psyq/libsnd/vmnote2.h"
#include "psyq/libsnd/vmpitch.h"

u32 _spu_note2pitch(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u32 _spu_pitch2note(s32 first,s32 second,u32 pitch);
s32 SsUtChangePitch(s32 raw_input, s32 bank, s32 program, s32 group, s32 unused, u32 first_input, u32 second_input);

u32 _spu_note2pitch(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 sum;
    s32 diff;
    s32 q;
    s32 shift;
    s32 rem;
    s32 rem2;
    s32 neg_shift;
    u32 res;

    arg3 += arg1;
    sum = arg3 & 0xFFFF;
    diff = (s16)((arg2 + (sum >> 7)) - arg0);
    arg3 = ({
        /* ASM_BARRIER: Keep the masked sum in $v1 for both the octave shift and fine mask. */
        register s32 s __asm__("$3") = sum;
        __asm__("" : "+r"(s));
        s & 0x7F;
    });
    q = diff / 12;
    shift = q - 2;
    /* ASM_BARRIER: Compute initial shift before the remainder check. */
    __asm__("" : "+r"(shift));
    rem = diff - (q * 12);
    if ((s16)rem < 0) {
        rem2 = rem + 12;
        shift = q - 3;
    } else {
        rem2 = rem;
    }
    /* ASM_BARRIER: Keep the unadjusted remainder live in $a0 so the positive path copies into $v1 in the delay slot. */
    __asm__("" : "+r"(rem2) : "r"(rem));
    res = (SvmSemitoneTable[(s16)rem2] * SvmPitchFineTable[(u16)arg3]) >> 16;
    if ((s16)shift >= 0) {
        res = 0x3FFF;
    } else {
        neg_shift = -(s16)shift;
        res += 1 << (neg_shift - 1);
        res >>= neg_shift;
    }
    return (u16)res;
}

/* Nonzero low16 pitch, with the fixed PAL tables: both searches always match. */
u32 _spu_pitch2note(s32 first,s32 second,u32 pitch) {
 s32 top=0;
 s32 i;
 s32 note;
 /* ASM_BARRIER: Preserve the fine-search result and accumulated caller word. */
 register s32 fine __asm__("$9");
 s32 value;
 u16 *table;
 u32 result;
 if((u16)pitch>=0x4000)pitch=0x3FFF;
 i=0;value=(u16)pitch;
 do {if((value>>i)&1)top=i;i++;}while(i<14);
 pitch=(u16)pitch<<(15-top);
 i=11;value=(u16)pitch;
 table=&SvmSemitoneTable[11];
 for(;i>=0;i--,table--) {
  if((u32)value>=*table){note=i;break;}
 }
 value=((u32)(u16)pitch<<15)/SvmSemitoneTable[(u16)note];
{
  u16 *fine_table;
  i=127;
  fine_table=&SvmPitchFineTable[127];
  value=(u16)value;
  for(;i>=0;i--,fine_table--) {
   if((u32)value>=*fine_table){fine=i;break;}
  }
 }
 {
  u32 total=(fine+1);
  u32 part;
  fine=second+total;
  part=(top-12);
  total=note+(first+part*12U);
  {
   first=(u16)fine;
   part=((u32)first)>>7;
   note=total+part;
   result=((u16)note)*0x100U;
   /* ASM_BARRIER: Finish the note bits before masking the independent fine bits. */
   __asm__("" : "+r"(result), "+r"(first));
   first&=0x7E;
   return result|first;
  }
 }
}

s32 SsUtChangePitch(s32 raw_input, s32 bank, s32 program, s32 group, s32 unused, u32 first_input, u32 second_input)
{
    s32 raw_voice=raw_input;
    u32 first=(u16)first_input;
    u32 second=(u16)second_input;
    s32 voice, offset;
    s16 *state;
    u8 *byte_page;
    s32 selected_bank;
    s32 selected_program;
    /* ASM_BARRIER: stage the bank address in the eventual first selector argument. */
    register u8 *bank_page __asm__("$4");
    /* ASM_BARRIER: stage the program address after the incoming bank argument dies. */
    register u8 *program_page __asm__("$5");
    /* ASM_BARRIER: use the original temporary for the final selector address. */
    register u8 *group_page __asm__("$3");
    u16 pitch;
    u32 pitch_first;
    u8 *flags_page;
    u8 flags;
    if ((u16)raw_voice >= 24) return -1;
    voice = (s16)raw_voice;
    offset = voice * sizeof(ChannelControlRow36);
    /* ASM_BARRIER: finish the row offset before replacing the incoming a0 value. */
    __asm__("" : : "r"(offset) : "$4");
    bank_page=(u8*)0x80110000U;
    /* ASM_BARRIER: retain the actual PAL bank-field page for indexed addressing. */
    __asm__("" : "+r"(bank_page));
    selected_bank=((ChannelControlRow36 *)(bank_page+offset-0x6F8))->match18;
    if (selected_bank != (s16)bank) return -1;
    program_page=(u8*)0x80110000U;
    /* ASM_BARRIER: retain the actual PAL program-field page for indexed addressing. */
    __asm__("" : "+r"(program_page));
    selected_program=((ChannelControlRow36 *)(program_page+offset-0x6F8))->match14;
    if (selected_program != (s16)program) return -1;
    group_page=(u8*)0x80110000U;
if (((ChannelControlRow36 *)(group_page+offset-0x6F8))->group0E != (s16)group) return -1;
    _SsVmVSetUp(selected_bank, selected_program);
    pitch_first=first;
    state = D_8011010C.fields08;
    state[0] = 0x21;
    state[2] = raw_voice;
byte_page=(u8*)0x80110000U;
((SpuPitchRequest*)((u8*)state-8))->instrument00=*(byte_page+offset-0x6E2);
    pitch = note2pitch2(pitch_first,second);
    *(u16 *)((voice * sizeof(SpuCachedVolumeEntry)) + _svm_sreg_dirty.voices[0].pad04) = pitch;
flags_page=(u8*)0x80110000U;
flags=*(flags_page+voice-0x1E8);
    _svm_sreg_dirty.flags[voice]=flags|4;
    return 0;
}
