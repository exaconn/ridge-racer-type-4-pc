#include "rr/track_lighting.h"
#include "rr/load_cursor.h"
#include "rr/matrix_workspace.h"
#include "rr/race_types.h"
/* RODATA 0x8001007c 0x3c */
#include "types.h"
#include "scratchpad.h"
#include "rr/renderer_setup.h"
#include "rr_asm.h"

/* Candidate module: CD read state machine, 80012578..80012A68.
 * Adjacent matching functions share one directory and location definition.
 * This grouping does not establish the original compiler unit boundary. */
#include "rr/cd.h"
#include "game/data/bankrec.h"
#include "game/data/cdfile1.h"
#include "game/gfx/poly1.h"
#include "psyq/libcd/dsfinish.h"
#include "game/car/carmove1.h"
#include "game/gfx/model.h"
#include "game/race/objdraw.h"
#include "psyq/libgpu/primatt8.h"
#include "game/gfx/sky.h"
#include "game/data/cdfile2.h"
#include "game/car/bounds.h"
#include "rr/bios_devices.h"
#include "rr/relative_file.h"
#include "game/course/trkintp2.h"

typedef struct {
    s32 header;
    s32 p4, p8, pC, p10, p14;
} Resource;

extern s32 CdFileState;
extern s32 CdFileSyncCount;
extern s32 CdFileIndexCurrent;
extern s32 (*CdFileStateHandlers[])();
extern s32 CdFileSectorOffset;
extern s32 CdFileSectors;
extern void *D_80109480;
extern s32 CdFileBytesRead;
extern s32 CdFileSyncResult;
extern CdLocation CdFileLocation;
extern u8 D_80109498;
extern CdFileEntry CdFileTable[];
extern unsigned char D_8009DFD8[];
extern unsigned char D_8009DFEC[];
extern int SceneLoadState;
extern int D_8010943C;
extern int D_8010BF60;
extern int D_80109440;
extern int D_80109444;
extern int D_80109448;
extern int D_8010944C;
extern int D_80109450;
extern int D_80109454;
void DrawSync(s32);
extern s32 CarOffsetTables[];
extern short CourseIndex;
extern short D_8009E02C[];
extern short D_800ABED6[];
extern short D_800ABEDC[];

s32 CdFileStateCheckShell(void);
s32 CdFileStateGetTN(void);
s32 CdFileStateReloadDirectory(void);
s32 CdFileStateSetMode(void);
s32 CdFileStateWaitSetMode(void);
s32 CdFileStateWaitShellClose(void);
s32 CdFileStateSetloc(void);
s32 CdFileStateWaitSetloc(void);
s32 CdFileStateStartRead(void);
s32 CdFileStateReadSync(void);
void CdFileFinish(void);
s32 CdFileStateRetry(void);
s32 CdFileReadStart(s32 a0, void *a1);
s32 CdFileReadRange(s32 a0, s32 a1, s32 a2, void *a3);
int ApplyCarWheelTexturePatch(int a0, int a1, int a2, int a3, int a4);
void AllocCarSlotBuffers(const CarModelHeader *a0, int a1);

int ComputeCarSurfaceLighting(char *a0_in, int a1_in);

s32 CdFileStateCheckShell(void) {
    s32 var_v0;

    CdControlB(1, 0, &D_80109498);
    var_v0 = D_80109498 & 0x10;
    if (var_v0 != 0) {
        CdFileState = 5;
        var_v0 = 0;
    }
    return var_v0;
}

s32 CdFileStateGetTN(void) {
    CdControlF(0x13, 0);
    CdFileState = 2;
    return 0;
}

s32 CdFileStateReloadDirectory(void) {
    if (CdFileSyncResult == 2) {
        LoadR4Directory();
        CdFileState = 3;
    } else {
        CdFileState = 1;
    }
    return 0;
}

s32 CdFileStateSetMode(void) {
    D_80109498 = 0x80;
    CdControlF(0xE, &D_80109498);
    CdFileState = 4;
    CdFileSyncCount = 0;
    return 0;
}

