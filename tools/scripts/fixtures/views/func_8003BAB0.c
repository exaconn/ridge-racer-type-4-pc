/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#define D_800F4D2C (*(unsigned char (*))&D_800F4D2C)

#undef D_8009F8FC

#undef D_800F4D2C

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define D_800F3B24 (*(int (*))&D_800F3B24)

#undef D_800F3B24

/* legacy typing of shared globals in this part of the unit */
#define D_8009F8FC (*(unsigned char (*))&D_8009F8FC)

#undef D_8009F8FC

/* legacy typing of shared globals in this part of the unit */
#define func_800436E8 ((void (*)())func_800436E8)

#define func_8004E008 ((void (*)())func_8004E008)

#define func_8004F600 ((void (*)())func_8004F600)

#undef func_800436E8

#undef func_8004E008

#undef func_8004F600

extern char D_8009F8FC;

extern unsigned char D_800F4D7C;

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

int func_8003BAB0(void) {
    int r = 0;
    switch (D_800F4D7C) {
    case 0:
        D_8009F8FC = 0;
        D_800F4D7C = 1;
        D_8010BFC8 = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, D_800F3914 + 0x38), LoadCursor) != 0) {
            D_800F4D7C = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded() != 0) {
            SetTrackNodeData(LoadCursor);
            D_800F4D7C = 3;
            D_800F2EC4 = TrackNodes + TrackNodeCount * 60;
    case 3:
            r = 1;
        }
        break;
    }
    return r;
}
