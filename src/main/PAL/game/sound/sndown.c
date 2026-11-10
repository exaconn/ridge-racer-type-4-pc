#include "types.h"
#include "rr/audio_bank_setup.h"
#include "game/sound/sndown.h"

void SlotSetupBank(AudioBankSetup *arg0);
void SetPlayerVoice(int a0, int a1, int a2, int a3);

void SlotSetupBank(AudioBankSetup *arg0)
{
  s32 zeroArg;
  u8 *toneArg;
  s32 new_var; u8 *slotBase;
  AudioState *write;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register s32 readOffset asm("$19");
  s32 writeOffset;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register s32 semitone asm("$16");
  s32 channelOffset;
  s32 channels[2];
  u8 toneRecord[32];
  u8 *tone;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  s32 bankIndex;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register s32 offset asm("$16");
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register s32 i asm("$21");
  s32 mode;s32 voice;
  s32 *channel;
  AudioSlot620 *slot;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register AudioState *owner asm("$20");
  u8 *programCursor;
  u8 *pitchCursor;
  s16 handle;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register s32 calculation asm("$2");
  AudioState *bankView;
  AudioState *bankPointer;
  /* ASM_BARRIER: retain this phase value in its original MIPS register. */
  register AudioBankSetup *input asm("$22") = arg0;
  AudioState *bankOwner;
  AudioState *read;
  u8 *loopRoot;
  new_var = input->slot0C;
  bankIndex = new_var;
  channels[0] = 5;
  channels[1] = 6;slotBase=(u8*)SoundSlots;
  /* ASM_BARRIER: preserve this arithmetic or helper boundary. */
  /* ASM_BARRIER: preserve this arithmetic or helper boundary. */
  asm("" : "+r"(input));
  calculation=((bankIndex*3)<<4)+bankIndex;offset=calculation<<5;
  channelOffset=bankIndex<<2;channel=(s32*)((u8*)channels+channelOffset);
  owner = (AudioState *) (slotBase - 0x38);
  handle = owner->bank24[*channel];
  slot = (AudioSlot620 *) (slotBase + offset);
  
  if (handle > (0 - 1))
  {
    SsVabClose(handle);
    owner->bank24[*channel] = -1;
  }
  PsyqBzero(slot, 0x620);
  mode = 0x5D000;
  if (bankIndex == 0)
  {
    mode = 0x6F000;
  }
  if (input->bank00 != ((const u8 *) (~0UL)))
  {
    handle = (s16) SsVabOpenHeadSticky(input->bank00, -1, mode);
    owner->bank24[*channel] = handle;
    bankView = (AudioState *) (((u8 *) owner) + offset);
    *((AudioBankStorage32*)&bankView->slots38[0].bank04) = handle;
  }
  if (input->data04 != ((const u16 *) (~0UL)))
  {
    bankPointer = (AudioState *) (((u8 *) owner) + offset);
    handle = (s16) SsVabTransBody(input->data04, bankPointer->slots38[0].bank04);
    owner->bank24[*channel] = handle;
    *((AudioBankStorage32*)&bankPointer->slots38[0].bank04) = handle;
    SsVabTransCompleted(1);
  }
  if (input->group08 != (AudioProgramData*)~0UL)
  {
    SlotLoadCurves(input->group08, bankIndex);
  }
  slot->flags00 = 1;
  slot->levels20.halves.level = 0x4B;
  slot->levels20.halves.gain = 0x80;
  slot->pan24 = 0x40;
  slot->reserved26 = 0;
  if (!bankIndex)
  {
    /* ASM_BARRIER: preserve branch structure. */
    asm("");voice=20;
  }
  else
  {
    voice = 0x10;
  }
  slot->voice08 = voice;
  slot->count0C = 4;
  /* ASM_BARRIER: preserve register live ranges before loop. */
  asm("" : : : "$21", "$22", "$2", "memory");
  i = 0;
  loopRoot = (u8 *) (&SoundState);
  /* ASM_BARRIER: preserve this arithmetic or helper boundary. */
  asm("" : : "r"(loopRoot));
  calculation=((bankIndex*3)<<4)+bankIndex;calculation<<=5;
  bankOwner=(AudioState *)(loopRoot+calculation);
  tone=toneRecord;
  readOffset=calculation;writeOffset=readOffset;
  do
  {
    zeroArg=0;toneArg=tone;
    read = (AudioState *)(readOffset+(unsigned long)loopRoot);
    write = (AudioState *)(writeOffset+(unsigned long)loopRoot);
    SsUtGetVagAtr(bankOwner->slots38[0].bank04, (s16)read->slots38[0].programs5DC[0][0], zeroArg, toneArg);
    writeOffset = writeOffset + 4;
    readOffset = readOffset + 2;
    semitone = 0x3C;
    write->slots38[0].pitch5F8[0][0].coarse = (s16) (semitone - toneRecord[12]);
    write->slots38[0].pitch5F8[0][0].fine = (s16) ((toneRecord[13] + toneRecord[12]) << 7);
    SsUtGetVagAtr(bankOwner->slots38[0].bank04, (s16)read->slots38[0].programs5DC[1][0], 0, tone);
    write->slots38[0].pitch5F8[1][0].coarse = (s16) (semitone - toneRecord[12]);
    i++;
    write->slots38[0].pitch5F8[1][0].fine = (s16) ((toneRecord[13] + toneRecord[12]) << 7);
  }
  while (i <= (5 - 1));
}

void ResetPlayerVoices(void) {
    int i;
    for (i = 0; i < 2; i++) {
        SoundState.playerVoices195C[i].mode08 = 0;
        SoundState.playerVoices195C[i].bank00 = SoundState.bank24[1];
        SoundState.playerVoices195C[i].voice04 = i + 12;
    }
}

void SetPlayerVoice(int a0, int a1, int a2, int a3) {
    SoundState.playerVoices195C[a3].mode08 = a0;
    if (a0 != 0) {
        SoundState.playerVoices195C[a3].pitch10 = a1;
        SoundState.playerVoices195C[a3].vol14 = a2;
    }
}
