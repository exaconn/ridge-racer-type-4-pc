#include "rr/overlay668_display.h"
void ovl668_func_80114E48(u8 *base){
 ((RrStrBufferSet*)D_800AC650.activeB8)->buffers[0]=base;
 base+=0xa000;
 ((RrStrBufferSet*)D_800AC650.activeB8)->buffers[1]=base;
 base+=0xa000;
 ((RrStrBufferSet*)D_800AC650.activeB8)->buffers[2]=base;
 base+=0x2d00;
 ((RrStrBufferSet*)D_800AC650.activeB8)->buffers[3]=base;
 base+=0x2d00;
 ((RrStrBufferSet*)D_800AC650.activeB8)->buffers[4]=base;
}
