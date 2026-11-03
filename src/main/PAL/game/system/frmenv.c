#include "rr/frame_environment.h"
#include "rr/frame_loop.h"
#include "game/car/carstat.h"
#include "game/data/cardini.h"
#include "game/data/cdfile1.h"
#include "game/data/trkld8.h"
#include "game/gfx/fadespr.h"
#include "game/gfx/model.h"
#include "game/menu/menu8.h"
#include "game/race/racesamp.h"
#include "game/race/ranking.h"
#include "psyq/libgpu/gpuenv.h"
#include "psyq/libsio/siostart.h"
#include "psyq/libspu/sinit2.h"
#include "game/data/ldscn1.h"
#include "game/menu/menusave.h"
#include "game/system/frmenv.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/audio_cd_controller.h"
#include "rr/library.h"
#include "rr/driver_control.h"
#include "rr/bios_devices.h"

extern int func_80052800();
extern int func_800929D4();
extern int func_800907FC();
extern int func_8004A678();
extern int func_80013890();
extern int func_8002B75C();
extern int func_8003A1B4();
extern int D_800102F0;
extern char D_800102F4[];
extern char D_80010300[];
extern char D_8001030C[];
extern char D_80010320[];
extern char D_80010328[];
extern char D_80010348[];
extern char D_80010350[];
extern char D_80010364[];
extern char D_8001036C[];
extern char D_80010384[];
extern char D_8001038C[];
extern char D_80010398[];
extern char D_800103A0[];
extern char D_800103AC[];
extern char D_800ABEF8[];
extern int D_800ACC18;
extern int D_800AD568;
extern signed char D_800AD4D8[];
extern short D_800AD508;
extern char D_800F2A68[];
extern char D_800F2FE0[];
extern char D_800F3920[];
extern char D_800F39D8[];
extern char D_800F3B28[];
extern char D_800F4D98[];
extern signed char D_800F2E30[];
extern int D_800F2DE8[];
extern int D_800F2E08[];
extern int D_800F4BD0[];
extern int D_800F3A30;
extern int D_800F4C24;
extern int D_800F4C2C;
extern int D_800F4CA8;
extern int D_800F4D40;
extern short D_800F4D44;
extern char *D_800F3B20;
extern void func_800912B4(s32,s32);
extern void func_800912D4(s32);
extern void *func_80092998(void *,s32,s32,s32,s32);

void func_8001ECE8(s32 r,s32 g,s16 b,u32 flags);

void func_8001E988(void)
{
    int i;
    int *p;
    char *s, *q;
    signed char *r;
    int k, c5;
    int m;

    func_8001E84C(D_800102F4);
    func_8008BE44();
    func_800123D4();
    func_8007FA54();
    func_8004EC28();
    func_80052800();
    func_800929D4(0);
    func_80092B48(0);
    func_80092CB8(0);
    func_800907FC();
    func_80077E10(1);
    func_8004A55C();
    func_8005C0C4();
    func_8004A678();
    func_8009A8B4();
    func_8009AEAC(1, 1, 0);
    func_80013890(5);
    D_800AC5DC = 0x80;
    D_800F4C24 = 0;
    D_800F4C2C = 0;
    D_800ACBF8 = 0;
    D_800F3A30 = 2;
    D_800AD568 = 0;
    func_80096434(D_800F3920, 0x7C);
    func_80096434(D_800F4D98, 0x20);
    func_80096434(D_800AD4D8, 0x2C);
    D_800AD4D8[0x17] = -1;
    func_80096434(D_800F2A68, 0x40);
    i = 7;
    do {
        D_800F2DE8[i] = (int)(D_800F2A68 + i * 8);
        i--;
    } while (i >= 0);
    func_80096434(D_800ABEF8, 0x320);
    func_80096434(D_800ABEF8 + 0x320, 0x320);
    D_800F4BE0 = 0;
    i = 0;
    c5 = 5;
    q = D_800F3920;
    do {
        *(volatile char *)(q + 0x6C) = c5;
        *(volatile char *)(q + 0x6D) = c5;
        *(volatile char *)(q + 0x6E) = 0;
        __asm__ volatile("" : : : "memory"); /* ASM_BARRIER: keep i++ after the byte stores like the original schedule */
        i++;
        q += 4;
    } while (i < 4);
    D_800AD508 = -1;
    func_8002B75C(D_800F3920, D_80010300);
    func_8002B75C(D_800F39D8, D_800F3920);
    func_8002B75C(D_800F39D8 + 0xC, D_800F3920);
    func_8002B75C(D_800F39D8 + 0x18, D_800F3920);
    func_8002B75C(D_800F39D8 + 0x24, D_800F3920);
    D_800F2E08[0] = (int)D_8001030C;
    D_800F2E08[4] = (int)D_80010320;
    D_800F2E08[1] = (int)D_80010328;
    D_800F2E08[5] = (int)D_80010348;
    D_800F2E08[2] = (int)D_80010350;
    D_800F2E08[6] = (int)D_80010364;
    D_800F2E08[3] = (int)D_8001036C;
    D_800F2E08[7] = (int)D_80010384;
    D_800F4BD0[0] = (int)D_8001038C;
    D_800F4BD0[1] = (int)D_80010398;
    D_800F4BD0[2] = (int)D_800103A0;
    D_800F4BD0[3] = (int)D_800103AC;
    D_800ACC18 = D_800102F0;
    D_800F4D40 = D_800102F0;
    func_8003A1B4();
    k = 0xAF000;
    m = -1;
    i = 2;
    D_800F4CA8 = 0;
    D_800F4D44 = 0;
    D_800AC5A0 = D_800102F0 + k;
    r = D_800F2E30 + i * 0x2C;
    do {
        r[0x17] = m;
        i--;
        r -= 0x2C;
    } while (i >= 0);
    func_8001EEE4(k, i, r, m);
    func_8001F194();
    func_8006F5F0(D_800F3B28);
    func_8006F5F0(D_800F3B28 + 0x840);
    func_8006F5F0(D_800F2FE0);
    D_800F3B20 = D_800F3B28;
    func_80031DB0();
    func_800373DC(1);
    func_800929D4(3);
}

