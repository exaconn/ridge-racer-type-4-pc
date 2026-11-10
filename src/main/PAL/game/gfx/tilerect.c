#include "psyq/libgte/rotmat.h"
#include "rr/model_table.h"
#include "rr/camera_matrices.h"
#include "rr/track_object_render.h"
#include "rr/gpu.h"
/* RODATA 0x8001011c 0x20 */
#include "scratchpad.h"
#include "rr/load_cursor.h"
#include "rr/tiled_rectangle.h"
#include "rr/geometry.h"
#include "rr_asm.h"
#include "game/data/cdfile2.h"
#include "game/effect/spfade.h"
#include "game/gfx/sky.h"
#include "game/data/ldscn5.h"
#include "game/gfx/gtemodel.h"
#include "game/gfx/tilerect.h"
#include "game/data/cdfile1.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/menu_background.h"
#include "rr/sprite_preset.h"
#include "rr/memory.h"
#include "rr/controller_hud.h"
#include "rr/course_choices.h"
#include "game/math/rotmat.h"
#include "game/menu/mnrows.h"
#include "game/gfx/light1.h"
#include "game/course/trkintp2.h"

#define GS ((SPD *)SCRATCHPAD)

typedef struct {
    MatrixWords32 matrix00;
    Vec3sFields8 unk20;
    char unk28[0x08];
    int unk30;
    int unk34;
    int w38;
    short s3C;
    short s3E;
    short s40;
    short s42;
    short s44;
    short s46;
    short s48;
    short s4A;
} S_8010BF70;

typedef struct {
    union {
        int word;
        struct { char b0, b1, b2, len; } b;
    } tag;
    int code;
} GpuCmd8;

typedef struct {
    GpuCmd8 *cur;
    union {
        int word;
        struct { char b0, b1, b2, len; } b;
    } *ot;
} SPD;

extern GpuRectangleFields D_80010104;
extern int SetMenuFrameOptions();
extern short D_800AC5D0;
extern int VSyncThreshold;
extern short D_800F3A26;
extern short ModeGroup;
extern short D_800F4D8A;
extern int D_8010BF74;
extern Vec3sFields8 D_8001010C;
extern GpuRectangleFields D_80010114;
extern int D_8009E268;
extern unsigned char D_800AD4EA;
extern int D_800F3A60;
extern S_8010BF70 *TileRectScene;

extern int TileFadeColorTable[];
extern int D_8009E36C[];

void EmitTiledRect(GpuRectangleFields *rectangle, s32 intensity, u32 *head);
void TileRectSceneModeEnter(void);
void TileRectSceneModeUpdate(void);

