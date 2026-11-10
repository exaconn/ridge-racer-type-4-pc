/* RODATA 0x80011068 0x20 */
#include "types.h"
#include "game/sound/sndvab.h"
#include "rr/load_cursor.h"
#include "game/data/cdfile2.h"
#include "game/course/trkres2.h"
#include "game/data/trkld1.h"
#include "game/gfx/gfxotag.h"
#include "game/data/cdfile1.h"
#include "rr/course_choices.h"
#include "game/gfx/model.h"
#include "rr/audio_bank_setup.h"
#include "game/sound/sndown.h"
#include "game/sound/sndsys.h"
#include "rr/audio_loop.h"
#include "game/course/trkintp2.h"

extern int D_800A02BC;
extern int D_800A029C[];
extern AudioBankSetup SlotBankSetup;

extern int D_8010C218;
extern int RaceContext[];
extern int D_800ACBBC;

void SetRaceLoadCursor(char *arg0);
char *GetRaceLoadCursor(void);
int LoadSlotBankStep(int a0, int a1);
int LoadComSlotBankStep(int a0);
int LoadSoundBanksStep();
int LoadSfxVabStep(int a0, int a1);
s32 LoadMenuSoundSeqStep(void);

int LoadCarSpecStep(int a0, int a1, int a2) {
  int v;
  int t;
  int e;
  e = 0x44;
  v = 0x1E;
  switch (RaceLoadState)
  {
    case 0:
      switch (GameModeType)
    {
      case 1:
        D_800A02BC = GetR4FileIndex(2, D_800A029C[a0] + (a2 + v));
        break;

      case 3:
        D_800A02BC = GetR4FileIndex(2, (a0 * 4) + (a2 + e));
        break;

      case 5:
        D_800A02BC = GetR4FileIndex(2, a0 + 0x74);
        break;

      case 6:
        D_800A02BC = GetR4FileIndex(2, (a0 * 2) + 0x64);
        break;

      default:
        RaceLoadState = 0;
        return 1;

    }

    case 1:
      v = CdFileReadStart(D_800A02BC, RaceLoadCursor);
      if (v != 0)
    {
      RaceLoadState++;
    case 2:
      t = CdFileBytesLoaded();
      if (t != 0)
      {
        SetCarSpecTable(RaceLoadCursor);
        v = AlignUp4(t);
        RaceLoadState = 0;
        RaceLoadCursor += v;
        return 1;
      }
      v = 0;
    }
      return v;
    default:
      return 0;
  }
}

void SetRaceLoadCursor(char *arg0) {
    RaceLoadCursor = arg0;
    RaceLoadState = 0;
}

char *GetRaceLoadCursor(void) {
    return RaceLoadCursor;
}

