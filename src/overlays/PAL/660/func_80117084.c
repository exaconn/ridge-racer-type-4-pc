#include "rr/overlay_race_state.h"
void ovl660_func_80117084(RrRaceVehicleViews *player){
 u8 *control=(u8 *)&player->parts.control;
 if((u32)D_800FF6B0-2U<2){
  s32 allow=0;
  if(*(s16 *)(control+0xB6)<=0)allow=player->vehicle.half1EA!=D_800AD522;
  else if(player->vehicle.first<=(s32)(0U-(u32)D_800F2EB8))allow=1;
  if(allow==1){D_800FF6B0=5;D_800AC5D0=75;func_8004FC18(0,71);}
 }
}
