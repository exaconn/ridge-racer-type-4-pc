/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "psyq/libgpu/gpudisp.h"
#include "rr/gpu.h"
#include "rr/library.h"
#include "rr/gpu_reset.h"
#include "rr/spu_tick.h"

#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))

typedef char DrawEnvironmentAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

typedef char GpuFillAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

typedef char GpuUploadAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

typedef char GpuDownloadAddressFitsPointer[
    sizeof(unsigned long) >= sizeof(void *) ? 1 : -1];

extern void PsyqMemcpy(void *, void *, s32);
extern GpuDriver40 *GpuFuncTable;
extern u32 GpuDrawEnvPacket[10];
extern u32 D_801127B0[4];

u32 GetODE(void);
void SetTexWindow(void *packet, GpuRectangleFields *window);
void SetDrawOffset(void *packet, GpuRectangleFields *rectangle);
void SetPriority(void *arg0, s32 arg1, s32 arg2);
void SetDrawStp(GpuDrawModePacket *packet, int enabled);
void SetDrawMode(void *output, s32 drawDisplay, s32 dither, s32 page, GpuRectangleFields *window);
void SetDrawEnv(u32 *arg0, DrawEnvironment *arg1);
void SetDrawEnv2(u32 *arg0, DrawEnvironment *arg1);
u32 get_mode(s32 drawDisplay, s32 dither, s32 page);
u32 get_cs(s32 x, s32 y);
u32 get_ce(s32 x, s32 y);
unsigned get_ofs(int a, int b);
u32 _status(void);
s32 _otc(u8 *address, s32 count);
s32 _clr(GpuRectangle *arg0, u32 arg1);
s32 _dws(GpuRectangle *arg0,const u32 *arg1);
s32 _drs(GpuRectangle *arg0,u32 *arg1);
void _ctl(u32 command);
s32 _getctl(s32 index);
int _cwb(int *a, int n);

DisplayEnvironment *PutDispEnv(DisplayEnvironment *env) {
    u32 mode;
    const GpuDebugSettings *debug;
    const volatile GpuRectangleFields *cachedDisplay, *cachedScreen;
    s32 y0, y1;

    debug = &GpuResetState.debug02;
    mode = 0x08000000;
    if (debug->level >= 2) {
        GPU_printf(StrPutDispEnvFmt, env);
    }
    {
        /* ASM_BARRIER: keep 0x05000000 in $v0 so disp.y shift gets $v1 */
        register u32 cmd5 asm("$2") = 0x05000000;
        GpuFuncTable->control10((((u16)env->display.y & 0x3FF) << 10) | (((u16)env->display.x & 0x3FF) | cmd5));
    }
    cachedDisplay = &GpuResetState.display6C.display;
    if (GpuResetState.display6C.flags10.word != env->flags10.word ||
        cachedDisplay->x != env->display.x ||
        cachedDisplay->y != env->display.y ||
        cachedDisplay->w != env->display.w ||
        cachedDisplay->h != env->display.h) {
        if ((env->flags10.bytes.mode = GetVideoMode()) == 1) {
            mode |= 0x08;
        }
        if (env->flags10.bytes.rgb24) {
            mode |= 0x10;
        }
        if (env->flags10.bytes.interlace) {
            mode |= 0x20;
        }
        if (GpuResetState.debug02.mode) {
            mode |= 0x80;
        }
        if (env->display.w >= 0x119) {
            if (env->display.w < 0x161) {
                mode |= 0x01;
            } else if (env->display.w < 0x191) {
                mode |= 0x40;
            } else if (env->display.w < 0x231) {
                mode |= 0x02;
            } else {
                mode |= 0x03;
            }
        }
        {
            s32 h = env->display.h;
            s32 v = env->flags10.bytes.mode ? (h < 0x121) : (h < 0x101);
            if (!v) {
                mode |= 0x24;
            }
        }
        GpuFuncTable->control10(mode);
        env->flags10.bytes.mode = 8;
    }
    cachedScreen = &GpuResetState.display6C.screen;
    if (cachedScreen->x != env->screen.x ||
        cachedScreen->y != env->screen.y ||
        cachedScreen->w != env->screen.w ||
        cachedScreen->h != env->screen.h ||
        env->flags10.bytes.mode == 8) {
        s32 x0;
        /* ASM_BARRIER: keep x1 in $v1 */
        register s32 x1 asm("$3");
        s32 dx;
        s32 w;
        s32 i;

        env->flags10.bytes.mode = GetVideoMode();
        y0 = env->screen.y + (env->flags10.bytes.mode ? 0x13 : 0x10);
        y1 = env->screen.h ? y0 + env->screen.h : y0 + 0xF0;
        if (env->display.w < 0x119) {
            i = 0;
        } else if (env->display.w < 0x161) {
            i = 1;
        } else if (env->display.w < 0x191) {
            i = 2;
        } else if (env->display.w < 0x231) {
            i = 3;
        } else {
            i = 4;
        }
        x0 = DispHRangeTable[env->flags10.bytes.mode][i].start + env->screen.x * DispDotClockDivisors[i];
        dx = DispHRangeTable[env->flags10.bytes.mode][i].end - DispHRangeTable[env->flags10.bytes.mode][i].start;
        w = env->screen.w;
        if (w) {
            dx = (dx * w) >> 8;
        }
        x1 = x0 + dx;
        /* ASM_BARRIER: force reload of env->flags10.bytes.mode after computing x1 */
        __asm__ volatile("" : : : "memory");
        if (env->flags10.bytes.mode) {
            x0 = CLAMP(x0, 0x21C, 0xC94);
            x1 = CLAMP(x1, x0 + DispDotClockDivisors[i] * 4, 0xCBC);
            y0 = CLAMP(y0, 0x13, 0x12F);
            y1 = CLAMP(y1, y0 + 2, 0x131);
        } else {
            x0 = CLAMP(x0, 0x1F4, 0xCB2);
            x1 = CLAMP(x1, x0 + DispDotClockDivisors[i] * 4, 0xCDA);
            y0 = CLAMP(y0, 0x10, 0x101);
            y1 = CLAMP(y1, y0 + 2, 0x102);
        }
        GpuFuncTable->control10(0x06000000 | ((x1 & 0xFFF) << 12) | (x0 & 0xFFF));
        x1 = (y1 & 0x3FF) << 10;
        GpuFuncTable->control10(0x07000000 | x1 | (y0 & 0x3FF));
    }
    PsyqMemcpy(&GpuResetState.display6C, env, sizeof(*env));
    return env;
}

