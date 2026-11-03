#include "rr/race.h"
#include "rr/track_lateral.h"

s32 func_80047CE0(RrRaceVehicle *input);
int func_80047DF8();

s32 func_80047CE0(RrRaceVehicle *input) {
    register RrRaceVehicle *vehicle=input;
    register s32 raw=vehicle->input;
    s32 rounded=raw;
    register s32 fraction;
    register u32 value;
    if(raw<0)rounded=raw+7;
    fraction=rounded>>3;
    if(raw) {
        register s32 span=vehicle->span;
        if(span>=fraction)goto interpolate;
        value=16;goto blend;
interpolate:
        {
            /* ASM_BARRIER: Read the signed maximum before preparing its scaled numerator. */
            register s32 maximum_signed __asm__("$2")=vehicle->maximum;
            /* ASM_BARRIER: Retain the signed numerator and DIV guard operands. */
            register s32 scaled __asm__("$3")=(maximum_signed-16)*4096;
            s32 quotient=scaled/span;
            s32 product=(s32)((s64)quotient*fraction);
            register u32 maximum=(u16)vehicle->maximum;
            /* ASM_BARRIER: Preserve the original signed divide-by-eight rounding temporary. */
    register s32 rounded_product=product>>12;
            if(product<0)rounded_product=(product+4095)>>12;
            value=maximum-(u32)rounded_product;
        }
blend:;
        {
            register s32 blend=vehicle->blend;
            if(blend>0) {
                register s32 delta;
                /* ASM_BARRIER: Retain the low wrapped product in the original rounding register. */
            register s32 product __asm__("$3");
                /* ASM_BARRIER: Load the final divisor after launching the blend multiply. */
                register s32 divisor __asm__("$2");
                /* ASM_BARRIER: Sign-extend the intermediate value before reusing the divisor register. */
                register s32 signed_value __asm__("$2");
                /* ASM_BARRIER: Keep the quotient in the original multiply source register. */
            register s32 quotient __asm__("$3");
                signed_value=(s16)value;
                delta=(s32)vehicle->maximum-signed_value;
                product=delta*blend;
                divisor=vehicle->divisor;
                if(divisor<0)divisor=-divisor;
                quotient=product/divisor;
                value=(u32)value+(u32)quotient;
            }
        }
    } else {
        value=0;
    }
    /* ASM_BARRIER: Keep final sign extension at the common return entry. */
    __asm__("" : "+r"(value) : : "$2");
    return (s16)value;
}

int func_80047DF8(a0, a1, a2, a3, a4) char *a0; int a1; int *a2; int *a3; int *a4;
{
    int v0, v1, s0, s1, s2, s3, s4, s5, r0;
    int r1;
    int t;
    v0 = *(short *)(a0 + 0x1D8) * 0xC;
    if (v0 < 0) v0 += 0xF;
    v1 = v0 >> 4;
    if (v1 < 0x200) v1 = 0x200;
    s0 = *(int *)(a0 + 0xF0);
    s1 = -*(short *)(a0 + 0x1D4) + 0x400;
    s1 &= 0xFFF;
    s2 = v1;
    if (s1 & 0x800) s1 |= ~0xFFF;
    s4 = 0;
    s3 = 0;
    s5 = 0;
    if (s2 > 0) {
        do {
            r0 = func_8004C454(s0, a1);
            r0 &= 0xFFF;
            v1 = s1 - r0;
            if (r0 & 0x800) {
                r0 |= ~0xFFF;
                v1 = s1 - r0;
            }
            v1 = v1 & 0xFFF;
            if (v1 & 0x800) v1 |= ~0xFFF;
            if (v1 > 0) s4 += v1; else s3 += v1;
            if (v1 < 0) v1 = -v1;
            /* ASM_BARRIER: stop reorg copying the slt into the bgez delay slot */
            __asm__("" : "=r"(v1) : "0"(v1));
            if (s5 < v1) s5 = v1;
            s0 -= 1;
            s1 = r0;
            if (s0 < 0) s0 += D_800ABECC;
            s2 -= D_800ABEA4[s0].segmentLength1C;
        } while (s2 > 0);
    }
    *a2 = s5 << 12;
    *a3 = s3;
    *a4 = s4;
    t = s4 - s3;
    r0 = t;
    r1 = r0 << 12;
    return r1 / 12;
}