s32 CdFileStateWaitSetMode(void) {
    if (CdFileSyncResult == 2) {
        if (++CdFileSyncCount >= 4) {
            CdFileSyncCount = 0;
            if (CdFileIndexCurrent < 0) {
                CdFileState = 0;
            } else {
                CdFileState = 6;
            }
        }
    } else {
        CdFileState = 3;
    }
    return 0;
}

s32 CdFileStateWaitShellClose(void) {
    CdControlB(1, 0, &D_80109498);
    if (!(D_80109498 & 0x10)) {
        CdFileState = 1;
    }
    return 0;
}

s32 CdFileStateSetloc(void) {
    CdControlF(2, &CdFileLocation);
    CdFileState = 7;
    return 0;
}

s32 CdFileStateWaitSetloc(void) {
    if (CdFileSyncResult == 2) {
        CdFileState = 8;
    } else {
        CdFileState = 0xB;
    }
    return 0;
}

s32 CdFileStateStartRead(void) {
    if (CdRead(CdFileSectors, D_80109480, 0x80)) {
        CdFileState = 9;
    } else {
        CdFileState = 0xB;
    }
    return 0;
}

s32 CdFileStateReadSync(void) {
    s32 r = CdReadSync(1, 0);
    if (r == 0) {
        CdFileState = 0xA;
    } else if (r == -1) {
        CdFileState = 0xB;
    }
    return 0;
}

void CdFileFinish(void) {
    CdFileIndexCurrent = -1;
    CdFileState = 0;
    CdFileBytesRead = CdFileSectors * 0x800U;
}

s32 CdFileStateRetry(void)
{
    CdControlB(1, 0, &D_80109498);
    if (D_80109498 & 0x10) {
        CdFileState = 5;
        return 0;
    }
    CdFileState = 6;
    return 0;
}

s32 CdFileStep(void) {
    if (CdFileState > 0) {
        CdFileSyncResult = CdSync(1, 0);
        if (CdFileSyncResult != 0) {
            return CdFileStateHandlers[CdFileState]();
        }
    }
    return 0;
}

s32 CdFileReadStart(s32 a0, void *a1) {
    if (CdFileState != 0) return 0;
    D_80109480 = a1;
    CdFileIndexCurrent = a0;
    CdFileState = 6;
    CdFileSectorOffset = 0;
    CdFileLocation = CdFileTable[a0].location;
    CdFileBytesRead = 0;
    CdFileSectors = CdFileTable[CdFileIndexCurrent].sectors;
    return 1;
}

s32 CdFileReadRange(s32 a0, s32 a1, s32 a2, void *a3) {
    if (CdFileState != 0) return 0;
    D_80109480 = a3;
    CdFileIndexCurrent = a0;
    CdFileState = 6;
    CdFileSectorOffset = a1;
    CdIntToPos((CdPosToInt(&CdFileTable[a0]) + (u32)a1), CdFileLocation.c);
    CdFileSectors = a2;
    CdFileBytesRead = 0;
    return 1;
}

s32 CdFileBytesLoaded(void) {
    return CdFileBytesRead;
}

void RelocateOffsetTable(int *a0)
{
    int n = *a0;
    int *p = a0 + 1;
    int t;
    if (n-- > 0) {
        int *q = a0 + 3;
        do {
            t = n--;
            *p += (int)a0;
            p += 3;
            *q += (int)a0;
            q += 3;
        } while (t > 0);
    }
}