DisplayEnvironment *GetDispEnv(DisplayEnvironment *environment) {
    PsyqMemcpy(environment, &GpuResetState.display6C, sizeof(*environment));
    return environment;
}


u32 GetODE(void) {
    return GpuFuncTable->status38() >> 0x1F;
}


void SetTexWindow(void *output, GpuRectangleFields *window) {
    GpuDrawModePacket *packet = output;
    packet->tag.bytes.length = 2;
    packet->command[0] = get_tw(window);
    packet->command[1] = 0;
}

void SetDrawArea(void *output, GpuRectangleFields *rectangle) {
    GpuDrawModePacket *packet = output;
    packet->tag.bytes.length = 2;
    packet->command[0] = get_cs(rectangle->x, rectangle->y);
    packet->command[1] = get_ce((s16)(((u16)rectangle->x + (u16)rectangle->w) - 1), (s16)(((u16)rectangle->y + (u16)rectangle->h) - 1));
}

void SetDrawOffset(void *output, GpuRectangleFields *rectangle) {
    GpuDrawModePacket *packet = output;
    packet->tag.bytes.length = 2;
    packet->command[0] = get_ofs(rectangle->x, rectangle->y);
    packet->command[1] = 0;
}

void SetPriority(void *output, s32 maskTest, s32 maskWrite) {
    GpuDrawModePacket *packet = output;
    u32 command;

    packet->tag.bytes.length = 2;
    command = 0xE6000000;
    if (maskTest != 0) {
        command = 0xE6000002;
    }
    packet->command[0] = (command | (maskWrite != 0));
    packet->command[1] = 0;
}

void SetDrawStp(GpuDrawModePacket *packet, int enabled) {
    packet->tag.bytes.length = 2;
    packet->command[0] = enabled ? 0xE6000001 : 0xE6000000;
    packet->command[1] = 0;
}

void SetDrawMode(void *output, s32 drawDisplay, s32 dither, s32 page, GpuRectangleFields *window) {
    GpuDrawModePacket *packet = output;
    packet->tag.bytes.length = 2;
    packet->command[0] = get_mode(drawDisplay, dither, page & 0xFFFF);
    packet->command[1] = get_tw(window);
}

