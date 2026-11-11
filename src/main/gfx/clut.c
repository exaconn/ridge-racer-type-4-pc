#include "game/gfx/clut.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/overlay662_palette_refresh.h"
#include "rr/race_setup.h"


/* Resident callback that reports success without changing state. */
s32 DebugPrintStub(const char *format, ...)
{
    (void)format;
    return 1;
}

void UploadCarClut(void *tim, s32 a1, s32 a2)
{
    GpuRectangle r;
    const u8 *a0;
    const u16 *s;
    u32 t;
    if (a1 < 0) {
        r.rect.x = a2 * 16 + 0x280;
        r.rect.y = 0x40;
        r.rect.w = 0x10;
        r.rect.h = 1;
        ClearImage2(&r, 0, 0, 0);
        DrawSync(0);
        return;
    }
    /* TIM bank: 0x840-byte entries; ID word, flags word, then CLUT/pixel blocks. */
    a0 = (const u8 *)tim + a1 * 0x840;
    a0 += 4;
    t = *(const u32 *)a0;
    a0 += 4;
    if (t & 8) {
        s = (const u16 *)a0;
        r.rect.x = a2 * 16 + 0x280;
        r.rect.y = 0x40;
        r.rect.w = s[4];
        r.rect.h = s[5];
        LoadImage(&r, (const u8 *)s + 0xC);
        DrawSync(0);
        a0 = (const u8 *)s + ((*(const u32 *)s >> 2) << 2);
    }
    s = (const u16 *)a0;
    r.rect.x = a2 * 16 + 0x280;
    r.rect.y = 0;
    r.rect.w = s[4];
    r.rect.h = s[5];
    if (r.rect.w > 0) {
        if (r.rect.h > 0) {
            LoadImage(&r, (const u8 *)s + 0xC);
            DrawSync(0);
        }
    }
}
