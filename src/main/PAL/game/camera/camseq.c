#include "rr/game_motion.h"

struct Q {
    char pad0[8]; int f8; char pad1[0x20 - 0xC]; unsigned char c20, c21, c22; char pad23; unsigned char c24, c25, c26; char pad27;
    unsigned char c28, c29, c2a; char pad2b; short h2c, h2e, h30; char pad32[2]; short h34, h36, h38; char pad3a[2]; short h3c, h3e, h40; char pad42[2];
    unsigned char c44, c45, c46; char pad47; int f48; char pad4c[0x60 - 0x4C]; int f60; short *f64; short f68, f6a; int f6c; int f70;
};

struct Q_1 { char pad[0x60]; int f60; int *f64; short f68, f6a; int f6c; int f70; };

extern int func_80096434();
extern int func_8005B8D8();
extern int func_8005D500();
extern int func_8005ECD4();
extern int func_8005ABAC();
extern struct { char pad[0x250]; char pad2[0x3D8 - 0x250]; struct Q *q; } D_800AC650;
extern short D_800A1E60[];
extern int *D_800A1F88[];
extern int D_800F7620;

void func_8005AC0C(RrMotionClock20 *clock);
void func_8005AF5C(void);
void func_8005B190(void);

void func_8005AC0C(RrMotionClock20 *clock) {
    s16 counter, next;
    s32 start, code, delta, units;
    s32 rate;
    register s32 velocity __asm__("$4");
    u32 tempo;
    RrMotionKey32 *key;
    RrMotionKey32 *finishKey;
    RrMotionKey32 *restKey;
    register RrMotionKey32 *scaleKey __asm__("$4");
    RrMotionTrack44 *track;
    RrMotionControllerPrefix *controller;

    controller = D_800ACA28;
    track = &controller->track;
    if (track->disabled == 0) {
        switch (controller->track.state) {
        case 0:
            if (clock->active != 0) {
                controller->track.position = track->key->from;
                track->scale = track->key->fromScale;
                code = track->key->timeCode;
                units = (code >> 8) * 12 + (code & 255);
                tempo = clock->tempo;
                /* Capture the clock before doubling the decoded time. */
                __asm__("" :: "r"(tempo));
                units *= 2;
                track->elapsed = 0;
                track->duration = (s16)((tempo * (u32)units) / 1200U);
                controller->track.state = (s32)((u32)controller->track.state + 1U);
                /* Fall through: the first interpolation step is immediate. */
        case 1:
                key = track->key;
                counter = track->elapsed;
                track->position.x = (u16)key->from.x +
                    ((key->to.x - key->from.x) * counter) / track->duration;
                track->position.y = (u16)key->from.y +
                    ((key->to.y - key->from.y) * counter) / track->duration;
                track->position.z = (u16)key->from.z +
                    ((key->to.z - key->from.z) * counter) / track->duration;
                scaleKey = key;
                delta = scaleKey->toScale;
                start = scaleKey->fromScale;
                delta = (s32)(((u32)delta - (u32)start) *
                              (u32)(s32)track->elapsed) / track->duration;
                next = (u16)track->elapsed + 1;
                track->elapsed = next;
                track->scale = (s32)((u32)delta + (u32)start);
                finishKey = key;
                if (track->duration < next) {
                    /* Keep the first destination write before the key reload. */
                    __asm__("" : "+r"(finishKey));
                    track->position.x = (u16)finishKey->to.x;
                    restKey = key;
                    __asm__("" : "+r"(restKey) :: "memory");
                    track->position.y = (u16)restKey->to.y;
                    track->position.z = (u16)restKey->to.z;
                    track->scale = restKey->toScale;
                    if (restKey->continueAfter == 0) {
                        track->state = 3;
                        return;
                    }
                    track->state = 2;
                    track->velocity = 800;
                    return;
                }
            } else {
                return;
            }
            break;
        case 2:
            /* Publish scale, damped velocity, then x/z/y in wire order. */
            velocity = track->velocity;
            rate = (s32)((u32)velocity << 5);
            {
                s32 current = track->scale;
                s32 carry = velocity;
                __asm__("" :: "r"(current));
                __asm__("" : "+r"(carry) : "r"(rate));
                rate = (s32)((u32)rate - (u32)velocity);
                track->scale = (s32)((u32)current + (u32)carry);
            }
            if (rate < 0) {
                rate += 31;
            }
            {
                u16 x, y, z;
                s32 damped;
                x = (u16)track->position.x;
                __asm__("" : "+r"(rate), "+r"(x));
                damped = rate >> 5;
                __asm__("" :: "r"(rate), "r"(damped));
                track->velocity = damped;
                z = (u16)track->position.z;
                track->position.x = x + 16;
                y = (u16)track->position.y;
                track->position.z = z + 8;
                __asm__("" :: "r"(y));
                track->position.y = y + 8;
            }
            break;
        }
    }
}