void EmitTiledRect(GpuRectangleFields *rectangle, s32 intensity, u32 *head) {
    register s32 value;
    register u32 command;
    register GpuRectangleFields *rect = rectangle;
    register u32 *ordering = head;
    register s32 height, y, width, x;
    register s32 tile_h, tile_w;
    /* ASM_BARRIER: keep the common tile limit in S2 across both loops. */
    register s32 limit asm("$18");
    /* ASM_BARRIER: retain the Y texture remainder in V1 until row packing. */
    register s32 v asm("$3");
    /* ASM_BARRIER: retain the X texture remainder in T3 for UV packing. */
    register s32 u asm("$11");
    register s32 shade = intensity;
    /* ASM_BARRIER: materialize the per-packet length in V0 after the tag store. */
    register s32 length asm("$2");
    register u32 packed;
    /* ASM_BARRIER: retain packed row fields in S0/A2/T9/T8 across the X loop. */
    register u32 top asm("$16"), bits_h asm("$6"), vpart asm("$25"), ypage asm("$24");
    /* ASM_BARRIER: stage E1000200 in A0 within each tile. */
    register u32 drawmode asm("$4");
    u32 high, middle;
    /* ASM_BARRIER: retain the signed page-field shift in V1. */
    register s32 xpage asm("$3");
    /* ASM_BARRIER: retain the output/biased-field cursors in T4/T0. */
    register u8 *packet asm("$12"), *fields asm("$8");
    GpuScratchPointers *scratch;
    shade = (shade < 0 ? 0 : shade) > 255 ? 255 : (shade < 0 ? 0 : shade);
    command = 0xE6000000U;
    high = shade * 0x10000U;
    packet = *(u8 **)SCRATCHPAD;
    ((u32 *)packet)[1] = command;
    middle = shade * 0x100U;
    middle |= 0x66000000U;
    high |= middle;
    shade = (shade|high);
    *(u32 *)packet = *ordering;
    packet[3] = 1;
    *ordering = (u32)(unsigned long)packet;
    ((u8 *)ordering)[3] = 0;
    height = rect->h;
    y = rect->y;
    packet+=sizeof(GpuSingleCommandPacket);
    if (height > 0) {
        limit = 256;
        do {
            v = y&255;
            value = limit-v;
            /* ASM_BARRIER: retain the unbounded row extent before the minimum. */
            __asm__("" : : "r"(value));
            tile_h = value;
            if (tile_h >= height) tile_h = height;
            width = rect->w;
            x = rect->x;
            value = y-1;
            if (width > 0) {

                top = value * 0x10000U;
                bits_h = tile_h * 0x10000U;
                /* ASM_BARRIER: keep the texture remainder separate from packed V. */
                __asm__("" : : "r"(v));
                vpart = (u32)v<<8;
                value = y&256;
                /* ASM_BARRIER: retain the signed Y-page shift after masking. */
                __asm__("" : "+r"(value));
                ypage = (value>>4);
                fields = packet+3;
                do {
                    u = x&63;
                    value = limit-u;
                    /* ASM_BARRIER: retain the unbounded column extent before the minimum. */
                    __asm__("" : : "r"(value));
                    tile_w = value;
                    if (tile_w >= width) tile_w = width;
                    drawmode = 0xE1000200U;

                    value = x&65535;
                    xpage = x&1023;
                    x+=tile_w;
                    packed = top|value;
                    ((u32 *)(fields+1))[2] = packed;
                    packed = bits_h|tile_w;
                    ((u32 *)(fields+1))[4] = packed;
                    packed = vpart|u;
                    /* ASM_BARRIER: retain an arithmetic X-page shift. */
                    __asm__("" : "+r"(xpage));
                    xpage >>= 6;
                    xpage |= 256;
                    xpage = ypage|xpage;
                    ((u32 *)(fields+1))[1] = shade;
                    ((u32 *)(fields+1))[3] = packed;
                    ((u32 *)(fields+1))[0] = xpage|drawmode;
                    packed = *ordering;
                    width-=tile_w;
                    *(u32 *)packet = packed;
                    length = 5;

                    fields[0] = length;
                    fields+=sizeof(TiledRectanglePacket);
                    *ordering = (u32)(unsigned long)packet;
                    packet+=sizeof(TiledRectanglePacket);
                    ((u8 *)ordering)[3] = 0;
                } while (width > 0);
            }
            height-=tile_h;
            y+=tile_h;
        } while (height > 0);
    }
    scratch = (GpuScratchPointers *)SCRATCHPAD;
    ((u32 *)packet)[1] = 0xE6000001U;
    *(u32 *)packet = scratch->order[703];
    packet[3] = 1;
    scratch->order[703] = (u32)(unsigned long)packet;
    ((u8 *)&scratch->order[703])[3] = 0;
    scratch->cursor = packet+sizeof(GpuSingleCommandPacket);
}

void TileRectSceneModeEnter(void) {
    GpuRectangle sp10;
    VSyncThreshold = 0x80;
    InitMenuArenas();
    SetMenuFrameOptions(0, 1, 0);
    UpdateScreenFade(0x1000C);
    sp10.rect = D_80010104;
    ClearImage2(&sp10, 0, 0, 0);
    D_800AC5D0 = 0;
    D_8010BF74 = 0;
    ModeGroup = 0;
    D_800F3A26 = 4;
    D_800F4D8A = 0;
}

