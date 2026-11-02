#include "rr/overlay668_frame_acquire.h"
u8 *ovl668_func_80115164(RrStrDisplayConfig *config) {
 RrGpuRectangleFields rectangle;
 u8 *data;RrLibQueueRecord32 *header;
 s32 retry=2000;RrStrPlaybackState *playback;
 while(func_8008F854(&data,&header)!=0) {if(--retry==0)return 0;}
 if((u32)header->value08>=D_800AC650.thresholdC8)D_800AC650.stopD0=1;
 if((u32)D_800AC650.counterC4>=D_800AC650.budgetCC)D_800AC650.stopD0=1;
 if(((RrStrPlaybackState*)D_800AC650.activeB8)->width00!=header->width10 || ((RrStrPlaybackState*)D_800AC650.activeB8)->height04!=header->height12) {
  ((RrStrPlaybackState*)D_800AC650.activeB8)->width00=header->width10;
  ((RrStrPlaybackState*)D_800AC650.activeB8)->height04=header->height12;
  playback=D_800AC650.activeB8;
  rectangle.x=0;rectangle.y=0;
  rectangle.w=(s32)((u32)playback->width00*3U)/2;
  rectangle.h=512;
  func_80092ED4(&rectangle,0,0,0);
 }
 config->rectangles[0].y=D_800ADAF0[0].environment.display.y+(s32)(256U-(u32)((RrStrPlaybackState*)D_800AC650.activeB8)->height04)/2;
 config->rectangles[1].y=D_800ADAF0[1].environment.display.y+(s32)(256U-(u32)((RrStrPlaybackState*)D_800AC650.activeB8)->height04)/2;
 config->rectangles[1].w=(s32)((u32)((RrStrPlaybackState*)D_800AC650.activeB8)->width00*3U)/2;
 config->rectangles[0].w=config->rectangles[1].w;
 config->rectangles[1].h=((RrStrPlaybackState*)D_800AC650.activeB8)->height04;
 config->rectangles[0].h=config->rectangles[1].h;
 config->rectangles[2].h=((RrStrPlaybackState*)D_800AC650.activeB8)->height04;
 D_800ADAF0[1].environment.display.w=((RrStrPlaybackState*)D_800AC650.activeB8)->width00;
 D_800ADAF0[0].environment.display.w=D_800ADAF0[1].environment.display.w;
 D_800ADAF0[1].environment.display.h=((RrStrPlaybackState*)D_800AC650.activeB8)->height04;
 D_800ADAF0[0].environment.display.h=D_800ADAF0[1].environment.display.h;
 return data;
}
