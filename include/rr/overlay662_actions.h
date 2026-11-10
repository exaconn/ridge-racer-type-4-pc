#ifndef RR_OVERLAY662_ACTIONS_H
#define RR_OVERLAY662_ACTIONS_H
#include "rr/overlay662_garage.h"
#include "rr/menu_choice.h"

typedef void (*GarageStep)(s32 code);
typedef struct {GarageStep callback;} GarageActionPrefix;
extern GarageActionPrefix *ovl662_D_80127470[];
/* Each used index selects a valid action object with a nonnull callback.
 * Within each pair1/2 and4/8, the lower bit has priority. */
void ovl662_func_8011DCDC(u32 flags);
RR_ASSERT_SIZE(GarageActionPrefix,4);
RR_ASSERT_OFFSET(GarageActionPrefix,callback,0);
#endif
