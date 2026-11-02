/* CC1_FLAGS: -mno-split-addresses */
/* RODATA 0x800117b8 0x3c */

typedef struct {
    short left;
    short right;
} SpuVolume;

typedef struct {
    unsigned int voice;
    unsigned int mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    unsigned short pitch;
    unsigned short note;
    unsigned short sample_note;
    short envx;
    unsigned int addr;
    unsigned int loop_addr;
    int a_mode;
    int s_mode;
    int r_mode;
    unsigned short ar;
    unsigned short dr;
    unsigned short sr;
    unsigned short rr;
    unsigned short sl;
    unsigned short adsr1;
    unsigned short adsr2;
} SpuVoiceAttr;

extern int func_8007F414(int, int);
extern short func_80084C74(unsigned int, int, unsigned int, int);
extern volatile unsigned short *D_800A47D8;
extern unsigned short D_800A4874[];

void func_80084674(SpuVoiceAttr *s0) {
    struct { int counter; volatile unsigned int accumulator; } work;
    int s4;
    int s3;
    /* ASM_BARRIER: Keep mask and zero-mask flag in s1 and s2 so incoming arg0 gets s0. */
    register unsigned int s1 __asm__("$17");
    /* ASM_BARRIER: Keep zero-mask flag in s2 as in original register allocation. */
    register int s2 __asm__("$18");
    int a0;
    unsigned int ua1;
    int a2;
    unsigned int a3;

    s1 = s0->mask;
    s2 = (s1 == 0);
    for (s4 = 0; s4 < 24; s4++) {
        if (s0->voice & (1 << s4)) {
            s3 = s4 * 8;
            if (s2 || (s1 & 0x10)) {
                D_800A47D8[s3 + 2] = s0->pitch;
            }
            if (s2 || (s1 & 0x40)) {
                D_800A4874[s4] = s0->sample_note;
            }
            if (s2 || (s1 & 0x20)) {
                ua1 = D_800A4874[s4];
                a3 = s0->note;
                D_800A47D8[s3 + 2] = func_80084C74(ua1 >> 8, ua1 & 0xFF, a3 >> 8, a3 & 0xFF);
            }
            if (s2 || (s1 & 0x1)) {
                a0 = 0;
                ua1 = s0->volume.left & 0x7FFF;
                if (s2 || (s1 & 0x4)) {
                    switch (s0->volmode.left) {
                    case 1:
                        a0 = 0x8000;
                        break;
                    case 2:
                        a0 = 0x9000;
                        break;
                    case 3:
                        a0 = 0xA000;
                        break;
                    case 4:
                        a0 = 0xB000;
                        break;
                    case 5:
                        a0 = 0xC000;
                        break;
                    case 6:
                        a0 = 0xD000;
                        break;
                    case 7:
                        a0 = 0xE000;
                        break;
                    }
                }
                if (a0 != 0) {
                    if (s0->volume.left >= 0x80) {
                        ua1 = 0x7F;
                    } else if (s0->volume.left < 0) {
                        ua1 = 0;
                    }
                }
                D_800A47D8[s3 + 0] = ua1 | a0;
            }
            if (s2 || (s1 & 0x2)) {
                a0 = 0;
                ua1 = s0->volume.right & 0x7FFF;
                if (s2 || (s1 & 0x8)) {
                    switch (s0->volmode.right) {
                    case 1:
                        a0 = 0x8000;
                        break;
                    case 2:
                        a0 = 0x9000;
                        break;
                    case 3:
                        a0 = 0xA000;
                        break;
                    case 4:
                        a0 = 0xB000;
                        break;
                    case 5:
                        a0 = 0xC000;
                        break;
                    case 6:
                        a0 = 0xD000;
                        break;
                    case 7:
                        a0 = 0xE000;
                        break;
                    }
                }
                if (a0 != 0) {
                    if (s0->volume.right >= 0x80) {
                        ua1 = 0x7F;
                    } else if (s0->volume.right < 0) {
                        ua1 = 0;
                    }
                }
                D_800A47D8[s3 + 1] = ua1 | a0;
            }
            if (s2 || (s1 & 0x80)) {
                func_8007F414(s3 | 3, s0->addr);
            }
            if (s2 || (s1 & 0x10000)) {
                func_8007F414(s3 | 7, s0->loop_addr);
            }
            if (s2 || (s1 & 0x20000)) {
                D_800A47D8[s3 + 4] = s0->adsr1;
            }
            if (s2 || (s1 & 0x40000)) {
                D_800A47D8[s3 + 5] = s0->adsr2;
            }
            if (s2 || (s1 & 0x800)) {
                ua1 = s0->ar;
                if (ua1 >= 0x80) {
                    ua1 = 0x7F;
                }
                a2 = 0;
                if (s2 || (s1 & 0x100)) {
                    switch (s0->a_mode) {
                    case 5:
                        a2 = 0x80;
                        break;
                    }
                }
                {
                    /* ASM_BARRIER: Keep loaded hardware register in v0 before masking into v1. */
                    register unsigned short v0 __asm__("$2") = D_800A47D8[s3 + 4];
                    /* ASM_BARRIER: Keep masked hardware register in v1 across envelope combine. */
                    register unsigned int v1 __asm__("$3") = v0 & 0xFF;
                    D_800A47D8[s3 + 4] = v1 | ((ua1 | a2) << 8);
                }
            }
            if (s2 || (s1 & 0x1000)) {
                ua1 = s0->dr;
                if (ua1 >= 0x10) {
                    ua1 = 0xF;
                }
                D_800A47D8[s3 + 4] = (D_800A47D8[s3 + 4] & 0xFF0F) | (ua1 << 4);
            }
            if (s2 || (s1 & 0x2000)) {
                ua1 = s0->sr;
                if (ua1 >= 0x80) {
                    ua1 = 0x7F;
                }
                a2 = 0x100;
                if (s2 || (s1 & 0x200)) {
                    switch (s0->s_mode) {
                    case 1:
                        a2 = 0;
                        break;
                    case 5:
                        a2 = 0x200;
                        break;
                    case 7:
                        a2 = 0x300;
                        break;
                    }
                }
                {
                    /* ASM_BARRIER: Keep loaded hardware register in v0 before masking into v1. */
                    register unsigned short v0 __asm__("$2") = D_800A47D8[s3 + 5];
                    /* ASM_BARRIER: Keep masked hardware register in v1 across envelope combine. */
                    register unsigned int v1 __asm__("$3") = v0 & 0x3F;
                    D_800A47D8[s3 + 5] = v1 | ((ua1 | a2) << 6);
                }
            }
            if (s2 || (s1 & 0x4000)) {
                ua1 = s0->rr;
                if (ua1 >= 0x20) {
                    ua1 = 0x1F;
                }
                a2 = 0;
                if (s2 || (s1 & 0x400)) {
                    switch (s0->r_mode) {
                    case 3:
                        break;
                    case 7:
                        a2 = 0x20;
                        break;
                    }
                }
                D_800A47D8[s3 + 5] = (D_800A47D8[s3 + 5] & 0xFFC0) | (ua1 | a2);
            }
            if (s2 || (s1 & 0x8000)) {
                ua1 = s0->sl;
                if (ua1 >= 0x10) {
                    ua1 = 0xF;
                }
                D_800A47D8[s3 + 4] = (D_800A47D8[s3 + 4] & 0xFFF0) | ua1;
            }
        }
    }
    work.accumulator = 1;
    work.counter = 0;
    goto test;
body:
    {
        /* ASM_BARRIER: Keep the loaded accumulator separate from the multiply result. */
        register unsigned int value __asm__("$3") = work.accumulator;
        work.accumulator = value * 13;
    }
    *(int *)&work.counter = *(volatile int *)&work.counter + 1;
test:
    if (*(int *)&work.counter < 2) goto body;
}
