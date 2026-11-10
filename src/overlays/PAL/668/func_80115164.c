#include "rr/overlay668_frame_acquire.h"
u8 *ovl668_acquire_stream_frame(StrDisplayConfig *config) {
 GpuRectangle rectangle;
 u8 *data;LibQueueRecord32 *header;
 s32 retry=2000;StrPlaybackState *playback;
 while(StGetNext(&data,&header)!=0) {if(--retry==0)return 0;}
 if(header->value08>=RaceContext.thresholdC8)RaceContext.stopD0=1;
 if(RaceContext.counterC4>=RaceContext.budgetCC)RaceContext.stopD0=1;
 if(((StrPlaybackState*)RaceContext.activeB8)->width00!=header->width10 || ((StrPlaybackState*)RaceContext.activeB8)->height04!=header->height12) {
  ((StrPlaybackState*)RaceContext.activeB8)->width00=header->width10;
  ((StrPlaybackState*)RaceContext.activeB8)->height04=header->height12;
  playback=RaceContext.activeB8;
  rectangle.rect.x=0;rectangle.rect.y=0;
  rectangle.rect.w=(s32)(playback->width00*3U)/2;
  rectangle.rect.h=512;
  ClearImage(&rectangle,0,0,0);
 }
 config->rectangles[0].y=DrawBuffers[0].environment.display5C.display.y+(s32)(256U-((StrPlaybackState*)RaceContext.activeB8)->height04)/2;
 config->rectangles[1].y=DrawBuffers[1].environment.display5C.display.y+(s32)(256U-((StrPlaybackState*)RaceContext.activeB8)->height04)/2;
 config->rectangles[1].w=(s32)(((StrPlaybackState*)RaceContext.activeB8)->width00*3U)/2;
 config->rectangles[0].w=config->rectangles[1].w;
 config->rectangles[1].h=((StrPlaybackState*)RaceContext.activeB8)->height04;
 config->rectangles[0].h=config->rectangles[1].h;
 config->rectangles[2].h=((StrPlaybackState*)RaceContext.activeB8)->height04;
 DrawBuffers[1].environment.display5C.display.w=((StrPlaybackState*)RaceContext.activeB8)->width00;
 DrawBuffers[0].environment.display5C.display.w=DrawBuffers[1].environment.display5C.display.w;
 DrawBuffers[1].environment.display5C.display.h=((StrPlaybackState*)RaceContext.activeB8)->height04;
 DrawBuffers[0].environment.display5C.display.h=DrawBuffers[1].environment.display5C.display.h;
 return data;
}
