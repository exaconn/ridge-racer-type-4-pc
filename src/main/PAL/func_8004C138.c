/* RODATA 0x8001104C 0x18 */
#include "rr/color_conversion.h"
void func_8004C138(const RrHsvColor input,RrRgbColor output) {
    s32 s=input[1];
    s32 hue=input[0];
    s32 v=input[2];
    s32 h=hue*4096;
    s32 sector,top,low,falling,rising;
    if(s<0)s=0;
    if(s>4096)s=4096;
    if(v<0)v=0;
    if(v>4096)v=4096;
    if(h<0)h+=1474560;
    h%=1474560;
    sector=h/245760;
    h%=245760;
    h/=60;
    top=(v*255)/4096;
    low=(((v*(4096-s))/4096)*255)/4096;
    falling=(((v*(4096-(s*h)/4096))/4096)*255)/4096;
    rising=(((v*(4096-(s*(4096-h))/4096))/4096)*255)/4096;
    switch(sector) {
    case 0:output[0]=top;output[1]=rising;output[2]=low;return;
    case 1:output[0]=falling;output[1]=top;output[2]=low;return;
    case 2:output[0]=low;output[1]=top;output[2]=rising;return;
    case 3:output[0]=low;output[1]=falling;output[2]=top;return;
    case 4:output[0]=rising;output[1]=low;output[2]=top;return;
    case 5:output[0]=top;output[1]=low;output[2]=falling;
    }
}