void PatchModelBankTexPages(RroModelBank *bank, s32 textureSlot) {
    u32 remainingStreams = bank->streamCount;
    RroBankCommand *stream = bank->streams;
    u32 slot = textureSlot;
    int textureX = ((slot << 6) + 0x180U);
    int pageX = (textureX & 0x3FF) >> 6;
    int clutX = (textureX >> 4) & 0x3F;
    int atlasU = (slot << 6);
    int coordinateMask = ~0xC0;
    int atlasClut = (slot + 0x1028U);
    int currentRemaining, z;

    /* Keep the outer loop condition above GCC's loop-top duplication threshold. */
    while (z = 0, ((currentRemaining = remainingStreams--) + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z + z) != 0) {
        char *commands = (char *)stream++->pointer;
        unsigned int header;
        while (header = *(unsigned int *)commands, commands += 4, header != 0) {
            int recordCount = header >> 16;
            switch (header & 0xFFFF) {
            case 0:
                commands += recordCount * 16;
                break;
            case 2:
                commands += recordCount * 24;
                break;
            case 1:
                while (recordCount-- > 0) {
                    RroTextureAttributes *texture = (RroTextureAttributes *)(commands + RR_FIELD_OFFSET(RroRawTexturedQuad, uv08));
                    texture->texturePage = (texture->texturePage & ~0x1F) | pageX;
                    texture->clut = (texture->clut & ~0x403C) | clutX;
                    commands += sizeof(RroRawTexturedQuad);
                }
                break;
            case 3:
                while (recordCount-- > 0) {
                    RroEnvironmentTextureAttributes *attributes = (RroEnvironmentTextureAttributes *)(commands + RR_FIELD_OFFSET(RroEnvMapQuad, uv10));
                    if (attributes->texture.texturePage < 0) {
                        attributes->texture.texturePage = 0xA;
                        attributes->texture.firstUV = ((s16)attributes->texture.firstUV & coordinateMask) | atlasU;
                        attributes->texture.secondUV = ((s16)attributes->texture.secondUV & coordinateMask) | atlasU;
                        attributes->thirdUV = (attributes->thirdUV & coordinateMask) | atlasU;
                        attributes->fourthUV = (attributes->fourthUV & coordinateMask) | atlasU;
                        attributes->texture.clut = (attributes->texture.clut & ~0x403C) | atlasClut;
                        attributes->control |= 0x78;
                    } else {
                        attributes->texture.texturePage = (attributes->texture.texturePage & ~0x1F) | pageX;
                        attributes->texture.clut = (attributes->texture.clut & ~0x403C) | clutX;
                        attributes->control |= 0x78;
                    }
                    commands += sizeof(RroEnvMapQuad);
                }
                break;
            }
        }
    }
}

void PatchOffsetTableTexPages(RroModelTable *table, s32 textureSlot) {
    /* PAL counters and tile coordinates wrap modulo 32 bits. */
    u32 remaining = table->count;
    RroEffectMesh *mesh = table->entries;
    u32 slot = textureSlot;
    int textureX = ((slot << 6) + 0x180U);
    int pageX = (textureX & 0x3FF) >> 6;
    int clutX = (textureX >> 4) & 0x3F;
    int pageMask = ~0x1F;
    int clutMask = ~0x403C;

    while ((s32)remaining-- > 0) {
        char *commands = (char *)mesh->commands;
        unsigned int header;
        mesh++;
        while (header = *(unsigned int *)commands, commands += 4, header != 0) {
            int recordCount = header >> 16;
            switch (header & 0xFFFF) {
            case 7:
                commands += recordCount * 20;
                break;
            case 8:
                commands += recordCount * 28;
                break;
            case 1:
                while (recordCount-- > 0) {
                    RroTextureAttributes *attributes = (RroTextureAttributes *)(commands + RR_FIELD_OFFSET(RroTexturedQuadPrefix, uv0C));
                    attributes->texturePage = (attributes->texturePage & pageMask) | pageX;
                    attributes->clut = (attributes->clut & clutMask) | clutX;
                    commands += sizeof(RroTexturedQuadPrefix);
                }
                break;
            case 9:
                commands += recordCount * 20;
                break;
            case 5:
            case 10:
                commands += recordCount * 24;
                break;
            case 3:
                commands += recordCount * 20;
                break;
            case 4:
                commands += recordCount * 12;
                break;
            case 6:
                commands += recordCount * 8;
                break;
            case 11:
                while (recordCount-- > 0) {
                    RroTextureAttributes *attributes = (RroTextureAttributes *)(commands + 20);
                    attributes->texturePage = (attributes->texturePage & pageMask) | pageX;
                    attributes->clut = (attributes->clut & clutMask) | clutX;
                    commands += 0x20;
                }
                break;
            case 14:
                while (recordCount-- > 0) {
                    RroTextureAttributes *attributes = (RroTextureAttributes *)(commands + 16);
                    attributes->texturePage = (attributes->texturePage & pageMask) | pageX;
                    attributes->clut = (attributes->clut & clutMask) | clutX;
                    commands += 0x1C;
                }
                break;
            case 15:
                while (recordCount-- > 0) {
                    RroTextureAttributes *attributes = (RroTextureAttributes *)(commands + 20);
                    attributes->texturePage = (attributes->texturePage & pageMask) | pageX;
                    attributes->clut = (attributes->clut & clutMask) | clutX;
                    commands += 0x24;
                }
                break;
            }
        }
    }
}

