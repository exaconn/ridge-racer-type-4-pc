#include "rr/gpu_address.h"
#include "rr/frame_storage_types.h"
#include "game/camera/viewcam.h"
#include "types.h"
#include "scratchpad.h"
#include "rr_asm.h"
#include "game/gfx/gtepoly1.h"
#include "rr/rro_model.h"
#include "rr/rro_effects.h"
#include "game/data/cdfile2.h"
#include "rr/fade_sprite.h"
#include "rr/audio_cd_controller.h"
#include "game/data/globals.h"
#include "game/data/screenframe.h"
#include "game/car/bounds.h"
#include "rr/audio_loop.h"
#include "psyq/libc/rand.h"

#define WORK ((RroEffectWork *)s2)
#define SLOT ((CarSlotBufferC8 *)s4)
/* ASM_BARRIER: empty asm that only reads its operands; keeps both values live to the end of the block
 * so gcc expands the signed divisions with a copy (bgez; move; addiu) as in the original. */
#define KEEP_LIVE2(a, b) __asm__ ("" : : "r"(a), "r"(b))

typedef struct {
    s16 unk0;
    u16 unk2;
    RroProjectedVertex *verts;
} TrailEntry4;

typedef struct {
    s16 unk0;
    s16 unk2;
    Vec3sFields8 *verts;
} TrailEntry8;

extern int MaskBitSetting;
extern s32 ModelTexturePage;
extern char UnknownRroTypeFmt[];
extern signed char RacePaused;
extern RroEffectColorTable D_8009E0B0;
extern int D_8009E0E0[];




void SetModelTexturePage(s32 arg0);
int DrawRroModel(const Vector *position, MatrixWords32 *rotation, s32 object, s32 flags);





void DrawWheelTrailEffects(void *arg0, s32 *arg1, void *arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

const u32 *EmitRroRawTexturedQuads(const u32 *commands, RroRenderWork *inputWork, s32 count) {
    /* Remaining numeric vertex-address/word lifetime; full width on PAL/LP64. */
    long valueOrAddress;
    int secondWord;
    u8 *wordCursor = (u8 *)commands;
    RroRenderWork *work = inputWork;
    int remaining = count;
    /* ASM_BARRIER: keep temp in $a3 */
    register int firstWord __asm__("$7");
    /* ASM_BARRIER: keep temp in $t0 */
    register int secondValue __asm__("$8");
    int thirdValue;
    /* ASM_BARRIER: keep poly+0x15 cursor in $t3 */
    register char *biasCursor __asm__("$11");
    GpuPolyFT4 *packet;
    long vertices;
    int fourthIndex;
    const RroRawTexturedQuad *quad;
    int depthShift;
    /* ASM_BARRIER: keep 0x2F000000 constant in $t9 */
    register int textureCommand __asm__("$25");
    int x0, x1, x2, x3, y0, y1, y2, y3;

    quad = (const RroRawTexturedQuad *)wordCursor;
    textureCommand = 0x2F000000;
    /* ASM_BARRIER: order quad and textureCommand initialization before loads */
    __asm__ ("" : "+r"(quad), "+r"(textureCommand));
    valueOrAddress = quad->vertices.words[0];
    secondWord = quad->vertices.words[1];
    vertices = (long)work->vertices00;
    thirdValue = valueOrAddress >> 16;
    fourthIndex = secondWord >> 16;
    valueOrAddress &= 0xFFFF;
    valueOrAddress = (valueOrAddress << 3) + vertices;
    firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    valueOrAddress = ((s16)thirdValue * RRO_VECTOR_STRIDE) + vertices;
    thirdValue = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    valueOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    secondWord &= 0xFFFF;
    work->vertices28[1].signedWords[1] = valueOrAddress;
    valueOrAddress = (secondWord << 3) + vertices;
    work->vertices28[0].signedWords[0] = firstWord;
    work->vertices28[0].signedWords[1] = secondValue;
    work->vertices28[1].signedWords[0] = thirdValue;
    firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
    secondWord = remaining;
    /* ASM_BARRIER: compute fourth vertex pointer and copy a2 to v1 */
    __asm__ ("" : "+r"(valueOrAddress), "+r"(secondWord));
    thirdValue = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    valueOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    work->vertices28[3].signedWords[1] = valueOrAddress;
    work->vertices28[2].signedWords[0] = firstWord;
    work->vertices28[2].signedWords[1] = secondValue;
    work->vertices28[3].signedWords[0] = thirdValue;
    packet = (GpuPolyFT4 *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
    depthShift = ((RroSceneScratchPrefix *)SCRATCHPAD)->depthShift60;
    remaining--;
    if (secondWord > 0) {
        biasCursor = (char *)&((const RroRawTexturedQuad *)wordCursor)->orderingBias15;
        do {
            RR_GTE14_LWC2_V012(&work->vertices28[0].signedWords[0]);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            valueOrAddress = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))[1].vertices.words[0];
            secondWord = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))[1].vertices.words[1];
            thirdValue = valueOrAddress >> 16;
            fourthIndex = secondWord >> 16;
            valueOrAddress &= 0xFFFF;
            valueOrAddress = (valueOrAddress << 3) + vertices;
            firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            secondWord &= 0xFFFF;
            work->vertices28[0].signedWords[0] = firstWord;
            work->vertices28[0].signedWords[1] = secondValue;
            RR_GTE14_NCLIP();

            valueOrAddress = ((s16)thirdValue * RRO_VECTOR_STRIDE) + vertices;
            firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_MAC0(&work->firstArea1C);
            RR_GTE14_SWC2_SXYZ(&work->projected50[0].word);
            RR_GTE14_LWC2_V0(&work->vertices28[3].signedWords[0]);
            work->vertices28[1].signedWords[0] = firstWord;
            work->vertices28[1].signedWords[1] = secondValue;
            RR_GTE14_RTPS();

            valueOrAddress = (secondWord << 3) + vertices;
            firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            work->vertices28[2].signedWords[0] = firstWord;
            work->vertices28[2].signedWords[1] = secondValue;
            RR_GTE14_AVSZ4();

            valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
            firstWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondValue = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_OTZ(&work->depth18);
            valueOrAddress = work->depth18;
            work->vertices28[3].signedWords[0] = firstWord;
            work->vertices28[3].signedWords[1] = secondValue;
            if (valueOrAddress > 0) {
                valueOrAddress >>= depthShift;
                work->depth18 = valueOrAddress;
                if (valueOrAddress < 0x1C0) {
                    RR_GTE14_NCLIP();
                    firstWord = work->projected50[0].word;
                    secondValue = work->projected50[1].word;
                    thirdValue = work->projected50[2].word;
                    valueOrAddress = (long)(&work->secondArea20);
                    packet->words.xy0 = firstWord;
                    packet->words.xy1 = secondValue;
                    packet->words.xy2 = thirdValue;
                    RR_GTE14_SWC2_MAC0(valueOrAddress);
                    if (work->firstArea1C > 0 || work->secondArea20 < 0) {
                        RR_GTE14_SWC2_SXY2(&work->projected50[3].word);
                        valueOrAddress = work->projected50[3].word;
                        packet->words.xy3 = valueOrAddress;

                        x0 = work->projected50[0].xy.x;
                        x1 = work->projected50[1].xy.x;
                        x2 = work->projected50[2].xy.x;
                        x3 = work->projected50[3].xy.x;
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = work->projected50[0].xy.y;
                            y1 = work->projected50[1].xy.y;
                            y2 = work->projected50[2].xy.y;
                            y3 = work->projected50[3].xy.y;
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                packet->words.color04 = textureCommand;
                                firstWord = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))->uv08;
                                thirdValue = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))->uv10;
                                secondValue = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))->uv0C;
                                valueOrAddress = thirdValue >> 16;
                                packet->words.uv0 = firstWord;
                                packet->words.uv1 = secondValue;
                                packet->words.uv2 = thirdValue;
                                packet->words.uv3 = valueOrAddress;

                                secondWord = work->depth18;
                                wordCursor = (u8 *)work->ordering14;
                                valueOrAddress = ((const RroRawTexturedQuad *)(biasCursor - RR_FIELD_OFFSET(RroRawTexturedQuad, orderingBias15)))->orderingBias15;
                                secondWord <<= 2;
                                wordCursor += secondWord;
                                valueOrAddress *= 4;
                                wordCursor += valueOrAddress;
                                valueOrAddress = *(int *)wordCursor;
                                packet->words.tag.word = valueOrAddress;
                                packet->words.tag.bytes.length = 9;
                                *(u32 *)wordCursor = GpuDmaEncodeAddress(packet);
                                packet++;
                                ((GpuDmaTag *)wordCursor)->bytes.length = 0;
                            }
                        }
                    }
                }
            }
            biasCursor += sizeof(RroRawTexturedQuad);
            quad++;
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
    return (const u32 *)quad;
}

