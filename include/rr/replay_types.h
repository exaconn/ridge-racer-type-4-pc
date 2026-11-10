#ifndef RR_REPLAY_TYPES_H
#define RR_REPLAY_TYPES_H
#include "rr/race_types.h"

/* Four encoded input bytes; byte-aligned transfers also support packed streams. */
typedef struct {u32 word;} __attribute__((packed)) ReplayFrame4;
typedef struct {
    ReplayFrame4 inputs[3601];
    RaceVehicle snapshots[8];
} ReplaySingleStorage;
typedef struct {
    ReplayFrame4 inputs[2][2701];
    RaceVehicle snapshots[4];
} ReplayTwoStorage;
typedef struct {
    ReplayFrame4 inputs[4][901];
    RaceVehicle snapshots[4];
} ReplayLinkStorage;
typedef union {
    ReplaySingleStorage single;
    ReplayTwoStorage two;
    ReplayLinkStorage link;
} ReplayStorage;
extern ReplayFrame4 *ReplayInputs[4];

RR_ASSERT_SIZE(ReplayFrame4,4);
#if defined(mips)
RR_ASSERT_OFFSET(ReplaySingleStorage,snapshots,0x3844);
RR_ASSERT_OFFSET(ReplayTwoStorage,snapshots,0x5468);
RR_ASSERT_OFFSET(ReplayLinkStorage,snapshots,0x3850);
RR_ASSERT_SIZE(ReplayStorage,0x60E8);
#endif
#endif
