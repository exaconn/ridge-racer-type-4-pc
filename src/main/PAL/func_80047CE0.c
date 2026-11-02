#include "rr/race.h"

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
