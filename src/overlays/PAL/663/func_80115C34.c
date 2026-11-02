#include "rr/overlay663_result_preview.h"
void ovl663_func_80115C34(void) {
 /* Preserve the observed byte stores and word-sized by-value transfer. */
 volatile RrPreviewRgb3 color;
 s32 scale;
 D_800F2DE4=(s32)((u32)D_800F2DE4+1U);
 switch((u16)D_800F4D8A) {
 case 0:
  if(D_800F4D80[3].byte14==2){func_8002AF8C(1,1,0);D_800F4D8A=1;}
  break;
 case 1:
  D_800F4D80[3].cells[0]=2;
  if(D_800F3A60){
   func_8004F24C(1);func_8002AF8C(0,1,0);
   D_800F4D80[3].cells[0]=1;D_800F4D80[3].byte14=3;D_800F4D8A=2;
   D_800F3AFC->state10=4;D_800F2AB0[1]=3;
  }
  break;
 case 2:
  if(D_800F3AFC->state10==11 && ((s8*)D_800F2AB0)[1]==5){
   ovl663_func_80115028(ovl663_D_801168C8);D_800F4C6A=1;D_800F3A26=75;D_800F4D8A=0;
  }
  break;
 }
 D_800ABEF8[0].rot54=(s32)(((u32)D_800ABEF8[0].rot54+8U)&4095U);
 func_80033A74(1);func_8006EF8C(6);
 if((u32)D_800F2AB0[1]-1U<2U){
  RrSelectionRows *selected=&D_800F4D98;
  selected->pad02[1]=2;func_80037198(0);selected->pad02[1]=0;
  func_8002E5A0(D_800ABEF8);
  if(((s8*)D_800F2AB0)[1]==1){
   s32 level=~(D_800F2AB0[2]*255/30);color.r=level;color.g=level;color.b=level;
   scale=D_800F2AB0[2]*256/30;
  }else{color.r=128;color.g=128;color.b=128;scale=256;}
  func_8002CC88(D_800ABEF8,color,scale,0);
 }
 func_8003EC88(3);func_8003D6F0(0);func_8003FBA8(0);func_8002A344(0x10022);
}
