#ifndef RR_EMBLEM_IMAGES_H
#define RR_EMBLEM_IMAGES_H
#include "rr/emblem_tim_types.h"
extern CustomEmblemImages1080 CustomEmblemBuffers;
extern EmblemTimImage840 SaveDataWork;
/* Palette/upload boundary accepts a complete TIM resource through a generic pointer. */
extern void *CustomEmblemTimPtr;
/* Loaded preset emblem TIM bank (0x800F3B24), indexed by preset emblem id. */
extern void *PresetEmblemTimPtr;
/* Garage decal TIM copies. Saved (A 0x800F3918, B 0x800F3910) point at the
 * editor arena images restored from a save; active (A 0x800F39A0,
 * B 0x800F399C) at the images of the editor's current buffer, which the save
 * builder writes back. */
extern EmblemTim840 *GarageSavedDecalTimA, *GarageSavedDecalTimB;
extern EmblemTim840 *GarageActiveDecalTimA, *GarageActiveDecalTimB;
#endif
