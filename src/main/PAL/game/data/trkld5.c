/* RODATA 0x80011088 0x14 */
#include "rr/track_resources.h"
#include "rr/load_cursor.h"
#include "game/data/cdfile2.h"
#include "game/data/trkld1.h"
#include "game/data/trkld5.h"
#include "game/course/trkres2.h"
#include "game/data/cdfile1.h"
#include "game/data/selection.h"
#include "rr/selection_types.h"

extern short ComDifficultyLevel;
extern short SelectedTeamIndex;
extern short CourseIndex;

int LoadTrackStep(void) {
    int r;
    int r2;
    int r3;
    switch (RaceLoadStepIndex) {
    case 0:
        r = CdFileReadStart(GetR4FileIndex(2, CourseIndex + 0x16), RaceLoadCursor);
        if (r == 0) goto out;
        goto inc;
    case 1:
        r2 = CdFileBytesLoaded();
        if (r2 != 0) {
            LoadTrackResource((TrackResourceFile *)RaceLoadCursor);
            RaceLoadCursor += AlignUp4(r2);
            RaceLoadStepIndex += 1;
        }
        break;
    case 2:
        r3 = LoadCarSpecStep(CourseIndex, SelectedTeamIndex, ComDifficultyLevel);
        goto chk;
    case 3:
        r3 = LoadCourseAssetsStep(CourseIndex, SelectedTeamIndex);
    chk:
        if (r3 != 0) {
    inc:
            RaceLoadStepIndex += 1;
        }
        break;
    case 4:
        RaceLoadStepIndex = 0;
        LoadCursor = RaceLoadCursor;
        return 1;
    }
    r = 0;
out:
    return r;
}

int GetPlayerCarVariantIndex(int i) {
    SelectionRow *base = (SelectionRow *)SelectionRows;
    SelectionRow *t = base + i;
    int r = 0;
    if (t->parameter != 0x27) {
        r = t->variant;
        if (t->parameter == 0x13) {
            r += 4;
        }
    } else if (t->variant == 0) {
        r = (t->selection == 2) * 8;
    }
    return r;
}