void SetDrawEnv(u32 *arg0, DrawEnvironment *arg1) {
    DrawEnvironment *env = arg1;
    u32 *output = arg0;
    GpuRectangle rect;
    s32 word_count;

    output[1] = get_cs(env->clip.x, env->clip.y);
    output[2] = get_ce((s16)((u16)env->clip.w + (u16)env->clip.x - 1), (s16)((u16)env->clip.y + (u16)env->clip.h - 1));
    output[3] = get_ofs(env->offset_x, env->offset_y);
    output[4] = get_mode(env->draw_display, env->dither, (u16)env->page);
    output[5] = get_tw(&env->window);
    output[6] = 0xE6000000;
    word_count = 7;
    if (env->background != 0) {
        u32 cmd;
        rect.rect.x = env->clip.x;
        rect.rect.y = env->clip.y;
        rect.rect.w = env->clip.w;
        rect.rect.h = env->clip.h;
        rect.rect.w = rect.rect.w < 0 ? 0 : (rect.rect.w > D_800ABAC4 - 1 ? D_800ABAC4 - 1 : rect.rect.w);
        rect.rect.h = rect.rect.h < 0 ? 0 : (rect.rect.h > D_800ABAC6 - 1 ? D_800ABAC6 - 1 : rect.rect.h);
        rect.rect.x -= (u16)env->offset_x;
        rect.rect.y -= (u16)env->offset_y;
        output[word_count++] = ((env->b << 16) | ((env->g << 8) | (cmd = 0x60000000))) | env->r;
        output[word_count++] = rect.words[0];
        output[word_count++] = rect.words[1];
    }
    ((u8 *)output)[3] = word_count - 1;
}
void SetDrawEnv2(u32 *arg0, DrawEnvironment *arg1) {
    DrawEnvironment *env = arg1;
    u32 *output = arg0;
    GpuRectangle rect;
    s32 word_count;

    output[1] = get_cs(env->clip.x, env->clip.y);
    output[2] = get_ce((s16)((u16)env->clip.w + (u16)env->clip.x - 1), (s16)((u16)env->clip.y + (u16)env->clip.h - 1));
    output[3] = get_ofs(env->offset_x, env->offset_y);
    output[4] = get_mode(env->draw_display, env->dither, (u16)env->page);
    output[5] = get_tw(&env->window);
    output[6] = 0xE6000000;
    word_count = 7;
    if (env->background != 0) {
        rect.rect.x = env->clip.x;
        rect.rect.y = env->clip.y;
        rect.rect.w = env->clip.w;
        rect.rect.h = env->clip.h;
        rect.rect.w = rect.rect.w < 0 ? 0 : (rect.rect.w > D_800ABAC4 - 1 ? D_800ABAC4 - 1 : rect.rect.w);
        rect.rect.h = rect.rect.h < 0 ? 0 : (rect.rect.h > D_800ABAC6 - 1 ? D_800ABAC6 - 1 : rect.rect.h);
        if (((u16)rect.rect.x & 63) || ((u16)rect.rect.w & 63)) {
            u32 cmd;
            u32 c_off = word_count++ * 4U;
            u32 xy_off = word_count++ * 4U;
            u32 sz_off = word_count++ * 4U;
            rect.rect.x -= (u16)env->offset_x;
            rect.rect.y -= (u16)env->offset_y;
            *(u32 *)(c_off + (unsigned long)output) = ((env->b << 16) | ((env->g << 8) | (cmd = 0x60000000))) | env->r;
            output[xy_off / 4U] = rect.words[0];
            output[sz_off / 4U] = rect.words[1];
        } else {
            u32 cmd;
            output[word_count++] = ((env->b << 16) | ((env->g << 8) | (cmd = 0x02000000))) | env->r;
            output[word_count++] = rect.words[0];
            output[word_count++] = rect.words[1];
        }
    }
    ((u8 *)output)[3] = word_count - 1;
}
u32 get_mode(s32 drawDisplay, s32 dither, s32 page) {
    return (dither ? 0xE1000200 : 0xE1000000) | (drawDisplay ? 0x400 : 0) | (page & 0x9FF);
}
u32 get_cs(s32 x, s32 y) {
    s16 clippedX = x;
    s16 clippedY = y;
    clippedX = clippedX < 0 ? 0 : (clippedX > D_800ABAC4 - 1 ? D_800ABAC4 - 1 : clippedX);
    clippedY = clippedY < 0 ? 0 : (clippedY > D_800ABAC6 - 1 ? D_800ABAC6 - 1 : clippedY);
    return 0xE3000000 | ((clippedY & 0x3FF) << 10) | (clippedX & 0x3FF);
}
u32 get_ce(s32 x, s32 y) {
    s16 clippedX = x;
    s16 clippedY = y;
    clippedX = clippedX < 0 ? 0 : (clippedX > D_800ABAC4 - 1 ? D_800ABAC4 - 1 : clippedX);
    clippedY = clippedY < 0 ? 0 : (clippedY > D_800ABAC6 - 1 ? D_800ABAC6 - 1 : clippedY);
    return 0xE4000000 | ((clippedY & 0x3FF) << 10) | (clippedX & 0x3FF);
}
u32 get_ofs(s32 x, s32 y) {
    return 0xE5000000U | ((y & 0x7FF) << 11) | (x & 0x7FF);
}

