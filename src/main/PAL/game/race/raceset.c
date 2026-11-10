#include "game/sound/sndque.h"
#include "rr/vehicle_table.h"
#include "rr/race_types.h"
#include "rr/menu_choice.h"
#include "game/ui/uipart1.h"
#include "game/race/raceset.h"
#include "rr/overlay661_init.h"
#include "rr/audio_loop.h"
#include "rr/menu_transition.h"
#include "game/menu/menu8.h"
#include "game/race/racesave.h"
#include "game/sound/sndctrl.h"
#include "game/race/racesetp.h"

extern u8 D_8009F910[];
extern s16 D_800AC5D0;
extern s32 CourseBestLap[2][8];
extern s16 ReverseCourse;
extern s32 TrackLength;
extern s32 CourseBestLapTimes[2][8][3];
extern s16 CourseIndex;
extern s32 NumLaps;
extern s32 CourseBestLapRecords[];
extern s32 LapFrameCounts[];
extern s32 TotalRaceTime;
extern s32 BestLapTime;
extern s32 LapTimes[];
extern s32 RaceState;
extern s32 BestSplitTimes[3];
extern s32 SplitTimes[3];
extern s32 D_8010C12C;
extern s16 D_8010C134;
extern s32 SplitDistances[3];
extern short NumCars;
extern unsigned short GameTypeSceneIds[];

void UpdateLapTiming(RaceVehicle *arg0);

