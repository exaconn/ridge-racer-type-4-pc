/* GCC_VERSION: 970318 */
#include "rr/gpu_address.h"
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "types.h"
#include "rr/gpu_reset.h"
#include "rr/gpu.h"
#include "rr/library.h"
#include "psyq/libgpu/gpudisp.h"
#include "psyq/libgpu/addprim.h"
#include "psyq/libgpu/gpubase2.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/spu_tick.h"
#include "rr/driver_control.h"

extern int SetGraphQueFmt[];
extern char DrawSyncCallbackFmt[];
extern char StrSetDispMaskFmt[];
extern s32 DrawSyncFmt;
extern char StrBadRectFmt[];
extern char StrRectFmt[];
extern char D_80011EC8[];
extern char StrClearImage[];
extern char ClearImage2Name[];
/* extern */
extern char LoadImageName[];
extern char ClearOTagFmt[];
extern char D_800ABB6C[];
extern int D_800ABB80;
extern char StrClearOTagRFmt[];
extern int PsyqMemcpy();
extern char PutDrawEnvFmt[];
extern char StrDrawOTagEnvFmt[];

u16 LoadTPage(int *pix, int tp, int abr, int x, int y, int w, int h);
u16 LoadClut(int a, int b, int c);
u16 LoadClut2(int a, int b, int c);
DisplayEnvironment *SetDefDispEnv(DisplayEnvironment *p, int a, int b, int c, int d);
int SetGraphQueue();
u8 GetGraphDebug(void);
void DrawSync(s32 arg0);
void checkRECT(const char *name,GpuRectangle *rectangle);
void ClearImage(GpuRectangle *a, int b, int c, int d);
void ClearImage2(GpuRectangle *a, int b, int c, int d);
void LoadImage(GpuRectangle *arg0, const void *arg1);
void StoreImage(GpuRectangle *a, void *b);
int MoveImage();
int *ClearOTag();
unsigned *ClearOTagR(unsigned *a, int b);
void DrawPrim(char *a0);
void DrawOTag(s32 ordering_table);

u16 LoadTPage(int *pix, int tp, int abr, int x, int y, int w, int h) {
    short sp[4];
    sp[0] = x;
    sp[1] = y;
    sp[3] = h;
    switch (tp) {
    case 0:
        sp[2] = w / 4;
        break;
    case 1:
        sp[2] = w / 2;
        break;
    case 2:
        sp[2] = w;
        break;
    }
    LoadImage(sp, pix);
    return GetTPage(tp, abr, x, y);
}

u16 LoadClut(int a, int b, int c) {
    short sp[4];
    sp[2] = 0x100;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    LoadImage(sp, a);
    return GetClut(b, c);
}

u16 LoadClut2(int a, int b, int c) {
    short sp[4];
    sp[2] = 0x10;
    sp[0] = b;
    sp[1] = c;
    sp[3] = 1;
    LoadImage(sp, a);
    return GetClut(b, c);
}

DrawEnvironment *SetDefDrawEnv(DrawEnvironment *environment, int x, int y, int width, int height) {
    int videoMode = GetVideoMode();
    environment->clip.x = x;
    environment->clip.y = y;
    environment->clip.w = width;
    environment->window.x = 0;
    environment->window.y = 0;
    environment->window.w = 0;
    environment->window.h = 0;
    environment->r = 0;
    environment->g = 0;
    environment->b = 0;
    environment->dither = 1;
    environment->clip.h = height;
    if (videoMode) environment->draw_display = height < 0x121; else environment->draw_display = height < 0x101;
    environment->offset_x = x;
    environment->offset_y = y;
    environment->page = 0xA;
    environment->background = 0;
    return environment;
}

DisplayEnvironment *SetDefDispEnv(DisplayEnvironment *p, int a, int b, int c, int d) {
    p->display.x = a; p->display.y = b; p->display.w = c;
    p->screen.x = 0; p->screen.y = 0; p->screen.w = 0; p->screen.h = 0;
    p->flags10.bytes.rgb24 = 0; p->flags10.bytes.interlace = 0; p->flags10.bytes.reserved = 0; p->flags10.bytes.mode = 0;
    p->display.h = d;
    return p;
}