u32 get_tw(GpuRectangleFields *window) {
    /* E2 offsets and masks are encoded in units of eight pixels. */
    struct {s32 offsetX, offsetY, maskWidth, maskHeight;} fields;
    u32 verticalBits, horizontalCommand, command;
    if (window == 0) {
        command = 0;
    } else {
        fields.offsetX = (u8)window->x >> 3;
        fields.maskWidth = ((-window->w) & 0xFF) >> 3;
        fields.offsetY = (u8)window->y >> 3;
        verticalBits = fields.offsetY << 15;
        fields.maskHeight = ((-window->h) & 0xFF) >> 3;
        horizontalCommand = (fields.offsetX << 10) | 0xE2000000U;
        command = ((verticalBits | horizontalCommand) | (fields.maskHeight << 5)) | fields.maskWidth;
    }
    return command;
}


u32 _status(void) {
    return *GPU_STATUS;
}


s32 _otc(u8 *address, s32 count) {
    u32 offset, mask;
    u32 *configuration = DPCR;
    *configuration |= 0x08000000;
    *DMA6_CHCR = 0;
    offset = count * 4U - 4;
    *DMA6_MADR = address + offset;
    *DMA6_BCR = count;
    *(u32 *)DMA6_CHCR = 0x11000002;
    set_alarm();
    while ((mask = 0x01000000, (*DMA6_CHCR & mask) != 0)) {
        if (get_alarm() != 0) return -1;
    }
    return count;
}
s32 _clr(GpuRectangle *arg0, u32 color) {
    GpuRectangle *r = arg0;
    r->rect.w = r->rect.w < 0 ? 0 : (r->rect.w > D_800ABAC4 - 1 ? D_800ABAC4 - 1 : r->rect.w);
    r->rect.h = r->rect.h < 0 ? 0 : (r->rect.h > D_800ABAC6 - 1 ? D_800ABAC6 - 1 : r->rect.h);
    if (((u16)r->rect.x & 63) || ((u16)r->rect.w & 63)) {
        u32 *packet = GpuDrawEnvPacket;
        u32 *secondary = D_801127B0;
        u32 e3 = 0xE3000000U, e5 = 0xE5000000U;
        packet[0] = ((unsigned long)secondary & 0xFFFFFFU) | 0x08000000U;
        packet[1] = e3;
        packet[2] = 0xE4FFFFFFU;
        packet[3] = e5;
        packet[4] = 0xE6000000U;
        packet[5] = 0xE1000000U | ((*GPU_STATUS) & 0x7FFU) | ((color >> 31) << 10);
        packet[6] = 0x60000000U | (color & 0xFFFFFFU);
        packet[7] = r->words[0];
        packet[8] = r->words[1];
        secondary[0] = 0x03FFFFFFU;
        secondary[1] = _param(3) | e3;
        secondary[2] = _param(4) | 0xE4000000U;
        secondary[3] = _param(5) | e5;
    } else {
        u32 *packet = GpuDrawEnvPacket;
        packet[0] = 0x05FFFFFFU;
        packet[1] = 0xE6000000U;
        packet[2] = 0xE1000000U | ((*GPU_STATUS) & 0x7FFU) | ((color >> 31) << 10);
        packet[3] = 0x02000000U | (color & 0xFFFFFFU);
        packet[4] = r->words[0];
        packet[5] = r->words[1];
    }
    _cwc((unsigned long)GpuDrawEnvPacket);
    return 0;
}
/* The small CPU tail uses compiler-fenced stores so GCC can place the final
 * write in a branch delay slot. The per-iteration and final memory fences
 * make every port write visible and ordered before subsequent accesses. */
