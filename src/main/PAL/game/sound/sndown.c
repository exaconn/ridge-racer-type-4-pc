#include "types.h"
#include "rr/audio_bank_setup.h"

typedef struct {
    u32 flags00;
    s32 bank04;
    s32 voice08;
    s32 count0C;
    s32 level10;
    s32 program14;
    s32 previous18;
    s32 reserved1C;
    s16 level20;
    s16 gain22;
    s16 pan24;
    s16 reserved26;
    u8 pad28[0x5DC - 0x28];
    s16 programs5DC[2][5];
    u16 duration5F0;
    u16 gain5F2;
    u16 spread5F4;
    u16 reserved5F6;
    struct {
        s16 coarse;
        s16 fine;
    } pitch5F8[2][5];
} RrSlot620;

typedef struct {
    u8 pad00[0x30];
    s16 bank30;
    u8 pad32[6];
    RrSlot620 slots38[4];
} RrAudioOwner;

typedef struct {
    s32 bank00;
    s32 data04;
    s32 group08;
} RrAudioSetupArg;

void func_80050978(RrAudioSetupArg *arg0);
void func_80050BCC(RrAudioBankSetup *arg0);

/* legacy typing of shared globals in this part of the unit */
#define D_800F4DB8 (*(RrAudioOwner (*))&D_800F4DB8)
#define func_8008A674 ((void (*)())func_8008A674)
#define func_80096434 ((void (*)())func_80096434)
#define func_8008AA74 ((s32 (*)())func_8008AA74)
#define func_8008AEC4 ((s32 (*)())func_8008AEC4)
#define func_8008B074 ((void (*)())func_8008B074)
#define func_80050894 ((void (*)())func_80050894)
#define func_800850C4 ((void (*)())func_800850C4)

void func_80050978(RrAudioSetupArg *arg0) {
    u8 sp10[32];
    s32 i, j, k;

    if (D_800F4DB8.bank30 >= 0) {
        func_8008A674(D_800F4DB8.bank30);
        D_800F4DB8.bank30 = -1;
    }

    func_80096434(&D_800F4DB8.slots38[2], 0x620);
    func_80096434(&D_800F4DB8.slots38[3], 0x620);

    if (arg0->bank00 != -1) {
        D_800F4DB8.bank30 = func_8008AA74(arg0->bank00, -1, 0x5D000);
        D_800F4DB8.slots38[2].bank04 = D_800F4DB8.slots38[3].bank04 = D_800F4DB8.bank30;
    }

    if (arg0->data04 != -1) {
        D_800F4DB8.bank30 = func_8008AEC4(arg0->data04, (s16)D_800F4DB8.slots38[2].bank04);
        D_800F4DB8.slots38[2].bank04 = D_800F4DB8.slots38[3].bank04 = D_800F4DB8.bank30;
        func_8008B074(1);
    }

    if (arg0->group08 != -1) {
        func_80050894(arg0->group08, 2);
        func_80050894(arg0->group08, 3);
    }

    for (j = 2; j < 4; j++) {
        D_800F4DB8.slots38[j].flags00 = 1;
        D_800F4DB8.slots38[j].level20 = 0x56;
        D_800F4DB8.slots38[j].gain22 = 0x80;
        D_800F4DB8.slots38[j].pan24 = 0x40;
        D_800F4DB8.slots38[j].reserved26 = 0;
        D_800F4DB8.slots38[j].count0C = 3;
    }

    D_800F4DB8.slots38[2].voice08 = 0xE;
    D_800F4DB8.slots38[3].voice08 = 0x11;

    for (i = 2; i < 4; i++) {
        for (j = 0; j < 5; j++) {
            func_800850C4((s16)D_800F4DB8.slots38[i].bank04, D_800F4DB8.slots38[i].programs5DC[0][j], 0, sp10);
            k = (i & 1) * 2 + 0x3C;
            D_800F4DB8.slots38[i].pitch5F8[0][j].coarse = k - sp10[12];
            D_800F4DB8.slots38[i].pitch5F8[0][j].fine = (sp10[13] + sp10[12]) << 7;
            func_800850C4((s16)D_800F4DB8.slots38[i].bank04, D_800F4DB8.slots38[i].programs5DC[1][j], 0, sp10);
            D_800F4DB8.slots38[i].pitch5F8[1][j].coarse = k - sp10[12];
            D_800F4DB8.slots38[i].pitch5F8[1][j].fine = (sp10[13] + sp10[12]) << 7;
        }
    }
}

#undef D_800F4DB8
#undef func_8008A674
#undef func_80096434
#undef func_8008AA74
#undef func_8008AEC4
#undef func_8008B074
#undef func_80050894
#undef func_800850C4

