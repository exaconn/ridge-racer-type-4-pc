#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define D_800F4C38 (*(unsigned char (*)[])&D_800F4C38)

#undef D_800F4C38

extern short CourseIndex;

extern int SetAnimatedTrackObjectState();

extern int ViewUsesCarTrackNode;

extern int DefaultSegmentVisibilityMask;

extern char D_800F4C38[];

void InitCourseViewState(void) {
    int i;
    char *p;
    char *g = (char *)0x1F800000;
    InitViewLighting();
    ViewUsesCarTrackNode = 1;
    DefaultSegmentVisibilityMask = 0x1400;
    *(int *)(g + 0x78) = 0;
    *(short *)(g + 0x5E) = 0;
    if (CourseIndex == 7) {
        for (i = 0; i < 5; i++) {
            SetAnimatedTrackObjectState(i, 6, 0, 0);
        }
    }
    i = 0x1F;
    p = D_800F4C38 + i;
    do {
        *p-- = 0;
        i--;
    } while (i >= 0);
}
