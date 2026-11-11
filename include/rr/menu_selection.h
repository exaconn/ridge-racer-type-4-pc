#ifndef RR_MENU_SELECTION_H
#define RR_MENU_SELECTION_H
#include "rr/layout.h"
extern u8 CoursePreviewSceneIds[8];
/* Race selection. CourseIndex (0x800F3914) is the course 0..7, decoded
 * from SelectedCourseChoice (0x800AC648); NumLaps (0x800F3A30) the lap count.
 * The link exchange sends all three to the peer. */
extern s16 ReverseCourse,SelectedCourseChoice,CourseIndex;
extern s32 NumLaps;
extern s32 ActivePlayerIndex;
/* Word result0..255: the original loads the byte with zero extension. */
s32 DecodeCourseIndex(s32 selection);
typedef struct {s16 first,second;} MenuGridPoint;
/* Car/emblem choice per player slot (0x800ACBD0). Slots 2-3 are named
 * LinkCarSyncStateBuf (0x800ACBD8) in the PS1 map; the link exchange
 * copies the host's slots 0-1 and the client's slots 2-3 to the peer. */
extern MenuGridPoint PlayerCourseChoices[4];
RR_ASSERT_SIZE(MenuGridPoint,4);
RR_ASSERT_OFFSET(MenuGridPoint,second,2);
#endif
