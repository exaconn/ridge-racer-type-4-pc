#ifndef RR_OVERLAY662_TIM_H
#define RR_OVERLAY662_TIM_H
#include "rr/overlay662_garage.h"
#include "rr/overlay662_pixels.h"
#include "rr/palette16.h"
#include "rr/emblem_images.h"
typedef EmblemTimLayout840 GarageTimLayout;
typedef EmblemTimImage840 GarageTimImage;
/* Select side0/1. Source TIM and destination are complete2112-byte owners;
 * struct copy requires disjoint owners or the exact same owner. Palette is a
 * genuine32-byte Palette16 owner. Image input covers512 words and may alias
 * the destination; compositor reads observe all preceding TIM/header writes. */
void ovl662_pack_emblem_tims(void);
RR_ASSERT_SIZE(GarageTimBytes,0x840);
RR_ASSERT_SIZE(GarageTimImage,0x840);
RR_ASSERT_SIZE(GarageTimLayout,0x840);
RR_ASSERT_OFFSET(GarageTimLayout,palette,0x14);
RR_ASSERT_OFFSET(GarageTimLayout,pixels,0x40);
#endif
