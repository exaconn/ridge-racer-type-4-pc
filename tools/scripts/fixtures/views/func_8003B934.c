/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#define D_800F4D2C (*(unsigned char (*))&D_800F4D2C)

#undef LoadIndex

#undef D_800F4D2C

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define D_800F3B24 (*(int (*))&D_800F3B24)

#undef D_800F3B24

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define func_800436E8 ((void (*)())func_800436E8)

#define RaceLoadReset ((void (*)())RaceLoadReset)

#define func_8004F600 ((void (*)())func_8004F600)

#undef func_800436E8

#undef RaceLoadReset

#undef func_8004F600

typedef struct { char p[7]; char b; } Z;

extern char LoadIndex;

extern unsigned char LoadState;

extern int GetR4FileIndex();

extern int CdFileReadStart();

extern int CdFileBytesLoaded();

extern char D_800FFC48[];

extern int CdFileReadRange();

extern int InitDialogueState();

extern int func_80029358();

extern int func_80043608();

extern int D_800F4CA8;

extern int D_800AD564;

extern char D_80104DB0[];

extern Z *D_800AC590;

int func_8003B934(void) {
    int r = 0;
    char *s;
    int a1;
    switch (LoadState) {
    case 0:
        LoadState = 1;
        LoadIndex = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x42), D_800FFC48)) {
            LoadState = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            s = D_800FFC48;
            func_80043608(s);
            s += 0x4E20;
            LoadState = 3;
            D_800AC590 = (Z *)s;
            InitDialogueState(s);
    case 3:
            if (D_800F4CA8 == 2) {
                a1 = 0x44;
            } else if (D_800F4CA8 == 4) {
                a1 = 0x46;
                D_800AC590->b = 2;
            } else {
                a1 = 0x45;
            }
            if (CdFileReadRange(GetR4FileIndex(1, a1), D_800AD564 << 2, 4, D_80104DB0)) {
                LoadState = 4;
            }
        }
        break;
    case 4:
        if (CdFileBytesLoaded()) {
            func_80029358(D_80104DB0);
            LoadState = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
