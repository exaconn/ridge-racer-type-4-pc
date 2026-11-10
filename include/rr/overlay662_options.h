#ifndef RR_OVERLAY662_OPTIONS_H
#define RR_OVERLAY662_OPTIONS_H
#include "rr/overlay662_garage.h"
/* Eight genuine ROM grids:40-byte rows, ten destination button indices.
 * Exact array boundaries run268CC..26C14. First three grids select horizontal
 * navigation; the remaining five select vertical navigation. */
struct GarageOptionRecord {s32 destinations[10];};
extern GarageOptionRecord ovl662_option_group_0[5];
extern GarageOptionRecord ovl662_option_group_1[2];
extern GarageOptionRecord ovl662_option_group_2[2];
extern GarageOptionRecord ovl662_option_group_3[2];
extern GarageOptionRecord ovl662_option_group_4[3];
extern GarageOptionRecord ovl662_option_group_5[5];
extern GarageOptionRecord ovl662_option_group_6[1];
extern GarageOptionRecord ovl662_option_group_7[1];
/* Nonzero fullword mode selects first group0..2; zero selects second group0..4.
 * The corresponding stored row must fit that grid and column must be0..9.
 * Its destination selects an initialized rectangle table entry. */
void ovl662_select_option_target(s32 mode);
RR_ASSERT_SIZE(GarageOptionRecord,40);
#endif
