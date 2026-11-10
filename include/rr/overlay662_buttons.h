#ifndef RR_OVERLAY662_BUTTONS_H
#define RR_OVERLAY662_BUTTONS_H
#include "rr/overlay662_garage.h"
#include "rr/overlay662_sprites.h"
/* Initialization loop at0x8011E3D0 installs46 pointers to these16-byte records by key0C.
 * A rectangle is the first member, so the existing rectangle table also
 * addresses its genuine containing button owner. */
typedef struct {
    GarageRectangle rectangle;
    s16 sprite08,action0A,key0C,timer0E;
} GarageButton;
extern GarageButton ovl662_D_80126F88[46];
/* Requires an initialized table slot0..45 and the sprite emitter contract.
 * Timer0 selects sprite08; nonzero selects sprite08+1. Only positive timers
 * decrement. Uses the scratch cursor and publishes the emitter's full pointer. */
void ovl662_func_8011B940(s32 index);
/* Same valid button/scratch contract. config1F is0..2 (initialized to0,
 * incremented modulo3). Emits fixed button and mode sprites at19/39,173;
 * both sprite choices are captured before either packet is emitted. */
void ovl662_func_8011BDC8(s32 index);
RR_ASSERT_SIZE(GarageButton,16);
RR_ASSERT_OFFSET(GarageButton,rectangle,0);
RR_ASSERT_OFFSET(GarageButton,sprite08,8);
RR_ASSERT_OFFSET(GarageButton,key0C,12);
RR_ASSERT_OFFSET(GarageButton,timer0E,14);
#endif