void TileRectSceneModeUpdate(void) {
    GpuRectangleFields sp10;
    int s1;

    UpdateScreenFade(0x1000C);
    if (D_800AC5D0 != 0) {
        D_800AC5D0 += 4;
        if (D_800AC5D0 >= 0x100) {
            ResetScreenCounters();
            ModeGroup = 1;
            D_800F3A26 = 0xB;
            D_800F4D8A = 0;
        }
    }

    switch (D_8010BF74) {
    case 0:
        if (CdFileReadStart(GetR4FileIndex(0, D_800AD4EA + 1), LoadCursor) == 0) {
            return;
        }
        D_8010BF74 = 1;
    case 1:
        if (CdFileBytesLoaded() == 0) {
            return;
        }
        LoadTimList(LoadCursor);
        D_8010BF74 = 2;
    case 2:
        if (CdFileReadStart(GetR4FileIndex(0, 0x14), LoadCursor) == 0) {
            return;
        }
        D_8010BF74 = 3;
    case 3:
        if (CdFileBytesLoaded() == 0) {
            return;
        }
        LoadTimImage(LoadCursor);
        D_8010BF74 = 4;
    case 4:
        if (CdFileReadStart(GetR4FileIndex(0, D_800AD4EA + 0xA), LoadCursor) == 0) {
            return;
        }
        D_8010BF74 = 5;
    case 5:
        s1 = CdFileBytesLoaded();
        if (s1 == 0) {
            return;
        }
        RelocateOffsetTable(D_800AC64C = LoadCursor);
        TileRectScene = (S_8010BF70 *)(LoadCursor += s1);
        LoadCursor = (char *)TileRectScene + 0x4C;
        SetupRenderer(3);
        TileRectScene->unk20 = D_8001010C;
        PsyqMemset(TileRectScene->unk28, 0, 0x10);
        {
            S_8010BF70 *p = TileRectScene;
            D_8010BF74 = 6;
            p->s48 = 0x40;
            /* ASM_BARRIER: keep p in $v0 across D_8010BF74 = 6 and field stores */
            __asm__("" : "+r"(p));
            p->w38 = 0;
            p->s3C = 0;
            p->s3E = 0;
            p->s40 = 0;
            p->s42 = 0;
            p->s44 = 0;
            p->s46 = 0;
            p->s4A = 0x1000;
            p->s3C = 0;
        }
    case 6:
        if (TileRectScene->s3C < 0x40) {
            if (TileRectScene->s3C == 8) {
                TileRectScene->s42 = 4;
            } else if (TileRectScene->s3C == 0x20) {
                TileRectScene->s44 = 4;
            }
            break;
        }
        SetMenuFrameOptions(1, 1, 0);
        D_8010BF74 = 7;
    case 7:
        if (D_800AC5D0 == 0 && D_800F3A60 != 0) {
            D_800AC5D0 = 4;
            TileRectScene->s44 = -4;
            TileRectScene->s42 = -4;
        }
        break;
    }

    TileRectScene->s3C++;
    TileRectScene->unk20.y -= 4;
    RampValueToLimit(&TileRectScene->s3E, &TileRectScene->s42, 0x80);
    RampValueToLimit(&TileRectScene->s40, &TileRectScene->s44, 0x80);

    if (TileRectScene->s4A != 0) {
        S_8010BF70 *p;
        /* ASM_BARRIER: keep interpolated IR2 result in $s2 */
        register int ir2 __asm__("$18");

        if (TileRectScene->s4A / 32 != 0) {
            TileRectScene->s4A -= TileRectScene->s4A / 32;
        } else if (TileRectScene->s4A >= 3) {
            TileRectScene->s4A -= 2;
        } else {
            TileRectScene->s4A = 0;
        }

        p = TileRectScene;
        RR_GTE_TILERECT_INTPL(ir2, p);
        D_8009E268 = ir2;
    }

    EmitPresetSprite(TileRectScene->s3E, 0x25);
    EmitPresetSprite(TileRectScene->s40, 0x26);

    GS->cur->code = 0xE1000229;
    GS->cur->tag.word = GS->ot[1].word;
    GS->cur->tag.b.len = 1;
    GS->ot[1].word = GS->cur;
    GS->ot[1].b.len = 0;
    GS->cur++;
    RampValueTo4096(&TileRectScene->s46, &TileRectScene->s48);

    if (TileRectScene->s46 != 0) {
        GpuRectangleFields *a2;
        SetSineLightColors(TileRectScene->s46);
        BuildCameraMatrices();
        RotMatrix(&TileRectScene->unk20, &TileRectScene->matrix00);
        /* ASM_BARRIER: Emit 8-byte unaligned struct copy using $8, $9, $10 without spilling $8 at earlier block move */
        __asm__(
            "addiu $6, $29, 0x10\n\t"
            "lui $2, %%hi(D_80010114)\n\t"
            "addiu $8, $2, %%lo(D_80010114)\n\t"
            "lwl $9, 3($8)\n\t"
            "lwr $9, 0($8)\n\t"
            "lwl $10, 7($8)\n\t"
            "lwr $10, 4($8)\n\t"
            "swl $9, 0x13($29)\n\t"
            "swr $9, 0x10($29)\n\t"
            "swl $10, 0x17($29)\n\t"
            "swr $10, 0x14($29)"
            : "=r"(a2), "=m"(sp10)
        );
        RenderSkyToRect(TileRectScene->unk28, TileRectScene, a2, D_8009E268);
    }
    GS->cur->code = 0xE6000001;
    GS->cur->tag.word = GS->ot[703].word;
    GS->cur->tag.b.len = 1;
    GS->ot[703].word = GS->cur;
    GS->ot[703].b.len = 0;
    GS->cur++;
}