void func_8005AF5C(void) {
    D_800AC650.q = (struct Q *)((char *)&D_800AC650 + 0x250);
    func_80096434(D_800AC650.q, 0x188);
    D_800AC650.q->f8 = -0x1388;
    D_800AC650.q->f48 = 0x1B8;
    D_800AC650.q->h2c = 0x1C1;
    D_800AC650.q->h2e = -0xDF8;
    D_800AC650.q->h30 = -0x79A;
    D_800AC650.q->c20 = 0xC0;
    D_800AC650.q->c21 = 0xC0;
    D_800AC650.q->c22 = 0xC0;
    D_800AC650.q->h34 = -0x64;
    D_800AC650.q->h36 = 0;
    D_800AC650.q->h38 = 0x64;
    D_800AC650.q->c24 = 0xC0;
    D_800AC650.q->c25 = 0xC0;
    D_800AC650.q->c26 = 0xC0;
    D_800AC650.q->h3c = -0xA4D;
    D_800AC650.q->h3e = -0x642;
    D_800AC650.q->h40 = 0xA86;
    D_800AC650.q->c28 = 0x20;
    D_800AC650.q->c29 = 0x20;
    D_800AC650.q->c2a = 0x20;
    D_800AC650.q->c44 = 0;
    D_800AC650.q->c45 = 0;
    D_800AC650.q->c46 = 0;
    D_800AC650.q->f64 = D_800A1E60;
    D_800AC650.q->f6c = D_800AC650.q->f64[D_800AC650.q->f68 * 4];
    D_800AC650.q->f70 = D_800AC650.q->f64[D_800AC650.q->f68 * 4 + 1];
    switch (D_800AC650.q->f6c) {
    case 0:
        func_8005B8D8(D_800AC650.q->f70);
        break;
    case 1:
        func_8005D500(D_800AC650.q->f70);
        break;
    case 2:
        func_8005ECD4(D_800AC650.q->f70);
        break;
    case 3:
        func_8005ABAC(D_800AC650.q->f70);
        break;
    }
}

void func_8005B190(void) {
    D_800AC650.q->f68 = 0;
    D_800AC650.q->f6a = 0;
    D_800AC650.q->f60 = 0;
    D_800AC650.q->f64 = D_800A1F88[D_800F7620];
    D_800AC650.q->f6c = ((short *)D_800AC650.q->f64)[D_800AC650.q->f68 * 4];
    D_800AC650.q->f70 = ((short *)D_800AC650.q->f64)[D_800AC650.q->f68 * 4 + 1];
    switch (D_800AC650.q->f6c) {
    case 0:
        func_8005B8D8(D_800AC650.q->f70);
        break;
    case 1:
        func_8005D500(D_800AC650.q->f70);
        break;
    case 2:
        func_8005ECD4(D_800AC650.q->f70);
        break;
    case 3:
        func_8005ABAC(D_800AC650.q->f70);
        break;
    }
}
