#include "psyq/libgpu/gpuenv.h"
#include "rr/overlay662_palette_refresh.h"
#include "rr/race_setup.h"


int DebugPrintStub(void);
void UploadCarClut(int a0, int a1, int a2);

/* Resident callback that reports success without changing state. */
int DebugPrintStub(void)
{
    return 1;
}

void UploadCarClut(int a0, int a1, int a2)
{
    GpuRectangle r;
    unsigned short *s;
    unsigned t;
    if (a1 < 0) {
        r.rect.x = a2 * 16 + 0x280;
        r.rect.y = 0x40;
        r.rect.w = 0x10;
        r.rect.h = 1;
        ClearImage2(&r, 0, 0, 0);
        DrawSync(0);
        return;
    }
    a0 += a1 * 0x840;
    a0 += 4;
    t = *(unsigned *)a0;
    a0 += 4;
    if (t & 8) {
        s = (unsigned short *)a0;
        r.rect.x = a2 * 16 + 0x280;
        r.rect.y = 0x40;
        r.rect.w = s[4];
        r.rect.h = s[5];
        LoadImage(&r, (char *)s + 0xC);
        DrawSync(0);
        a0 = (int)s + ((*(unsigned *)s >> 2) << 2);
    }
    s = (unsigned short *)a0;
    r.rect.x = a2 * 16 + 0x280;
    r.rect.y = 0;
    r.rect.w = s[4];
    r.rect.h = s[5];
    if (r.rect.w > 0) {
        if (r.rect.h > 0) {
            LoadImage(&r, (char *)s + 0xC);
            DrawSync(0);
        }
    }
}