s32 ResetGraph(s32 mode)
{
    GpuResetState80 *state;
    switch(mode & 7) {
    case 0:
    case 3:
        DebugPrintStub(ResetGraphFmt,&GpuDriverTable,&GpuResetState);
    case 5:
        state=&GpuResetState;
        GPU_memset((s8 *)state,0,sizeof(*state));
        ResetCallback();
        GPU_cw((unsigned long)GpuFuncTable & 0xFFFFFF);
        state->kind00=_reset(mode);
        state->enabled01=1;
        /* Preserve the explicit byte reload after the reset service. */
        state->debug02.width=D_800ABB40[*(volatile u8 *)&state->kind00].value;
        state->debug02.height=D_800ABB4C.dimensions[state->kind00].value;
        GPU_memset((s8 *)&state->draw10,-1,sizeof(state->draw10));
        GPU_memset((s8 *)&state->display6C,-1,sizeof(state->display6C));
        return state->kind00;
    default:
        if(D_800ABAC2.level>=2) GPU_printf(StrResetGraphFmt,mode);
        return GpuFuncTable->reset34(1);
    }
}

s32 SetGraphDebug(s32 enabled) {
    u8 *state = &D_800ABAC2.level;
    s32 previous;
    previous = state[0];
    state[0] = enabled;
    if (enabled & 0xFF) {
        extern const char SetGraphDebugFmt[];
        GPU_printf(SetGraphDebugFmt, state[0], state[-2], state[1]);
        return previous;
    }
    return previous;
}

int SetGraphQueue(a0) int a0; {
    unsigned char *p;
    int old;
    p = &GpuResetState.enabled01;
    old = p[0];
    if (p[1] >= 2) {
        GPU_printf(SetGraphQueFmt, a0);
    }
    if (a0 != p[0]) {
        GpuFuncTable->reset34(1);
        p[0] = a0;
        DMACallback(2, 0);
    }
    return old;
}

u8 GetGraphDebug(void) {
    return D_800ABAC2.level;
}

GpuCompletionCallback DrawSyncCallback(GpuCompletionCallback callback) {
    GpuCompletionCallback previous;
    if (GpuResetState.debug02.level >= 2) GPU_printf(DrawSyncCallbackFmt, callback);
    previous = GpuResetState.completion0C;
    GpuResetState.completion0C = callback;
    return previous;
}

void SetDispMask(s32 enabled) {
    unsigned char *p = &GpuResetState.debug02.level;
    if (*p >= 2) GPU_printf(StrSetDispMaskFmt, enabled);
    if (enabled == 0) GPU_memset((s8 *)&GpuResetState.display6C, -1, sizeof(GpuResetState.display6C));
    GpuFuncTable->control10(enabled ? 0x3000000 : 0x3000001);
}

void DrawSync(s32 arg0) {
    if (D_800ABAC2.level >= 2U) {
        GPU_printf(&DrawSyncFmt, arg0);
    }
    GpuFuncTable->sync3C(arg0);
}

void checkRECT(const char *name,GpuRectangle *rectangle) {
    const char *label=name;
    GpuRectangle *rect=rectangle;
    GpuDebugSettings *settings=&D_800ABAC2;
    const char *format;
    switch(settings->level) {
    case 1: {
        s32 w=rect->rect.w;
        s32 maxw=settings->width;
        s32 x;
        s32 y;
        s32 h;
        s32 maxh;
        s32 comparison;
        if(maxw<w) goto invalid;
        x=rect->rect.x;
        comparison=maxw<w+x;
        if(comparison) goto invalid;
        y=rect->rect.y;
        maxh=settings->height;
        if(maxh<y) goto invalid;
        h=rect->rect.h;
        comparison=maxh<y+h;
        if(comparison) goto invalid;
        if(w<=0) goto invalid;
        if(x<0) goto invalid;
        if(y<0) goto invalid;
        if(h>0) return;
    }
invalid:
        format=StrBadRectFmt;
        goto report;
    case 2:
        format=D_80011EC8;
report:
        GPU_printf(format,label);
        {
            s32 x=rect->rect.x;
            s32 y=rect->rect.y;
            s32 w=rect->rect.w;
            s32 h=rect->rect.h;
            /* ASM_BARRIER: Reload all rectangle fields before fetching the second diagnostic callback. */
            __asm__("" : : "r"(x),"r"(y),"r"(w),"r"(h) : "$2");
            GPU_printf(StrRectFmt,x,y,w,h);
        }
    }
}

void ClearImage(GpuRectangle *a, int b, int c, int d)
{
    u32 v;
    u8 nv;
    nv = b & 0xFF;
    checkRECT(StrClearImage, a);
    v = (((d & 0xFF) << 16) | ((c & 0xFF) << 8)) | nv;
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->clear0C, a, 8, v);
}

