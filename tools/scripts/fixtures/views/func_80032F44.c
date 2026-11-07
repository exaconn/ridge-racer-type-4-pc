extern int D_800ACC14;

extern signed char D_8009F688[];

extern int D_8009F654[];

extern int D_8009F66C[];

extern short D_800F4D44;

extern short D_800F3914;

extern unsigned char CourseLapCounts[];

extern int NumLaps;

extern short D_8009F678[];

extern int D_800F3B00[];

extern int D_800AC5B0[];

extern int CourseBestLap[][8];

extern int CourseBestTotal[][8];

extern short NumCars;

extern short D_800ABEC8;

extern int func_8001FF84();

extern int AddDrawModePrim();

extern int AddTilePrim();

extern int FormatDecimalDigits();

extern int DrawDigitString16();

extern char *FormatMillisecondsTime();

extern int func_80032EC8();

void DrawCourseInfoPanel(int arg0, int arg1) {
    char sp28[8];
    int i;
    int s4;
    int p;
    char *b;

    p = *(int *)0x1F800000;
    for (i = 0; i < 4; i++) {
        int v = D_8009F688[i + (arg1 << 2)];
        int y;
        if (v == -1) {
            break;
        }
        y = 0x128 + i * 0x10;
        p = func_8001FF84(D_800ACC14 + 0x70, p, 0x16, y, &D_8009F654[v], 0x7FA4, 1);
        if (i == 1) {
            p = func_8001FF84(D_800ACC14 + 0x70, p, 0x4A, y, &D_8009F654[8], 0x7FA4, 1);
        }
    }

    s4 = arg0 >= 8;
    arg0 &= 7;
    p = func_8001FF84(D_800ACC14 + 0x70, p, 0x3A, 0x104, &D_8009F66C[s4], s4 + 0x75E4, 0);
    p = AddDrawModePrim(D_800ACC14 + 0x70, p, 0x59);
    *(int *)0x1F800000 = AddTilePrim(D_800ACC14 + 0x70, p, 0x10, 0xE0, 2, 0x88, 0x4C, 0x4C, 0x4C);

    if (D_800F4D44 < 2) {
        NumLaps = CourseLapCounts[D_800F3914 + (D_800F4D44 == 1 ? 8 : 0)];
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
            FormatDecimalDigits(sp28, D_8009F678[arg0], 3);
        }
        DrawDigitString16(D_800ACC14 + 0x70, 0x2E + i * 8, 0x128 + i * 0x10, sp28);
    }

    if (arg1 == 0) {
        for (i = 0; i < 2; i++) {
            int val;
            if (D_800F4D44 == 1) {
                if (i == 0) {
                    val = D_800F3B00[D_800F3914];
                } else {
                    val = D_800AC5B0[D_800F3914];
                }
            } else {
                if (i == 0) {
                    val = CourseBestLap[s4][D_800F3914];
                } else {
                    val = CourseBestTotal[s4][D_800F3914];
                }
            }
            DrawDigitString16(D_800ACC14 + 0x70, 0x44 + i * 8, 0x148 + i * 0x10, FormatMillisecondsTime(val));
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
                FormatDecimalDigits(sp28, D_800ABEC8 + 1, 0);
            }
            DrawDigitString16(D_800ACC14 + 0x70, 0x3A - i * 0xC, 0x148 + i * 0x10, sp28);
        }
    }

    *(int *)0x1F800000 = func_80032EC8(D_800ACC14 + 0x70, *(int *)0x1F800000, 0x16, 0xE4, arg0);
}
