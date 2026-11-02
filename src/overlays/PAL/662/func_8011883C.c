#include "rr/overlay662_time.h"
u8 *ovl662_func_8011883C(s32 value){
 if(value>=0){
  s32 seconds=value/3000;
  s32 minutes=seconds/60;
  seconds%=60;
  ovl662_D_80126244[0]=minutes/10+'0';
  ovl662_D_80126244[1]=minutes%10+'0';
  ovl662_D_80126244[3]=seconds/10+'0';
  ovl662_D_80126244[4]=seconds%10+'0';
 }else{
  ovl662_D_80126244[0]='-';ovl662_D_80126244[1]='-';
  ovl662_D_80126244[3]='-';ovl662_D_80126244[4]='-';
 }
 return ovl662_D_80126244;
}
