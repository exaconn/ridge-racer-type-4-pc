#ifndef RR_RACE_AUDIO_RESOURCES_H
#define RR_RACE_AUDIO_RESOURCES_H

#include "rr/layout.h"

/* Signed section offsets are relative to the live file base. These are wire
 * headers; their words are never used as native pointer cells. */
typedef struct {
    u32 reserved00;
    s32 modelBankOffset, relocationOffset;
    s32 sequenceOffsets[2], bankHeaderOffsets[2], bankSampleOffsets[2];
    s32 imageListOffset;
} TrackAudioResourceHeader;

typedef struct {
    u32 reserved00;
    s32 modelBankOffset, imageListOffset;
} TrophyResourceHeader;

typedef struct {
    u32 reserved00;
    s32 bankHeaderOffset, bankSamplesOffset, imageListOffset;
} AttractAudioResourceHeader;

typedef struct {
    u32 reserved00;
    s32 imageListOffset, imageOffset;
} MenuCarImageResourceHeader;

typedef struct {
    u32 reserved00;
    s32 modelBankOffset, meshTableOffset, cameraTracksOffset;
} MenuCarModelResourceHeader;

RR_ASSERT_SIZE(TrackAudioResourceHeader, 40);
RR_ASSERT_OFFSET(TrackAudioResourceHeader, sequenceOffsets, 12);
RR_ASSERT_OFFSET(TrackAudioResourceHeader, bankHeaderOffsets, 20);
RR_ASSERT_OFFSET(TrackAudioResourceHeader, bankSampleOffsets, 28);
RR_ASSERT_OFFSET(TrackAudioResourceHeader, imageListOffset, 36);
RR_ASSERT_SIZE(TrophyResourceHeader, 12);
RR_ASSERT_SIZE(AttractAudioResourceHeader, 16);
RR_ASSERT_SIZE(MenuCarImageResourceHeader, 12);
RR_ASSERT_SIZE(MenuCarModelResourceHeader, 16);

#endif
