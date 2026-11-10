/* RODATA 0x800110a0 0x1c */
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

extern char *TrackSoundPayloadPtr;

void SetupRaceSoundCarConfig(void);
int LoadTrophyModelStep(void);

/* Offsets in the track audio resource are relative to its live load base. */
typedef struct {
    u32 tag;
    s32 modelOffset,relocationOffset;
    s32 sequenceOffsets[2],bankHeaderOffsets[2],bankSampleOffsets[2];
    s32 imageListOffset;
} TrackSoundHeader;
RR_ASSERT_SIZE(TrackSoundHeader,40);
RR_ASSERT_OFFSET(TrackSoundHeader,sequenceOffsets,12);
RR_ASSERT_OFFSET(TrackSoundHeader,bankSampleOffsets,28);
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
    s32 bank, started, result;
    VabLoadConfig *configs;
    char *resource;
    switch (RaceLoadStepIndex) {
    case 0:
        result = started = CdFileReadStart(GetR4FileIndex(2, 1), RaceLoadCursor);
        if (started == 0) break;
        RaceLoadStepIndex = RaceLoadStepIndex + 1;
    case 1:
        if (CdFileBytesLoaded() == 0) { result = 0; break; }
        resource = (char *)(RaceLoadCursor + ((const TrackSoundHeader *)RaceLoadCursor)->modelOffset);
        RegisterModelBank((RroModelBank *)(resource), 7);
        bank = 0;
        RelocateOffsetTable(RaceLoadCursor + ((const TrackSoundHeader *)RaceLoadCursor)->relocationOffset);
        configs = &CourseVabLoadConfigs[1];
        TrackSoundPayloadPtr = (char *)(RaceLoadCursor + ((const TrackSoundHeader *)RaceLoadCursor)->relocationOffset);
        do {
            VabLoadArgs.header = (u8 *)(RaceLoadCursor + *(((const TrackSoundHeader *)RaceLoadCursor)->bankHeaderOffsets + bank));
            VabLoadArgs.samples = (const u16 *)(RaceLoadCursor + *(((const TrackSoundHeader *)RaceLoadCursor)->bankSampleOffsets + bank));
            LoadVab(&VabLoadArgs, configs[bank].open_mode, configs[bank].bank_slot);
            SoundState.seqAddr2858[bank] = (u8 *)(RaceLoadCursor + *(((const TrackSoundHeader *)RaceLoadCursor)->sequenceOffsets + bank));
            bank++;
        } while (bank < 2);
        LoadTimList(RaceLoadCursor + ((const TrackSoundHeader *)RaceLoadCursor)->imageListOffset);
        resource = (char *)AlignUp4((unsigned long)(RaceLoadCursor + ((const TrackSoundHeader *)RaceLoadCursor)->bankSampleOffsets[0]));
        LoadCursor = resource;
        RaceLoadCursor = resource;
        RaceLoadStepIndex = 0;
        SoundCloseVabsAndReset();
        result = 1;
        break;
    default:
        result = 0;
        break;
    }
    return result;
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
    VabLoadConfig *q;
    switch (RaceLoadStepIndex) {
    case 0:
        r = CdFileReadStart(GetR4FileIndex(2, 0), RaceLoadCursor);
        if (r != 0) {
            RaceLoadStepIndex++;
    case 1:
            if (CdFileBytesLoaded()) {
                q = &CourseVabLoadConfigs[6];
                VabLoadArgs.header = (u8 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[1]);
                VabLoadArgs.samples = (const u16 *)(RaceLoadCursor + ((int *)RaceLoadCursor)[2]);
                LoadVab(&VabLoadArgs, q->open_mode, q->bank_slot);
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
