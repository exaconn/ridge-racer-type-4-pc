#include "rr/overlay662_car_layout.h"
void ovl662_func_8011512C(void) {
    s32 i;
    RrRaceVehicle *vehicle;
    RrProjectionScratchPrefix *scratch;
    /* Keep the per-vehicle rotation temporary in the call-result register. */
    register s32 rotation __asm__("$2");
    void *arena;
    func_8002A604();
    D_800F2DE4=0;
    D_800ABEB0=0;
    D_800AC64A=0;
    D_800AC5E0=4;
    func_8003A198();
    D_800F4D70.second=D_800102F0+0x2D000;
    func_8003A1B4();
    D_800ACC18=D_800F4D70.second;
    func_8003E928(0,24,88,68,32,0,0,1,0,21);
    func_8003E928(1,24,128,68,32,0,0,1,0,102);
    func_8003E928(2,24,168,68,32,0,0,1,0,22);
    func_8003E928(3,24,208,68,32,0,0,1,0,85);
    func_80013890(5);
    func_8003A198();
    func_80037110();
    i=0;
    vehicle=D_800ABEF8;
    ovl662_D_801272D4=0;
    scratch=RR_PROJECTION_SCRATCH;
    scratch->word18=120;
    scratch->word08=-16;
    scratch->word0C=-248;
    scratch->word1C=0;
    scratch->word20=0;
    scratch->word10=-96;
    arena=D_800F4D40;
    D_800ACC18=arena;
    D_800F4D70.first=arena;
    do {
        /* Keep one full vehicle cursor through both setup calls. */
        __asm__("" : "+r"(vehicle));
        func_8002E4F4(vehicle);
        vehicle->pos20[1]=10;
        vehicle->pos20[2]=308;
        rotation=2048;
        vehicle->rot54=rotation;
        /* Publish this vehicle before advancing the loop counter. */
        __asm__("" : "+r"(i) : : "memory");
        i++;
        vehicle++;
    } while(i<2);
    func_80037040((s8*)D_800F4D98.bytes,D_800ABE08[1][ovl662_D_801272E4].glyph);
    D_800ACBD0[0].first=D_800ABE08[1][ovl662_D_801272E4].first;
    D_800ACBD0[0].second=D_800ABE08[1][ovl662_D_801272E4].second;
    ovl662_D_801272E0=func_800370BC(D_800ABE08[1]);
    func_8002AF8C(0,1,1);
}