void DrawTrackObjectBillboard(int modelIndex) {
    TrackObjectBillboardWork *work = &((TrackObjectBillboardScratch *)SCRATCHPAD)->work7C;
    RroSceneScratchPrefix *scene;
    const RroModelTable *table;
    GpuPolyFT4 *packet;
    const TrackObjectQuad *quad;
    const Vec3s8 *vertices;
    /* ASM_BARRIER: pin count to $t8 ($24) to match original caller-saved loop allocation */
    register int count asm("$24");
    u32 *orderingBase;
    int depthShift;
    int depthBucket;
    int word0, word1, word2, word3;
    u32 phaseOffset;

    work->rotation00.w[4] = 0x4000;
    work->rotation00.w[2] = 0x4000;
    work->rotation00.w[0] = 0x4000;
    work->rotation00.w[3] = 0;
    work->rotation00.w[1] = 0;
    RR_GTE_SETROT(&work->rotation00);
    RR_GTE_SETLLM(&((RroSceneScratchPrefix *)SCRATCHPAD)->view.matrix28);
    RR_GTE_SETBK_P(CameraViewMatrix.fields.translation14);

    scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    table = (const RroModelTable *)((const u8 *)scene->modelEntries48 - RR_FIELD_OFFSET(RroModelTable, entries));
    packet = (GpuPolyFT4 *)scene->view.projection.pointers.cursor;
    quad = (const TrackObjectQuad *)table->entries[modelIndex].commands;
    vertices = table->entries[modelIndex].vertices;
    count = ((const RroBodyCommandHeader *)quad)->fields.count;
    quad = (const TrackObjectQuad *)((const u8 *)quad + sizeof(RroBodyCommandHeader));
    word0 = quad->translationXY1C;
    word2 = quad->translationZ1A;
    /* ASM_BARRIER: schedule count post-decrement and order global-alloc priorities quad ($t3) > packet ($t7) > vertices ($t9) */
    __asm__("" : "+r"(count) : "r"(word0), "r"(word2), "r"(quad), "r"(packet), "r"(packet));
    work->input2C.signedWords[0] = word0;
    work->input2C.fields.z = word2;
    orderingBase = scene->view.projection.pointers.order + 128;
    depthShift = scene->depthShift60 + 2;
    while (count-- > 0) {
        RR_GTE_MVMVA_LLM_BK_LDV0(&work->input2C.signedWords[0]);
        word0 = quad[1].translationXY1C;
        word2 = quad[1].translationZ1A;
        work->input2C.signedWords[0] = word0;
        work->input2C.fields.z = word2;
        RR_GTE_SWC2_IR3(&work->depth28);
        word0 = work->depth28;
        if (word0 >= 0) {
            depthBucket = word0 >> depthShift;
            if (depthBucket < 0x1C0) {
                RR_GTE_TR_FROM_IR();
                word0 = quad->vertices.words[0];
                word1 = quad->vertices.words[1];
                word2 = word0 >> 16;
                word3 = word1 >> 16;
                word0 &= 0xFFFF;
                word1 &= 0xFFFF;
                RR_GTE_RTPT_LDV(
                    &vertices[word0],
                    &vertices[word2],
                    &vertices[word1]
                );
                word0 = quad->uv0C;
                word2 = quad->uv10;
                packet->words.uv0 = word0;
                packet->words.uv1 = word2;
                word0 = quad->uv14;
                word2 = word0 >> 16;
                packet->words.uv2 = word0;
                packet->words.uv3 = word2;
                RR_GTE_SWC2_SXYZ(&work->projected34[0].word);
                RR_GTE_RTPS_LDV(&vertices[word3]);
                word2 = work->projected34[0].word;
                word1 = work->projected34[1].word;
                word3 = work->projected34[2].word;
                word0 = TileFadeColorTable[(ScreenFrame - (phaseOffset = quad->flashPhase18 - 0x40)) & 0x3F];
                packet->words.xy0 = word2;
                packet->words.xy1 = word1;
                packet->words.xy2 = word3;
                packet->words.color04 = word0;
                RR_GTE_SWC2_SXY2(&work->projected34[3].word);
                packet->words.xy3 = work->projected34[3].word;
                {
                    int x0 = work->projected34[0].xy.x;
                    int x1 = work->projected34[1].xy.x;
                    int x2 = work->projected34[2].xy.x;
                    int x3 = work->projected34[3].xy.x;
                    if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                        (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                        int y0 = work->projected34[0].xy.y;
                        int y1 = work->projected34[1].xy.y;
                        int y2 = work->projected34[2].xy.y;
                        int y3 = work->projected34[3].xy.y;
                        if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                            packet->words.tag.word = (orderingBase + depthBucket)[quad->orderingBias19];
                            packet->words.tag.bytes.length = 9;
                            (orderingBase + depthBucket)[quad->orderingBias19] = (u32)(unsigned long)packet;
                            packet++;
                            ((GpuDmaTag *)&(orderingBase + depthBucket)[quad->orderingBias19])->bytes.length = 0;
                        }
                    }
                }
            }
        }
        quad++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
}

