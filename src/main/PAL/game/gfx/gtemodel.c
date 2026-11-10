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
extern VectorWords16 IntroCameraPosKeyframes;

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
        const TrackObjectPlacement *p = (const TrackObjectPlacement *)placements;
        do {
            int nextXY, nextZ, rawDepth, bucketDepth;
            RR_GTE14_LWC2_V0(&work->input24);
            RR_GTE14_NOP2();
            RR_GTE_OP(0x4A4A2012);
            nextXY = p[1].xy00;
            nextZ = p[1].z04;
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
                        (u32 *)(orderingBase + bucketDepth * 4 + p->orderingBias07 * 4),
                        packetCursor);
                }
            }
            p++;
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = packetCursor;
}

void DrawTrackObjectModel(int modelIndex, int flashing) {
    /* ASM_BARRIER: keep scratch pointer (SCRATCHPAD + 0x7C) in $a2 */
    register TrackObjectRenderWork *work __asm__("$6") = &((TrackObjectScratch *)SCRATCHPAD)->work7C;
    RroSceneScratchPrefix *sceneRoot;
    const RroModelTable *table;
    GpuPolyFT4 *packetCursor;
    const TrackObjectQuad *quadCursor;
    const Vec3s8 *vertices;
    int remaining;
    u32 *orderingBase;
    int depthShift;
    /* ASM_BARRIER: keep GTE IR3 / otz index in $t3 */
    register int depth __asm__("$11");

    work->flashing54 = flashing;
    sceneRoot = (RroSceneScratchPrefix *)SCRATCHPAD;
    table = (const RroModelTable *)((const u8 *)sceneRoot->modelEntries48 - RR_FIELD_OFFSET(RroModelTable, entries));
    packetCursor = (GpuPolyFT4 *)sceneRoot->view.projection.pointers.cursor;
    quadCursor = (const TrackObjectQuad *)table->entries[modelIndex].commands;
    vertices = table->entries[modelIndex].vertices;
    remaining = ((const RroBodyCommandHeader *)quadCursor)->fields.count;
    quadCursor = (const TrackObjectQuad *)((const u8 *)quadCursor + sizeof(RroBodyCommandHeader));
    {
        int xy = quadCursor->translationXY1C;
        int z = quadCursor->translationZ1A;
        work->input00.signedWords[0] = xy;
        work->input00.fields.z = z;
    }
    orderingBase = sceneRoot->view.projection.pointers.order + 128;
    depthShift = sceneRoot->depthShift60 + 2;
    while (remaining-- > 0) {
        RR_GTE14_LWC2_V0(&work->input00);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4A4A2012);
        {
            int nextXY = quadCursor[1].translationXY1C;
            int nextZ = quadCursor[1].translationZ1A;
            work->input00.signedWords[0] = nextXY;
            work->input00.fields.z = nextZ;
        }
        RR_GTE_READ_IR3_NOP(depth);
        if (depth > 0) {
            /* ASM_BARRIER: keep blez delay slot as nop before srav */
            __asm__ ("");
            depth >>= depthShift;
            if (depth < 0x1C0) {
                int word0, word1, word2, word3;
                work->depth50 = depth;
                RR_GTE_TR_FROM_IR();
                word0 = quadCursor->vertices.words[0];
                word1 = quadCursor->vertices.words[1];
                word2 = word0 >> 16;
                word3 = word1 >> 16;
                word0 &= 0xFFFF;
                word1 &= 0xFFFF;
                RR_GTE_LWC2_V012_3P(&vertices[word0], &vertices[word2], &vertices[word1]);
                RR_GTE_RTPT_NOP2();
                word0 = quadCursor->uv0C;
                word2 = quadCursor->uv10;
                packetCursor->words.uv0 = word0;
                packetCursor->words.uv1 = word2;
                word0 = quadCursor->uv14;
                word2 = word0 >> 16;
                packetCursor->words.uv2 = word0;
                packetCursor->words.uv3 = word2;
                RR_GTE_SWC2_SXYZ(&work->projected58[0]);
                RR_GTE_RTPS_LDV(&vertices[word3]);
                if (work->flashing54 != 0) {
                    word0 = FlashingLightColorTable[((u32)ScreenFrame + quadCursor->flashPhase18) & 0x1F];
                } else {
                    word0 = FlashingLightColorTable[0];
                }
                word2 = work->projected58[0].word;
                word1 = work->projected58[1].word;
                word3 = work->projected58[2].word;
                packetCursor->words.color04 = word0;
                packetCursor->words.xy0 = word2;
                packetCursor->words.xy1 = word1;
                packetCursor->words.xy2 = word3;
                RR_GTE_SWC2_SXY2(&work->projected58[3]);
                packetCursor->words.xy3 = work->projected58[3].word;
                {
                    int x0 = work->projected58[0].xy.x;
                    int x1 = work->projected58[1].xy.x;
                    int x2 = work->projected58[2].xy.x;
                    int x3 = work->projected58[3].xy.x;
                    if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                        (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                        int y0 = work->projected58[0].xy.y;
                        int y1 = work->projected58[1].xy.y;
                        int y2 = work->projected58[2].xy.y;
                        int y3 = work->projected58[3].xy.y;
                        if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                            (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100)) {
                            packetCursor->words.tag.word = (orderingBase + work->depth50)[quadCursor->orderingBias19];
                            packetCursor->words.tag.bytes.length = 9;
                            (orderingBase + work->depth50)[quadCursor->orderingBias19] = (u32)(unsigned long)packetCursor;
                            packetCursor++;
                            ((GpuDmaTag *)&(orderingBase + work->depth50)[quadCursor->orderingBias19])->bytes.length = 0;
                        }
                    }
                }
            }
        }
        quadCursor++;
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

    IntroSceneStatePtr = LoadCursor;
    LoadCursor = (u8 *)LoadCursor + sizeof(*IntroSceneStatePtr);
    SetupRenderer(1);
    sp18.rect = D_800101BC;
    ClearImage2(&sp18, 0, 0, 0);
    ((ProjectionScratchPrefix *)SCRATCHPAD)->pose08.angles10 = D_800101C4;
    ((ProjectionScratchPrefix *)SCRATCHPAD)->pose08.position00 = IntroCameraPosKeyframes;
    IntroSceneStatePtr->mat20.w[1] = 0xFAAB;
    IntroSceneStatePtr->mat20.w[2] = 0xFAAB0000;
    IntroSceneStatePtr->mat20.w[0] = 0;
    IntroSceneStatePtr->mat20.w[3] = 0;
    IntroSceneStatePtr->mat20.w[4] = -0x555;
    direction->fields.z = -0x555;
    direction->fields.y = 0;
    direction->fields.x = 0;
    angles->fields.x = -0x200;
    angles->fields.y = 0x200;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&IntroSceneStatePtr->mat00.halves[0]);
    angles->fields.x = 0x400;
    angles->fields.y = 0x400;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&IntroSceneStatePtr->mat00.halves[3]);
    angles->fields.x = 0x400;
    angles->fields.y = 0x400;
    angles->fields.z = 0;
    RotMatrix(&angles->fields, &work->matrix);
    ApplyMatrixSV(&work->matrix, direction, (s16 *)&IntroSceneStatePtr->mat00.halves[6]);
    scene = IntroSceneStatePtr;
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
    AllocMenuGridRecords(SharedWorkArena, 1);
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