const u32 *EmitRroLitGouraudQuads(const u32 *commands, RroRenderWork *inputWork, s32 count) {
    /* ASM_BARRIER: keep temp in $v0 */
    register long valueOrAddress __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register long indexOrAddress __asm__("$3");
    /* ASM_BARRIER: retain command/normal-address/depth phases in $a0 */
    register long addressOrDepth __asm__("$4") = (long)commands;
    /* Retained context-address/word lifetime; full width on PAL/LP64. */
    long firstWordOrContext = (long)inputWork;
    int remaining = count;
    int secondVertexWord;
    int indexWord;
    int secondWord;
    /* ASM_BARRIER: keep context pointer in $t2 */
    register RroRenderWork *work __asm__("$10");
    int fourthIndex;
    GpuMaskedG4Packet *packet;
    long vertices;
    long normals;
    const RroLitGouraudQuad *quad;
    int maskCommand;
    int depthShift;
    int x0, x1, x2, x3, y0, y1, y2, y3;

    quad = (const RroLitGouraudQuad *)addressOrDepth;
    work = (RroRenderWork *)firstWordOrContext;
    maskCommand = 0xE6000000;
    valueOrAddress = quad->vertices.words[0];
    indexWord = quad->vertices.words[1];
    vertices = (long)work->vertices00;
    indexOrAddress = valueOrAddress >> 16;
    fourthIndex = indexWord >> 16;
    valueOrAddress &= 0xFFFF;
    valueOrAddress = (valueOrAddress << 3) + vertices;
    firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    /* ASM_BARRIER: load first vertex words before second vertex address */
    __asm__ ("" : : "r"(firstWordOrContext), "r"(secondVertexWord));
    valueOrAddress = ((s16)indexOrAddress * RRO_VECTOR_STRIDE) + vertices;
    indexWord &= 0xFFFF;
    /* ASM_BARRIER: compute second vertex pointer and capture the third index */
    __asm__ ("" : "+r"(valueOrAddress), "+r"(indexWord));
    secondWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    valueOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    /* ASM_BARRIER: load second vertex words before capturing the loop count */
    __asm__ ("" : : "r"(secondWord), "r"(valueOrAddress));
    indexOrAddress = remaining;
    work->vertices28[1].signedWords[1] = valueOrAddress;
    valueOrAddress = (indexWord << 3) + vertices;
    work->vertices28[0].signedWords[0] = firstWordOrContext;
    work->vertices28[0].signedWords[1] = secondVertexWord;
    work->vertices28[1].signedWords[0] = secondWord;
    firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
    secondWord = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    valueOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    work->vertices28[3].signedWords[1] = valueOrAddress;
    work->vertices28[2].signedWords[0] = firstWordOrContext;
    work->vertices28[2].signedWords[1] = secondVertexWord;
    work->vertices28[3].signedWords[0] = secondWord;
    packet = (GpuMaskedG4Packet *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
    depthShift = ((RroSceneScratchPrefix *)SCRATCHPAD)->depthShift60;
    remaining--;
    if (indexOrAddress > 0) {
        do {
            RR_GTE14_LWC2_V012(&work->vertices28[0].signedWords[0]);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            valueOrAddress = quad[1].vertices.words[0];
            indexWord = quad[1].vertices.words[1];
            indexOrAddress = valueOrAddress >> 16;
            fourthIndex = indexWord >> 16;
            valueOrAddress &= 0xFFFF;
            valueOrAddress = (valueOrAddress << 3) + vertices;
            firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            indexWord &= 0xFFFF;
            work->vertices28[0].signedWords[0] = firstWordOrContext;
            work->vertices28[0].signedWords[1] = secondVertexWord;
            RR_GTE14_NCLIP();

            valueOrAddress = ((s16)indexOrAddress * RRO_VECTOR_STRIDE) + vertices;
            /* ASM_BARRIER: compute second vertex pointer in $v0 */
            __asm__ ("" : : "r"(valueOrAddress));
            firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_MAC0(&work->firstArea1C);
            RR_GTE14_SWC2_SXYZ(&work->projected50[0].word);
            RR_GTE14_LWC2_V0(&work->vertices28[3].signedWords[0]);
            work->vertices28[1].signedWords[0] = firstWordOrContext;
            work->vertices28[1].signedWords[1] = secondVertexWord;
            RR_GTE14_RTPS();

            valueOrAddress = (indexWord << 3) + vertices;
            firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            work->vertices28[2].signedWords[0] = firstWordOrContext;
            work->vertices28[2].signedWords[1] = secondVertexWord;
            RR_GTE14_AVSZ4();

            valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
            firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_OTZ(&work->depth18);
            valueOrAddress = work->depth18;
            work->vertices28[3].signedWords[0] = firstWordOrContext;
            work->vertices28[3].signedWords[1] = secondVertexWord;
            if (valueOrAddress > 0) {
                valueOrAddress >>= depthShift;
                work->depth18 = valueOrAddress;
                if (valueOrAddress < 0x1C0) {
                    RR_GTE14_NCLIP();
                    firstWordOrContext = work->projected50[0].word;
                    secondVertexWord = work->projected50[1].word;
                    secondWord = work->projected50[2].word;
                    packet->xy0C = firstWordOrContext;
                    packet->xy14 = secondVertexWord;
                    packet->xy1C = secondWord;
                    RR_GTE14_SWC2_MAC0(&work->secondArea20);
                    if (work->firstArea1C > 0 || work->secondArea20 < 0) {
                        RR_GTE14_SWC2_SXY2(&work->projected50[3].word);
                        valueOrAddress = work->projected50[3].word;
                        packet->xy24 = valueOrAddress;

                        x0 = work->projected50[0].xy.x;
                        x1 = work->projected50[1].xy.x;
                        x2 = work->projected50[2].xy.x;
                        x3 = work->projected50[3].xy.x;
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = work->projected50[0].xy.y;
                            y1 = work->projected50[1].xy.y;
                            y2 = work->projected50[2].xy.y;
                            y3 = work->projected50[3].xy.y;
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                valueOrAddress = quad->normals.words[0];
                                indexWord = quad->normals.words[1];
                                normals = (long)work->normals04;
                                indexOrAddress = valueOrAddress >> 16;
                                fourthIndex = indexWord >> 16;
                                valueOrAddress &= 0xFFFF;
                                indexWord &= 0xFFFF;
                                addressOrDepth = valueOrAddress << 3;
                                addressOrDepth = normals + addressOrDepth;
                                indexOrAddress = normals + ((s16)indexOrAddress * RRO_VECTOR_STRIDE);
                                valueOrAddress = normals + (indexWord << 3);
                                RR_GTE14_LWC2_V0_V1_V2(addressOrDepth, indexOrAddress, valueOrAddress);
                                RR_GTE14_LWC2_RGB(&quad->color10);
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();

                                valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + normals;
                                firstWordOrContext = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
                                secondVertexWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
                                work->normal48.signedWords[0] = firstWordOrContext;
                                work->normal48.signedWords[1] = secondVertexWord;
                                RR_GTE14_LIGHT1();

                                addressOrDepth = work->depth18;
                                indexOrAddress = (long)work->ordering14;
                                valueOrAddress = quad->orderingBias15;
                                addressOrDepth <<= 2;
                                indexOrAddress += addressOrDepth;
                                valueOrAddress *= 4;
                                indexOrAddress += valueOrAddress;
                                valueOrAddress = *(int *)indexOrAddress;
                                packet->tag.word = valueOrAddress;
                                valueOrAddress = 0xA;
                                packet->tag.bytes.length = valueOrAddress;
                                *(u32 *)indexOrAddress = GpuDmaEncodeAddress(packet);
                                ((char *)indexOrAddress)[3] = 0;
                                indexOrAddress = MaskBitSetting;
                                valueOrAddress = maskCommand | 1;
                                packet->setMask04 = valueOrAddress;
                                indexOrAddress = maskCommand | indexOrAddress;
                                packet->restoreMask28 = indexOrAddress;
                                RR_GTE14_LIGHT2();
                                RR_GTE14_SWC2_RGB012(&packet->setMask04);
                                RR_GTE14_LWC2_V0(&work->normal48.signedWords[0]);
                                RR_GTE14_NOP2();
                                RR_GTE14_LIGHT0();
                                RR_GTE14_SWC2_RGB2(&packet->color20);
                                packet++;
                            }
                        }
                    }
                }
            }
            quad++;
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
    return (const u32 *)quad;
}

const u32 *EmitRroEnvMapQuads(const u32 *commands, RroRenderWork *inputWork, s32 count) {
    /* ASM_BARRIER: keep temp in $v0 */
    register long valueOrAddress __asm__("$2");
    /* ASM_BARRIER: keep temp in $v1 */
    register long wordOrAddress __asm__("$3");
    long wordOrHead;
    /* ASM_BARRIER: retain index/normal-address phases in $a1 */
    register long indexOrAddress __asm__("$5");
    int indexWord = count;
    /* ASM_BARRIER: keep work context in $t0 */
    register RroRenderWork *work __asm__("$8");
    /* ASM_BARRIER: keep envmap U in $t1 */
    register int envU __asm__("$9");
    /* ASM_BARRIER: keep envmap V in $t2 */
    register int envV __asm__("$10");
    char *packetCursor;
    /* ASM_BARRIER: keep fourth vertex index in $t5 */
    register int fourthIndex __asm__("$13");
    long vertices;
    long normals;
    /* ASM_BARRIER: keep temp in $t7 */
    register int screenWord __asm__("$15");
    int envVShift;
    /* ASM_BARRIER: keep loop counter in $t9 */
    register int remaining __asm__("$25");
    const RroEnvMapQuad *quad;
    int maskCommand;
    int maskedLength;
    int depthShift;
    int restoreMask;
    int x0, x1, x2, x3, y0, y1, y2, y3;

    quad = (const RroEnvMapQuad *)commands;
    work = inputWork;
    remaining = indexWord;
    /* ASM_BARRIER: retain command, work and count before initial loads */
    __asm__("" : "+r"(work), "+r"(remaining) : "r"(quad));
    maskCommand = 0xE6000000;
    valueOrAddress = quad->vertices.words[0];
    indexWord = quad->vertices.words[1];
    vertices = (long)work->vertices00;
    indexOrAddress = valueOrAddress >> 16;
    fourthIndex = indexWord >> 16;
    valueOrAddress &= 0xFFFF;
    valueOrAddress = (valueOrAddress << 3) + vertices;
    wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    /* ASM_BARRIER: load first vertex words before second vertex address */
    __asm__ ("" : : "r"(wordOrAddress), "r"(wordOrHead));
    valueOrAddress = ((s16)indexOrAddress * RRO_VECTOR_STRIDE) + vertices;
    indexWord &= 0xFFFF;
    /* ASM_BARRIER: compute second vertex pointer and capture the third index */
    __asm__ ("" : "+r"(valueOrAddress) : "r"(indexWord));
    indexOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    screenWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    valueOrAddress = (indexWord << 3) + vertices;
    /* ASM_BARRIER: compute third vertex pointer before storing 0x28..0x34 */
    __asm__ ("" : "+r"(valueOrAddress));
    work->vertices28[0].signedWords[0] = wordOrAddress;
    work->vertices28[0].signedWords[1] = wordOrHead;
    work->vertices28[1].signedWords[0] = indexOrAddress;
    work->vertices28[1].signedWords[1] = screenWord;
    wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    /* ASM_BARRIER: load third vertex words before fourth vertex address */
    valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
    /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
    __asm__ volatile("" : "+r"(valueOrAddress));
    indexOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
    screenWord = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
    work->vertices28[2].signedWords[1] = wordOrHead;
    wordOrHead = (long)SCRATCHPAD;
    /* These retained packed-XY word views still constrain global-load order. */
    *(s32 *)((u8 *)work + RR_FIELD_OFFSET(RroRenderWork, vertices28[2].signedWords[0])) = wordOrAddress;
    *(s32 *)((u8 *)work + RR_FIELD_OFFSET(RroRenderWork, vertices28[3].signedWords[0])) = indexOrAddress;
    valueOrAddress = MaskBitSetting;
    wordOrAddress = 0x100;
    work->vertices28[3].signedWords[1] = screenWord;
    restoreMask = valueOrAddress | maskCommand;
    valueOrAddress = (long)CurrentDrawBuf;
    packetCursor = (char *)((RroSceneScratchPrefix *)wordOrHead)->view.projection.pointers.cursor;
    valueOrAddress = ((FrameStorage *)valueOrAddress)->buffer.environment00.draw00.clip.h;
    depthShift = ((RroSceneScratchPrefix *)wordOrHead)->depthShift60;
    envVShift = 7;
    if (valueOrAddress == wordOrAddress) {
        envVShift = 6;
    }
    if (remaining-- > 0) {
        maskedLength = 0xD;
        do {
            RR_GTE14_LWC2_V012(&work->vertices28[0].signedWords[0]);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();

            valueOrAddress = quad[1].vertices.words[0];
            indexWord = quad[1].vertices.words[1];
            indexOrAddress = valueOrAddress >> 16;
            fourthIndex = indexWord >> 16;
            valueOrAddress &= 0xFFFF;
            valueOrAddress = (valueOrAddress << 3) + vertices;
            wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            indexWord &= 0xFFFF;
            work->vertices28[0].signedWords[0] = wordOrAddress;
            work->vertices28[0].signedWords[1] = wordOrHead;
            RR_GTE14_NCLIP();

            valueOrAddress = ((s16)indexOrAddress * RRO_VECTOR_STRIDE) + vertices;
            /* ASM_BARRIER: compute second vertex pointer in $v0 */
            __asm__ ("" : "+r"(valueOrAddress));
            wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_MAC0(&work->firstArea1C);
            RR_GTE14_SWC2_SXYZ(&work->projected50[0].word);
            RR_GTE14_LWC2_V0(&work->vertices28[3].signedWords[0]);
            work->vertices28[1].signedWords[0] = wordOrAddress;
            work->vertices28[1].signedWords[1] = wordOrHead;
            RR_GTE14_RTPS();

            valueOrAddress = (indexWord << 3) + vertices;
            wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            work->vertices28[2].signedWords[0] = wordOrAddress;
            work->vertices28[2].signedWords[1] = wordOrHead;
            RR_GTE14_AVSZ4();

            valueOrAddress = ((s16)fourthIndex * RRO_VECTOR_STRIDE) + vertices;
            /* ASM_BARRIER: compute fourth vertex pointer in $v0 */
            __asm__ ("" : "+r"(valueOrAddress));
            wordOrAddress = ((const Vec3s8 *)valueOrAddress)->signedWords[0];
            wordOrHead = ((const Vec3s8 *)valueOrAddress)->signedWords[1];
            RR_GTE14_SWC2_OTZ(&work->depth18);
            valueOrAddress = work->depth18;
            work->vertices28[3].signedWords[0] = wordOrAddress;
            work->vertices28[3].signedWords[1] = wordOrHead;
            if (valueOrAddress > 0) {
                valueOrAddress >>= depthShift;
                work->depth18 = valueOrAddress;
                if (valueOrAddress < 0x1C0) {
                    RR_GTE14_NCLIP();
                    wordOrAddress = work->projected50[0].word;
                    wordOrHead = work->projected50[1].word;
                    indexOrAddress = work->projected50[2].word;
                    ((GpuEnvMapEmission *)packetCursor)->primary00.body08.xy0 = wordOrAddress;
                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.xy0 = wordOrAddress;
                    ((GpuEnvMapEmission *)packetCursor)->primary00.body08.xy1 = wordOrHead;
                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.xy1 = wordOrHead;
                    ((GpuEnvMapEmission *)packetCursor)->primary00.body08.xy2 = indexOrAddress;
                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.xy2 = indexOrAddress;
                    RR_GTE14_SWC2_MAC0(&work->secondArea20);
                    if (work->firstArea1C > 0 || work->secondArea20 < 0) {
                        RR_GTE_MFC2_SXY2_NOP(screenWord);
                        work->projected50[3].word = screenWord;
                        ((GpuEnvMapEmission *)packetCursor)->primary00.body08.xy3 = screenWord;
                        ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.xy3 = screenWord;

                        x0 = work->projected50[0].xy.x;
                        x1 = work->projected50[1].xy.x;
                        x2 = work->projected50[2].xy.x;
                        x3 = work->projected50[3].xy.x;
                        if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0) &&
                            (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140)) {
                            y0 = work->projected50[0].xy.y;
                            y1 = work->projected50[1].xy.y;
                            y2 = work->projected50[2].xy.y;
                            y3 = work->projected50[3].xy.y;
                            if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0) &&
                                (y0 < 0x200 || y1 < 0x200 || y2 < 0x200 || y3 < 0x200)) {
                                wordOrAddress = quad->normals.words[0];
                                valueOrAddress = quad->normals.words[1];
                                normals = (long)work->normals04;
                                wordOrHead = wordOrAddress >> 16;
                                wordOrAddress &= 0xFFFF;
                                indexOrAddress = valueOrAddress >> 16;
                                valueOrAddress &= 0xFFFF;
                                wordOrAddress <<= 3;
                                wordOrAddress = normals + wordOrAddress;
                                wordOrHead = normals + ((s16)wordOrHead * RRO_VECTOR_STRIDE);
                                valueOrAddress = normals + (valueOrAddress << 3);
                                RR_GTE14_LWC2_V0_V1_V2(wordOrAddress, wordOrHead, valueOrAddress);
                                RR_GTE14_NOP2();
                                RR_GTE_OP(0x4A4A6412);

                                indexOrAddress = ((s16)indexOrAddress * RRO_VECTOR_STRIDE) + normals;
                                wordOrAddress = ((const Vec3s8 *)indexOrAddress)->signedWords[0];
                                wordOrHead = ((const Vec3s8 *)indexOrAddress)->signedWords[1];
                                work->normal48.signedWords[0] = wordOrAddress;
                                work->normal48.signedWords[1] = wordOrHead;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                wordOrAddress = quad->uv10;
                                wordOrHead = quad->uv14;
                                RR_GTE_OP(0x4A4AE412);
                                valueOrAddress = work->flags08;
                                valueOrAddress = (u32)wordOrAddress + valueOrAddress;
                                ((GpuEnvMapEmission *)packetCursor)->primary00.body08.uv0 = valueOrAddress;
                                ((GpuEnvMapEmission *)packetCursor)->primary00.body08.uv1 = wordOrHead;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                wordOrAddress = quad->uv18;
                                wordOrHead = wordOrAddress >> 16;
                                RR_GTE_OP(0x4A4B6412);
                                ((GpuEnvMapEmission *)packetCursor)->primary00.body08.uv2 = wordOrAddress;
                                ((GpuEnvMapEmission *)packetCursor)->primary00.body08.uv3 = wordOrHead;
                                RR_GTE_SQR12();
                                RR_GTE_SQR12();
                                RR_GTE_OP(0x4B38041C);

                                RR_GTE_SWC2_RGB012_ENV(&((GpuEnvMapEmission *)packetCursor)->primary00.command04.word);

                                valueOrAddress = work->depth18;
                                wordOrHead = (long)work->ordering14;
                                wordOrAddress = quad->orderingBias1D;
                                indexOrAddress = quad->control1E;
                                valueOrAddress <<= 2;
                                wordOrHead += valueOrAddress;
                                wordOrAddress = (s32)((u32)wordOrAddress << 2);
                                valueOrAddress = indexOrAddress & 3;
                                wordOrHead += wordOrAddress;
                                if (valueOrAddress == 0) {
                                    RR_GTE14_LWC2_V0(&work->normal48.signedWords[0]);
                                    RR_GTE14_NOP2();
                                    RR_GTE14_LIGHT0();
                                    valueOrAddress = indexOrAddress >> 1;
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.body08.color0.bytes.code = valueOrAddress;
                                    valueOrAddress = *(int *)wordOrHead;
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.command04.word = valueOrAddress;
                                    valueOrAddress = 0xC;
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.command04.directTag.bytes.length = valueOrAddress;
                                    *(u32 *)wordOrHead = GpuDmaEncodeAddress(&((GpuEnvMapEmission *)packetCursor)->primary00.command04.word);
                                    ((char *)wordOrHead)[3] = 0;
                                    RR_GTE14_SWC2_RGB2(&((GpuEnvMapEmission *)packetCursor)->primary00.body08.color3);
                                    packetCursor += sizeof(GpuEnvMapBlock);
                                } else {
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.command04.word = maskCommand;
                                    valueOrAddress = quad->control1E;
                                    valueOrAddress &= 3;
                                    valueOrAddress = maskCommand | valueOrAddress;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.command04.word = valueOrAddress;
                                    RR_GTE_SWC2_RGB012_ENV(&((GpuEnvMapEmission *)packetCursor)->reflected38.command04.word);
                                    valueOrAddress = 0x3E;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.color0.bytes.code = valueOrAddress;
                                    RR_GTE_OP(0x4A486012);
                                    valueOrAddress = *(int *)wordOrHead;
                                    /* Original checks encoded address+4, including any low tag bits;
                                     * this is the first payload byte, not the opcode at byte7. */
                                    wordOrAddress = GpuDmaReadPayloadByte(valueOrAddress);
                                    valueOrAddress = 0xE6;
                                    if (wordOrAddress != valueOrAddress) {
                                        ((GpuEnvMapEmission *)packetCursor)->restore70.command = restoreMask;
                                        valueOrAddress = *(int *)wordOrHead;
                                        ((GpuEnvMapEmission *)packetCursor)->restore70.tag.word = valueOrAddress;
                                        valueOrAddress = 1;
                                        ((GpuEnvMapEmission *)packetCursor)->restore70.tag.bytes.length = valueOrAddress;
                                        valueOrAddress = GpuDmaEncodeAddress(packetCursor + 0x70);
                                        *(u32 *)wordOrHead = valueOrAddress;
                                        ((char *)wordOrHead)[3] = 0;
                                    }
                                    valueOrAddress = *(int *)wordOrHead;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.tag00.word = valueOrAddress;
                                    valueOrAddress = GpuDmaEncodeAddress(packetCursor + 0x38);
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.tag00.bytes.length = maskedLength;
                                    *(u32 *)wordOrHead = valueOrAddress;
                                    ((char *)wordOrHead)[3] = 0;
                                    valueOrAddress = *(int *)wordOrHead;
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.tag00.word = valueOrAddress;
                                    ((GpuEnvMapEmission *)packetCursor)->primary00.tag00.bytes.length = maskedLength;
                                    *(u32 *)wordOrHead = GpuDmaEncodeAddress(packetCursor);
                                    ((char *)wordOrHead)[3] = 0;

                                    RR_GTE_MFC2_IR1_IR2(envU, envV);
                                    valueOrAddress = envU >> 6;
                                    envU = valueOrAddress + 0xC0;
                                    valueOrAddress = envV >> envVShift;
                                    envV = valueOrAddress + 0xC0;
                                    valueOrAddress = (u32)envV << 8;
                                    wordOrAddress = work->envClut0C;
                                    valueOrAddress |= envU;
                                    valueOrAddress |= wordOrAddress;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.uv0 = valueOrAddress;

                                    RR_GTE14_LWC2_V0(&work->normal48.signedWords[0]);
                                    RR_GTE14_NOP2();
                                    RR_GTE14_LIGHT0();
                                    RR_GTE14_SWC2_RGB2(&((GpuEnvMapEmission *)packetCursor)->primary00.body08.color3);
                                    RR_GTE14_SWC2_RGB2(&((GpuEnvMapEmission *)packetCursor)->reflected38.body08.color3);
                                    RR_NOP_DEP();
                                    RR_GTE_OP(0x4A48E012);
                                    RR_GTE_MFC2_IR1_IR2(envU, envV);
                                    RR_NOP_DEP();
                                    RR_GTE_OP(0x4A496012);

                                    valueOrAddress = envU >> 6;
                                    envU = valueOrAddress + 0xC0;
                                    valueOrAddress = envV >> envVShift;
                                    envV = valueOrAddress + 0xC0;
                                    valueOrAddress = (u32)envV << 8;
                                    wordOrAddress = work->envTpage10;
                                    valueOrAddress |= envU;
                                    valueOrAddress |= wordOrAddress;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.uv1 = valueOrAddress;

                                    RR_GTE_MFC2_IR1_IR2(envU, envV);
                                    valueOrAddress = envU >> 6;
                                    envU = valueOrAddress + 0xC0;
                                    valueOrAddress = envV >> envVShift;
                                    envV = valueOrAddress + 0xC0;
                                    RR_GTE_OP(0x4A486012);
                                    valueOrAddress = ((u32)envV << 8) | envU;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.uv2 = valueOrAddress;

                                    RR_GTE_MFC2_IR1_IR2(envU, envV);
                                    valueOrAddress = envU >> 6;
                                    envU = valueOrAddress + 0xC0;
                                    valueOrAddress = envV >> envVShift;
                                    envV = valueOrAddress + 0xC0;
                                    valueOrAddress = ((u32)envV << 8) | envU;
                                    ((GpuEnvMapEmission *)packetCursor)->reflected38.body08.uv3 = valueOrAddress;
                                    packetCursor += sizeof(GpuEnvMapEmission);
                                }
                            }
                        }
                    }
                }
            }
            quad++;
        } while (remaining-- > 0);
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packetCursor;
    return (const u32 *)quad;
}

