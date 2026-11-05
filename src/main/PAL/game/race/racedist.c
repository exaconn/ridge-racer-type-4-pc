#include "types.h"
#include "game/car/car.h"
#include "game/course/control.h"
#include "rr/trig.h"
#include "rr/track_interpolation.h"
#include "rr/race.h"
#include "game/race/racedist.h"

#define SIN(x) (((x) & 0x800) ? (((x) & 0x400) ? -D_800A68C0[0x400 - ((x) & 0x3FF)] : -D_800A68C0[(x) & 0x3FF]) : (((x) & 0x400) ? D_800A68C0[0x400 - ((x) & 0x3FF)] : D_800A68C0[(x) & 0x3FF]))

extern Car D_80107198[];
extern Car *CarTable[];
extern RrRaceStandingsTable *D_800AC738;
extern s32 D_800AC664;
extern s16 D_800AC59C;


void func_80049C68(void) {
    int i, j;
    Car *v;
    Control *c;
    RrRaceStandingsTable *t;
    int a0;
    RrRaceStandingEntry0C tmp;

    t = D_800AC738;
    for (i = 0; i < 0xB; i++) {
        v = &D_80107198[i];
        c = (Control *)((char *)v + 0x1F4);
        if (v->unk1E4 > 0) {
            a0 = -v->unk1D4 + 0x400;
            a0 -= D_800ABEA4[v->wF0].heading12;
            c->unk58 = -((SIN(a0) * v->unk1D8) / 4096);
        }
    }

    if (D_800AC664 > 0) {
        v = &D_800ABEF8[0];
        a0 = -v->unk1D4 + 0x400;
        a0 -= D_800ABEA4[v->wF0].heading12;
        t->unk00 = -((SIN(a0) * v->unk1D8) / 4096);
    }

    if (D_800AC664 == 2) {
        v = &D_800ABEF8[1];
        a0 = -v->unk1D4 + 0x400;
        a0 -= D_800ABEA4[v->wF0].heading12;
        t->unk04 = -((SIN(a0) * v->unk1D8) / 4096);
    }

    for (i = 0; i < D_800AC59C; i++) {
        v = CarTable[i];
        t->entries18[i].unk00 = i;
        t->entries18[i].totalDist04 = v->w184 + v->w188;
        t->entries18[i].vehicle08 = (RrRaceVehicle *)v;
    }

    for (i = 1; i < D_800AC59C; i++) {
        tmp = t->entries18[i];
        for (j = i - 1; j >= 0 && t->entries18[j].totalDist04 < tmp.totalDist04; j--) {
            t->entries18[j + 1] = t->entries18[j];
        }
        t->entries18[j + 1] = tmp;
    }
}

void func_8004A080(RrRaceVehicle *input) {
    register RrRaceDistanceContextPrefix *context=&D_800AC650;
    register s32 one=1;
    s32 mode;
    RrRaceDistanceResult *out;
    RrRaceVehicle *current;
    mode=context->mode;
    out=context->result;
    current=input;
    if(mode==one) {
        register RrRaceVehicle *base;
        u32 limit=0x7FFF0000U;
        /* ASM_BARRIER: Retain the two reference-distance load temporaries. */
        register u32 first __asm__("$3"),second __asm__("$5");
        u32 own_first,own_second;
        base=D_800ABEF8;
        first=base->first;second=base->second;
        own_first=current->first;own_second=current->second;
        out->second=limit|0xFFFF;out->selected=0;
        out->first=(first+second)-(own_first+own_second);
    } else if(mode==2) {
        /* ASM_BARRIER: Reload the first published distance after the second input sum. */
        register s32 a __asm__("$4");
        /* ASM_BARRIER: Keep the second difference and its wrapped absolute value together. */
        register s32 b __asm__("$2");
        register RrRaceVehicle *base=D_800ABEF8;
        out->first=((u32)base[0].first+(u32)base[0].second)-((u32)current->first+(u32)current->second);
        {
            /* ASM_BARRIER: Preserve the second reference component while loading current inputs. */
            register u32 left,extra __asm__("$5");
            /* ASM_BARRIER: Sum the current components in the original temporary. */
            register u32 right __asm__("$3"),right_extra;
            left=base[1].first;extra=base[1].second;
            right=current->first;right_extra=current->second;
            left+=extra;right+=right_extra;
            /* ASM_BARRIER: Finish the current sum before reloading the published first result. */
            __asm__("" : "+r"(right) : : "$4");
            a=*(volatile s32 *)&out->first;
            b=left-right;
        }
        out->second=b;
        /* ASM_BARRIER: Publish the second result before either absolute-value branch. */
        __asm__("" : "+r"(b) : : "memory");
        if(b<0)b=0U-(u32)b;
        if(a<0)a=0U-(u32)a;
        if(b>=a)out->selected=0;else out->selected=one;
    } else {
        out->first=0;out->second=0;out->selected=0;
    }
}