void UpdateLapTiming(RaceVehicle *arg0) {
    RaceVehicleControl *s0;
    s16 v1;
    s32 msg;
    s32 target;

    s0 = &arg0->control;
    v1 = s0->dynamics94.steering.count22;
    if (v1 > 0) {
        if (v1 <= NumLaps) {
            LapFrameCounts[v1 - 1] += 1;
            if (LapFrameCounts[s0->dynamics94.steering.count22 - 1] > 0xFFFF) {
                LapFrameCounts[s0->dynamics94.steering.count22 - 1] = 0x10000;
            }
            LapTimes[s0->dynamics94.steering.count22 - 1] = FramesToMilliseconds(
                LapFrameCounts[s0->dynamics94.steering.count22 - 1],
                PsyqRand() % 40
            );
            if (LapTimes[s0->dynamics94.steering.count22 - 1] > 0x927BE) {
                LapTimes[s0->dynamics94.steering.count22 - 1] = 0x927BF;
            }
            if (GameModeType == 0) {
                SplitTimes[D_8010C12C] = LapTimes[s0->dynamics94.steering.count22 - 1];
            }
        }
    }
    if (s0->dynamics94.steering.count22 * TrackLength <= arg0->second + arg0->first) {
        if (s0->dynamics94.steering.count22 <= NumLaps) {
            s0->dynamics94.steering.count22 = (u16)s0->dynamics94.steering.count22 + 1;
            if (s0->dynamics94.steering.count22 == 1) {
                SplitDeltaTimer = 0;
            } else {
                SplitDeltaTimer = 0x3C;
            }
            D_8010C108 = SplitTimes[D_8010C12C];
            D_8010C10C = BestSplitTimes[D_8010C12C];
            D_8010C104 = D_8010C12C;
            D_8010C12C = 0;
            D_8010C134 = 0;
            if (BestLapTime > LapTimes[s0->dynamics94.steering.count22 - 2]) {
                if (s0->dynamics94.steering.count22 != 1) {
                    SoundQueueEnqueue(9, 0);
                    BestLapTime = LapTimes[s0->dynamics94.steering.count22 - 2];
                    s0->dynamics94.steering.finish20 = (u16)s0->dynamics94.steering.count22 - 2;
                    if (GameModeType == 0) {
                        BestSplitTimes[0] = SplitTimes[0];
                        BestSplitTimes[1] = SplitTimes[1];
                        BestSplitTimes[2] = SplitTimes[2];
                    }
                }
            }
            if ((u16)(CourseIndex - 6) < 2U) {
                target = NumLaps - 1;
            } else {
                target = NumLaps;
            }
            if (s0->dynamics94.steering.count22 == target) {
                SaveCarSnapshots(0);
            } else if (s0->dynamics94.steering.count22 > NumLaps) {
                if (RaceState < 4) {
                    D_8010C134 = -1;
                    RaceState = 4;
                    D_800AC5D0 = 0x4B;
                    SoundSetMasterVolume(0, 0x47);
                    msg = 0x13;
                    if (GameModeType == 1) {
                        /* ASM_BARRIER: clobber msg on entry to mode-1 branch so CSE retains msg = 0x13 and frees $a0 for phase17 */
                        __asm__ ("" : "=r"(msg));
                        if (arg0->half1EE <= D_8009F910[SelectionState.phase17 / 2]) {
                            msg = 0x13;
                        } else {
                            if (SelectionState.retries08[SelectionState.phase17] > 0) {
                                msg = 0x14;
                            } else {
                                msg = -1;
                            }
                        }
                    }
                    if (msg >= 0) {
                        SoundQueueEnqueue(msg, 1);
                    }
                    if (arg0->half1EE < 8 && GameModeType < 2) {
                        msg = 0;
                        TotalRaceTime = 0;
                        if (NumLaps > 0) {
                            do {
                                TotalRaceTime += LapTimes[msg];
                                msg++;
                            } while (msg < NumLaps);
                        }
                        if (TotalRaceTime > 0x927BE) {
                            TotalRaceTime = 0x927BF;
                        }
                        if (GameModeType == 0) {
                            if (CourseBestTotal[ReverseCourse][CourseIndex] > TotalRaceTime) {
                                CourseBestTotal[ReverseCourse][CourseIndex] = TotalRaceTime;
                            }
                            if (CourseBestLap[ReverseCourse][CourseIndex] > BestLapTime) {
                                CourseBestLap[ReverseCourse][CourseIndex] = BestLapTime;
                            }
                            CourseBestLapTimes[ReverseCourse][CourseIndex][0] = BestSplitTimes[0];
                            CourseBestLapTimes[ReverseCourse][CourseIndex][1] = BestSplitTimes[1];
                            CourseBestLapTimes[ReverseCourse][CourseIndex][2] = BestSplitTimes[2];
                        } else if (GameModeType == 1) {
                            if (CourseBestTotalTimes[CourseIndex] > TotalRaceTime) {
                                CourseBestTotalTimes[CourseIndex] = TotalRaceTime;
                            }
                            if (CourseBestLapRecords[CourseIndex] > BestLapTime) {
                                CourseBestLapRecords[CourseIndex] = BestLapTime;
                            }
                        }
                    }
                }
            }
        } else {
            goto check_split;
        }
    } else {
check_split:
        if (SplitDistances[D_8010C12C] + TrackLength * (s0->dynamics94.steering.count22 - 1) <= arg0->second + arg0->first) {
            if (GameModeType == 0) {
                SplitDeltaTimer = 0x3C;
                D_8010C104 = D_8010C12C;
                D_8010C108 = SplitTimes[D_8010C12C];
                D_8010C10C = BestSplitTimes[D_8010C12C];
                D_8010C12C += 1;
            }
        }
    }
    if (D_8010C134 == 0) {
        msg = 9 - (NumLaps - s0->dynamics94.steering.count22);
        if (msg == 9) {
            msg = 0xF;
        }
        if (msg >= 6) {
            SoundQueueEnqueue(msg, 0);
        }
        D_8010C134 = -1;
    }
}

void UpdateRaceRanking(void)
{
  int i;
  int j;
  if (RaceState < 4)
  {
    for (i = 0; i < NumCars; i++)
    {
      RankEntry8 *q = RankTable[i];
      RaceVehicle **o = &CarTable[q->carIndex04];
      q->score00 = (*o)->first + (*o)->second;
      (*o)->half1EE = i;
    }
    for (i = 0; i < NumCars - 1; i++)
    {
      for (j = NumCars - 1; i < j; j--)
      {
        RankEntry8 *a2 = RankTable[j - 1];
        RankEntry8 *a1 = RankTable[j];
        int a0 = a2->score00;
        int v1 = a1->score00;
        if (a0 < v1 || (a0 == v1 && a1->carIndex04 == 0))
        {
          RankTable[j] = a2;
          RankTable[j - 1] = a1;
        }
      }
    }
  }
}

void SetupRaceAndSceneId(void) {
    SetupRace();
    ActiveSceneId = GameTypeSceneIds[GameModeType];
}