void DrawTrackObjectFlashQuads(int modelIndex) {
    TrackObjectFlashWork *work = &((TrackObjectFlashScratch *)SCRATCHPAD)->work7C;
    /* ASM_BARRIER: pin scratchpad base pointer to $a1 ($5) to match original pre-loop CSE */
    register RroSceneScratchPrefix *scene asm("$5");
    const RroModelTable *table;
    GpuPolyFT4 *packet;
    const RroTexturedQuadPrefix *quad;
    const Vec3s8 *vertices;
    /* ASM_BARRIER: pin count to $t3 ($11) to match original caller-saved loop allocation */
    register int count asm("$11");
    u32 *orderingBase;
    int windingMask;
    int depthShift;
    int word0, word1, word2, word3, entryOffset, renderFlags;

    RR_GTE_SETROT(&((RroSceneScratchPrefix *)SCRATCHPAD)->view.matrix28);
    RR_GTE_SETTR(((RroSceneScratchPrefix *)SCRATCHPAD)->view.matrix28.fields.translation14);

    entryOffset = modelIndex * sizeof(RroEffectMesh);
    scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    /* ASM_BARRIER: keep SCRATCHPAD base pointer in $a1 and finish modelIndex*12 before table load so table gets $a0 */
    __asm__("" : "+r"(scene) : "r"(entryOffset));
    table = (const RroModelTable *)((const u8 *)scene->modelEntries48 - RR_FIELD_OFFSET(RroModelTable, entries));
    packet = (GpuPolyFT4 *)scene->view.projection.pointers.cursor;
    depthShift = scene->depthShift60 + 2;
    quad = (const RroTexturedQuadPrefix *)table->entries[modelIndex].commands;
    orderingBase = scene->view.projection.pointers.order + 128;
    vertices = table->entries[modelIndex].vertices;
    count = ((const RroBodyCommandHeader *)quad)->fields.count;
    quad = (const RroTexturedQuadPrefix *)((const u8 *)quad + sizeof(RroBodyCommandHeader));
    renderFlags = scene->renderFlags64;
    /* ASM_BARRIER: materialize count in $t3 after scene[0x64/4] load and order global-alloc priorities quad ($t1) > packet ($t2) > vertices ($t4) > orderingBase ($t5) */
    __asm__("" : "+r"(count) : "r"(quad), "r"(quad), "r"(quad), "r"(orderingBase), "r"(renderFlags));
    windingMask = renderFlags ? (-2147483647 - 1) : 0;
    while (count-- > 0) {
        RR_GTE_RTPT_LDV(
            &vertices[quad->vertices.indices[0]],
            &vertices[quad->vertices.indices[1]],
            &vertices[quad->vertices.indices[2]]
        );
        word3 = quad->uv0C;
        word2 = quad->uv10;
        packet->words.uv0 = word3;
        packet->words.uv1 = word2;
        word3 = quad->uv14;
        word2 = word3 >> 16;
        packet->words.uv2 = word3;
        packet->words.uv3 = word2;
        RR_GTE14_NCLIP();
        RR_GTE_SWC2_SXYZ(&work->projected24[0].word);
        RR_GTE14_SWC2_MAC0(&work->area20);
        if ((work->area20 ^ windingMask) >= 0) {
            RR_GTE_RTPS_LDV(&vertices[quad->vertices.indices[3]]);
            word3 = D_8009E36C[(0U + ScreenFrame + quad->flashPhase18) & 0x3F];
            word2 = work->projected24[0].word;
            word0 = work->projected24[1].word;
            word1 = work->projected24[2].word;
            packet->words.xy0 = word2;
            packet->words.xy1 = word0;
            packet->words.xy2 = word1;
            packet->words.color04 = word3;
            RR_GTE14_NOP2();
            RR_GTE14_AVSZ4();
            RR_GTE_SWC2_SXY2(&work->projected24[3].word);
            RR_GTE14_SWC2_OTZ(&work->depth1C);
            packet->words.xy3 = work->projected24[3].word;
            {
                int otz = work->depth1C;
                if (otz > 0) {
                    otz >>= depthShift;
                    work->depth1C = otz;
                    if (otz < 0x1C0) {
                        int x0 = work->projected24[0].xy.x;
                        int x1 = work->projected24[1].xy.x;
                        int x2 = work->projected24[2].xy.x;
                        int x3 = work->projected24[3].xy.x;
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            int y0 = work->projected24[0].xy.y;
                            int y1 = work->projected24[1].xy.y;
                            int y2 = work->projected24[2].xy.y;
                            int y3 = work->projected24[3].xy.y;
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                                /* ASM_BARRIER: give packet+3 GIV higher priority than quad+0x19 GIV ($a2 vs $t0) and windingMask higher priority than depthShift ($t6 vs $t7) */
                                __asm__("" : : "m"(((char *)packet)[1]), "m"(((char *)packet)[2]), "r"(quad), "r"(packet), "r"(packet), "r"(windingMask));
                                packet->words.tag.word = (orderingBase + work->depth1C)[quad->orderingBias19];
                                packet->words.tag.bytes.length = 9;
                                (orderingBase + work->depth1C)[quad->orderingBias19] = (u32)(unsigned long)packet;
                                packet++;
                                ((GpuDmaTag *)&(orderingBase + work->depth1C)[quad->orderingBias19])->bytes.length = 0;
                            }
                        }
                    }
                }
            }
        }
        quad++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
}

