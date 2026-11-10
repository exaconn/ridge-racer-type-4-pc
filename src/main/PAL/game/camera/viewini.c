#include "types.h"
#include "scratchpad.h"
#include "game/gfx/model.h"
#include "game/gfx/poly1.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/overlay668_camera.h"
#include "game/data/globals.h"
#include "game/camera/viewini.h"
#include "rr/relative_file.h"



typedef struct {
    u8 reserved00[0x5E];
    s16 h5E;
    u8 reserved60[0x78 - 0x60];
    s32 w78;
    u8 reserved7C[0x128 - 0x7C];
    s32 w128;
} ViewInitScratch;

extern int CourseFarColorTable[];
extern int ViewUsesCarTrackNode;

void InitViewLighting(void)
{
    int *t;
    int *b;
    int i;
    ColorMatrix = D_800A3B00;
    D_800F38F8[0] = D_800A3B20;
    D_800F38F8[1] = D_800A3B28;
    D_800F38F8[2] = D_800A3B30;
    D_800AD5BC[0] = 0x1000;
    D_800AD5BC[1] = 0x1000;
    D_800AD5BC[2] = 0x1000;
    BackColorR = 0x13;
    BackColorG = 0x1D;
    BackColorB = 0x1C;
    b = CourseFarColorTable;
    i = CourseIndex;
    t = b + i * 3;
    FarColorR = t[0];
    FarColorG = t[1];
    FarColorB = t[2];
}

void InitCourseViewState(void) {
    int i;
    char *p;
    ViewInitScratch *g = (ViewInitScratch *)SCRATCHPAD;
    InitViewLighting();
    ViewUsesCarTrackNode = 1;
    D_800AD5B8 = 0x1400;
    g->w78 = 0;
    g->h5E = 0;
    if (CourseIndex == 7) {
        for (i = 0; i < 5; i++) {
            SetAnimatedTrackObjectState(i, 6, 0, 0);
        }
    }
    i = 0x1F;
    p = (char *)&D_800F4C38 + i;
    do {
        *p-- = 0;
        i--;
    } while (i >= 0);
}

void BuildViewSegmentObjectList(int player) {
    /* The serialized cursor passes its word count before halfword indices. */
    u32 *source;
    unsigned int i;
    D_8010E690 = &D_8010E290;
    source = (u32 *)GetSegmentViewTable(player);
    D_800ADAD0 = (CourseViewIndices *)source;
    D_8010E290.stream[0].count = *source;
    if (D_8010E290.stream[0].count >= 0x100) {
        D_8010E290.stream[0].count = COURSE_VISIBLE_CAPACITY;
    }
    source++;
    i = 0;
    if (D_8010E290.stream[0].count != 0) {
        do {
            D_8010E290.stream[i+1].record = &RelFileViewRecords[((u16 *)source)[i]];
            i++;
        } while (i < D_8010E290.stream[0].count);
    }
}

void UpdateVisibleViewRecords(void) {
    /* This cursor changes from the counted owner to its pointer-array tail. */
    union {VisibleCourseViews *list;CourseViewRecord **next;} cursor;
    CourseViewRecord *record;
    int extra;
    unsigned int i;
    unsigned int count;

    cursor.list = D_8010E690;
    count = cursor.list->objects.count;
    cursor.next = cursor.list->objects.records + count;
    for (extra = 0; extra < D_800F4C38.count; extra++) {
        record = &RelFileViewRecords[D_800F4C38.indices[extra]];
        for (i = 0; i < D_8010E690->objects.count; i++) {
            if (record == D_8010E690->objects.records[i]) {
                break;
            }
        }
        if (i == D_8010E690->objects.count) {
            *cursor.next = record;
            cursor.next++;
            count++;
        }
    }
    D_8010E690->objects.count = count;
}

void CourseViewInitNoop(void) {
}

void ResetViewScratchMask(void) {
    ((ViewInitScratch *)SCRATCHPAD)->w128 = 0x1F;
}