s32 _dws(GpuRectangle *arg0, const u32 *arg1) {
    s32 count, n, blocks, mode;
    set_alarm();
    mode = 0;
    arg0->rect.w = arg0->rect.w < 0 ? 0 : (arg0->rect.w > D_800ABAC4 ? D_800ABAC4 : arg0->rect.w);
    arg0->rect.h = arg0->rect.h < 0 ? 0 : (arg0->rect.h > D_800ABAC6 ? D_800ABAC6 : arg0->rect.h);
    count = (arg0->rect.w * arg0->rect.h + 1) / 2;
    if (count <= 0) return -1;
    n = count % 16;
    blocks = count / 16;
    while (!(*GPU_STATUS & 0x04000000U)) {
        if (get_alarm()) return -1;
    }
    *GPU_STATUS = 0x04000000U;
    *GPU_DATA = 0x01000000U;
    *GPU_DATA = mode ? 0xB0000000U : 0xA0000000U;
    *GPU_DATA = arg0->words[0];
    *GPU_DATA = arg0->words[1];
    n--;
    if (n != -1) {
        do {
            u32 word = *arg1++;
            n--;
            *(u32 *)GPU_DATA = word;
        } while (n != -1);
    }
    if (blocks) {
        *GPU_STATUS = 0x04000002U;
        *DMA2_MADR = (u32)(unsigned long)arg1;
        *DMA2_BCR = (blocks << 16) | 16U;
        *DMA2_CHCR = 0x01000201U;
    }
    return 0;
}
s32 _drs(GpuRectangle *arg0, u32 *arg1) {
    s32 count, n, blocks;
    set_alarm();
    arg0->rect.w = arg0->rect.w < 0 ? 0 : (arg0->rect.w > D_800ABAC4 ? D_800ABAC4 : arg0->rect.w);
    arg0->rect.h = arg0->rect.h < 0 ? 0 : (arg0->rect.h > D_800ABAC6 ? D_800ABAC6 : arg0->rect.h);
    count = (arg0->rect.w * arg0->rect.h + 1) / 2;
    if (count <= 0) return -1;
    n = count % 16;
    blocks = count / 16;
    while (!(*GPU_STATUS & 0x04000000U)) {
        if (get_alarm()) return -1;
    }
    *GPU_STATUS = 0x04000000U;
    *GPU_DATA = 0x01000000U;
    *GPU_DATA = 0xC0000000U;
    *GPU_DATA = arg0->words[0];
    *GPU_DATA = arg0->words[1];
    while (!(*GPU_STATUS & 0x08000000U)) {
        if (get_alarm()) return -1;
    }
    n--;
    if (n != -1) {
        do {
            u32 word = *GPU_DATA;
            n--;
            *arg1++ = word;
        } while (n != -1);
    }
    if (blocks) {
        *GPU_STATUS = 0x04000003U;
        *DMA2_MADR = (u32)(unsigned long)arg1;
        *DMA2_BCR = (blocks << 16) | 16U;
        *DMA2_CHCR = 0x01000200U;
    }
    return 0;
}
void _ctl(u32 command) {
    /* ASM_BARRIER: reuse v0 for the port pointer and then the command index. */
    register u32 index __asm__("$2");
    /* ASM_BARRIER: retain the original assembler temporary for the history address. */
    register u8 *base __asm__("$1");
    {
        /* ASM_BARRIER: keep the command-port pointer in v0 until the word is written. */
        register u32 *port __asm__("$2") = GPU_STATUS;
        *port = command;
    }
    index = command >> 24;
    /* ASM_BARRIER: finish the port write before binding the history address. */
    __asm__ volatile("" : "=r"(index) : "0"(index) : "$1", "memory");
    /* PAL RAM mapping: GpuCtlShadow is base plus 0x27D0. */
    base = (u8 *)0x80110000U;
    base += index;
    base[0x27D0] = command;
}


s32 _getctl(s32 index) {
    /* PAL RAM mapping: GpuCtlShadow is 0x27D0 bytes into this page. */
    u8 *base = (u8 *)0x80110000U;
    /* ASM_BARRIER: retain the base register through the indexed byte load. */
    __asm__("" : "=r"(base) : "0"(base));
    base += index;
    return base[0x27D0];
}

int _cwb(int *a, int n) {
    int i = n - 1;
    *(int *)GPU_STATUS = 0x04000000;
    if (n) {
        do { int v = *a; a++; i--; *GPU_DATA = v; } while (i != -1);
    }
    return 0;
}

void _cwc(unsigned long address) {
    *GPU_STATUS=0x04000002;
    *DMA2_MADR=address;
    *DMA2_BCR=0;
    *(u32 *)DMA2_CHCR=0x01000401;
}

s32 _param(s32 a) {
    *GPU_STATUS = a | 0x10000000;
    return *GPU_DATA & 0xFFFFFF;
}