u8 *EmitModelTrisGT3(const TrackObjectLitWork *work, u32 *head, u8 *packetCursor) {
    const TrackObjectTriangle *triangle = (const TrackObjectTriangle *)work->commands30;
    const Vec3s8 *vertices = work->vertices2C;
    /* ASM_BARRIER: allocate count to $t5 ($13) so vertices gets $t6 ($14) */
    register int count __asm__("$13") = ((unsigned short *)triangle)[1];
    int x0, x1, x2, y0, y1, y2;
    u8 *ret;

    triangle = (const TrackObjectTriangle *)((const u8 *)triangle + 4);
    do {
        x0 = triangle->vertices.words[0];
        x2 = triangle->vertices.indices[2];
        x1 = x0 >> 16;
        x0 &= 0xFFFF;
        RR_GTE_RTPT_LDV(
            &vertices[x0],
            &vertices[x1],
            &vertices[x2]
        );
        x0 = triangle->colors08[0];
        x1 = triangle->colors08[1];
        x2 = triangle->colors08[2];
        ((GpuPolyG3 *)packetCursor)->color0 = x0;
        ((GpuPolyG3 *)packetCursor)->color1 = x1;
        ((GpuPolyG3 *)packetCursor)->color2 = x2;
        RR_GTE_STORE_READ_SXY3(packetCursor, x0, x1, x2);
        y0 = x0 >> 16;
        y1 = x1 >> 16;
        y2 = x2 >> 16;
        if ((y0 >= 0 || y1 >= 0 || y2 >= 0) &&
            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100)) {
            x0 = (x0 * 0x10000U);
            x1 = (x1 * 0x10000U);
            x2 = (x2 * 0x10000U);
            x0 >>= 16;
            x1 >>= 16;
            x2 >>= 16;
            if ((x0 >= 0 || x1 >= 0 || x2 >= 0) &&
                (x0 < 0x140 || x1 < 0x140 || x2 < 0x140)) {
                ((GpuPolyG3 *)packetCursor)->tag.word = *head;
                ((GpuDmaTag *)packetCursor)->bytes.length = 6;
                *head = (u32)(unsigned long)packetCursor;
                packetCursor += sizeof(GpuPolyG3);
                ((char *)head)[3] = 0;
            }
        }
        triangle++;
        count--;
    } while (count > 0);
    ((GpuCommandPacket *)packetCursor)->command = 0xE1000220;
    ((GpuPolyG3 *)packetCursor)->tag.word = *head;
    ((GpuDmaTag *)packetCursor)->bytes.length = 1;
    ret = packetCursor + sizeof(GpuCommandPacket);
    *head = (u32)(unsigned long)packetCursor;
    ((char *)head)[3] = 0;
    return ret;
}
