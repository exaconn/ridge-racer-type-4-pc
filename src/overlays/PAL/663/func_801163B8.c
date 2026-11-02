#include "rr/overlay663_records.h"
void *ovl663_func_801163B8(u32 *ordering,void *cursor,s32 row) {
 s32 i=0,x,y,clut,rowY;
 s32 group=D_800AC648>=8;
 y=D_800F3AFC[row].y06+40;
 do {
  if(i==ovl663_D_80116898) {if(func_800904F4((s32)(((u32)D_800ABEB0<<7)-682U))<0)clut=32761;else clut=32760;}else clut=32760;
  x=D_800F3AFC[row].x04+8;
  cursor=func_8001FF84(ordering,cursor,x,y,&D_8009FAA0[i],clut,0);
  x+=ovl663_D_8011688C[0];
  cursor=func_8001FD20(ordering,cursor,x,y-2,1,20,255,255,255);
  x+=ovl663_D_8011688C[1];
  cursor=func_8001F5B4(ordering,cursor,x,y,(char*)ovl663_func_80115EE8(D_800FF700[group][D_800F3914][i].time),clut,0,0);
  x+=ovl663_D_8011688C[2];
  cursor=func_8001FD20(ordering,cursor,x,y-2,1,20,255,255,255);
  x+=ovl663_D_8011688C[3];
  cursor=func_8001F5B4(ordering,cursor,x,y,D_800FF700[group][D_800F3914][i].name,clut,0,0);
  y+=24;i++;
 }while(i<5);
 x+=52;
 rowY=D_800F3AFC[row].y06; y=rowY+40;
 cursor=func_8001FD20(ordering,cursor,x,y-2,1,116,255,255,255);
 cursor=func_8001FCC8(ordering,cursor,56);
 x+=16;
 cursor=func_80032EC8(ordering,cursor,x,y,D_800AC648&7);
 cursor=func_8001FF84(ordering,cursor,x+8,rowY+112,&ovl663_D_80116880[group],group+30180,0);
 return func_8001FCC8(ordering,cursor,89);
}