void ClearImage2(GpuRectangle *a, int b, int c, int d)
{
    u32 v;
    u8 nv;
    int t, u, k;
    nv = b & 0xFF;
    checkRECT(ClearImage2Name, a);
    u = (d & 0xFF) << 16;
    t = (c & 0xFF) << 8;
    k = 0x80000000;
    v = (u | (t | k)) | nv;
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->clear0C, a, 8, v);
}

void LoadImage(GpuRectangle *arg0, const void *arg1) {
    checkRECT(LoadImageName, arg0);
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->upload20, arg0, 8, (unsigned long)arg1);
}

void StoreImage(GpuRectangle *a, void *b) {
    checkRECT(StrStoreImage, a);
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->download1C, a, 8, (unsigned long)b);
}

int MoveImage(a0, a1, a2)
GpuRectangle *a0; int a1; int a2;
{
    GpuMovePayload12 *p;
    GpuDriver40 *o;
    int w;
    checkRECT(MoveImageNameStr, a0);
    if (a0->rect.w == 0) return -1;
    if (a0->rect.h != 0) {
        p = &GpuMovePayload;
        w = a0->words[0];
        o = GpuFuncTable;
        p->destination = (a2 * 0x10000U) | (a1 & 0xFFFF);
        p->position = w;
        p->size = a0->words[1];
        return o->dispatch((GpuOpFunc)o->draw_ordering_table, (u8 *)p - 8, 0x14, 0);
    }
    return -1;
}

int *ClearOTag(a0, a1) int *a0; int a1; {
int n = a1;
    int *q; int *t; int m; int (*f)();
    if (D_800ABAC2.level >= 2) { f = GPU_printf; q = (int *)ClearOTagFmt; f(q, a0, n); }
    n--;
    if (n != 0) {
        do {
            n--;
            q = a0 + 1;
            ((char *)a0)[3] = 0;
            m = (int)q & 0xFFFFFF; *a0 = (*a0 & 0xFF000000) | m;
            a0 = q;
        } while (n != 0);
    }
    t = &D_800ABB80;
    *t = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *a0 = (int)t & 0xFFFFFF;
    return a0;
}

unsigned *ClearOTagR(unsigned *a, int b) {
    int *q;
    if (D_800ABAC2.level >= 2) GPU_printf(StrClearOTagRFmt, a, b);
    GpuFuncTable->clearOrdering2C((u8 *)a, b);
    q = &D_800ABB80;
    *q = ((int)D_800ABB6C & 0xFFFFFF) | 0x04000000;
    *a = (int)q & 0xFFFFFF;
    return a;
}

void DrawPrim(char *a0)
{
    int t = a0[3];
    GpuFuncTable->sync3C(0);
    GpuFuncTable->writeBuffer14((int *)(a0 + 4), t);
}

void DrawOTag(s32 ordering_table) {
    if (D_800ABAC2.level >= 2) GPU_printf(D_80011F38, ordering_table);
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->draw_ordering_table, (void *)ordering_table, 0, 0);
}

DrawEnvironment *PutDrawEnv(DrawEnvironment *environment) {
    const u8 *level = &GpuResetState.debug02.level;
    if (*level >= 2) {
        GPU_printf(PutDrawEnvFmt, environment);
    }
    SetDrawEnv2(environment->draw_packet1C, environment);
    environment->draw_packet1C[0] |= 0xFFFFFF;
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->draw_ordering_table, environment->draw_packet1C, 0x40, 0);
    PsyqMemcpy(&GpuResetState.draw10, environment, sizeof(*environment));
    return environment;
}

void DrawOTagEnv(u32 *ordering, DrawEnvironment *environment) {
    u32 *packet;
    if (GpuResetState.debug02.level >= 2) GPU_printf(StrDrawOTagEnvFmt, ordering, environment);
    packet = environment->draw_packet1C;
    SetDrawEnv2(packet, environment);
    environment->draw_packet1C[0] = (environment->draw_packet1C[0] & 0xFF000000) | (GpuDmaEncodeAddress(ordering) & 0xFFFFFF);
    GpuFuncTable->dispatch((GpuOpFunc)GpuFuncTable->draw_ordering_table, packet, 0x40, 0);
    PsyqMemcpy(&GpuResetState.draw10, environment, sizeof(*environment));
}

DrawEnvironment *GetDrawEnv(DrawEnvironment *environment) { PsyqMemcpy(environment, &GpuResetState.draw10, sizeof(*environment)); return environment; }
