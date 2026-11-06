#include "rr/overlay667_packet.h"
void ovl667_func_80114AF8(unsigned long configuration) {
    s32 work;
    u32 *base;
    /* Three word lanes in the genuine24-word ring. Keep cursor arithmetic
     * at full pointer width: after the last store, the unused second/third
     * integer cursors advance beyond the ring without forming C pointers. */
    unsigned long first;
    unsigned long second;
    unsigned long third;
    SetDispMask(0);
    func_8009AEAC(1,3,configuration);
    func_8009AEAC(1,4,4);
    func_8009AEAC(4,0,(unsigned long)ovl667_func_8011492C);
    work=0;
    base=ovl667_D_80118F20.words;
    third=(unsigned long)base+8;
    second=(unsigned long)base+4;
    first=(unsigned long)base;
    ovl667_D_80118EEF=0;
    ovl667_D_80118F8D=0;
    ovl667_D_80118F8C=7;
    ovl667_D_80118F08=0;
    ovl667_D_80118F09=0;
    ovl667_D_80118EFC=0;
    ovl667_D_80118F04=0;
    ovl667_D_80118EF8=0;
    ovl667_D_80118F00=0;
    ovl667_D_80118EE8=0;
    ovl667_D_80118EC8=0;
    ovl667_D_80118EED=0;
    ovl667_D_80118EEE=0;
    ((u8 *)&ovl667_D_80118EEC)[0]=0;
    do {
        *(u32 *)first=0;
        *(u32 *)second=0;
        *(u32 *)third=0;
        third+=12;
        second+=12;
        work++;
        first+=12;
    } while (work<8);
    ovl667_D_80118F80.words[0]=0;
    ovl667_D_80118F80.words[1]=0;
    ovl667_D_80118F80.words[2]=0;
    ovl667_D_80118F10.words[0]=0;
    ovl667_D_80118F10.words[1]=0;
    ovl667_D_80118F10.words[2]=0;
    D_800F3AF0.words[0]=0;
    D_800F3AF0.words[1]=0;
    D_800ACBF0.words[0]=0;
    D_800ACBF0.words[1]=0;
    D_800ACBE0.words[0]=0;
    D_800ACBE0.words[1]=0;
    ovl667_D_80118ED0=0;
    ovl667_D_80118ED4=0;
    if (func_8009AEAC(3,1,0) != 0) {
        func_8009AEAC(3,0,1);
    } else {
        func_8009AEAC(3,0,1);
        while (func_8009AEAC(3,1,0)==0) {
            work=((u32)func_8009AEAC(0,0,0)>>7)&3;
            if (work==0 || work==3) break;
        }
    }
    func_8009AEAC(5,0,0);
    VSync(0);
    func_8009AEAC(3,0,0);
}