void func_80050BCC(RrAudioBankSetup *arg0)
{
  register u8 *bankOwner asm("$23");
  register u8 *read asm("$18");
  u8 *loopRoot;register s32 zeroArg asm("$6");register u8 *toneArg asm("$7");
  s32 new_var; u8 *slotBase;
  register u8 *write asm("$17");
  register s32 readOffset asm("$19");
  register s32 writeOffset asm("$20");
  register s32 semitone asm("$16");
register s32 channelOffset asm("$4");
  s32 channels[2];
  RrSpuTone32 toneRecord;
  register u8 *tone asm("$22");
  register s32 bankIndex asm("$18");
  register s32 offset asm("$16");
  register s32 i asm("$21");
  s32 mode;register s32 voice asm("$2");
  register s32 *channel asm("$19");
  RrAudioSlot620 *slot;
  register RrAudioState *owner asm("$20");
  u8 *programCursor;
  u8 *pitchCursor;
  s16 handle;register s32 calculation asm("$2");
  register RrAudioState *bankView asm("$3");
  register RrAudioState *bankPointer asm("$16");
  register RrAudioBankSetup *input asm("$22") = arg0;
  new_var = input->slot0C;
  bankIndex = new_var;
  channels[0] = 5;
  channels[1] = 6;slotBase=(u8*)D_800F4DF0;asm("" : : "r"(slotBase));
  asm("" : "+r"(input));
  calculation=((bankIndex*3)<<4)+bankIndex;asm("" : "+r"(calculation));offset=calculation<<5;
  channelOffset=bankIndex<<2;channel=(s32*)((u8*)channels+channelOffset);
  owner = (RrAudioState *) (slotBase - 0x38);
  handle = *((s16 *) ((((u8 *) owner) + 0x24) + ((*channel) * 2)));
  slot = (RrAudioSlot620 *) (slotBase + offset);
  
  if (handle > (0 - 1))
  {
    func_8008A674(handle);
    *((s16 *) ((((u8 *) owner) + 0x24) + ((*channel) * 2))) = -1;
  }
  func_80096434(slot, 0x620);
  mode = 0x5D000;
  if (bankIndex == 0)
  {
    mode = 0x6F000;
  }
  if (input->bank00 != ((const u8 *) (~0UL)))
  {
    handle = (s16) func_8008AA74(input->bank00, -1, mode);
    *((s16 *) ((((u8 *) owner) + 0x24) + ((*channel) * 2))) = handle;
    bankView = (RrAudioState *) (((u8 *) owner) + offset);
    *((RrAudioBankStorage32*)&bankView->slots38[0].bank04) = (s32) handle;
  }
  if (input->data04 != ((const u16 *) (~0UL)))
  {
    bankPointer = (RrAudioState *) (((u8 *) owner) + offset);
    handle = (s16) func_8008AEC4(input->data04, bankPointer->slots38[0].bank04);
    *((s16 *) ((((u8 *) owner) + 0x24) + ((*channel) * 2))) = handle;
    *((RrAudioBankStorage32*)&bankPointer->slots38[0].bank04) = (s32) handle;
    func_8008B074(1);
  }
  if (input->group08 != (RrAudioProgramData*)~0UL)
  {
    func_80050894(input->group08, bankIndex);
  }
  slot->flags00 = 1;
  slot->levels20.halves.level = 0x4B;
  slot->levels20.halves.gain = 0x80;
  slot->pan24 = 0x40;
  slot->reserved26 = 0;
  if (!bankIndex)
  {
    asm("");voice=20;
  }
  else
  {
    voice = 0x10;
  }
  slot->voice08 = voice;
  slot->count0C = 4;asm("" : : : "$21", "$22", "$2", "memory");
  i = 0;
  loopRoot = (u8 *) (&D_800F4DB8);asm("" : : "r"(loopRoot));
  calculation=((bankIndex*3)<<4)+bankIndex;asm("" : "+r"(calculation));calculation<<=5;
  bankOwner=loopRoot+calculation;
tone=(u8*)&toneRecord;
readOffset=calculation;writeOffset=readOffset;
  do
  {
zeroArg=0;toneArg=tone;
    read = (u8*)((unsigned long)readOffset+(unsigned long)loopRoot);
    write = (u8*)((unsigned long)writeOffset+(unsigned long)loopRoot);
asm("" : : "r"(write));
    func_800850C4(*((s16 *) (bankOwner + 0x3C)), *((s16 *) (read + 0x614)), zeroArg, toneArg);
    writeOffset = writeOffset + 4;
    readOffset = readOffset + 2;
    semitone = 0x3C;
    *((s16 *) (write + 0x630)) = (s16) (semitone - ((u8 *) (&toneRecord))[12]);
    *((s16 *) (write + 0x632)) = (s16) ((((u8 *) (&toneRecord))[13] + ((u8 *) (&toneRecord))[12]) << 7);
    func_800850C4(*((s16 *) (bankOwner + 0x3C)), *((s16 *) (read + 0x61E)), 0, tone);
    *((s16 *) (write + 0x644)) = (s16) (semitone - ((u8 *) (&toneRecord))[12]);
    i++;
    *((s16 *) (write + 0x646)) = (s16) ((((u8 *) (&toneRecord))[13] + ((u8 *) (&toneRecord))[12]) << 7);
  }
  while (i <= (5 - 1));
}