int ApplyCarWheelTexturePatch(int a0, int a1, int a2, int a3, int a4)
{
    int b1 = a1;
    int xx;
    /* ASM_BARRIER: pins a<<4 temp to a3 ($7) to match reg choice */
    register int b16 asm("$7");
    int t;
    int c;
    int a, b, x;
    char *s0, *p0, *p1;
    char * volatile *g;
    /* ASM_BARRIER: keeps (SCRATCHPAD + 0x7C) pointer in a1 ($5) */
    register short *sp asm("$5");

    sp = (short *)(SCRATCHPAD + 0x7C);
    if (a0 == 2 && b1 == 0 && a2 == 0x13)
        return 0x13;
    t = D_8009DFEC[b1 * 16 + a3];
    /* ASM_BARRIER: forces copy t->a (original addu a0,v1,zero) */
    __asm__("" : "=r"(a) : "0"(t));
    xx = (t & 7) * 0x10 + 0x340; x = xx;
    if (t < 0) a = t + 7;
    a >>= 3;
    c = D_8009DFD8[a2];
    b16 = a << 4; b = b16 + (a << 2);
    if (c >= 4) {
        if (c >= 7)
            b += 0x78;
        else
            b += 0x3C;
    }
    g = (char * *)SCRATCHPAD;
    s0 = g[0];
    sp[0] = xx;
    sp[1] = b;
    ((int *)sp)[1] = 0x140010;
    SetDrawMove(s0, sp, (a4 << 6) + 0x1B0, 0xD8);
    *(int *)s0 = *(int *)g[1];
    s0[3] = 5;
    *(int *)g[1] = s0;
    g[1][3] = 0;
    s0 += 0x18;
    g[0] = s0;
    ((int *)s0)[1] = 0xE6000000;
    p1 = g[1]; p0 = g[0]; *(int *)p0 = *(int *)p1;
    g[0][3] = 1;
    *(int *)g[1] = g[0];
    g[1][3] = 0;
    g[0] += 8;
    /* ASM_BARRIER: keeps li v0,0 after final sw (stops sched hoisting it) */
    { int r; __asm__ volatile("" : "=r"(r) : "0"(0)); return r; }
}

void AllocCarSlotBuffers(const CarModelHeader *a0, int a1)
{
    int s1;
    u8 *cursor;
    int v1;
    u32 historyBytes, primaryBytes;
    CarSlotBufferC8 *s0;
    CarTrailBuffer *p;

    s0 = &CarSlotBuffers[a1 * 4];
    cursor = LoadCursor;
    primaryBytes = ((a0->primary10.vertexCount + 2) / 3) * (3 * sizeof(RroProjectedVertex));
    historyBytes = ((a0->history34.vertexCount + 2) / 3) * (3 * sizeof(RroProjectedVertex));
    s1 = 4;
    do {
        p = s0->main14;
        v1 = 3;
        s0->model = a0;
        s0->color08 = 0;
        s0->mainCount0C = 0;
        s0->mainIndex10 = 0;
        s0->secondCount34 = 0;
        s0->secondIndex38 = 0;
        s0->phase5C = 0;
        s0->intensity60 = 0;
        s0->intensity64 = 0;
        s0->historyCount68 = 0;
        s0->historyIndex6C = 0;
        s0->overlay70 = 0;
        do {
            p->vertices = cursor;
            p++;
            v1 -= 1;
            cursor += primaryBytes;
        } while (v1 >= 0);
        p = s0->second3C;
        v1 = 3;
        do {
            p->vertices = cursor;
            p++;
            v1 -= 1;
            cursor += historyBytes;
        } while (v1 >= 0);
        p = s0->history84;
        v1 = 7;
        do {
            p->vertices = cursor;
            p++;
            v1 -= 1;
            cursor += historyBytes;
        } while (v1 >= 0);
        s0++;
        s1 -= 1;
    } while (s1 > 0);
    LoadCursor = cursor;
    if (CarBoundsTable[a1]->wheelRotationAxis == 2) {
        s0 = &CarSlotBuffers[a1 * 4];
        s1 = 4;
        do {
            s1 -= 1;
            s0->sparksC4 = LoadCursor;
            PsyqBzero(LoadCursor, sizeof(RroSpark) * 16);
            s0++;
            LoadCursor = (u8 *)LoadCursor + sizeof(RroSpark) * 16;
        } while (s1 > 0);
    }
}

