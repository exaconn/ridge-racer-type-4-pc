#include "psyq/libgte/rotmat.h"
#include "game/gfx/tilerect.h"
#include "psyq/libgte/rotmatz.h"
#include "rr/track_object_render.h"
#include "rr/gpu.h"
#include "scratchpad.h"
#include "rr/load_cursor.h"
#include "rr/camera_matrices.h"
#include "rr/projection_state.h"
#include "game/math/rotmat.h"
#include "rr/renderer_setup.h"
#include "psyq/libgte/gtemat5.h"
#include "rr_asm.h"
#include "game/menu/mnsel.h"
#include "game/gfx/gtemodel.h"
#include "game/gfx/skyrot1.h"
#include "psyq/libgpu/gpuenv.h"
#include "rr/memory.h"
#include "rr/course_choices.h"
#include "rr/menu_grid.h"
#include "rr/work_arena.h"
#include "game/data/screenframe.h"

typedef struct { int a, b, c, d; } V16;

extern int FlashingLightColorTable[];
extern GpuRectangleFields D_800101BC;
extern VectorWords16 D_800101C4;
extern VectorWords16 D_800101D4;
extern VectorWords16 D_8009E540;

void DrawTrackObjectModel(int a0, int a1);

void DrawTrackObjectLitModel(int placementIndex, int modelIndex)
{
    TrackObjectLitWork *work = &((TrackObjectLitScratch *)SCRATCHPAD)->work7C;
    /* This retained temporary holds the full scene address, then the shift.
     * Separating those lifetimes still changes the generated instructions. */
    long sceneOrShift;
    long entryAddress;
    /* ASM_BARRIER: keep modelIndex*12 offset and entryAddress[3]/entryAddress[1] temp in $v1 */
    register long entryValue __asm__("$3");
    RroSceneScratchPrefix *sceneRoot;
    char *placements;
    int remaining, depthLimit, instanceCount;
    u8 *packetCursor;
    long orderingBase;
    char *placementCursor;
    int *gteTable;
    int angle, translationXY, translationZ;
    long ordering;

    entryValue = modelIndex * (sizeof(RroEffectMesh) / sizeof(u32));
    sceneRoot = (RroSceneScratchPrefix *)SCRATCHPAD;
    /* ASM_BARRIER: load SCRATCHPAD into $a1 after modelIndex*3 before shifting by 2 */
    __asm__("" : "+r"(entryValue) : "r"(sceneRoot));
    sceneOrShift = (long)sceneRoot;
    /* ASM_BARRIER: copy $a1 to $s3 before shifting entryValue by 2 */
    __asm__("" : "+r"(sceneOrShift), "+r"(entryValue));
    entryValue <<= 2;
    entryAddress = (long)((RroSceneScratchPrefix *)sceneOrShift)->modelEntries48;
    angle = ((RroSceneScratchPrefix *)sceneOrShift)->view.projection.pose08.angles10.xyz.z;

    entryAddress -= 4;
    entryAddress += entryValue;
    /* ASM_BARRIER: retain the entry address until the model pointer loads. */
    __asm__ volatile("" : "+r"(entryAddress) : "r"(sceneRoot));
    entryValue = (long)((const RroEffectMesh *)(entryAddress + 4))->commands;
    work->commands30 = (const u32 *)entryValue;
    entryValue = (long)((const RroEffectMesh *)(entryAddress + 4))->vertices;
    work->rotation00.w[0] = 0x1000;
    work->rotation00.w[4] = 0x1000;
    work->rotation00.w[2] = 0x1000;
    work->rotation00.w[3] = 0;
    work->rotation00.w[1] = 0;
    work->vertices2C = (const Vec3s8 *)entryValue;
    RotMatrixZ(angle, &work->rotation00);
    RR_GTE_SETROT(&work->rotation00);
    gteTable = CameraViewMatrix.w;
    RR_GTE_SETLLM(gteTable);
    gteTable = D_800FF6A4;
    RR_GTE_SETBK_P(gteTable);
    {
        long off0 = placementIndex * sizeof(RroEffectMesh);
        depthLimit = 0x12A;
        placements = (char *)((const RroEffectMesh *)((const u8 *)((RroSceneScratchPrefix *)sceneOrShift)->modelEntries48 + off0))->commands;
    }
    remaining = ((unsigned short *)placements)[1];
    placements += 4;
    if (((RroSceneScratchPrefix *)sceneOrShift)->renderFlags64) depthLimit = 0x95;
    instanceCount = remaining;
    remaining--;

    packetCursor = ((RroSceneScratchPrefix *)sceneOrShift)->view.projection.pointers.cursor;
    ordering = (long)((RroSceneScratchPrefix *)sceneOrShift)->view.projection.pointers.order;

    translationXY = ((const TrackObjectPlacement *)placements)->xy00;
    translationZ = ((const TrackObjectPlacement *)placements)->z04;
    work->input24.signedWords[0] = translationXY;
    work->input24.fields.z = translationZ;

    sceneOrShift = ((RroSceneScratchPrefix *)sceneOrShift)->depthShift60;
    orderingBase = ordering + 0x200;
    if (instanceCount > 0) {
        placementCursor = placements + 7;
        do {
            int nextXY, nextZ, rawDepth, bucketDepth;
            /* ASM_BARRIER: prevent loop strength reduction from creating a second pointer GIV for placementCursor+1/placementCursor+5 */
            __asm__("" : "+r"(placementCursor));
            RR_GTE_MVMVA_LLM_BK_LDV0(&work->input24);
            nextXY = ((const TrackObjectPlacement *)(placementCursor - 7))[1].xy00;
            nextZ = ((const TrackObjectPlacement *)(placementCursor - 7))[1].z04;
            work->input24.signedWords[0] = nextXY;
            work->input24.fields.z = nextZ;
            RR_GTE_SWC2_IR3(&work->depth54);
            rawDepth = work->depth54;
            bucketDepth = rawDepth >> sceneOrShift;
            if (rawDepth > 0) {
                if (bucketDepth < depthLimit) {
                    RR_GTE_TR_FROM_IR();
                    packetCursor = EmitModelTrisGT3(
                        work,
                        (u32 *)(orderingBase + bucketDepth * 4 +
                            ((const TrackObjectPlacement *)(placementCursor - 7))->orderingBias07 * 4),
                        packetCursor);
                }
            }
            placementCursor += sizeof(TrackObjectPlacement);
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = packetCursor;
}

void DrawTrackObjectModel(int modelIndex, int flashing) {
    /* ASM_BARRIER: keep scratch pointer (SCRATCHPAD + 0x7C) in $a2 */
    register TrackObjectRenderWork *work __asm__("$6") = &((TrackObjectScratch *)SCRATCHPAD)->work7C;
    /* Full-width address/scalar temporaries remain until their lifetimes can
     * be separated without the retained register and scheduling constraints. */
    long r_a1;
    RroSceneScratchPrefix *sceneRoot;
    /* ASM_BARRIER: keep temp/count/vertex ptr in $a0 */
    register long r_a0 __asm__("$4");
    /* ASM_BARRIER: keep temp in $v0 */
    register long r_v0 __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register long r_v1 __asm__("$3");
    long r_a3;
    /* ASM_BARRIER: keep GTE IR3 / otz index in $t3 */
    register int depth __asm__("$11");
    int remaining;
    const char *vertices;
    long orderingBase;
    int depthShift;
    char *quadCursor, *packetCursor;
    int x0, x1, x2, x3, y0, y1, y2, y3, cval;

    work->flashing54 = flashing;
    sceneRoot = (RroSceneScratchPrefix *)SCRATCHPAD;
    r_v0 = modelIndex * sizeof(RroEffectMesh);
    r_v1 = (long)sceneRoot->modelEntries48;
    packetCursor = (char *)sceneRoot->view.projection.pointers.cursor;

    r_v1 -= 4;
    r_v1 += r_v0;
    quadCursor = (char *)((RroEffectMesh *)(r_v1 + 4))->commands;
    vertices = (const char *)((RroEffectMesh *)(r_v1 + 4))->vertices;
    remaining = ((unsigned short *)quadCursor)[1];
    quadCursor += 4;
    r_v0 = ((const TrackObjectQuad *)quadCursor)->translationXY1C;
    r_v1 = ((const TrackObjectQuad *)quadCursor)->translationZ1A;
    r_a0 = remaining;
    remaining--;

    work->input00.signedWords[0] = r_v0;
    work->input00.fields.z = r_v1;
    r_v0 = (long)sceneRoot->view.projection.pointers.order;
    r_v1 = sceneRoot->depthShift60;

    orderingBase = r_v0 + 0x200;
    depthShift = r_v1 + 2;
    if (r_a0 > 0) {
        do {
            RR_GTE_MVMVA_LLM_BK_LDV0(&work->input00);

            r_v0 = ((const TrackObjectQuad *)quadCursor)[1].translationXY1C;
            r_v1 = ((const TrackObjectQuad *)quadCursor)[1].translationZ1A;
            /* ASM_BARRIER: load the next translation before staging the GTE input. */
            __asm__ ("" : "+r"(r_v0), "+r"(r_v1));
            work->input00.signedWords[0] = r_v0;
            work->input00.fields.z = r_v1;

            RR_GTE_READ_IR3(depth);
            RR_NOP_DEP_ON(depth);
            if (depth > 0) {
                /* ASM_BARRIER: keep blez delay slot as nop before srav */
                __asm__ ("");
                depth >>= depthShift;
                if (depth < 0x1C0) {
                    work->depth50 = depth;
                    RR_GTE_TR_FROM_IR();

                    r_v0 = ((const TrackObjectQuad *)quadCursor)->vertices.words[0];
                    r_a1 = ((const TrackObjectQuad *)quadCursor)->vertices.words[1];
                    /* ASM_BARRIER: load packed vertex indices before unpacking */
                    __asm__ ("" : "+r"(r_v0), "+r"(r_a1));
                    r_v1 = r_v0 >> 16;
                    r_a3 = r_a1 >> 16;
                    r_v0 &= 0xFFFF;
                    r_a1 &= 0xFFFF;
                    /* ASM_BARRIER: order index shifts and masks before address math */
                    __asm__ ("" : "+r"(r_v1), "+r"(r_a3), "+r"(r_v0), "+r"(r_a1));
                    r_a0 = r_v0 << 3;
                    /* ASM_BARRIER: shift first vertex index into $a0 before adding vertices */
                    __asm__ ("" : "+r"(r_a0));
                    r_a0 = (long)(vertices + r_a0);
                    /* ASM_BARRIER: compute first vertex address in $a0 */
                    __asm__ ("" : "+r"(r_a0));
                    r_v1 = (long)(vertices + (r_v1 * 8));
                    /* ASM_BARRIER: compute second vertex address in $v1 */
                    __asm__ ("" : "+r"(r_v1));
                    r_v0 = (long)(vertices + (r_a1 << 3));
                    /* ASM_BARRIER: compute third vertex address in $v0 */
                    __asm__ ("" : "+r"(r_v0));
                    RR_GTE_RTPT_LDV(r_a0, r_v1, r_v0);

                    r_v0 = ((const TrackObjectQuad *)quadCursor)->uv0C;
                    r_v1 = ((const TrackObjectQuad *)quadCursor)->uv10;
                    /* ASM_BARRIER: load first two UV words before storing */
                    __asm__ ("" : "+r"(r_v0), "+r"(r_v1));
                    ((GpuPolyFT4 *)packetCursor)->words.uv0 = r_v0;
                    ((GpuPolyFT4 *)packetCursor)->words.uv1 = r_v1;
                    r_v0 = ((const TrackObjectQuad *)quadCursor)->uv14;
                    r_v1 = r_v0 >> 16;
                    ((GpuPolyFT4 *)packetCursor)->words.uv2 = r_v0;
                    r_v0 = (long)&work->projected58[0];
                    ((GpuPolyFT4 *)packetCursor)->words.uv3 = r_v1;
                    RR_GTE_SWC2_SXYZ(r_v0);

                    r_v0 = (long)(vertices + (r_a3 * 8));
                    RR_GTE_RTPS_LDV(r_v0);

                    if (work->flashing54 != 0) {
                        cval = FlashingLightColorTable[((u32)ScreenFrame + ((const TrackObjectQuad *)quadCursor)->flashPhase18) & 0x1F];
                    } else {
                        cval = FlashingLightColorTable[0];
                    }
                    r_v0 = cval;

                    r_v1 = work->projected58[0].word;
                    r_a1 = work->projected58[1].word;
                    r_a3 = work->projected58[2].word;
                    /* ASM_BARRIER: load SXY0..SXY2 before storing to packet */
                    __asm__ ("" : "+r"(r_v0), "+r"(r_v1), "+r"(r_a1), "+r"(r_a3));
                    ((GpuPolyFT4 *)packetCursor)->words.color04 = r_v0;
                    r_v0 = (long)&work->projected58[3];

                    ((GpuPolyFT4 *)packetCursor)->words.xy0 = r_v1;
                    ((GpuPolyFT4 *)packetCursor)->words.xy1 = r_a1;
                    ((GpuPolyFT4 *)packetCursor)->words.xy2 = r_a3;
                    RR_GTE_SWC2_SXY2(r_v0);
                    r_v0 = work->projected58[3].word;
                    ((GpuPolyFT4 *)packetCursor)->words.xy3 = r_v0;

                    x0 = work->projected58[0].xy.x;
                    x1 = work->projected58[1].xy.x;
                    x2 = work->projected58[2].xy.x;
                    x3 = work->projected58[3].xy.x;
                    if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                        (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                        y0 = work->projected58[0].xy.y;
                        y1 = work->projected58[1].xy.y;
                        y2 = work->projected58[2].xy.y;
                        y3 = work->projected58[3].xy.y;
                        if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                            ((GpuPolyFT4 *)packetCursor)->words.tag.word = *(int *)(((s32)((u32)((const TrackObjectQuad *)quadCursor)->orderingBias19 << 2)) + ((work->depth50 << 2) + orderingBase));
                            ((GpuPolyFT4 *)packetCursor)->words.tag.bytes.length = 9;

                            *(int *)(((s32)((u32)((const TrackObjectQuad *)quadCursor)->orderingBias19 << 2)) + ((work->depth50 << 2) + orderingBase)) = (u32)(unsigned long)packetCursor;
                            packetCursor += sizeof(GpuPolyFT4);

                            ((char *)(((s32)((u32)((const TrackObjectQuad *)quadCursor)->orderingBias19 << 2)) + ((work->depth50 << 2) + orderingBase)))[3] = 0;
                        }
                    }
                }
            }
            quadCursor += sizeof(TrackObjectQuad);
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packetCursor;
}

void InitIntroSceneLights(void) {
    GpuRectangle sp18;
    struct IntroLightTransformWork {
        Vec3s8 angles;
        Vec3s8 direction;
        MatrixWords32 matrix;
    } *work = (struct IntroLightTransformWork *)(SCRATCHPAD + 0x7C);
    Vec3s8 *angles = &work->angles;
    Vec3s8 *direction = &work->direction;
    IntroSceneState *scene;
    int i;

    D_8010BF78 = LoadCursor;
    LoadCursor = (u8 *)LoadCursor + sizeof(*D_8010BF78);
    SetupRenderer(1);
    sp18.rect = D_800101BC;
    ClearImage2(&sp18, 0, 0, 0);
    ((ProjectionScratchPrefix *)SCRATCHPAD)->pose08.angles10 = D_800101C4;
    ((ProjectionScratchPrefix *)SCRATCHPAD)->pose08.position00 = D_8009E540;
    D_8010BF78->mat20.w[1] = 0xFAAB;
    D_8010BF78->mat20.w[2] = 0xFAAB0000;
    D_8010BF78->mat20.w[0] = 0;
    D_8010BF78->mat20.w[3] = 0;
    D_8010BF78->mat20.w[4] = -0x555;
    direction->fields.z = -0x555;
    direction->fields.y = 0;
    direction->fields.x = 0;
    angles->fields.x = -0x200;
    angles->fields.y = 0x200;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&D_8010BF78->mat00.halves[0]);
    angles->fields.x = 0x400;
    angles->fields.y = 0x400;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&D_8010BF78->mat00.halves[3]);
    angles->fields.x = 0x400;
    angles->fields.y = 0x400;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&D_8010BF78->mat00.halves[6]);
    scene = D_8010BF78;
    i = 7;
    scene->s48 = 0;
    scene->s4A = 0;
    scene->s4C = 0;
    scene->s4E = 0;
    scene->s50 = 0;
    scene->s52 = 0;
    scene->s54 = 0;
    scene->s56 = 0;
    scene->s58 = 0;
    scene->s5A = 0;
    scene->s5C = 0;
    scene->s5E = 0;
    scene->s60 = 0;
    scene->s62 = 0;
    scene->s64 = 0;
    scene->s66 = 0;
    scene->s68 = 0;
    scene->s6A = 0;
    scene->s6C = 0;
    scene->s6E = 0;
    scene->s70 = 0;
    scene->s72 = 0;
    scene->s74 = 0;
    scene->s76 = 0;
    scene->s78 = 0;
    scene->s7A = 0;
    scene->s7C = 0;
    scene->s7E = 0;
    scene->s80 = 0;
    scene->s82 = 0;
    scene->s84 = 0;
    scene->s86 = 0;
    scene->s88 = 0;
    scene->s8A = 0;
    scene->s8C = 0;
    scene->s8E = 0;
    scene->s90 = 0;
    scene->s92 = 0;
    scene->s94 = 0;
    scene->s96 = 0;
    scene->s98 = 0;
    scene->s9A = 0;
    scene->s9C = 0;
    scene->s9E = 0;
    scene->sA0 = 0;
    scene->uA2 = 0;
    scene->sA4 = 0;
    scene->sA6 = 0;
    scene->uA8 = 0;
    do {
        scene->objectYawAA[i] = 0;
        i--;
    } while (i >= 0);
    AllocMenuGridRecords(D_800FFC48, 1);
    InitMenuGrid(0, 1, 3, 0xB4, 0x60);
    PsyqMemset((u8 *)MenuGridRecords + 0x1C, 0, 0x10);
    *(VectorWords16 *)&MenuGridRecords->x0C = D_800101D4;
}

void BuildScaledCameraMatrices(void)
{
    ViewMatrixScratch48 *scratch;
    BuildCameraMatrices();
    scratch = (ViewMatrixScratch48 *)SCRATCHPAD;
    /* Double the camera matrix's Y row, retaining halfword wraparound. */
    scratch->matrix28.halves[3] <<= 1;
    scratch->matrix28.halves[4] <<= 1;
    scratch->matrix28.halves[5] <<= 1;
}