void SetModelTexturePage(s32 arg0) {
    ModelTexturePage = arg0;
}

int DrawRroModel(const Vector *position, MatrixWords32 *rotation, s32 object, s32 flags)
{
    RroRenderWork *work = &((RroModelScratch *)SCRATCHPAD)->work7C.render;
    const u32 *commands;
    RroSceneScratchPrefix *scratch;
    s32 command;
    u32 clutY, texturePage;
    s32 count;
    /* ASM_BARRIER: pin a to $5 to match original register allocation */
    register u32 textureX __asm__("$5");
    unsigned long textureFormat;
    u32 textureY;

    if (ModelTexturePage < 0) {
        return 0;
    }
    SetGteObjectTransform(&((RroModelScratch *)SCRATCHPAD)->work7C.transform,
                          (const s16 *)position, rotation->w);
    textureFormat = 3;
    work->flags08 = flags;
    textureX = ModelTexturePage & 0xF;
    textureX <<= 6;
    textureY = ModelTexturePage & 0x10;
    textureY <<= 4;
    work->envClut0C = (((textureY + 0x7F) << 6) | ((textureX >> 4) | textureFormat)) << 16;
    clutY = textureY >> 4;
    texturePage = (textureX >> 6) | 0x20;
    work->envTpage10 = (clutY | texturePage) << 16;
    scratch = (RroSceneScratchPrefix *)SCRATCHPAD;
    work->vertices00 = scratch->vertices54;
    work->normals04 = scratch->normals58;
    work->ordering14 = scratch->view.projection.pointers.order + 0x80;
    commands = scratch->commands50[object].pointer;
    while ((command = *commands++) != 0) {
        count = command >> 16;
        command &= 0xFFFF;
        switch (command) {
        case 0:
            commands += count * 4;
            break;
        case 1:
            commands += count * 6;
            break;
        case 2:
            commands = EmitRroLitGouraudQuads(commands, work, count);
            break;
        case 3:
            RR_GTE_MTC2_RGB(0x3C808080);
            commands = EmitRroEnvMapQuads(commands, work, count);
            break;
        default:
            return DebugPrintStub(UnknownRroTypeFmt, command, count);
        }
    }
    return 1;
}