int RequestSceneLoad(int a0) {
    if (SceneLoadState != 0) {
        return 0;
    }
    D_8010943C = a0;
    return SceneLoadState = 1;
}

int SceneLoadStep(void)
{
    int *a1;
    /* ASM_BARRIER: pin saved regs to original assignment */
    register int s1 __asm__("$17");
    int s2,s5,s6,s7,fp;
    TrackObjectFile *trackObjects;
    register int s4;
    int sp10, sp14;
    RroModelTableWire *modelTable;
    /* ASM_BARRIER: pin temps to the original v0/v1/a0 choices */
    int a0;
    /* ASM_BARRIER: pin u to $2 */
    register int u __asm__("$2");
    /* ASM_BARRIER: pin t to $3 */
    register int t __asm__("$3");
    /* ASM_BARRIER: pin pw to $2 */
    register int *pw __asm__("$2"); int s0;

    /* This legacy loader relocates encoded 32-bit PAL address words in the
     * resource payload. These words are not the native arena pointer root. */
    switch (SceneLoadState) {
    case 1:
        if (CdFileReadStart(GetCourseFileIndex(D_8010943C), LoadCursor) == 0)
            return 0;
        SceneLoadState = 2;
    case 2:
        if (CdFileBytesLoaded() == 0)
            return 0;
        a0 = ((Resource *)LoadCursor)->p4 + (unsigned long)LoadCursor;
        s0 = ((Resource *)LoadCursor)->p8 + (unsigned long)LoadCursor;
        ((Resource *)LoadCursor)->p4 = a0;
        ((Resource *)LoadCursor)->p8 = s0;
        LoadTimList(a0);
        LoadTimList(s0);
        SceneLoadState = 3;
    case 3:
        if (CdFileReadStart(GetCourseFileIndex(D_8010943C) + 1, LoadCursor) == 0)
            return 0;
        SceneLoadState = 4;
    case 4:
        s1 = CdFileBytesLoaded();
        if (s1 == 0)
            return 0;
        a1 = (int *)LoadCursor;
        a1++; t = *a1; sp10 = t + (unsigned long)LoadCursor; *a1 = sp10;
        a1++; t = *a1; s2 = t + (unsigned long)LoadCursor; *a1 = s2;
        a1++; t = *a1; sp14 = t + (unsigned long)LoadCursor; *a1 = sp14;
        a1++; t = *a1; modelTable = (RroModelTableWire *)(t + (unsigned long)LoadCursor); *a1 = modelTable;
        a1++; t = *a1; fp = t + (unsigned long)LoadCursor; *a1 = fp;
        a1++; t = *a1; s6 = t + (unsigned long)LoadCursor; *a1 = s6;
        a1++; t = *a1; s7 = t + (unsigned long)LoadCursor; *a1 = s7;
        a1++; t = *a1; trackObjects = (TrackObjectFile *)(t + (unsigned long)LoadCursor); *a1 = trackObjects;
        a1++; t = *a1; s5 = t + (unsigned long)LoadCursor; *a1 = s5;
        /* ASM_BARRIER: hoist lui of D_8010943C above the store (hi part only) */
        a1++; t = *a1; u = LoadCursor; s4 = t + u; __asm__("" : "=r"(pw) : "0"(0x80110000)); *a1 = s4;
        a1++;
        if (pw[-0x6BC4 / 4] == 7) {
            /* Publish the PAL wire word, retaining the complete C address. */
            void *arena = (void *)((s32)*a1 + (unsigned long)LoadCursor);
            *a1 = arena;
            InitAnimatedTrackObjects(arena);
        }
        TrackSkyTable = s2;
        LoadCursor = LoadCursor + s1;
        RelocateRelativeFile(sp10);
        RelocateOffsetTable(s2);
        RegisterModelBank((RroModelBank *)sp14, 4);
        RelocateModelTable(modelTable);
        SetTrackNodeData(fp);
        SetLightTable((TrackLighting2C *)s6);
        D_8010BF60 = s7;
        LoadCursor = (u8 *)LoadCursor + LoadTrackObjects(trackObjects, LoadCursor);
        SetSegmentVisibilityTable(s5);
        SetOctantLookupTable(s4);
        SceneLoadState = 0;
        return 1;
    }
}

