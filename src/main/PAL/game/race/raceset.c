#include "rr/race_types.h"
#include "rr/menu_choice.h"
#include "game/ui/uipart1.h"
#include "game/race/raceset.h"
#include "rr/overlay661_init.h"
#include "rr/audio_loop.h"
#include "rr/menu_transition.h"
#include "game/race/raceset.h"

typedef struct { int key; short idx; } W10_Q;

typedef struct { char p[0x184]; int a; int b; char q[0x62]; short n; } W10_O;

void SaveCarSnapshots(s32);
void *SoundQueueEnqueue(s32, s32);
void SoundSetMasterVolume(s16, s16);
extern u8 D_8009F910[];
extern s16 D_800AC5D0;
extern s32 CourseBestLap[2][8];
extern s16 ReverseCourse;
extern s32 TrackLength;
extern s32 D_800F3838[2][8][3];
extern s16 CourseIndex;
extern s32 NumLaps;
extern s32 D_800F3B00[];
extern s32 LapFrameCounts[];
extern s32 TotalRaceTime;
extern s32 BestLapTime;
extern s32 LapTimes[];
extern s32 RaceState;
extern s32 D_8010C0F8[3];
extern s32 D_8010C120[3];
extern s32 D_8010C12C;
extern s16 D_8010C134;
extern s32 D_8010C140[3];
extern short NumCars;
extern W10_Q *RankTable[];
extern W10_O *CarTable[];
extern void SetupRace();
extern unsigned short D_8009F914[];

void UpdateLapTiming(RrRaceVehicle *arg0);
void UpdateRaceRanking(void);
void func_8003CAB4(void);

void UpdateLapTiming(RrRaceVehicle *arg0) {
    RrRaceVehicleControl *s0;
    s16 v1;
    s32 msg;
    s32 target;

    s0 = (RrRaceVehicleControl *)((u8 *)arg0 + 0x1F4);
    v1 = s0->steering94.count22;
    if (v1 > 0) {
        if (v1 <= NumLaps) {
            LapFrameCounts[v1 - 1] += 1;
            if (LapFrameCounts[s0->steering94.count22 - 1] > 0xFFFF) {
                LapFrameCounts[s0->steering94.count22 - 1] = 0x10000;
            }
            LapTimes[s0->steering94.count22 - 1] = FramesToMilliseconds(
                LapFrameCounts[s0->steering94.count22 - 1],
                func_80096464() % 40
            );
            if (LapTimes[s0->steering94.count22 - 1] > 0x927BE) {
                LapTimes[s0->steering94.count22 - 1] = 0x927BF;
            }
            if (D_800F4D44 == 0) {
                D_8010C120[D_8010C12C] = LapTimes[s0->steering94.count22 - 1];
            }
        }
    }
    if (s0->steering94.count22 * TrackLength <= arg0->second + arg0->first) {
        if (s0->steering94.count22 <= NumLaps) {
            s0->steering94.count22 = (u16)s0->steering94.count22 + 1;
            if (s0->steering94.count22 == 1) {
                D_8010C114 = 0;
            } else {
                D_8010C114 = 0x3C;
            }
            D_8010C108 = D_8010C120[D_8010C12C];
            D_8010C10C = D_8010C0F8[D_8010C12C];
            D_8010C104 = D_8010C12C;
            D_8010C12C = 0;
            D_8010C134 = 0;
            if (BestLapTime > LapTimes[s0->steering94.count22 - 2]) {
                if (s0->steering94.count22 != 1) {
                    SoundQueueEnqueue(9, 0);
                    BestLapTime = LapTimes[s0->steering94.count22 - 2];
                    s0->steering94.finish20 = (u16)s0->steering94.count22 - 2;
                    if (D_800F4D44 == 0) {
                        D_8010C0F8[0] = D_8010C120[0];
                        D_8010C0F8[1] = D_8010C120[1];
                        D_8010C0F8[2] = D_8010C120[2];
                    }
                }
            }
            if ((u16)(CourseIndex - 6) < 2U) {
                target = NumLaps - 1;
            } else {
                target = NumLaps;
            }
            if (s0->steering94.count22 == target) {
                SaveCarSnapshots(0);
            } else if (s0->steering94.count22 > NumLaps) {
                if (RaceState < 4) {
                    D_8010C134 = -1;
                    RaceState = 4;
                    D_800AC5D0 = 0x4B;
                    SoundSetMasterVolume(0, 0x47);
                    msg = 0x13;
                    if (D_800F4D44 == 1) {
                        /* ASM_BARRIER: clobber msg on entry to mode-1 branch so CSE retains msg = 0x13 and frees $a0 for phase17 */
                        __asm__ volatile("" : "=r"(msg));
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
                    if (arg0->half1EE < 8 && D_800F4D44 < 2) {
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
                        if (D_800F4D44 == 0) {
                            if (CourseBestTotal[ReverseCourse][CourseIndex] > TotalRaceTime) {
                                CourseBestTotal[ReverseCourse][CourseIndex] = TotalRaceTime;
                            }
                            if (CourseBestLap[ReverseCourse][CourseIndex] > BestLapTime) {
                                CourseBestLap[ReverseCourse][CourseIndex] = BestLapTime;
                            }
                            D_800F3838[ReverseCourse][CourseIndex][0] = D_8010C0F8[0];
                            D_800F3838[ReverseCourse][CourseIndex][1] = D_8010C0F8[1];
                            D_800F3838[ReverseCourse][CourseIndex][2] = D_8010C0F8[2];
                        } else if (D_800F4D44 == 1) {
                            if (CourseBestTotalTimes[CourseIndex] > TotalRaceTime) {
                                CourseBestTotalTimes[CourseIndex] = TotalRaceTime;
                            }
                            if (D_800F3B00[CourseIndex] > BestLapTime) {
                                D_800F3B00[CourseIndex] = BestLapTime;
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
        if (D_8010C140[D_8010C12C] + TrackLength * (s0->steering94.count22 - 1) <= arg0->second + arg0->first) {
            if (D_800F4D44 == 0) {
                D_8010C114 = 0x3C;
                D_8010C104 = D_8010C12C;
                D_8010C108 = D_8010C120[D_8010C12C];
                D_8010C10C = D_8010C0F8[D_8010C12C];
                D_8010C12C += 1;
            }
        }
    }
    if (D_8010C134 == 0) {
        msg = 9 - (NumLaps - s0->steering94.count22);
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
      W10_Q *q = RankTable[i];
      W10_O **o = &CarTable[q->idx];
      q->key = (*o)->a + (*o)->b;
      (*o)->n = i;
    }
    for (i = 0; i < NumCars - 1; i++)
    {
      for (j = NumCars - 1; i < j; j--)
      {
        W10_Q *a2 = RankTable[j - 1];
        W10_Q *a1 = RankTable[j];
        int a0 = a2->key;
        int v1 = a1->key;
        if (a0 < v1 || (a0 == v1 && a1->idx == 0))
        {
          RankTable[j] = a2;
          RankTable[j - 1] = a1;
        }
      }
    }
  }
}

void func_8003CAB4(void) {
    SetupRace();
    D_800AD50A = D_8009F914[D_800F4D44];
}
