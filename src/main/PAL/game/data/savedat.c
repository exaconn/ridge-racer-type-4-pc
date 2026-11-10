/* RODATA 0x800114a0 0xc4 */
#include "game/gfx/color.h"
#include "psyq/libc/printf.h"
#include "game/data/savedat.h"
#include "game/data/savetype.h"
#include "game/data/globals.h"
#include "rr/menu_selection.h"
#include "rr/overlay662_frame_step.h"
#include "rr/overlay668_playback_step.h"
#include "game/race/raceset.h"
#include "rr/course_choices.h"
#include "rr/audio_loop.h"
#include "rr/overlay667_course_initializer.h"
#include "rr/string_tail_copy.h"
typedef struct { char b[0x80]; } B;

extern int CourseBestLap[2][8];
extern Struct10 RaceRecords[2][8][5];
extern Struct2C SaveSlotSelections[3];
extern int CourseBestLapTimes[2][8][3];
extern Struct840 *D_800F3910;
extern Struct840 *D_800F3918;
extern int D_800F3B28[];
extern short GpSaveSlotCursor;
extern Struct10 LapRecords[2][8][5];
extern Struct2C D_8010B258[3];
extern CourseChoice4 D_8010BFD8[2][2][9];
extern StructC D_80118420[];
extern CourseChoice4 D_80118458[2][2][9];
extern int D_801272F0[];
extern char StrFmtPercentS[];
extern GpuRectangle D_8010EF98;
extern char D_80011568[];
extern GpuRectangle D_8010EFA0;

void BuildCardHeaderAnim(char *a0, int a1, int a2, int a3, int a4, int a5);

int ApplySaveData(int arg0) {
    int a2 = 0;
    SaveBuf *s3;
    SaveBuf11 *p;
    int a1;
    int s0;
    int t0;
    int k;
    int v10;
    int v11;

    switch (arg0) {
    case 0:
    case 1:
    case 0x30:
    default:
        s3 = SaveBuffer;
        a1 = 0;
        for (s0 = 0; s0 < 0x6A9U; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            SaveProgress = s3->f280;
            Options = s3->f2FC;
            for (s0 = 0; s0 < 3; s0++) {
                if (arg0 == 0x30) {
                    SaveSlotSelections[s0] = s3->f360[s0];
                } else {
                    D_8010B258[s0] = s3->f360[s0];
                }
            }
            for (s0 = 0; s0 < 9; s0++) {
                CourseChoiceTable[1][s0] = s3->f33C[s0];
            }
            for (s0 = 0; s0 < 2; s0++) {
                for (t0 = 0; t0 < 8; t0++) {
                    for (k = 0; k < 3; k++) {
                        CourseBestLapTimes[s0][t0][k] = s3->f3E4[s0][t0][k];
                    }
                    for (k = 0; k < 5; k++) {
                        LapRecords[s0][t0][k] = s3->f4A4[s0][t0][k];
                        RaceRecords[s0][t0][k] = s3->f9A4[s0][t0][k];
                    }
                    CourseBestLap[s0][t0] = LapRecords[s0][t0][0].w[3];
                    CourseBestTotal[s0][t0] = RaceRecords[s0][t0][0].w[3];
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                D_800F3B28[s0] = s3->fEA4[s0];
            }
            ApplySoundVolumes();
            a2 = 1;
        }
        break;
    case 2:
        s3 = SaveBuffer;
        a1 = 0;
        for (s0 = 0; s0 < 0x6A9U; s0++) {
            a1 += ((int *)s3)[s0];
        }
        if (s3->f1AA4 == a1) {
            for (s0 = 0; s0 < 9; s0++) {
                if (GameModeType == 2 || GameModeType == 3) {
                    D_80118458[D_800AC5D4 & 1][1][s0] = s3->f33C[s0];
                    CopyStringClearTail(&D_80118420[D_800AC5D4 & 1], &s3->f280);
                } else if (GameModeType == 4) {
                    D_8010BFD8[D_800AC5D4 & 1][1][s0] = s3->f33C[s0];
                    CopyStringClearTail(&D_8010C088[D_800AC5D4], &s3->f280);
                }
            }
            for (s0 = 0; s0 < 0x210; s0++) {
                *(int *)((D_800AC5D4 & 1) * 0x840 + (s0 << 2) + (int)D_800F3B28) = s3->fEA4[s0];
            }
            a2 = 1;
        }
        break;
    case 0x10:
        v10 = *(int *)SaveBuffer;
        D_801272F0[GpSaveSlotCursor] = v10;
        a2 = 1;
        break;
    case 0x11:
        v11 = (p = (SaveBuf11 *)SaveBuffer)->f280;
        D_800F4D38 = D_801272F0[GpSaveSlotCursor] = v11;
        for (t0 = 0; t0 < 2; t0++) {
            for (s0 = 0; s0 < 0x840; s0++) {
                if (t0 == 0) {
                    D_800F3918->b[s0] = p->f300[0][s0];
                } else {
                    D_800F3910->b[s0] = p->f300[t0][s0];
                }
            }
        }
        a2 = 1;
        break;
    }
    return a2;
}

void BuildCardHeaderAnim(char *a0, int a1, int a2, int a3, int a4, int a5) {
    int i;
    int *new_var;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x13;
    a0[3] = 1;
    PsyqSprintf(a0 + 4, StrFmtPercentS, a1);
    D_8010EF98.rect.x = a2;
    D_8010EF98.rect.w = 0x10;
    D_8010EF98.rect.y = a3;
    D_8010EF98.rect.h = 1;
    StoreImage(&D_8010EF98, a0 + 0x60);
    DrawSync(0);
    i = 0;
    do {
        D_8010EF98.rect.x = a4;
        D_8010EF98.rect.y = a5;
        D_8010EF98.rect.w = 4;
        D_8010EF98.rect.h = 0x10;
        /* (new_var = &a4) is a dead address-of that keeps gcc from hoisting the li 0x10 out of the loop (found by permuter) */
        StoreImage(&D_8010EF98, a0 + 0x80 + (*(new_var = &a4), i) * 0x80);
        DrawSync(0);
        i++;
        a4 += 4;
    } while (i < 3);
}

void BuildCardHeaderStatic(unsigned char *a0, int a1, int a2, int a3, int a4, int a5, B *a6) {
    int i;
    a0[0] = 0x53;
    a0[1] = 0x43;
    a0[2] = 0x11;
    a0[3] = 1;
    PsyqSprintf(a0 + 4, StrFmtPercentS, a1);
    a0[0x50] = 1;
    a0[0x51] = 0;
    PsyqSprintf(a0 + 0x52, StrFmtPercentS, D_80011568);
    D_8010EFA0.rect.x = a2;
    D_8010EFA0.rect.w = 0x10;
    D_8010EFA0.rect.y = a3;
    D_8010EFA0.rect.h = 1;
    StoreImage(&D_8010EFA0, a0 + 0x60);
    DrawSync(0);
    i = 0;
    do {
        D_8010EFA0.rect.x = a4;
        D_8010EFA0.rect.y = a5;
        D_8010EFA0.rect.w = 4;
        D_8010EFA0.rect.h = 0x10;
        StoreImage(&D_8010EFA0, a0 + 0x80 + i * 0x80);
        DrawSync(0);
        i++;
        a4 += 4;
    } while (i <= 0);
    ((B *)a0)[2] = *a6;
    ((B *)a0)[3] = *a6;
    ((B *)a0)[4] = *a6;
}
