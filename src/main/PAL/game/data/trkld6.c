/* RODATA 0x800110a0 0x1c */
/* CC1_FLAGS: -fno-strength-reduce */
#include "game/data/cdfile2.h"
#include "rr/load_cursor.h"
#include "game/data/trkld1.h"
#include "game/data/trkld5.h"
#include "game/data/trkld6.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "game/sound/sndcmd.h"
#include "rr/controller_hud.h"
#include "rr/course_choices.h"
#include "game/gfx/model.h"
#include "rr/audio_state_types.h"
#include "rr/audio_loop.h"
#include "rr/sequence_queue.h"
#include "game/course/trkintp2.h"
#include "game/sound/sndvab.h"
extern int RaceContext[];
extern short SelectedMakerIndex;
extern unsigned short SavedCourseIndex;
extern short CourseIndex;
extern int D_800A0234[];
extern char *D_800ACBB4;
extern char *VabLoadArgs[2];
extern int D_800A0270[];

void SetupRaceSoundCarConfig(void);
int LoadTrophyModelStep(void);

void SetupRaceSoundCarConfig(void) {
    switch (GameModeType) {
    case 6:
        SoundState.unk19CC = 1;
        RaceContext[0x56C / 4] = 5;
        RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(0);
        RaceContext[0x574 / 4] = -1;
        SoundState.split10 = 0;
        RaceContext[0x578 / 4] = SelectedMakerIndex + 1;
        break;
    case 0:
        SoundState.unk19CC = 1;
        RaceContext[0x56C / 4] = 5;
        RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(0);
        RaceContext[0x574 / 4] = -1;
        RaceContext[0x578 / 4] = -1;
        SoundState.split10 = 0;
        break;
    default:
    case 1:
    case 5:
        SoundState.unk19CC = 0;
        RaceContext[0x56C / 4] = 4;
        RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(0);
        RaceContext[0x574 / 4] = -1;
        RaceContext[0x578 / 4] = 0;
        SoundState.split10 = 0;
        break;
    case 2:
    case 3:
        SoundState.unk19CC = 1;
        RaceContext[0x56C / 4] = 5;
        RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(0);
        RaceContext[0x574 / 4] = GetPlayerCarVariantIndex(1);
        RaceContext[0x578 / 4] = -1;
        SoundState.split10 = 1;
        break;
    case 4:
        SoundSeqVariantIndex = 1;
        RaceContext[0x56C / 4] = 5;
        if (LinkHostFlag != 0) {
            RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(0);
            RaceContext[0x574 / 4] = GetPlayerCarVariantIndex(1);
        } else {
            RaceContext[0x570 / 4] = GetPlayerCarVariantIndex(2);
            RaceContext[0x574 / 4] = GetPlayerCarVariantIndex(3);
        }
        ComSoundBankIndex = -1;
        DualPlayerSoundEnabled = 1;
        break;
    }
}

int LoadRaceSoundAndTrackStep(void) {
    switch (RaceSoundTrackLoadState) {
    case 0:
        SetupRaceSoundCarConfig();
        RaceSoundTrackLoadState += 1;
    case 1:
        if (LoadRaceSoundStep() != 0) {
            RaceLoadStepIndex = 0;
            RaceLoadState = 0;
            RaceLoadSubState = 0;
            RaceLoadCursor = LoadCursor;
            *(int *)&SavedCourseIndex = CourseIndex;
            if (CourseIndex >= 8) {
                CourseIndex = 7;
            }
            RaceSoundTrackLoadState += 1;
        }
        return 0;
    case 2:
        if (LoadTrackStep() != 0) {
            RaceSoundTrackLoadState += 1;
            CourseIndex = SavedCourseIndex;
        }
        return 0;
    case 3:
        return 1;
    default:
        return 0;
    }
}

int LoadTrackVabsStep(void)
{
    int i, i4, r, ret;
    int *s1;
    char *s3;
    char *t;
    switch (RaceLoadStepIndex) {
    case 0:
        ret = r = CdFileReadStart(GetR4FileIndex(2, 1), RaceLoadCursor);
        if (r == 0) break;
        RaceLoadStepIndex = RaceLoadStepIndex + 1;
    case 1:
        if (CdFileBytesLoaded() == 0) { ret = 0; break; }
        t = (char *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
        RegisterModelBank((RroModelBank *)(t), 7);
        i = 0;
        RelocateOffsetTable(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
        s3 = (char *)&SoundState;
        s1 = D_800A0234;
        D_800ACBB4 = (char *)(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
        do {
            VabLoadArgs[0] = (char *)(RaceLoadCursor + ((int *)(RaceLoadCursor + i * 4))[5]);
            VabLoadArgs[1] = (char *)(RaceLoadCursor + ((int *)(RaceLoadCursor + i * 4))[7]);
            LoadVab(VabLoadArgs, s1[1], s1[2]);
            s1 += 3;
            ((AudioState *)s3)->seqAddr2858[0] = (char *)(RaceLoadCursor + ((int *)(RaceLoadCursor + i * 4))[3]);
            s3 += 4;
            i++;
        } while (i < 2);
        LoadTimList(RaceLoadCursor + ((int *)RaceLoadCursor)[9]);
        t = (char *)AlignUp4(RaceLoadCursor + ((int *)RaceLoadCursor)[7]);
        LoadCursor = t;
        RaceLoadCursor = t;
        RaceLoadStepIndex = 0;
        SoundCloseVabsAndReset();
        ret = 1;
        break;
    default:
        ret = 0;
        break;
    }
    return ret;
}

int LoadTrophyModelStep(void) {
    int v;
    switch (RaceLoadStepIndex) {
    case 0:
        v = CdFileReadStart(GetR4FileIndex(2, 2), RaceLoadCursor);
        if (v != 0) {
            RaceLoadStepIndex += 1;
    case 1:
            if (CdFileBytesLoaded() != 0) {
                RegisterModelBank((RroModelBank *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]), 8);
                LoadTimList(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
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

int LoadAttractRaceStep(void) {
    int r;
    int *q;
    switch (RaceLoadStepIndex) {
    case 0:
        r = CdFileReadStart(GetR4FileIndex(2, 0), RaceLoadCursor);
        if (r != 0) {
            RaceLoadStepIndex++;
    case 1:
            if (CdFileBytesLoaded()) {
                q = D_800A0270;
                VabLoadArgs[0] = (char *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                VabLoadArgs[1] = (char *)(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                LoadVab(VabLoadArgs, q[1], q[2]);
                LoadTimList(RaceLoadCursor + ((int *)RaceLoadCursor)[3]);
                RaceLoadStepIndex = 0;
                RaceLoadCursor = RaceLoadCursor + ((int *)RaceLoadCursor)[2];
                LoadCursor = (void *)RaceLoadCursor;
                return 1;
            }
            r = 0;
        }
        break;
    default:
        r = 0;
    }
    return r;
}
