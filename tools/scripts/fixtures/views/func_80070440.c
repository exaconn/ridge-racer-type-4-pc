#include "types.h"

/* legacy typing of shared globals in this part of the unit */
#define RearViewMirrorY (*(unsigned short (*))&RearViewMirrorY)

#undef RearViewMirrorY

/* legacy typing of shared globals in this part of the unit */
#define D_8010E78C ((char *)D_8010E78C)

#undef D_8010E78C

/* legacy typing of shared globals in this part of the unit */
#define D_8010E78C ((O *)D_8010E78C)

#undef D_8010E78C

/* legacy typing of shared globals in this part of the unit */
#define SegmentVisibilityTable (*(s32 (*))&SegmentVisibilityTable)

#undef SegmentVisibilityTable

/* legacy typing of shared globals in this part of the unit */
#define OctantLookupTable (*(s32 (*))&OctantLookupTable)

#undef OctantLookupTable

extern s16 RearViewMirrorEnabled;

extern s32 RearViewMirrorY;

extern s32 RearViewMirrorVisible;

void InitRearViewMirrorState(void) {
    RearViewMirrorEnabled = 1;
    RearViewMirrorY = -0x24;
    RearViewMirrorVisible = 0;
}
