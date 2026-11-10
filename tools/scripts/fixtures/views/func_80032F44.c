extern int CurrentDrawBuf;

extern signed char CourseInfoRowOrder[];

extern int CourseInfoLabelUvs[];

extern int CourseInfoDirectionUvs[];

extern short GameModeType;

extern short CourseIndex;

extern unsigned char CourseLapCounts[];

extern int NumLaps;

extern short CourseLengthMetersTable[];

extern int CourseBestLapRecords[];

extern int CourseBestTotalTimes[];

extern int CourseBestLap[][8];

extern int CourseBestTotal[][8];

extern short NumCars;

extern short ComDifficultyLevel;

extern int AddSpritePrim();

extern int AddDrawModePrim();

extern int AddTilePrim();

extern int FormatDecimalDigits();

extern int DrawDigitString16();

extern char *FormatMillisecondsTime();

extern int EmitSpinningWheelIcon();

void DrawCourseInfoPanel(int arg0, int arg1) {
    char sp28[8];
    int i;
    int s4;
    int p;
    char *b;

    p = *(int *)0x1F800000;
    for (i = 0; i < 4; i++) {
        int v = CourseInfoRowOrder[i + (arg1 << 2)];
        int y;
        if (v == -1) {
            break;
        }
        y = 0x128 + i * 0x10;
        p = AddSpritePrim(CurrentDrawBuf + 0x70, p, 0x16, y, &CourseInfoLabelUvs[v], 0x7FA4, 1);
        if (i == 1) {
            p = AddSpritePrim(CurrentDrawBuf + 0x70, p, 0x4A, y, &CourseInfoLabelUvs[8], 0x7FA4, 1);
        }
    }

    s4 = arg0 >= 8;
    arg0 &= 7;
    p = AddSpritePrim(CurrentDrawBuf + 0x70, p, 0x3A, 0x104, &CourseInfoDirectionUvs[s4], s4 + 0x75E4, 0);
    p = AddDrawModePrim(CurrentDrawBuf + 0x70, p, 0x59);
    *(int *)0x1F800000 = AddTilePrim(CurrentDrawBuf + 0x70, p, 0x10, 0xE0, 2, 0x88, 0x4C, 0x4C, 0x4C);

    if (GameModeType < 2) {
        NumLaps = CourseLapCounts[CourseIndex + (GameModeType == 1 ? 8 : 0)];
    }

    i = 7;
    b = sp28 + 7;
    do {
        *b = 0;
        i--;
        b--;
    } while (i >= 0);

    for (i = 0; i < 2; i++) {
        if (i == 0) {
            FormatDecimalDigits(sp28, NumLaps, 0);
        } else {
            FormatDecimalDigits(sp28, CourseLengthMetersTable[arg0], 3);
        }
        DrawDigitString16(CurrentDrawBuf + 0x70, 0x2E + i * 8, 0x128 + i * 0x10, sp28);
    }

    if (arg1 == 0) {
        for (i = 0; i < 2; i++) {
            int val;
            if (GameModeType == 1) {
                if (i == 0) {
                    val = CourseBestLapRecords[CourseIndex];
                } else {
                    val = CourseBestTotalTimes[CourseIndex];
                }
            } else {
                if (i == 0) {
                    val = CourseBestLap[s4][CourseIndex];
                } else {
                    val = CourseBestTotal[s4][CourseIndex];
                }
            }
            DrawDigitString16(CurrentDrawBuf + 0x70, 0x44 + i * 8, 0x148 + i * 0x10, FormatMillisecondsTime(val));
        }
    } else if (arg1 == 1) {
        i = 7;
        b = sp28 + 7;
        do {
            *b = 0;
            i--;
            b--;
        } while (i >= 0);

        for (i = 0; i < 2; i++) {
            if (i == 0) {
                FormatDecimalDigits(sp28, NumCars - 2, 0);
            } else {
                FormatDecimalDigits(sp28, ComDifficultyLevel + 1, 0);
            }
            DrawDigitString16(CurrentDrawBuf + 0x70, 0x3A - i * 0xC, 0x148 + i * 0x10, sp28);
        }
    }

    *(int *)0x1F800000 = EmitSpinningWheelIcon(CurrentDrawBuf + 0x70, *(int *)0x1F800000, 0x16, 0xE4, arg0);
}
