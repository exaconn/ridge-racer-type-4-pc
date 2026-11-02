#ifndef RR_AUDIO_BANK_SETUP_H
#define RR_AUDIO_BANK_SETUP_H
#include "rr/audio_state_types.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_voice_cache.h"
#include "rr/bios_devices.h"
/* func80050894 copies the complete program payload into a slot. */
typedef struct B {char b[0x5BC];} RrAudioProgramData;
typedef struct {
 const u8 *bank00;
 const u16 *data04;
 RrAudioProgramData *group08;
 s32 slot0C;
} RrAudioBankSetup;
RR_ASSERT_SIZE(RrAudioBankSetup,0x10);
RR_ASSERT_OFFSET(RrAudioBankSetup,slot0C,0x0C);
/* Bank publication writes both the signed bank half and reserved upper half. */
typedef s32 RrAudioBankStorage32 __attribute__((__may_alias__));
extern RrAudioState D_800F4DB8;
/* Alias of slots38 in the same owner; never a separate allocation. */
extern RrAudioSlot620 D_800F4DF0[4];
s32 func_8008A674(s16);
s32 func_8008AA74(const u8 *,s32,s32);
s32 func_8008B074(s32);
void func_80050894(RrAudioProgramData *,s32);
#endif