int RequestCarLoad(int a0, int a1, int a2, int a3, int a4) {
    int r;
    if (SceneLoadState != 0) {
        return 0;
    }
    D_80109440 = GetCarFileIndex(a0, a1, a2, a3);
    r = 1;
    D_80109444 = a4;
    D_80109448 = a1;
    D_8010944C = a0;
    D_80109450 = a2;
    D_80109454 = a3;
    SceneLoadState = r;
    return r;
}

s32 CarLoadStep(void) {
    s16 rect[4];
    void *s0;
    void *s1;
    void *s2;
    void *s3;

    switch (SceneLoadState) {
    case 1:
        if (CdFileReadStart(D_80109440, LoadCursor) == 0) {
            return 0;
        }
        SceneLoadState = 2;
    case 2:
        if (CdFileBytesLoaded() == 0) {
            return 0;
        }
        {
            s32 x;
            ((Resource *)LoadCursor)->p4 += (s32)LoadCursor;
            ((Resource *)LoadCursor)->p8 += (s32)LoadCursor;
            ((Resource *)LoadCursor)->pC += (s32)LoadCursor;
            ((Resource *)LoadCursor)->p10 += (s32)LoadCursor;
            ((Resource *)LoadCursor)->p14 += (s32)LoadCursor;
            s0 = (void *)((Resource *)LoadCursor)->pC;
            s1 = (void *)((Resource *)LoadCursor)->p10;
            s2 = (void *)((Resource *)LoadCursor)->p14;
            s3 = (void *)((Resource *)LoadCursor)->p8;
            SetCarRecordPtr((CarRecord *)((Resource *)LoadCursor)->p4, D_80109444);
            CarBoundsTable[D_80109444] = (CarBounds *)s3;
            RegisterModelBank(s0, D_80109444);
            /* ASM_BARRIER: preserve local-alloc priority of s0 ($s0) over s3 ($s3) */
            __asm__("" : : "r"(s0));
            PatchModelBankTexPages(s0, D_80109444);
            CarOffsetTables[D_80109444] = s1;
            RelocateOffsetTable(s1);
            PatchOffsetTableTexPages(s1, D_80109444);
            rect[2] = 0x40;
            rect[1] = 0;
            rect[3] = 0x100;
            x = (D_80109444 << 6) + 0x180;
            rect[0] = x;
        }
        LoadImage(rect, s2);
        DrawSync(0);
        LoadCursor = s2;
        AllocCarSlotBuffers((CarModelHeader *)CarOffsetTables[D_80109444], D_80109444);
        if (D_80109454 != 0) {
            ApplyCarWheelTexturePatch(D_80109448, D_8010944C, D_80109450, D_80109454 - 1, D_80109444);
        }
        SceneLoadState = 0;
        return 1;
    }
}

int RequestVramImageLoad(void) {
    if (SceneLoadState != 0) return 0;
    return SceneLoadState = 1;
}

