/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#define MainMenuChoice (*(unsigned char (*))&MainMenuChoice)

#undef LoadIndex

#undef MainMenuChoice

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
#define PresetEmblemTimPtr (*(int (*))&PresetEmblemTimPtr)

#undef PresetEmblemTimPtr

/* legacy typing of shared globals in this part of the unit */
#define LoadIndex (*(unsigned char (*))&LoadIndex)

#undef LoadIndex

/* legacy typing of shared globals in this part of the unit */
#define LoadTimList ((void (*)())LoadTimList)

#define RaceLoadReset ((void (*)())RaceLoadReset)

#define SoundCloseVabsAndReset ((void (*)())SoundCloseVabsAndReset)

#undef LoadTimList

#undef RaceLoadReset

#undef SoundCloseVabsAndReset

typedef struct { char p[7]; char b; } Z;

extern char LoadIndex;

extern unsigned char LoadState;

extern int GetR4FileIndex();

extern int CdFileReadStart();

extern int CdFileBytesLoaded();

extern char SharedWorkArena[];

extern int CdFileReadRange();

extern int InitDialogueState();

extern int SetupDialogueTable();

extern int LoadTimImage();

extern int DialogueSceneMode;

extern int SelectedStoryBranchId;

extern char SceneOffsetLookupBuf[];

extern Z *CardDialogueStatePtr;

int LoadMenuCarModelsStep(void) {
    int r = 0;
    char *s;
    int a1;
    switch (LoadState) {
    case 0:
        LoadState = 1;
        LoadIndex = 0;
    case 1:
        if (CdFileReadStart(GetR4FileIndex(1, 0x42), SharedWorkArena)) {
            LoadState = 2;
        }
        break;
    case 2:
        if (CdFileBytesLoaded()) {
            s = SharedWorkArena;
            LoadTimImage(s);
            s += 0x4E20;
            LoadState = 3;
            CardDialogueStatePtr = (Z *)s;
            InitDialogueState(s);
    case 3:
            if (DialogueSceneMode == 2) {
                a1 = 0x44;
            } else if (DialogueSceneMode == 4) {
                a1 = 0x46;
                CardDialogueStatePtr->b = 2;
            } else {
                a1 = 0x45;
            }
            if (CdFileReadRange(GetR4FileIndex(1, a1), SelectedStoryBranchId << 2, 4, SceneOffsetLookupBuf)) {
                LoadState = 4;
            }
        }
        break;
    case 4:
        if (CdFileBytesLoaded()) {
            SetupDialogueTable(SceneOffsetLookupBuf);
            LoadState = 5;
    case 5:
            r = 1;
        }
        break;
    }
    return r;
}
