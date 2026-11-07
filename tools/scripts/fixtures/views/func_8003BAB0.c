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

extern char LoadIndex;

extern unsigned char LoadState;

extern int GetR4FileIndex();

extern int CdFileReadStart();

extern int CdFileBytesLoaded();

extern int LoadCursor;

extern short D_800F3914;

extern void SetTrackNodeData();

extern char *TrackNodes;

extern int TrackNodeCount;

extern char *D_800F2EC4;

extern int D_8010BFC8;

int LoadTrackNodesStep(void) {
    int r = 0;
    switch (LoadState) {
    case 0:
        LoadIndex = 0;
        LoadState = 1;
        D_8010BFC8 = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_800F3914 + 0x38), LoadCursor) != 0) {
            LoadState = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            SetTrackNodeData(LoadCursor);
            LoadState = 3;
            D_800F2EC4 = TrackNodes + TrackNodeCount * 60;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
