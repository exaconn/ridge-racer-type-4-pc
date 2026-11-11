#ifndef RR_OVERLAY661_RACE_CONTROLLER_H
#define RR_OVERLAY661_RACE_CONTROLLER_H
#include "rr/overlay660_race_controller.h"
#include "scratchpad.h"
#include "rr/race_checkpoint_types.h"
typedef struct {s32 x,y,z,w;} Rr661FrameVector16;
/* Scratch view: the GPU pointer words at 0 live in RR_GPU_SCRATCH. */
typedef struct {
    u8 gpu_slot00[8];
    Rr661FrameVector16 position08,rotation18;
    u8 reserved28[0x5E - 0x28];
    s16 mode5E;
} Rr661FrameScratch;
#ifndef RR_661_FRAME_SCRATCH
#define RR_661_FRAME_SCRATCH ((Rr661FrameScratch *)SCRATCHPAD)
#endif
#ifndef RR_661_FRAME_MODE
#define RR_661_FRAME_MODE (RR_661_FRAME_SCRATCH->mode5E)
#endif
extern Rr661FrameVector16 ovl661_camera_rotations[2],ovl661_camera_positions[2];
extern s32 TotalRaceTime;
extern s16 ovl661_finish_ranks[2];
void DrawHudNumber5(s32,s32);
void UpdateHudRankDisplay(s32,s32);
void DrawLapCounter(s32,s32);
void UpdateFinalLapHudSprite(s32);
void DrawSplitScreenDivider(s32);
void DrawTimeRemainingForPlayer(s32,s32);
void SoundSetMasterVolume(s16,s16);
void ovl661_update_checkpoint(RaceVehicle *);
/* The view keeps the PAL offsets: an 8-byte placeholder for the GPU
 * pointers (RR_GPU_SCRATCH), then two four-word vectors and the mode word.
 * Services may replace frame roots and mutate both complete vector tables,
 * vehicle/input owners and the later-read counters. Frame/tick arithmetic
 * wraps32; rendering visits both players and publishes current scratch state. */
void ovl661_update_race(void);
RR_ASSERT_SIZE(Rr661FrameVector16,0x10);
RR_ASSERT_OFFSET(Rr661FrameScratch,position08,0x08);
RR_ASSERT_OFFSET(Rr661FrameScratch,rotation18,0x18);
RR_ASSERT_OFFSET(Rr661FrameScratch,mode5E,0x5E);
RR_ASSERT_SIZE(Rr661FrameScratch,0x60);
#endif