void DrawSparkLines(RroSpark *sparks, RroEffectWork *inputWork, s32 speed) {
    struct {
        u32 *ordering;
        s32 depthShift;
        u32 drawMode;
        s32 spawnGate;
        union {s32 word; u16 halves[2];} launchZ;
        Vec3s8 *input;
    } state;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register long v0 __asm__("$2");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register long v1 __asm__("$3");
    s32 a0;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 a1 __asm__("$5");
    s32 a2;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register long a3 __asm__("$7");
    /* ASM_BARRIER: PsyQ-style register allocation */
    register s32 s0 __asm__("$16");
    s32 s1;
    /* ASM_BARRIER: PsyQ-style register allocation */
    register char *motionCursor __asm__("$18");
    s32 life;
    char *packetCursor;
    RroEffectWork *work;
    char *payloadCursor;
    RroSpark *spark;
    s32 fp;

    spark = sparks;
    work = inputWork;
    s0 = speed;
    /* Address temporaries retain native width; their shared scalar/pointer
     * lifetimes remain reconstruction debt. Only DMA links encode addresses. */
    RR_GTE_SETFC_BYTES3(&work->colors15C.palette.rgb160[5]);

    a3 = 0x42000000;
    RR_GTE_MTC2_RGB(a3);

    a3 = 0xE1000220;
    fp = 0x10;
    v0 = (long)SCRATCHPAD;
    state.drawMode = a3;
    packetCursor = (char *)((RroSceneScratchPrefix *)v0)->view.projection.pointers.cursor;
    v1 = (long)((RroSceneScratchPrefix *)v0)->view.projection.pointers.order;
    v0 = ((RroSceneScratchPrefix *)v0)->depthShift60;
    v1 += 0x1B0;
    v0 += 2;
    state.ordering = (u32 *)v1;
    state.depthShift = v0;
    v0 = PsyqRand() & 3;
    s0 = (0U - (0U + s0 + 0xA0));
    a3 = (long)(&work->verticesE0[0]);
    motionCursor = (char *)&spark->motion.fields.z;
    payloadCursor = packetCursor + 4;
    state.spawnGate = v0;
    state.launchZ.word = s0;
    state.input = (Vec3s8 *)a3;
    /* ASM_BARRIER: retain spawn gate, launch velocity and input initialization before the loop. */
    __asm__ ("" : "+r"(work) : : "memory");

    do {
        v0 = spark->timer;
        life = (v0 - 0xAAU);
        if (life <= 0) {
            a3 = state.spawnGate;
            if (a3 != 0) {
                goto L_next;
            }
            /* ASM_BARRIER: prevent a3 = 1 from filling bnez delay slot */
            __asm__ volatile("" : "+r"(a3));
            a3 = 1;
            state.spawnGate = a3;
            life = 0x1000;
            v0 = PsyqRand() & 0x1FF;
            s0 = v0 - 0x100;
            v0 = PsyqRand() & 0x1FF;
            s1 = v0 - 0x100;
            if (s0 == 0) {
                s0 = 0x100;
            }
            if (s1 == 0) {
                s1 = 0x100;
            }
            v0 = s1 + 0x140;
            ((RroSpark *)(motionCursor - 8))->motion.fields.x = s0;
            ((RroSpark *)(motionCursor - 8))->motion.fields.y = v0;
            v0 = (PsyqRand() & 0x7F) - 0xC30;
            ((RroSpark *)(motionCursor - 8))->motion.fields.z = v0;
            v0 = s0 / 2;
            ((RroSpark *)(motionCursor - 8))->motion.fields.velocityX = v0;
            v0 = s1 / 2;
            ((RroSpark *)(motionCursor - 8))->motion.fields.velocityY = v0;
            a3 = state.launchZ.halves[0];
            ((RroSpark *)(motionCursor - 8))->motion.fields.velocityZ = a3;
        }
        spark->timer = life;
        RR_GTE_MTC2_IR0(life);
        RR_GTE14_NOP2();
        RR_GTE_OP(0x4A780010);
        RR_GTE14_SWC2_RGB2(&((RroSparkPacket *)packetCursor)->color08);

        s0 = ((RroSpark *)(motionCursor - 8))->motion.words[0];
        a2 = ((RroSpark *)(motionCursor - 8))->motion.words[1];
        a0 = ((RroSpark *)(motionCursor - 8))->motion.words[2];
        s1 = s0 >> 16;
        v0 = (s32)(s0 * 0x10000U);
        s0 = v0 >> 16;
        a1 = a2 >> 16;
        v1 = (s32)(a2 * 0x10000U);
        a2 = v1 >> 16;
        v0 >>= 20;
        *(s16 *)(&work->verticesE0[0]) = v0;
        v0 = s1 >> 4;
        v1 >>= 20;
        work->verticesE0[0].fields.z = v1;
        v1 = a0 >> 16;
        a0 = (a0 * 0x10000U);
        work->verticesE0[0].fields.y = v0;

        a3 = (long)state.input;
        RR_GTE14_LWC2_V1(a3);

        v0 = (s0 + (a0 >> 19)) >> 4;
        *(s16 *)(&work->verticesE0[0]) = v0;
        v0 = (s1 + (v1 >> 3)) >> 4;
        work->verticesE0[0].fields.y = v0;
        v0 = (a2 + (a1 >> 3)) >> 4;
        work->verticesE0[0].fields.z = v0;

        RR_GTE_RTPT_LDV2(a3);

        a0 = s0 + (a0 >> 20);
        v1 = s1 + (v1 >> 4);
        a1 = a2 + (a1 >> 4);
        v0 = (long)(&work->depth70);
        ((RroSpark *)(motionCursor - 8))->motion.fields.x = a0;
        ((RroSpark *)(motionCursor - 8))->motion.fields.y = v1;
        ((RroSpark *)(motionCursor - 8))->motion.fields.z = a1;
        RR_GTE14_SWC2_SZ3(v0);

        v0 = work->depth70;
        if (v0 > 0) {
            a3 = state.depthShift;
            v1 = v0 >> a3;
            if (v1 < 0x1C0) {
                RR_GTE14_SWC2_SXY1(&((RroSparkPacket *)packetCursor)->firstXY0C);
                RR_GTE14_SWC2_SXY2(&((RroSparkPacket *)packetCursor)->secondXY10);

                a3 = (long)state.ordering;

                v1 <<= 2;
                v1 += a3;
                v0 = *(s32 *)v1;
                ((RroSparkPacket *)packetCursor)->tag.word = v0;
                ((RroSparkPacket *)(payloadCursor - 4))->tag.bytes.length = 4;
                *(u32 *)v1 = GpuDmaEncodeAddress(packetCursor);
                ((u8 *)v1)[3] = 0;

                a3 = state.drawMode;
                /* ASM_BARRIER: order state.drawMode load before packetCursor increment in its load delay slot */
                __asm__ ("" : "+r"(a3), "+r"(packetCursor));
                packetCursor += sizeof(RroSparkPacket);
                ((RroSparkPacket *)(payloadCursor - 4))->drawMode04 = a3;
                payloadCursor += sizeof(RroSparkPacket);
            }
        }
L_next:
        motionCursor += sizeof(RroSpark);
        fp--;
        spark++;
    } while (fp > 0);

    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packetCursor;
}

void EmitCarShadowQuads(const RroEffectMesh *mesh, RroEffectWork *inputWork) {
    RroEffectWork *work = inputWork;
    char *a0;
    const RroTexturedQuadPrefix *quad;
    RroSceneScratchPrefix *scratch;
    const u32 *commands;
    const void *firstVertex, *secondVertex, *thirdVertex;
    s32 v0;
    long v1;
    /* ASM_BARRIER: keep a1 in $5 */
    register s32 a1 __asm__("$5");
    s32 a2;
    s32 secondArea;
    unsigned long depthAddress, orderingAddress;

    commands = mesh->commands;
    a0 = (char *)mesh->vertices;
    quad = (const RroTexturedQuadPrefix *)(commands + 1);
    a1 = ((const RroTexturedQuadPrefix *)(commands + 1))->vertices.indices[0];
    v1 = quad->vertices.indices[1];
    v0 = quad->vertices.indices[2];
    firstVertex = a0 + (a1 << 3);
    secondVertex = a0 + (v1 << 3);
    thirdVertex = a0 + (v0 << 3);
    RR_GTE_RTPT_LDV(firstVertex, secondVertex, thirdVertex);
    RR_GTE14_NOP2();
    RR_GTE14_NCLIP();
    RR_GTE14_SWC2_SXYZ(&work->projection188.shadow.xy[0]);
    RR_GTE14_SWC2_MAC0(&work->firstArea74);

    v0 = quad->vertices.indices[3];
    v0 <<= 3;
    a0 += v0;
    RR_GTE_RTPS_LDV(a0);
    RR_GTE14_AVSZ4();
    RR_GTE14_NCLIP();
    RR_GTE14_SWC2_SXY2(&work->projection188.shadow.xy[3]);
    RR_GTE14_SWC2_MAC0(&work->secondArea78);
    RR_GTE14_SWC2_OTZ(&work->depth70);

    v1 = work->depth70;
    scratch = (RroSceneScratchPrefix *)SCRATCHPAD;
    if (v1 <= 0) {
        return;
    }
    v0 = scratch->depthShift60;
    a1 = v1 >> v0;
    work->depth70 = a1;
    if (a1 >= 0x1C0) {
        return;
    }

    v0 = work->firstArea74;
    v1 = work->windingBC;
    secondArea = work->secondArea78;
    v0 ^= v1;
    secondArea ^= v1;
    if (v0 <= 0 && secondArea >= 0) return;

    /* This legacy address calculation retains the full native pointer width.
     * An ordinary pointer spelling still changes instruction selection.
     * Only the DMA tag write narrows an address to an encoded hardware word. */
    v0 = a1 << 2;
    v1 = quad->orderingBias19;
    a0 = (char *)scratch->view.projection.pointers.order;
    v1 *= 4;
    depthAddress = v0 + (unsigned long)a0;
    v1 = (v1 + depthAddress);
    a0 = (char *)scratch->view.projection.pointers.cursor;
    v0 = ((s32 *)v1)[0x200 / 4];
    ((GpuPolyFT4 *)a0)->words.tag.word = v0;
    ((GpuPolyFT4 *)a0)->words.tag.bytes.length = 9;

    v1 = quad->orderingBias19;
    v0 = work->depth70;
    orderingAddress = (unsigned long)scratch->view.projection.pointers.order;
    v1 *= 4;
    v0 <<= 2;
    depthAddress = v0 + orderingAddress;
    v1 = (v1 + depthAddress);
    ((u32 *)v1)[0x200 / 4] = GpuDmaEncodeAddress(a0);

    v1 = quad->orderingBias19;
    v0 = work->depth70;
    orderingAddress = (unsigned long)scratch->view.projection.pointers.order;
    v1 *= 4;
    v0 <<= 2;
    depthAddress = v0 + orderingAddress;
    v1 = (v1 + depthAddress);
    ((u8 *)v1)[0x203] = 0;

    a1 = work->projection188.shadow.xy[0];
    a2 = work->projection188.shadow.xy[1];
    v1 = work->projection188.shadow.xy[2];
    v0 = work->projection188.shadow.xy[3];
    ((GpuPolyFT4 *)a0)->words.xy0 = a1;
    ((GpuPolyFT4 *)a0)->words.xy1 = a2;
    ((GpuPolyFT4 *)a0)->words.xy2 = v1;
    ((GpuPolyFT4 *)a0)->words.xy3 = v0;

    v0 = work->colors15C.palette.rgb160[4];
    ((GpuPolyFT4 *)a0)->words.color04 = v0;
    ((GpuPolyFT4 *)a0)->fields.code = 0x2E;

    a1 = quad->uv0C;
    v1 = quad->uv14;
    a2 = quad->uv10;
    v0 = v1 >> 16;
    ((GpuPolyFT4 *)a0)->words.uv0 = a1;
    ((GpuPolyFT4 *)a0)->words.uv1 = a2;
    ((GpuPolyFT4 *)a0)->words.uv2 = v1;
    ((GpuPolyFT4 *)a0)->words.uv3 = v0;
    a0 += sizeof(GpuPolyFT4);
    scratch->view.projection.pointers.cursor = (u8 *)a0;
}

