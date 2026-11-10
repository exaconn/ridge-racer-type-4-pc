#ifndef RR_GAME_CAMERA_VIEWINI_H
#define RR_GAME_CAMERA_VIEWINI_H

#include "types.h"
#include "rr/course_view_types.h"
#include "rr/matrix_words.h"
#include "rr/geometry.h"

extern VisibleCourseViews PrimaryVisibleViews;
extern VisibleCourseViews *ActiveVisibleViews;
extern CourseViewIndices *D_800ADAD0;
extern AdditionalCourseViews D_800F4C38;

/* Shared course-light defaults, live RGB matrix, directions and strengths. */
extern MatrixWords32 TitleLogoWaveOffsets;
extern MatrixWords32 ColorMatrix;
extern Vec3sFields8 MenuBgScrollSpeedX, MenuBgScrollSpeedY, MenuBgTileAnimFrames;
extern Vec3sFields8 LightSourceAngles[3];
extern u16 LightSourceIntensities[3];

void InitViewLighting(void);
void InitCourseViewState(void);
void BuildViewSegmentObjectList(int player);
void UpdateVisibleViewRecords(void);
void CourseViewInitNoop(void);
void ResetViewScratchMask();

#endif /* RR_GAME_CAMERA_VIEWINI_H */