int VramImageLoadStep(void) {
    short sp[4];
    switch (SceneLoadState) {
    case 1:
        if (CdFileReadStart(GetR4FileIndex(0, 0), LoadCursor) == 0) return 0;
        SceneLoadState = 2;
    case 2:
        if (CdFileBytesLoaded() == 0) return 0;
        sp[0] = 0x340;
        sp[2] = 0x80;
        sp[1] = 0;
        sp[3] = 0xB4;
        LoadImage(sp, LoadCursor);
        DrawSync(0);
        SceneLoadState = 0;
        return 1;
    default:
        return 2;
    }
}

void SetupRenderer(s32 shift) {
    RendererScratch *scratch;
    ((RendererScratch*)SCRATCHPAD)->depthShift60=shift;
    scratch=(RendererScratch*)SCRATCHPAD;
    scratch->renderFlags64=MirrorRenderFlag;
    D_800F3A20.b=128;
    D_800F3A20.g=128;
    D_800F3A20.r=128;
    D_800F3A20.code=0x2C;
    D_800F3A18.b=255;
    D_800F3A18.g=255;
    D_800F3A18.r=255;
    D_800F3A18.code=0x3C;
    scratch->viewport6C.rect.w=320;
    scratch->viewport6C.rect.x=0;
    scratch->viewport6C.rect.y=0;
    scratch->viewport6C.rect.h=256;
}