void EmitCarBodyQuads(const RroEffectMesh *inputMesh, RroEffectWork *inputWork) {
    const RroEffectMesh *mesh = inputMesh;
    s32 remainingVertices;
    const Vec3s8 *a2 = (const Vec3s8 *)mesh->vertices;
    /* ASM_BARRIER: keep work context in $t0 as in original GTE routine */
    register RroEffectWork *work __asm__("$8") = inputWork;
    char *t1;
    const Vec3s8 *staged;
    const u32 *commands;
    RroSceneScratchPrefix *scene;
    const RroBodyQuad *t3;
    s32 t2;
    s32 t5;
    s32 t6;
    char *t4;

    {
        s32 w0 = a2[0].signedWords[0], w1 = a2[0].signedWords[1];
        t1 = (char *)&work->projection188.pipeline;
        work->verticesE0[0].signedWords[0] = w0;
        work->verticesE0[0].signedWords[1] = w1;
        w0 = a2[1].signedWords[0]; w1 = a2[1].signedWords[1];
        staged = &work->verticesE0[0];
        work->verticesE0[1].signedWords[0] = w0;
        work->verticesE0[1].signedWords[1] = w1;
        w0 = a2[2].signedWords[0]; w1 = a2[2].signedWords[1];
        work->verticesE0[2].signedWords[0] = w0;
        work->verticesE0[2].signedWords[1] = w1;
    }

    remainingVertices = mesh->vertexCount;

    do {
        s32 w0, w1;
        RR_GTE14_LWC2_V012(staged);
        a2 += 3;
        remainingVertices -= 3;
        t1 += 0x18;
        RR_GTE14_NOP2();
        RR_GTE14_RTPT();

        w0 = a2[0].signedWords[0]; w1 = a2[0].signedWords[1];
        work->verticesE0[0].signedWords[0] = w0;
        work->verticesE0[0].signedWords[1] = w1;

        w0 = a2[1].signedWords[0]; w1 = a2[1].signedWords[1];
        work->verticesE0[1].signedWords[0] = w0;
        work->verticesE0[1].signedWords[1] = w1;

        w0 = a2[2].signedWords[0]; w1 = a2[2].signedWords[1];
        work->verticesE0[2].signedWords[0] = w0;
        work->verticesE0[2].signedWords[1] = w1;

        GTE_STORE_SXY_SZ(t1);
    } while (remainingVertices > 0);

    t6 = 0xE1000220;

    commands = mesh->commands;
    scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    t3 = (const RroBodyQuad *)(commands + 1);
    t2 = ((const RroBodyCommandHeader *)commands)->fields.count;
    t5 = scene->depthShift60 + 4;
    if (work->facing98.z > 0) {
        t4 = (char *)scene->view.projection.pointers.order + 0x1C4;
    } else {
        t4 = (char *)scene->view.projection.pointers.order + 0x1B0;
    }

    t1 = (char *)SCRATCHPAD;
    t1 = (char *)((RroSceneScratchPrefix *)t1)->view.projection.pointers.cursor;

    while (t2-- > 0) {
        /* ASM_BARRIER: keep scratch vertex pointer/word in $v0 */
        register long v0 __asm__("$2");
        long v1 = t3->vertices[0];
        s32 thirdIndex;
        /* ASM_BARRIER: keep third vertex pointer in $a1 */
        register char *a1 __asm__("$5");
        s32 t7, t8, t9, depthPart;
        const u8 *p3;

        v0 = t3->vertices[1];
        thirdIndex = t3->vertices[2];
        v1 = (long)((char *)work + (v1 << 3));
        v0 = (long)((char *)work + (v0 << 3));
        a1 = (char *)work + thirdIndex * sizeof(RroProjectedVertex);
        t7 = *(s32 *)((char *)v1 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sxy));
        t8 = *(s32 *)((char *)v0 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sxy));
        t9 = *(const s32 *)(a1 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sxy));
        RR_GTE_NCLIP_SXY3(t7, t8, t9);

        v1 = *(s32 *)((char *)v1 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sz));
        depthPart = *(s32 *)((char *)v0 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sz));
        v0 = t3->vertices[3];
        v1 = ((u32)v1 + depthPart);
        p3 = (const u8 *)work + v0 * sizeof(RroProjectedVertex);
        depthPart = *(const s32 *)(a1 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sz));
        v0 = *(const s32 *)(p3 + RR_FIELD_OFFSET(RroEffectWork, projection188.mesh.vertices[0].sz));
        v1 = ((u32)v1 + depthPart);
        v1 = ((u32)v1 + v0);
        if (v1 > 0) {
            v1 >>= t5;
            if (v1 < 0x1C0) {
                RR_GTE14_SWC2_MAC0(&work->firstArea74);
                v0 = work->firstArea74;
                if (v0 != 0) {
                    v1 <<= 2;
                    v1 = (long)(v1 + t4);
                    ((RroBodyPacket *)t1)->tag.word = *(s32 *)v1;
                    ((RroBodyPacket *)t1)->tag.bytes.length = 9;
                    *(u32 *)v1 = GpuDmaEncodeAddress(t1);
                    ((u8 *)v1)[3] = 0;

                    ((RroBodyPacket *)t1)->vertices[0].color = work->colors15C.palette.rgb160[t3->colors[0]];
                    ((RroBodyPacket *)t1)->vertices[1].color = work->colors15C.palette.rgb160[t3->colors[1]];
                    ((RroBodyPacket *)t1)->vertices[2].color = work->colors15C.palette.rgb160[t3->colors[2]];
                    ((RroBodyPacket *)t1)->vertices[3].color = work->colors15C.palette.rgb160[t3->colors[3]];

                    ((RroBodyPacket *)t1)->vertices[0].xy = work->projection188.mesh.vertices[t3->vertices[0]].sxy;
                    ((RroBodyPacket *)t1)->vertices[1].xy = work->projection188.mesh.vertices[t3->vertices[1]].sxy;
                    ((RroBodyPacket *)t1)->vertices[2].xy = work->projection188.mesh.vertices[t3->vertices[2]].sxy;
                    ((RroBodyPacket *)t1)->vertices[3].xy = work->projection188.mesh.vertices[t3->vertices[3]].sxy;
                    ((RroBodyPacket *)t1)->drawMode04 = t6;
                    t1 += sizeof(RroBodyPacket);
                }
            }
        }
        t3++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)t1;
}
void TransformRroVertices(CarSlotBufferC8 *slot, RroEffectWork *work)
{
    s32 intensity;
    u32 *input;
    u8 (*farColor)[4];
    s32 *output;
    s32 remaining, speed, frame;

    RR_GTE_SETTR(work->object00.fields.translation14);
    if (RacePaused != 0) {
        intensity = slot->intensity64;
    } else {
        intensity = (work->vehicle90->input - 0x3E0) * 2;
        intensity = (intensity < 0 ? 0 : intensity) > 0x1000 ? 0x1000 : (intensity < 0 ? 0 : intensity);
        intensity += PsyqRand() & 0xF;
        slot->intensity64 = intensity;
    }
    RR_GTE_MTC2_IR0(intensity);
    input = D_8009E0B0.input;
    farColor = D_8009E0B0.farColor;
    output = work->colors15C.words;
    remaining = 6;
    do {
        RR_GTE_LWC2_RGB(input);
        RR_GTE_DPCS_FC4(farColor);
        input++;
        farColor++;
        output++;
        RR_GTE_SWC2_RGB2(output);
        remaining--;
    } while (remaining > 0);
    speed = work->vehicle90->input;
    DrawSparkLines(slot->sparksC4, work, speed / 4);
    frame = ScreenFrame & 3;
    EmitCarBodyQuads(&slot->model->body64[frame], work);
    {
        s32 jitter = (PsyqRand() & 3) - 1;
        work->jitterTranslationAC.x = ((u32)work->object00.fields.translation14[0] + jitter);
    }
    {
        s32 jitter = (PsyqRand() & 3) - 1;
        work->jitterTranslationAC.y = ((u32)work->object00.fields.translation14[1] + jitter);
    }
    work->jitterTranslationAC.z = work->object00.fields.translation14[2];
    RR_GTE_SETTR(&work->jitterTranslationAC);
    EmitCarShadowQuads(&slot->model->shadow58, work);
}

void DrawGlowFan(CarSlotBufferC8 *slot, RroEffectWork *work) {
    int depth;
    int *ring;
    int *projected;
    int remaining;
    u32 color;
    int centerXY;
    RroGlowPacket *packet;
    u32 *head;
    RroSceneScratchPrefix *scratch;
    int drawMode;
    int triangleCommand;

    RR_GTE_SETTR(work->object00.fields.translation14);
    work->inputC8.fields.x = 0;
    work->inputC8.fields.y = -0x5C;
    work->inputC8.fields.z = 0x98;
    RR_GTE_RTPS_LDV(&work->inputC8);
    RR_GTE_SWC2_SXY2(work->projection188.fan.xy + 2);
    RR_GTE_TR_FROM_IR();
    RR_GTE_READ_IR3_NOP(depth);
    if (depth >= 0) {
        depth >>= ((RroSceneScratchPrefix *)SCRATCHPAD)->depthShift60 + 2;
        if (depth < 0x1C0) {
            RR_GTE_SETROT_IDENTITY_CLEAR_Z();
            ring = D_8009E0E0;
            projected = work->projection188.fan.xy;
            remaining = 0x11;
            do {
                RR_GTE_RTPT_PACKED_XY(ring);
                ring += 3;
                projected += 3;
                RR_GTE_SWC2_SXYZ(projected);
                remaining -= 3;
            } while (remaining > 0);
            drawMode = 0xE1000220;
            projected = work->projection188.fan.xy + 3;
            remaining = 0x10;
            color = slot->color08;
            scratch = (RroSceneScratchPrefix *)SCRATCHPAD;
            triangleCommand = 0x32000000;
            centerXY = work->projection188.fan.xy[2];
            packet = (RroGlowPacket *)scratch->view.projection.pointers.cursor;
            color = (color << 8) | (color | triangleCommand);
            /* Legacy address ordering: full pointer width, pending an ordinary
             * pointer spelling that preserves the relocated match. */
            head = (u32 *)((depth << 2) + (unsigned long)scratch->view.projection.pointers.order + 0x1A4);
            do {
                packet->centerXY0C = centerXY;
                packet->firstXY14 = projected[0];
                packet->secondXY1C = projected[1];
                remaining--;
                packet->centerColor08 = color;
                packet->secondColor18 = 0;
                packet->firstColor10 = 0;
                packet->drawMode04 = drawMode;
                projected++;
                packet->tag.word = *head;
                packet->tag.bytes.length = 7;
                *head = GpuDmaEncodeAddress(packet);
                packet++;
                ((char *)head)[3] = 0;
            } while (remaining > 0);
            ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
        }
    }
}

