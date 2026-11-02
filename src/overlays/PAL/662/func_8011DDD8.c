#include "rr/overlay662_garage.h"
s32 ovl662_func_8011DDD8(s32 mode)
{
    if(mode){
        ovl662_D_80127668.words[2]++;
        if(ovl662_D_80127668.words[2]>=47)ovl662_D_80127668.words[2]=40;
        if(ovl662_D_80127668.words[2]==1 || ovl662_D_80127668.words[2]==40)return 1;return 0;
    }else{
        ovl662_D_80127668.words[3]++;
        if(ovl662_D_80127668.words[3]>=47)ovl662_D_80127668.words[3]=40;
        if(ovl662_D_80127668.words[3]==1 || ovl662_D_80127668.words[3]==40)return 1;return 0;
    }
}
