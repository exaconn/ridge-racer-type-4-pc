#ifndef RR_AUDIO_BANK_SETUP_H
#define RR_AUDIO_BANK_SETUP_H
#include "rr/audio_state_types.h"
#include "rr/spu_tone_snapshot.h"
#include "rr/spu_voice_cache.h"
#include "rr/bios_devices.h"
#include "psyq/libsnd/vmcfg.h"
/* func80050894 copies the complete program payload into a slot. */
typedef struct B {char b[0x5BC];} AudioProgramData;
typedef struct {
 const u8 *bank00;
 const u16 *data04;
 AudioProgramData *group08;
 s32 slot0C;
} AudioBankSetup;
RR_ASSERT_SIZE(AudioBankSetup,0x10);
RR_ASSERT_OFFSET(AudioBankSetup,slot0C,0x0C);
extern AudioState SoundState;
/* Alias of slots38 in the same owner; never a separate allocation. */
extern AudioSlot620 SoundSlots[4];
s32 SsVabOpenHeadSticky(const u8 *,s32,s32);
s32 SsVabTransCompleted(s32);
void SlotLoadCurves(AudioProgramData *,s32);
#endif