const RroTexturedQuadPrefix *EmitProjectedQuadsFT4(const RroTexturedQuadPrefix *quad, s32 count, RroEffectWork *work) {
    const RroProjectedVertex *vertices = &work->projection188.pipeline[3];
    /* Existing volatile scratch reads preserve their original observation order. */
    volatile RroSceneScratchPrefix *scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    s32 shift = scene->depthShift60 + 4;
    GpuPolyFT4 *packet = (GpuPolyFT4 *)scene->view.projection.pointers.cursor;
    s32 winding = work->windingBC;

    while (count-- > 0) {
        s32 firstIndices = quad->vertices.words[0];
        s32 secondIndices = quad->vertices.words[1];
        s32 i1 = firstIndices >> 16;
        s32 i0 = firstIndices & 0xFFFF;
        s32 i3 = secondIndices >> 16;
        s32 i2 = secondIndices & 0xFFFF;
        s32 xy0 = vertices[i0].sxy;
        s32 xy1 = vertices[i1].sxy;
        s32 xy2 = vertices[i2].sxy;
        s32 xy3 = vertices[i3].sxy;
        s32 depth;

        RR_GTE_NCLIP_SXY3(xy0, xy1, xy2);
        packet->words.xy0 = xy0;
        packet->words.xy1 = xy1;
        packet->words.xy2 = xy2;
        packet->words.xy3 = xy3;
        depth = (u32)vertices[i0].sz + vertices[i1].sz + vertices[i2].sz + vertices[i3].sz;
        RR_GTE14_SWC2_MAC0(&work->firstArea74);
        if ((work->firstArea74 ^ winding) > 0 && depth > 0) {
            depth >>= shift;
            if (depth < 0x1C0) {
                u32 *head = work->ordering6C + depth + quad->orderingBias19;
                packet->words.tag.word = *head;
                packet->words.tag.bytes.length = 9;
                *head = GpuDmaEncodeAddress(packet);
                ((GpuDmaTag *)head)->bytes.length = 0;
                packet->words.uv0 = quad->uv0C;
                packet->words.uv1 = quad->uv10;
                packet->words.uv2 = quad->uv14;
                packet->words.uv3 = quad->uv14 >> 16;
                packet->words.color04 = work->colors15C.palette.rgb160[0];
                packet++;
            }
        }
        quad++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
    return quad;
}

void EmitProjectedLitTrisGT3(const RroTrailPanelCommand *command, s32 count, RroEffectWork *work) {
    GpuPolyG4 *packet = (GpuPolyG4 *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
    s32 primaryColor = work->colors15C.palette.rgb160[0] | 0x3A000000;
    s32 shift = ((RroSceneScratchPrefix *)SCRATCHPAD)->depthShift60 + 3;
    const RroProjectedVertex *base = work->projectedD0;
    const RroProjectedVertex *alternate = work->projectedD4;
    s32 secondaryColor = work->colors15C.palette.rgb160[1];
    s32 winding = work->windingBC;
    u32 drawMode = 0xE1000220;

    while (count-- > 0) {
        s32 sxy0 = base[command->outer[0]].sxy;
        s32 sxy1 = base[command->outer[1]].sxy;
        s32 sxy2 = base[command->outer[2]].sxy;
        s32 depth;

        RR_GTE_LDSXY3(sxy0, sxy1, sxy2);
        depth = base[command->center].sz >> shift;
        work->depth70 = depth;
        if (depth - 1U < 0x1BFU) {
            s32 x0, x1, x2, x3, x4, x5;
            s32 y0, y1, y2, y3, y4, y5;

            RR_GTE14_NCLIP();
            packet[0].words.xy1 = sxy0;
            packet[1].words.xy1 = sxy1;
            packet[2].words.xy1 = sxy2;
            x0 = (s16)(base[command->outer[0]].sxy);
            x1 = (s16)(base[command->outer[1]].sxy);
            x2 = (s16)(base[command->outer[2]].sxy);
            x3 = (s16)(alternate[command->outer[0]].sxy);
            x4 = (s16)(alternate[command->outer[1]].sxy);
            x5 = (s16)(alternate[command->outer[2]].sxy);
            RR_GTE14_SWC2_MAC0(&work->firstArea74);
            if ((work->firstArea74 ^ winding) > 0) {
                if ((x0 >= 0 || x1 >= 0 || x2 >= 0 || x3 >= 0 || x4 >= 0 || x5 >= 0) &&
                    (x0 < 0x140 || x1 < 0x140 || x2 < 0x140 || x3 < 0x140 || x4 < 0x140 || x5 < 0x140)) {
                    y0 = (s16)(base[command->outer[0]].sxy >> 16);
                    y1 = (s16)(base[command->outer[1]].sxy >> 16);
                    y2 = (s16)(base[command->outer[2]].sxy >> 16);
                    y3 = (s16)(alternate[command->outer[0]].sxy >> 16);
                    y4 = (s16)(alternate[command->outer[1]].sxy >> 16);
                    y5 = (s16)(alternate[command->outer[2]].sxy >> 16);
                    if ((y0 >= 0 || y1 >= 0 || y2 >= 0 || y3 >= 0 || y4 >= 0 || y5 >= 0) &&
                        (y0 < 0x100 || y1 < 0x100 || y2 < 0x100 || y3 < 0x100 || y4 < 0x100 || y5 < 0x100)) {
                        s32 baseCenter, alternateCenter;
                        u32 *head;

                        sxy0 = alternate[command->outer[0]].sxy;
                        sxy1 = alternate[command->outer[1]].sxy;
                        sxy2 = alternate[command->outer[2]].sxy;
                        packet[0].words.xy3 = sxy0;
                        packet[1].words.xy3 = sxy1;
                        packet[2].words.xy3 = sxy2;
                        baseCenter = base[command->center].sxy;
                        packet[2].words.xy0 = baseCenter;
                        packet[1].words.xy0 = baseCenter;
                        packet[0].words.xy0 = baseCenter;
                        alternateCenter = alternate[command->center].sxy;
                        packet[2].words.color04 = primaryColor;
                        packet[1].words.color04 = primaryColor;
                        packet[0].words.color04 = primaryColor;
                        packet[2].words.color14 = secondaryColor;
                        packet[1].words.color14 = secondaryColor;
                        packet[0].words.color14 = secondaryColor;
                        packet[2].words.color1C = 0;
                        packet[1].words.color1C = 0;
                        packet[0].words.color1C = 0;
                        packet[2].words.color0C = 0;
                        packet[1].words.color0C = 0;
                        packet[0].words.color0C = 0;
                        packet[2].words.xy2 = alternateCenter;
                        packet[1].words.xy2 = alternateCenter;
                        packet[0].words.xy2 = alternateCenter;
                        head = work->ordering6C + work->depth70 + command->orderingBias09;
                        packet[0].tag.word = *head;
                        packet[0].tag.bytes.length = 8;
                        *head = GpuDmaEncodeAddress(packet);
                        ((GpuDmaTag *)head)->bytes.length = 0;
                        packet[1].tag.word = *head;
                        packet[1].tag.bytes.length = 8;
                        *head = GpuDmaEncodeAddress(&packet[1]);
                        ((GpuDmaTag *)head)->bytes.length = 0;
                        packet[2].tag.word = *head;
                        packet[2].tag.bytes.length = 8;
                        *head = GpuDmaEncodeAddress(&packet[2]);
                        ((GpuDmaTag *)head)->bytes.length = 0;
                        /* Three G4 panels are followed by one E1 command. */
                        packet += 3;
                        ((GpuCommandPacket *)packet)->command = drawMode;
                        ((GpuCommandPacket *)packet)->tag.word = *head;
                        ((GpuCommandPacket *)packet)->tag.bytes.length = 1;
                        *head = GpuDmaEncodeAddress(packet);
                        packet = (GpuPolyG4 *)((GpuCommandPacket *)packet + 1);
                        ((GpuDmaTag *)head)->bytes.length = 0;
                    }
                }
            }
        }
        command++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
}

const RroGlowPointCommand *EmitProjectedLitQuadsGT4(const RroGlowPointCommand *input, s32 count, RroEffectWork *work) {
    const RroGlowPointCommand *points = input;
    s32 offsetX = (s32)(0U - work->facing98.x) / 64;
    s32 offsetY = (s32)(0U - work->facing98.y) / 64;
    RroGlowProjection *projection = work->projection188.glow.points;
    s32 remaining = count;
    s32 shift = ((RroSceneScratchPrefix *)SCRATCHPAD)->depthShift60 + 2;
    GpuPolyG3 *packet;
    u32 drawMode;

    while (remaining > 0) {
        s32 rawDepth, depth;
        RR_GTE_RTPS_LDV(&points->position);
        RR_GTE14_SWC2_SZ3(&work->depth70);
        rawDepth = work->depth70;
        depth = rawDepth >> shift;
        if (rawDepth <= 0 || depth >= 0x1C0) {
            projection->ordering = 0;
        } else {
            RR_GTE_STORE_IR_MEM(&projection->translation);
            RR_GTE14_SWC2_SXY2(&projection->centerXY);
            projection->translation.x += offsetX;
            projection->translation.y += offsetY;
            projection->ordering = work->ordering6C + depth + points->fields.orderingBias07;
        }
        remaining--;
        projection++;
        points++;
    }
    work->matrix44.glow.rotation48.w[4] = 0x1000;
    work->matrix44.glow.rotation48.w[2] = 0x1000;
    work->matrix44.glow.rotation48.w[0] = 0x1000;
    work->matrix44.glow.rotation48.w[3] = 0;
    work->matrix44.glow.rotation48.w[1] = 0;
    RR_GTE_SETROT(work->matrix44.glow.rotation48.w);

    drawMode = 0xE1000220;
    packet = (GpuPolyG3 *)((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor;
    projection = work->projection188.glow.points;
    while (count > 0) {
        u32 *head = projection->ordering;
        if (head) {
            const Vec3s8 *ring;
            s32 center;
            /* ASM_BARRIER: retained projected edge binding from the previous reconstruction. */
            register s32 previous __asm__("$15");
            s32 color;
            RR_GTE_SETTR(&projection->translation);
            ring = D_8009E124;
            center = projection->centerXY;
            RR_GTE_RTPS_LDV(ring);
            RR_GTE_MFC2(previous, 14);
            remaining = 8;
            color = work->colors15C.palette.rgb160[0];
            do {
                ring++;
                RR_GTE_RTPS_LDV(ring);
                packet->xy0 = center;
                packet->xy1 = previous;
                packet->color0 = color;
                packet->color2 = 0;
                packet->color1 = 0;
                packet->tag.word = *head;
                packet->tag.bytes.length = 6;
                *head = GpuDmaEncodeAddress(packet);
                ((GpuDmaTag *)head)->bytes.length = 0;
                RR_GTE_MFC2_SXY2_NOP(previous);
                packet->xy2 = previous;
                packet++;
                remaining--;
            } while (remaining > 0);
            ((GpuCommandPacket *)packet)->command = drawMode;
            ((GpuCommandPacket *)packet)->tag.word = *head;
            ((GpuCommandPacket *)packet)->tag.bytes.length = 1;
            *head = GpuDmaEncodeAddress(packet);
            packet = (GpuPolyG3 *)((GpuCommandPacket *)packet + 1);
            ((GpuDmaTag *)head)->bytes.length = 0;
        }
        count--;
        projection++;
    }
    ((RroSceneScratchPrefix *)SCRATCHPAD)->view.projection.pointers.cursor = (u8 *)packet;
    return points;
}

void DrawWheelTrailEffects(void *arg0, s32 *arg1, void *arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6) {
    char *s2 = (char *)(SCRATCHPAD + 0x7C);
    char *s4 = (char *)&CarSlotBuffers[arg4 * 2];
    s32 s0;
    s32 a0;
    TrailEntry8 *a2;
    char *vdst;
    const u32 *p;
    char *vsrc;
    s32 vcnt;

    WORK->flags8C = arg5;
    if (((s32 *)SCRATCHPAD)[0x64 / 4] != 0) {
        s4 += sizeof(CarSlotBufferC8);
        WORK->windingBC = 0x80000000;
    } else {
        WORK->windingBC = 0;
    }
    WORK->vehicle90 = arg0;
    WORK->ordering6C = (u32 *)(((s32 *)SCRATCHPAD)[1] + 0x200 + arg6 * 4);
    WORK->bounds94 = CarBoundsTable[arg4 / 2];

    if (arg1 == 0 || (WORK->flags8C & 1) == 0) {
        SLOT->mainCount0C = 0;
        SLOT->secondCount34 = 0;
        SLOT->historyCount68 = 0;
        SLOT->phase5C = 0;
    }

    if (WORK->bounds94->wheelRotationAxis == 3) {
        if (RacePaused == 0) {
            s0 = SLOT->intensity60;
            if (WORK->flags8C & 1) {
                a0 = (WORK->vehicle90->input - 0x358) / 4;
                if (s0 < a0) {
                    s0 = a0;
                } else if (a0 < s0) {
                    s0 -= 4;
                }
            } else if (s0 != 0) {
                s0 -= 4;
            }
            s0 = (s0 < 0 ? 0 : s0) > 0xF0 ? 0xF0 : (s0 < 0 ? 0 : s0);
            SLOT->intensity60 = s0;
            if (WORK->flags8C & 1) {
                s0 += PsyqRand() & 0xF;
            }
            SLOT->color08 = s0;
        }
        if (SLOT->color08 <= 0) {
            return;
        }
    } else {
        s0 = 0x80;
        if ((WORK->flags8C & 1) == 0) {
            s0 = 0;
            a0 = 0x80;
            SLOT->mainCount0C = 0;
            SLOT->secondCount34 = 0;
            SLOT->historyCount68 = 0;
            SLOT->phase5C = 0;
        } else {
            a0 = 0xF0;
        }
        if (RacePaused == 0) {
            s32 v1;
            s32 d = a0 - s0;
            if (WORK->flags8C & 2) {
                v1 = SLOT->intensity60 + d / 4;
                if (v1 < a0) {
                    SLOT->intensity60 = v1;
                } else {
                    SLOT->intensity60 = a0;
                }
                v1 = SLOT->intensity64 + a0 / 4;
                if (v1 < a0) {
                    SLOT->intensity64 = v1;
                } else {
                    SLOT->intensity64 = a0;
                }
            } else {
                v1 = SLOT->intensity60 - d / 8;
                if (v1 > s0) {
                    SLOT->intensity60 = v1;
                } else {
                    SLOT->intensity60 = s0;
                }
                v1 = SLOT->intensity64 - a0 / 8;
                if (v1 > 0) {
                    SLOT->intensity64 = v1;
                } else {
                    SLOT->intensity64 = 0;
                }
            }
            KEEP_LIVE2(a0, d);
        }
    }

    if (arg1 == 0) {
        return;
    }

    WORK->worldPosition38.x = arg1[0] - 0x8000;
    WORK->worldPosition38.y = arg1[1];
    WORK->worldPosition38.z = arg1[2] - 0x8000;
    {
        s32 *g = (s32 *)SCRATCHPAD;
        WORK->inputC8.fields.x = arg1[0] - g[2];
        WORK->inputC8.fields.y = arg1[1] - g[3];
        WORK->inputC8.fields.z = arg1[2] - g[4];
    }
    RR_GTE_SETROT((void *)SCRATCH_ROT_MATRIX);
    RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
    RR_GTE_SWC2_MAC123(s2 + 0x14);
    GTE_SETROT_AND_SAVE(arg2, s2);

    WORK->inputC8.fields.y = 0;
    WORK->inputC8.fields.x = 0;
    WORK->inputC8.fields.z = -0x1000;

    RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
    RR_GTE_SWC2_MAC123(s2 + 0x98);

    if (WORK->bounds94->wheelRotationAxis == 3 && GameModeType == 0) {
        DrawGlowFan(SLOT, WORK);
        return;
    }

    if (SLOT->overlay70 != 0 && (WORK->flags8C & 3) == 3) {
        if (SLOT->phase5C == 0) {
            SLOT->storedPosition74.x = WORK->worldPosition38.x;
            SLOT->storedPosition74.y = WORK->worldPosition38.y;
            SLOT->storedPosition74.z = WORK->worldPosition38.z;
            SLOT->phase5C = 1;
        }
    } else if (SLOT->phase5C == 1) {
        SLOT->phase5C = 2;
    }

    if (WORK->bounds94->wheelRotationAxis == 2 && (GameModeType == 6 || GameModeType == 0)) {
        if (ActiveViewportIndex != 6) {
            TransformRroVertices(SLOT, WORK);
        }
    }

    RR_GTE_SETFC(0, 0, 0);
    GTE_CTC2_28(0);

    if (WORK->flags8C & 1) {
        s32 v1;
        RR_GTE_LWC2_RGB((char *)(WORK->bounds94 + 1));
        v1 = WORK->facing98.z;
        if (v1 >= -0xEC8) {
            char *a0;
            char *a1;
            if (RacePaused != 0) {
                WORK->blendC0 = SLOT->color08;
            } else {
                s32 a0_val;
                s32 v0_val;
                WORK->temporaryC4 = v1;
                if (v1 < 0) {
                    WORK->temporaryC4 = 0;
                } else if (v1 >= 0x1000) {
                    WORK->temporaryC4 = 0xFFF;
                }
                v0_val = PsyqRand();
                v1 = (WORK->facing98.z + 0xEC8) / 2;
                a0_val = v0_val & WORK->temporaryC4;
                WORK->blendC0 = a0_val;
                if (v1 > 0x200) {
                    v1 = 0x200;
                }
                v0_val = (a0_val >> 2) + v1;
                WORK->blendC0 = v0_val;
                if (v0_val > 0xE00) {
                    WORK->blendC0 = 0xE00;
                }
                v0_val = 0x1000 - WORK->blendC0;
                WORK->blendC0 = v0_val;
                SLOT->color08 = v0_val;
            }
            RR_GTE_SETTR(s2 + 0x14);
            WORK->vertices7C = SLOT->model->primary10.vertices;
            a1 = ((char * volatile *)s2)[0x7C / 4];
            WORK->vertexCount88 = SLOT->model->primary10.vertexCount;
            v1 = ((volatile s32 *)s2)[0x88 / 4];
            WORK->commands84 = SLOT->model->primary10.commands;
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE_SWC2_IR0(s2 + 0xDC);
                RR_GTE14_LWC2_V012(a1);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                a1 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
            if (RacePaused == 0) {
                s32 idx = SLOT->mainIndex10;
                s32 n = SLOT->mainCount0C;
                s32 off = (idx << 3) + 0x14;
                TrailEntry4 *a2 = (TrailEntry4 *)(s4 + off);
                s32 rem = 4 - idx;
                RroProjectedVertex *dst;
                RroProjectedVertex *src;
                if (n > 0) {
                    do {
                        a2->unk0 += a2->unk0 / 2;
                        if (a2->unk0 > 0x1000) {
                            a2->unk0 = 0x1000;
                        }
                        rem--;
                        a2++;
                        if (rem <= 0) {
                            a2 = (TrailEntry4 *)(s4 + 0x14);
                            rem = 4;
                        }
                        n--;
                    } while (n > 0);
                }
                if (SLOT->mainCount0C < 4) {
                    SLOT->mainCount0C += 1;
                }
                SLOT->mainIndex10 = (SLOT->mainIndex10 + 3) % 4;
                a2 = (TrailEntry4 *)(s4 + ((SLOT->mainIndex10 << 3) + 0x14));
                v1 = 0x1000 - WORK->temporaryC4;
                if (v1 >= 0x200) {
                    a2->unk0 = v1;
                } else {
                    a2->unk0 = 0x200;
                }
                a2->unk2 = (u16)WORK->interpolationDC;
                n = WORK->vertexCount88;
                dst = a2->verts;
                src = (RroProjectedVertex *)(s2 + 0x1A0);
                while (n > 0) {
                    s32 x = src->sxy;
                    s32 y = src->sz;
                    src++;
                    n--;
                    dst->sxy = x;
                    dst->sz = y;
                    dst++;
                }
            }
            if (SLOT->mainCount0C >= 2) {
                s32 *out = (s32 *)(s2 + 0x160);
                s32 idx = SLOT->mainIndex10;
                s32 s3 = SLOT->mainCount0C - 1;
                s32 off = (idx << 3) + 0x14;
                TrailEntry4 *s0 = (TrailEntry4 *)(s4 + off);
                s32 s1 = 4 - idx;
                while (s3 > 0) {
                    s32 unk0 = s0->unk0;
                    RR_GTE_MTC2_IR0(unk0);
                    RR_GTE14_NOP2();
                    RR_GTE_OP(0x4A780010);
                    RR_GTE_SWC2_RGB2(out);
                    out++;
                    s1--;
                    s0++;
                    if (s1 <= 0) {
                        s0 = (TrailEntry4 *)(s4 + 0x14);
                        s1 = 4;
                    }
                    s3--;
                }
                *out = 0;
                WORK->projectedD4 = (RroProjectedVertex *)(s2 + 0x1A0);
                {
                s32 i2 = SLOT->mainIndex10;
                WORK->colorsD8 = (s32 *)(s2 + 0x168);
                s1 = 4 - i2;
                off = (i2 << 3) + 0x14;
                }
                s3 = SLOT->mainCount0C - 1;
                s0 = (TrailEntry4 *)(s4 + off);
                while (s3 > 0) {
                    p = WORK->commands84;
                    WORK->projectedD0 = WORK->projectedD4;
                    WORK->projectedD4 = s0->verts;
                    EmitProjectedLitTrisGT3((const RroTrailPanelCommand *)(p + 1), *(volatile s32 *)p >> 16, WORK);
                    WORK->colors15C.palette.rgb160[0] = WORK->colors15C.palette.rgb160[1];
                    s1--;
                    WORK->colors15C.palette.rgb160[1] = *WORK->colorsD8++;
                    s0++;
                    if (s1 <= 0) {
                        s0 = (TrailEntry4 *)(s4 + 0x14);
                        s1 = 4;
                    }
                    s3--;
                }
            }
            {
                char *v0 = (char *)SLOT->model->secondary1C.vertices;
                v1 = SLOT->model->secondary1C.vertexCount;
                a0 = s2 + 0x188;
                while (v1 > 0) {
                    RR_GTE14_LWC2_V012(v0);
                    RR_GTE14_NOP2();
                    RR_GTE14_RTPT();
                    v0 += 0x18;
                    a0 += 0x18;
                    GTE_STORE_SXY_SZ(a0);
                    v1 -= 3;
                }
            }
            WORK->colors15C.palette.rgb160[0] = 0x2E808080;
            p = SLOT->model->secondary1C.commands;
            EmitProjectedQuadsFT4((const RroTexturedQuadPrefix *)(p + 1), *(volatile s32 *)p >> 16, WORK);
            GTE_LWC2_IR0(s2 + 0xC0);
            RR_GTE14_NOP2();
            RR_GTE_OP(0x4A780010);
            RR_GTE_SWC2_RGB2(s2 + 0x160);
            WORK->colors15C.palette.rgb160[1] = 0;
            WORK->colors15C.palette.rgb160[0] = (WORK->colors15C.palette.rgb160[0] & 0xFFFFFF) | 0x32000000;
            p = SLOT->model->litQuadCommands0C;
            EmitProjectedLitQuadsGT4((const RroGlowPointCommand *)(p + 1), *(volatile s32 *)p >> 16, WORK);
        } else {
            SLOT->mainCount0C = 0;
        }
    }

    if (WORK->facing98.z >= 0x7D0) {
        s32 v1 = SLOT->phase5C;
        SLOT->secondCount34 = 0;
        if (v1 == 0) {
            return;
        }
        if (v1 == 1) {
            SLOT->phase5C = 2;
        }
    } else {
        s32 v1;
        char *a0;
        char *a1;
        RR_GTE_SETROT(s2);
        RR_GTE_SETTR(s2 + 0x14);
        GTE_CTC2_27(0x88);
        WORK->vertices7C = SLOT->model->history34.vertices;
        a1 = ((char * volatile *)s2)[0x7C / 4];
        WORK->vertexCount88 = SLOT->model->history34.vertexCount;
        v1 = ((volatile s32 *)s2)[0x88 / 4];
        WORK->commands84 = SLOT->model->history34.commands;
        a0 = s2 + 0x188;
        while (v1 > 0) {
            RR_GTE_SWC2_IR0(s2 + 0xDC);
            RR_GTE14_LWC2_V012(a1);
            RR_GTE14_NOP2();
            RR_GTE14_RTPT();
            a1 += 0x18;
            a0 += 0x18;
            GTE_STORE_SXY_SZ(a0);
            v1 -= 3;
        }
        if (RacePaused == 0) {
            s32 idx;
            TrailEntry4 *entry;
            s32 n;
            RroProjectedVertex *dst;
            RroProjectedVertex *src;
            if (SLOT->secondCount34 < 4) {
                SLOT->secondCount34 += 1;
            }
            idx = (SLOT->secondIndex38 + 3) % 4;
            SLOT->secondIndex38 = idx;
            entry = (TrailEntry4 *)(s4 + ((idx << 3) + 0x3C));
            entry->unk0 = (s16)SLOT->intensity60;
            n = WORK->vertexCount88;
            dst = entry->verts;
            src = (RroProjectedVertex *)(s2 + 0x1A0);
            while (n > 0) {
                s32 x = src->sxy;
                s32 y = src->sz;
                src++;
                n--;
                dst->sxy = x;
                dst->sz = y;
                dst++;
            }
        }
        if (SLOT->secondCount34 >= 2) {
            s32 cnt = SLOT->secondCount34;
            s32 col = SLOT->intensity60;
            s32 s0;
            s32 *out;
            s32 s1;
            GTE_DPCS_COL(col, WORK->interpolationDC);
            out = (s32 *)(s2 + 0x160);
            s0 = cnt - 1;
            while (s0 > 0) {
                if (col > 0) {
                    *out = col;
                } else {
                    *out = 0;
                }
                col -= col / 4;
                s0--;
                out++;
            }
            *out = 0;
            WORK->projectedD4 = (RroProjectedVertex *)(s2 + 0x1A0);
            WORK->colorsD8 = (s32 *)(s2 + 0x168);
            s0 = SLOT->secondCount34 - 1;
            s1 = SLOT->secondIndex38;
            while (s0 > 0) {
                WORK->projectedD0 = WORK->projectedD4;
                s1 = (s1 + 1) % 4;
                p = WORK->commands84;
                WORK->projectedD4 = ((TrailEntry4 *)(s4 + (s1 << 3) + 0x3C))->verts;
                EmitProjectedLitTrisGT3((const RroTrailPanelCommand *)(p + 1), *(volatile s32 *)p >> 16, WORK);
                if (WORK->colors15C.palette.rgb160[1] <= 0) {
                    break;
                }
                do {
                    WORK->colors15C.palette.rgb160[0] = WORK->colors15C.palette.rgb160[1];
                } while (0);
                {
                    s32 *q;
                    q = WORK->colorsD8;
                    s0--;
                    col = q;
                    q = (s32 *)*q;
                    col += 4;
                    WORK->colorsD8 = (s32 *)col;
                    WORK->colors15C.palette.rgb160[1] = q;
                }
            }
        }
        {
            char *v0 = (char *)SLOT->model->trailing40.vertices;
            v1 = SLOT->model->trailing40.vertexCount;
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE14_LWC2_V012(v0);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                v0 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
        }
        {
            s32 c = SLOT->intensity60;
            WORK->colors15C.palette.rgb160[0] = 0x2E000000 | (c << 16) | (c << 8) | c;
            p = SLOT->model->trailing40.commands;
            EmitProjectedQuadsFT4((const RroTexturedQuadPrefix *)(p + 1), *(volatile s32 *)p >> 16, WORK);
        }
        {
            char *v0 = (char *)SLOT->model->trailing28.vertices;
            v1 = SLOT->model->trailing28.vertexCount;
            a0 = s2 + 0x188;
            while (v1 > 0) {
                RR_GTE14_LWC2_V012(v0);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                v0 += 0x18;
                a0 += 0x18;
                GTE_STORE_SXY_SZ(a0);
                v1 -= 3;
            }
        }
        {
            s32 c = SLOT->intensity64;
            WORK->colors15C.palette.rgb160[0] = 0x2E000000 | (c << 16) | (c << 8) | c;
            p = SLOT->model->trailing28.commands;
            EmitProjectedQuadsFT4((const RroTexturedQuadPrefix *)(p + 1), *(volatile s32 *)p >> 16, WORK);
        }
    }

    GTE_CTC2_27(0x88);
    if (SLOT->phase5C == 0) {
        return;
    }

    WORK->vertices7C = SLOT->model->history34.vertices;
    WORK->vertexCount88 = SLOT->model->history34.vertexCount;
    WORK->commands84 = SLOT->model->history34.commands;
    WORK->trailVerticesA8 = ((TrailEntry8 *)(s4 + (SLOT->historyIndex6C << 3) + 0x84))->verts;

    if (SLOT->phase5C == 1) {
        RR_GTE_SETROT(arg3);
        WORK->jitterTranslationAC.x = WORK->worldPosition38.x - SLOT->storedPosition74.x;
        WORK->jitterTranslationAC.y = WORK->worldPosition38.y - SLOT->storedPosition74.y;
        WORK->jitterTranslationAC.z = WORK->worldPosition38.z - SLOT->storedPosition74.z;
        RR_GTE_SETTR(s2 + 0xAC);
        vdst = (char *)(WORK->trailVerticesA8 = (Vec3sFields8 *)(s2 + 0xE0));
        vsrc = (char *)WORK->vertices7C;
        vcnt = WORK->vertexCount88;
        while (vcnt > 0) {
            RR_GTE_MVMVA_ROT_TR_V0(vsrc);
            RR_GTE_STORE_IR_S16(vdst);
            vsrc += 8;
            vcnt--;
            vdst += 8;
        }
        if (RacePaused == 0) {
            SLOT->historyCount68 += 1;
            if (SLOT->historyCount68 == 8) {
                SLOT->phase5C = 2;
            }
            SLOT->historyIndex6C = (SLOT->historyIndex6C + 7) % 8;
            a2 = (TrailEntry8 *)(s4 + ((SLOT->historyIndex6C << 3) + 0x84));
            vdst = (char *)a2->verts;
            a2->unk0 = (s16)SLOT->intensity60;
            vcnt = WORK->vertexCount88;
            vsrc = (char *)WORK->trailVerticesA8;
            while (vcnt > 0) {
                *(Vec3sFields8 *)vdst = *(Vec3sFields8 *)vsrc;
                vsrc += 8;
                vcnt--;
                vdst += 8;
            }
        }
    }

    {
        s32 nleft = SLOT->historyCount68;
        if (nleft != 0) {
        s32 dec8;
        s32 i8;
        s32 s5;
        dec8 = (SLOT->phase5C == 2) ? 8 : 4;
        i8 = SLOT->historyIndex6C;
        while (nleft > 0) {
            a2 = (TrailEntry8 *)(s4 + ((i8 << 3) + 0x84));
            i8 = (i8 + 1) % 8;
            s5 = a2->unk0;
            s5 -= dec8;
            if (s5 < 0) {
                s5 = 0;
            }
            nleft--;
            a2->unk0 = s5;
        }
        if (s5 == 0) {
            SLOT->historyCount68 -= 1;
        }
        if (SLOT->historyCount68 == 0) {
            SLOT->phase5C = 0;
            return;
        }
        if (SLOT->historyCount68 >= 2) {
            /* ASM_BARRIER: pin the RTPT vertex counter to $v1 (original register choice) */
            register s32 nvert __asm__("$3");
            s32 s0;
            s32 s1;
            RR_GTE_SETROT((void *)SCRATCH_ROT_MATRIX);
            {
                char *t = s2 + 0x1A0;
                s32 *g;
                s32 m;
                do {
                    m = -0x8000;
                } while (0);
                g = (s32 *)SCRATCHPAD;
                WORK->projectedD0 = (RroProjectedVertex *)t;
                WORK->inputC8.fields.x = SLOT->storedPosition74.x + m - g[2];
                WORK->inputC8.fields.y = SLOT->storedPosition74.y - g[3];
                WORK->inputC8.fields.z = SLOT->storedPosition74.z + m - g[4];
            }
            RR_GTE_MVMVA_ROT_V0_NOCV(s2 + 0xC8);
            RR_GTE_SWC2_MAC123(s2 + 0x38);
            RR_GTE_SETTR_AT20(s2 + 0x24);
            vsrc = (char *)WORK->trailVerticesA8;
            vdst = (char *)WORK->projectedD0 - 0x18;
            nvert = WORK->vertexCount88;
            while (nvert > 0) {
                RR_GTE14_LWC2_V012(vsrc);
                RR_GTE14_NOP2();
                RR_GTE14_RTPT();
                vsrc += 0x18;
                vdst += 0x18;
                GTE_STORE_SXY_SZ(vdst);
                nvert -= 3;
            }
            WORK->projectedD4 = (RroProjectedVertex *)(vdst + 0x18);
            s0 = SLOT->historyIndex6C;
            WORK->colors15C.palette.rgb160[0] = 0;
            s1 = SLOT->historyCount68 - 1;
            while (s1 > 0) {
                s32 off;
                void *d0;
                void *d4;
                s0 = (s0 + 1) % 8;
                off = (s0 << 3) + 0x84;
                a2 = (TrailEntry8 *)(s4 + off);
                vsrc = (char *)a2->verts;
                vdst = (char *)WORK->projectedD4 - 0x18;
                nvert = WORK->vertexCount88;
                while (nvert > 0) {
                    RR_GTE_SWC2_IR0(s2 + 0xDC);
                    RR_GTE14_LWC2_V012(vsrc);
                    RR_GTE14_NOP2();
                    RR_GTE14_RTPT();
                    vsrc += 0x18;
                    vdst += 0x18;
                    GTE_STORE_SXY_SZ(vdst);
                    nvert -= 3;
                }
                if (s1 == 1) {
                    WORK->colors15C.palette.rgb160[1] = 0;
                } else {
                    u16 c;
                    GTE_LWC2_IR0_MEM(s2 + 0xDC);
                    c = a2->unk0;
                    GTE_MTC2_RGB_DPCS(c);
                    RR_GTE_SWC2_RGB2(s2 + 0x164);
                }
                p = WORK->commands84;
                EmitProjectedLitTrisGT3((const RroTrailPanelCommand *)(p + 1), *(volatile s32 *)p >> 16, WORK);
                do {
                    d4 = WORK->projectedD4;
                    d0 = WORK->projectedD0;
                } while (0);
                nvert = WORK->colors15C.palette.rgb160[1];
                WORK->projectedD0 = d4;
                WORK->projectedD4 = d0;
                if (nvert <= 0) {
                    break;
                }
                s1--;
                WORK->colors15C.palette.rgb160[0] = nvert;
            }
        }
        }
    }
}
