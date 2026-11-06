#include "rr/overlay662_palette_initializer.h"
void ovl662_func_80119B44(void) {
 s32 i;s16 val;RrBadgeRow1C *badge;
 D_800F2DE4=0;ScreenFrame=0;ovl662_D_801272D4=0;ovl662_D_801272C0=0;D_800AC64A=0;
 D_800ACC18=D_800F4D70.second;
 if(!ovl662_D_801272E8)D_800AC5E0=2;else D_800AC5E0=1;
 func_8003DC9C(0,128,128,64,128,0,0);
 func_8003E928(0,12,64,68,32,0,0,1,0,83);
 func_8003E928(1,12,104,68,32,0,0,1,0,85);
 func_8003D228(0,108,320,112,34,0,0x601);
 badge=D_800F3AFC;ovl662_D_8012739A=25;badge->half02=1;
 for(i=0;i<25;i++) {
  if(i>=21&&D_800F3920.prefix.reserved50.bytes[i-13]==0)ovl662_D_801272A0[i]=25;
  else ovl662_D_801272A0[i]=i;
 }
 ovl662_D_80127398=0;
 if(ovl662_D_801272E8==1){ovl662_D_801272E2=0;ovl662_D_801272BE=0;}
 else{val=D_800ABE08[1][ovl662_D_801272E4].second;ovl662_D_801272E2=val;
 if((s16)val>0)ovl662_D_801272BE=val-1;else ovl662_D_801272BE=0;}
 ovl662_D_801272E2=(s8)ovl662_D_801272A0[ovl662_D_801272BE];
 val=ovl662_D_801272E2;
 if(!val)func_8001E854(D_800F3B20,0,0);else func_8001E854(D_800F3B24,val-1,0);
 ovl662_D_801272D4=0;
}