void func_8001ECE0(s32 bank) {
}

void func_8001ECE8(s32 r,s32 g,s16 b,u32 flags) {
    /* ASM_BARRIER: keep flags in S0 until the arena replaces their dead value. */
    register u32 input asm("$16")=flags;
    /* ASM_BARRIER: retain red in the original saved register across environment helpers. */
    register s32 red asm("$23")=r;
    register s32 green;
    register u32 temp;
    /* ASM_BARRIER: retain interlace independently of height and offset. */
    register s32 interlace asm("$19");
    register u32 height;
    register u32 other_y;
    register s32 width;
    /* ASM_BARRIER: retain the wide-mode bit across environment helpers. */
    register u32 wide asm("$21");
    /* ASM_BARRIER: preserve flags until the RGB24 loop value is extracted. */
    register u32 saved_flags asm("$22");
    /* ASM_BARRIER: reuse S0 for the arena after the input flags have been consumed. */
    register RrFrameStorage *env asm("$16");
    RrGpuRectangleFields window;
    register s32 i;
    register u16 *settings;
    register s32 one;
    /* ASM_BARRIER: keep the loop wide-mode value in A3 after the helper calls. */
    register u32 loop_wide asm("$7");
    /* ASM_BARRIER: keep the loop RGB24 value in T0 after the helper calls. */
    register u32 rgb24 asm("$8");
    register s32 center_x;
    register u32 center_y;
    /* ASM_BARRIER: capture red and flags before assigning green and geometry temporaries. */
    __asm__("" : "+r"(red), "+r"(input) : : "$2", "$30");
    green=g;
    center_x=160;
    /* ASM_BARRIER: capture green and the X center before using V0 for geometry. */
    __asm__("" : "+r"(green), "+r"(center_x) : : "$2");
    temp=input>>1;
    /* ASM_BARRIER: preserve the intermediate before masking its low bit. */
    __asm__("" : "+r"(temp));
    interlace=temp&1;
    temp=interlace+1;
    height=temp<<8;
    center_y=height>>1;
    /* ASM_BARRIER: finish the Y center before reusing V0 for the opposite bank offset. */
    __asm__("" : "+r"(center_y) : : "$2");
    temp=1-interlace;
    other_y=temp<<8;
    func_800912B4(center_x,center_y);
    func_800912D4(290);
    width=320;
    wide=input&4;
    saved_flags=input;
    if(wide)width=480;
    env=D_800ADAF0;
    func_800928E4(env,0,0,width,height);
    func_800928E4(&D_800D0268,0,other_y,width,height);
    func_80092998(&env->environment.display,0,other_y,width,height);
    func_80092998(&D_800D02C4,0,0,width,height);
    i=0;
    rgb24=saved_flags&1;
    one=1;
    settings=D_800AD528.halves;
    loop_wide=wide;
    window.y=0;window.x=0;window.h=0;window.w=0;
loop:
    {
        env->environment.r=red;env->environment.g=green;env->environment.b=b;
        env->environment.rgb24=rgb24;
        env->environment.dither=one;
        env->environment.window=window;
        env->environment.screen.x=settings[0x34/2];
        env->environment.screen.y=settings[0x38/2];
        env->environment.display.h=height;
        env->environment.interlace=interlace;
        env->environment.screen.h=256;
        if(loop_wide) {
            env->environment.high_res=one;
            env->environment.display.w=(env->environment.display.w*2)/3;
        }
        env++;
    }
    i++;
    if(i<2)goto loop;
    func_8001ECE0(0);
    func_8001ECE0(1);
}