int LoadMenuCarStep(int a0, int a1) {
    int v, t, e;
    e = 0x86;
    switch (RaceLoadState) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, (a0 * 4) + (a1 + e)), RaceLoadCursor);
        if (v != 0) {
            RaceLoadState++;
    case 1:
            t = CdFileBytesLoaded();
            if (t != 0) {
                LoadTimList((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                LoadTimImage((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                RaceLoadState++;
            }
            v = 0;
        }
        return v;
    case 2:
        v = CdFileReadStart(GetR4FileIndex(2, 0x96), RaceLoadCursor);
        if (v != 0) {
            RaceLoadState++;
    case 3:
            t = CdFileBytesLoaded();
            if (t != 0) {
                RegisterModelBank((RroModelBank *)((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[1]), 8);
                RelocateOffsetTable((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                RaceContext[0x560 / 4] = ((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                ((int *)RaceContext[0x24C / 4])[0x138 / 4] = ((char *)RaceLoadCursor + ((int *)RaceLoadCursor)[3]);
                v = AlignUp4(t);
                RaceLoadState = 0;
                RaceLoadCursor += v;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}

int LoadCourseAssetsStep(int a0, int a1) {
    int v, t;
    switch (RaceLoadState) {
    case 0:
        if (GameModeType != 1) return 1;
        v = CdFileReadStart(GetR4FileIndex(2, a1 * 2 + 0x7D), RaceLoadCursor);
        if (v == 0) goto out;
        RaceLoadState++;
        goto c1;
    case 2:
        v = CdFileReadStart(GetR4FileIndex(2, a1 * 2 + 0x7C), RaceLoadCursor);
        if (v == 0) goto out;
        RaceLoadState++;
    case 3:
        t = CdFileBytesLoaded();
        if (t != 0) {
            RegisterModelBank((RroModelBank *)(RaceLoadCursor), 8);
            v = AlignUp4(t);
            RaceLoadCursor += v;
            if (D_8009FE60[a0] < 7) {
                RaceLoadState = 0;
                return 1;
            }
            goto inc;
        }
        return 0;
    case 4:
        v = CdFileReadStart(GetR4FileIndex(2, 0x85), RaceLoadCursor);
        if (v == 0) goto out;
        RaceLoadState++;
    case 1:
    case 5:
    c1:
        t = CdFileBytesLoaded();
        if (t != 0) {
            LoadTimList(RaceLoadCursor);
        inc:
            RaceLoadState++;
        }
        return 0;
    case 6:
        v = CdFileReadStart(GetR4FileIndex(2, 0x84), RaceLoadCursor);
        if (v == 0) goto out;
        RaceLoadState++;
    case 7:
        t = CdFileBytesLoaded();
        if (t != 0) {
            RegisterModelBank((RroModelBank *)(RaceLoadCursor), 9);
            v = AlignUp4(t);
            RaceLoadCursor += v;
            RaceLoadState = 0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
out:
    return v;
}

int LoadSlotBankStep(int a0, int a1) {
    int v;
    if (a0 < 0) return 1;
    switch (RaceLoadSubState) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, a0 + 3), RaceLoadCursor);
        if (v != 0) {
            RaceLoadSubState += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                SlotBankSetup.group08 = (const u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                SlotBankSetup.bank00 = (u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                SlotBankSetup.data04 = (const u16 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[3]);
                SlotBankSetup.slot0C = a1;
                RaceLoadCursor += AlignUp4(((int *)RaceLoadCursor)[3]);
                RaceLoadSubState += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        SlotSetupBank(&SlotBankSetup);
        RaceLoadSubState = 0;
        return 1;
    default:
        return 0;
    }
}

int LoadComSlotBankStep(int a0) {
    int v;
    if (a0 < 0) return 1;
    switch (RaceLoadSubState) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, a0 + 0xC), RaceLoadCursor);
        if (v != 0) {
            RaceLoadSubState += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                SlotBankSetup.group08 = (const u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                SlotBankSetup.bank00 = (u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                SlotBankSetup.data04 = (const u16 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[3]);
                SlotBankSetup.slot0C = 0;
                RaceLoadCursor += AlignUp4(((int *)RaceLoadCursor)[3]);
                RaceLoadSubState += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        ComSlotSetupBank(&SlotBankSetup);
        RaceLoadSubState = 0;
        return 1;
    default:
        return 0;
    }
}

int LoadSoundBanksStep(int a0, int a1, int a2) {
    switch (RaceLoadState) {
    case 0:
        if (LoadSlotBankStep(a0, 0) == 0) return 0;
        if (a1 >= 0) RaceLoadState = 1;
        else RaceLoadState = 2;
        break;
    case 1:
        if (LoadSlotBankStep(a1, 1)) {
            RaceLoadState = 0;
            return 1;
        }
        break;
    case 2:
        if (LoadComSlotBankStep(a2)) {
            RaceLoadState = 0;
            return 1;
        }
    }
    return 0;
}

int LoadSfxVabStep(int a0, int a1) {
    int prefixWords = 0;
    int sampleBytes;
    VabLoadConfig *config = &D_800A0228[a0];
    int readStatus;
    char *samples;
    switch (RaceLoadSubState) {
    case 0:
        readStatus = CdFileReadStart(GetR4FileIndex(2, config->file_index), RaceLoadCursor);
        if (readStatus != 0) {
            RaceLoadSubState += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                if (a1 > 0) prefixWords += a1;
                VabLoadArgs.header = (u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[prefixWords + 1]);
                samples = RaceLoadCursor + ((int *)RaceLoadCursor)[prefixWords + 2];
                VabLoadArgs.samples = (const u16 *)samples;
                sampleBytes = samples - RaceLoadCursor;
                LoadVab(&VabLoadArgs, config->open_mode, config->bank_slot);
                if (a1 > 0) {
                    SoundState.seqAddr2858[D_8010C218] = (char *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                }
                sampleBytes = AlignUp4(sampleBytes);
                RaceLoadSubState = 0;
                RaceLoadCursor += sampleBytes;
                return 1;
            }
            readStatus = 0;
        }
        return readStatus;
    default:
        return 0;
    }
}

void RaceLoadReset(void) {
    RaceLoadStepIndex = 0;
    RaceLoadState = 0;
    RaceLoadSubState = 0;
    RaceSoundTrackLoadState = 0;
    RaceLoadCursor = LoadCursor;
}

int LoadVabPairStep(char *a0) {
    int v;
    VabLoadConfig *t = D_800A0228;
    switch (RaceLoadStepIndex) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, 0x11), a0);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                RaceLoadStepIndex += 1;
            }
            v = 0;
        }
        return v;
    case 2:
        v = CdFileReadStart(GetR4FileIndex(2, 0x12), RaceLoadCursor);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 3:
            if (CdFileBytesLoaded() != 0) {
                VabLoadArgs.header = (u8 *)a0;
                VabLoadArgs.samples = (const u16 *)RaceLoadCursor;
                LoadVab(&VabLoadArgs, t->open_mode, t->bank_slot);
                RaceLoadStepIndex = 0;
                return 1;
            }
            v = 0;
        }
        return v;
    default:
        return 0;
    }
}

s32 LoadMenuSoundVabStep(void) {
    return 1;
}

s32 LoadMenuSoundSeqStep(void) {
    return 1;
}

int LoadRaceSoundStep(void) {
    switch (RaceLoadStepIndex) {
    case 0:
        if (LoadSfxVabStep(3, 0) != 0) {
            RaceLoadStepIndex += 1;
        }
        return 0;
    case 1:
        if (LoadSfxVabStep(D_800ACBBC, 0) != 0) {
            RaceLoadStepIndex += 1;
        }
        return 0;
    case 2:
        if (LoadSoundBanksStep(RaceContext[0x570 / 4], RaceContext[0x574 / 4], RaceContext[0x578 / 4]) != 0) {
            RaceLoadStepIndex = 0;
            LoadCursor = RaceLoadCursor;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
}