int ComputeCarSurfaceLighting(char *a0_in, int a1_in) {
    /* ASM_BARRIER: keep temp in $v0 */
    register int v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register int v1 __asm__("$3");
    int a0 = a0_in;
    int a1 = a1_in;
    /* ASM_BARRIER: keep temp in $a2 */
    register int a2 __asm__("$6");
    /* ASM_BARRIER: keep temp in $a3 */
    register int a3 __asm__("$7");
    int t0;
    int t1;
    int t2;
    /* ASM_BARRIER: keep a1_in copy in $t3 */
    register int t3 __asm__("$11");
    int *t7;
    short *t8;

    t3 = a1;
    t7 = (int *)(SCRATCHPAD + 0x7C);
    /* ASM_BARRIER: order t3 and t7 setup */
    __asm__ ("" : "+r"(t3), "+r"(t7));
    t7[0] = t3;
    a3 = ((RaceVehicle *)a0)->lateralOffset172;
    t2 = ((RaceVehicle *)a0)->trackFraction170;
    v0 = ((RaceVehicle *)a0)->trackNodeF0;
    v1 = D_8010BF60;
    t1 = a3 << 3;
    a3 = t1 + 0x2000;
    t2 <<= 2;
    v0 = (v0 << 1) + v1;
    t0 = ((short *)v0)[0];
    a2 = ((short *)v0)[1];
    a1 = a3;
    if (a3 < 0) {
        a1 = t1 + 0x2FFF;
    }
    v1 = a1 >> 12;
    v0 = (v1 << 1) + v1;
    a0 = v0 + 3;
    a1 = a2 >> a0;
    v1 = a2 >> v0;
    a2 = v1 & 7;
    a0 = t0 >> a0;
    v0 = t0 >> v0;
    t0 = v0 & 7;
    a3 &= 0xFFF;
    a1 &= 7;
    v0 = 0x800F0000;
    /* ASM_BARRIER: stage the CourseIndex page in $v0 before masking a0. */
    __asm__ ("" : "+r"(v0) : "r"(a0));
    a0 &= 7;
    v1 = ((short *)v0)[0x3914 / 2];
    v0 = D_8009E02C;
    a2 <<= 1;
    a1 <<= 1;
    a0 <<= 1;
    v1 = (v1 << 4) + v0;
    a2 += v1;
    a1 += v1;
    v0 = (t0 << 1) + v1;
    a0 += v1;
    /* ASM_BARRIER: order table element addresses before loads */
    __asm__ ("" : "+r"(a2), "+r"(a1), "+r"(v0), "+r"(a0));
    v1 = *(short *)a2;
    a1 = *(short *)a1;
    a2 = *(short *)v0;
    v0 = *(short *)a0;
    RR_GTE_INTPL_TRACK_TABLE(v0, v1, a1, a2, a3, t2);

    a0 = t3 & 0xF;
    v0 = (t3 >> 4) & 0xF;
    a1 = 0x800F0000;
    /* ASM_BARRIER: stage the TrackLightTable page in $a1 before index scaling. */
    __asm__ ("" : "+r"(a1) : "r"(a0), "r"(v0));
    v1 = (((v0 << 1) + v0) << 2) - v0;
    v1 <<= 2;
    t7[2] = v0;
    v0 = (((a0 << 1) + a0) << 2) - a0;
    a1 = ((int *)a1)[0x4D90 / 4];
    v0 <<= 2;
    t7[1] = a0;
    v1 = a1 + v1;
    a1 = a1 + v0;

    a2 = ((int *)v1)[4];
    a3 = ((TrackLighting2C *)v1)->lighting04.matrix.fields.vectors[0].z;
    a0 = ((int *)a1)[4];
    t2 = ((TrackLighting2C *)a1)->lighting04.matrix.fields.vectors[0].z;
    t0 = a2 >> 16;
    v0 = a2 << 16;
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    a0 = v0 >> 16;
    RR_GTE_SETFC(a2, t0, a3);
    RR_GTE_MTC2_IR123_INTPL(a0, t1, t2);

    t8 = (s16 *)LightMatrix.halves;
    a2 = ((int *)v1)[6];
    a3 = ((TrackLighting2C *)v1)->lighting04.matrix.fields.vectors[1].z;
    a0 = ((int *)a1)[6];
    t2 = ((TrackLighting2C *)a1)->lighting04.matrix.fields.vectors[1].z;
    t0 = a2 >> 16;
    v0 = a2 << 16;
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    a0 = v0 >> 16;
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    RR_GTE_MTC2_IR123_INTPL(a0, t1, t2);

    t8 = D_800ABED6;
    a2 = ((int *)v1)[8];
    a3 = ((TrackLighting2C *)v1)->lighting04.matrix.fields.vectors[2].z;
    a0 = ((int *)a1)[8];
    t2 = ((TrackLighting2C *)a1)->lighting04.matrix.fields.vectors[2].z;
    t0 = a2 >> 16;
    v0 = a2 << 16;
    a2 = v0 >> 16;
    t1 = a0 >> 16;
    v0 = a0 << 16;
    a0 = v0 >> 16;
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    RR_GTE_MTC2_IR123_INTPL(a0, t1, t2);

    t8 = D_800ABEDC;
    a3 = ((int *)v1)[10];
    a0 = ((int *)a1)[10];
    v0 = a3 << 4;
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    a3 = v0 & 0xFF0;
    RR_GTE_STORE_IR_S16(t8);
    RR_GTE_SETFC(a2, t0, a3);
    GTE_MTC2_RGB_DPCS(a0);

    a3 = ((int *)v1)[1];
    a0 = ((int *)a1)[1];
    v0 = a3 << 4;
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    a3 = v0 & 0xFF0;
    RR_GTE_BK_FROM_IR();
    RR_GTE_SETFC(a2, t0, a3);
    GTE_MTC2_RGB_DPCS(a0);

    t8 = (short *)(SCRATCHPAD + 0xB4);
    a3 = ((int *)v1)[2];
    a0 = ((int *)a1)[2];
    v0 = a3 << 4;
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    a3 = v0 & 0xFF0;
    RR_GTE_ST3_COL(t8);
    RR_GTE_SETFC(a2, t0, a3);
    GTE_MTC2_RGB_DPCS(a0);

    t8 = (short *)(SCRATCHPAD + 0xB6);
    a3 = ((int *)v1)[3];
    a0 = ((int *)a1)[3];
    v0 = a3 << 4;
    a2 = v0 & 0xFF0;
    v0 = a3 >> 4;
    t0 = v0 & 0xFF0;
    v0 = a3 >> 12;
    a3 = v0 & 0xFF0;
    RR_GTE_ST3_COL(t8);
    RR_GTE_SETFC(a2, t0, a3);
    GTE_MTC2_RGB_DPCS(a0);

    t8 = (short *)(SCRATCHPAD + 0xB8);
    RR_GTE_ST3_COL(t8);
    t8 = (short *)(SCRATCHPAD + 0xB4);
    RR_GTE_SETLCM(t8);
    return t7[1];
}
