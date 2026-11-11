#include "rr/rro_model.h"
#include "psyq/libgte/gtemat7.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat3.h"
#include "psyq/libgte/gtemat.h"
#include "rr/matrix_workspace.h"
#include "types.h"
#include "scratchpad.h"
#include "psyq/libgte/libgte.h"
#include "rr_asm.h"
#include "game/camera/viewcam.h"
#include "game/course/trkclip.h"
#include "rr/overlay662_palette_refresh.h"
#include "rr/race_types.h"
#include "game/gfx/model.h"
#include "rr/relative_file.h"
#include "game/math/rotmat.h"

static __inline__ const u8 *RroModelBankAddress(const RroModelBank *bank,u32 relative) {
    return (const u8 *)((s32)relative + (unsigned long)bank);
}

#define G ((RroSceneScratchPrefix *)SCRATCHPAD)

extern int ViewUsesCarTrackNode;
extern short ModelTrackSegmentResult[2];
extern s16 *CarBoundsTable[];
extern int ActiveModelVertexCount;
extern int ModelBoneMatrixTable[];
extern char OpeningLensFlareSprites[];

void SetFogNearFar(s32 a0, s32 a1, s32 a2);
s32 DrawArticulatedCarModel(RaceVehicle *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void SetVramImageSlot(int a0, int a1);
void LoadVramImageSlot(int a0);

int GetViewTrackSegment(const ProjectionView32 *view, s32 player) {
    int node, yaw;
    if (ViewUsesCarTrackNode == 0) {
        node = GetViewTrackNode(view->position00.words, player);
        yaw = view->angles10.xyz.y;
        GetTrackSegmentAndYaw(node, yaw, ModelTrackSegmentResult);
        return ModelTrackSegmentResult[0];
    }
    return GetViewTrackNode(view->position00.words, player) / 5;
}

s32 YawToOctant(const ProjectionView32 *view) {
    return (((view->angles10.xyz.y & 0xFFF) + 0x100) / 0x200) & 7;
}

CourseViewIndices *GetSegmentViewTable(s32 player) {
    const ProjectionView32 *view = &G->view.projection.pose08;
    int segment = GetViewTrackSegment(view, player);
    return (CourseViewIndices *)RelFileSegmentViewSlots[segment * 8 + YawToOctant(view)].absolute;
}

extern double __muldf3(double, double);
void SetFogNearFar(s32 a0, s32 a1, s32 a2) {
    double d2, d0, d1, d6, t0, d4, d2_out;
    s32 r0, r1;

    d2 = a2;
    d0 = a0;
    d1 = a1;
    d6 = __muldf3(d2, 65536.0);
    d4 = 16777216.0 / (d6 * ((1.0 / d1) - (1.0 / d0)));
    t0 = d6 / d0;
    d2_out = t0 * (-d4);
    if (d4 > 32767.0 || d4 < -32768.0) {
        if (d4 > 0.0) {
            d4 = 32767.0;
        } else {
            d4 = -32768.0;
        }
        d2_out = t0 * (-d4);
        d2_out = d2_out + (4096.0 - ((d2_out + d4 * (d6 / d1)) * (1.0 / 4096.0))) * 4096.0;
    }
    if (d2_out > 2147483647.0) {
        d2_out = 2147483647.0;
    }
    if (d2_out < -2147483648.0) {
        d2_out = -2147483648.0;
    }
    r0 = d4;
    r1 = d2_out;
    GTE_CTC2_27(r0);
    GTE_CTC2_28(r1);
}
void InitTimTemplate(EmblemTimImage840 *image) {
    u32 *words = image->words;
    int n;
    *words++ = 0x10;
    *words++ = 8;
    *words++ = 0x2C;
    *words++ = 0x400280;
    *words++ = 0x10010;
    *words++ = 0x7FFF0000;
    *words++ = 0x7C1F03FF;
    *words++ = 0x7FE0001F;
    *words++ = 0x7C0003E0;
    *words++ = 0x42100000;
    *words++ = 0x40100210;
    *words++ = 0x42000010;
    *words++ = 0x40000200;
    *words++ = 0x80C;
    *words++ = 0x280;
    *words++ = 0x400010;
    n = 0x1FF;
    do {
        *words = 0;
        n--;
        words++;
    } while (n >= 0);
}

s32 DrawArticulatedCarModel(RaceVehicle *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    MatrixWords32 m10;
    MatrixWords32 m30;
    MatrixWords32 m50;
    MatrixWords32 m70;
    MatrixWords32 m90;
    MatrixWords32 mB0;
    SVECTOR svD0;
    MatrixWords32 mD8[6];
    VECTOR v198[5];
    s32 dist;
    s32 i;
    s32 s5;
    s32 a1;

    SelectModelBank(arg1);
    v198[0].vx = *(s16 *)&arg0->position20.words[0] - (s16)G->view.projection.pose08.position00.xyz.x;
    v198[0].vy = 0;
    v198[0].vz = *(s16 *)&arg0->position20.words[2] - (s16)G->view.projection.pose08.position00.xyz.z;
    if (v198[0].vx < 0) {
        v198[0].vx = -v198[0].vx;
    }
    if (v198[0].vz < 0) {
        v198[0].vz = -v198[0].vz;
    }
    dist = v198[0].vx + v198[0].vz;
    switch (arg2) {
    case 0:
        i = dist;
        break;
    case 1:
        i = arg3 - 1;
        break;
    case 2:
        i = arg4 - 1;
        break;
    case 3:
        i = arg5 - 1;
        break;
    }
    if (i < arg3) {
        s5 = 0;
        if (G->renderFlags64 != 0) {
            s5 = -0x60;
        }
        BuildRotMatrixY(&m90, -arg0->rot54);
        BuildRotMatrixX(&m50, -arg0->rot50);
        MulMatrix2(&m90, &m50);
        BuildRotMatrixZ(&m70, -arg0->rot58);
        MulMatrix2(&m50, &m70);
        MulMatrix0(&LightMatrix.matrix, &m70, &mB0);
        SetLightMatrix(&mB0);
        BuildRotMatrixY(&m10, -arg0->rot54);
        MulMatrix2((MatrixWords32 *)SCRATCH_ROT_MATRIX, &m10);
        BuildRotMatrixX(&mD8[0], arg0->wheelRotX1A8 - arg0->rot50);
        BuildRotMatrixZ(&m50, arg0->wheelRotZ1AA - arg0->rot58);
        MulMatrix(&mD8[0], &m50);
        MulMatrix2(&m10, &mD8[0]);
        v198[0] = *(VECTOR *)&arg0->bodyPosition;
        v198[1].vx = arg0->ir70[0] + arg0->ir60[0];
        v198[1].vy = arg0->ir70[1] + arg0->ir60[1];
        v198[1].vz = arg0->ir70[2] + arg0->ir60[2];
        BuildRotMatrixX(&mD8[1], -arg0->ang80);
        BuildRotMatrixZ(&m50, -arg0->ang84);
        MulMatrix(&mD8[1], &m50);
        MulMatrix2(&m10, &mD8[1]);
        v198[2].vx = arg0->worldPosition10.words[0] + arg0->ir60[0];
        v198[2].vy = arg0->worldPosition10.words[1] + arg0->ir60[1];
        v198[2].vz = arg0->worldPosition10.words[2] + arg0->ir60[2];
        BuildRotMatrixX(&m30, -arg0->rot50);
        BuildRotMatrixX(&mD8[3], arg0->wheelSteering17C);
        BuildRotMatrixZ(&mD8[2], -arg0->rot58);
        MulMatrix2(&m30, &mD8[2]);
        m30 = mD8[2];
        MulMatrix2(&m10, &mD8[2]);
        MulMatrix2(&mD8[2], &mD8[3]);
        BuildRotMatrixY(&mD8[4], arg0->lateralMotion178 / 12);
        BuildRotMatrixX(&mD8[5], arg0->wheelSteering17C);
        MulMatrix2(&m30, &mD8[4]);
        MulMatrix2(&m10, &mD8[4]);
        MulMatrix2(&mD8[4], &mD8[5]);
        MulMatrix(&m90, &m30);
        RR_GTE_SETROT(&m90);
        for (i = 0; i < 2; i++) {
            /* ASM_BARRIER: pin x to v0 to match branch delay slot and load scheduling */
            register s32 x asm("$2") = CarBoundsTable[0][0];
            if (i & 1) {
                x = -x;
            }
            svD0.vx = x << 2;
            /* ASM_BARRIER: force reload of CarBoundsTable[0] after storing svD0.vx */
            __asm__ ("" : : : "memory");
            svD0.vy = (u16)CarBoundsTable[0][1] << 2;
            svD0.vz = -((u16)CarBoundsTable[0][2]) << 2;
            RR_GTE_MVMVA_ROT_V0_NOCV(&svD0);
            RR_GTE_STORE_IR(&v198[i + 3]);
            v198[i + 3].vx += v198[2].vx;
            v198[i + 3].vy += v198[2].vy;
            v198[i + 3].vz += v198[2].vz;
        }
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[0], mD8[0].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 4) {
            a1 = 3;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[1], mD8[1].w);
        G->draw74.draw.counter = 0x05000000;
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, ModelBankFormat < 1, arg6, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[2], mD8[2].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 10) {
            a1 = 9;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[2], mD8[3].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 7) {
            a1 = 6;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, arg6, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[3], mD8[4].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 9) {
            a1 = 8;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[4], mD8[4].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 8) {
            a1 = 7;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[3], mD8[5].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 6) {
            a1 = 5;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, arg6, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[4], mD8[5].w);
        a1 = 1;
        G->draw74.draw.counter = 0x05000000;
        if (ModelBankFormat >= 5) {
            a1 = 4;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, arg6, s5);
    } else if (i < arg4) {
        s5 = 0;
        if (G->renderFlags64 != 0) {
            s5 = -0x60;
        }
        BuildRotMatrixY(&m90, -arg0->rot54);
        BuildRotMatrixX(&m50, -arg0->rot50);
        MulMatrix2(&m90, &m50);
        BuildRotMatrixZ(&m70, -arg0->rot58);
        MulMatrix2(&m50, &m70);
        MulMatrix0(&LightMatrix.matrix, &m70, &mB0);
        SetLightMatrix(&mB0);
        BuildRotMatrixY(&m10, -arg0->rot54);
        MulMatrix2((MatrixWords32 *)SCRATCH_ROT_MATRIX, &m10);
        BuildRotMatrixX(&mD8[0], arg0->wheelRotX1A8 - arg0->rot50);
        BuildRotMatrixZ(&m50, arg0->wheelRotZ1AA - arg0->rot58);
        MulMatrix(&mD8[0], &m50);
        MulMatrix2(&m10, &mD8[0]);
        v198[0] = *(VECTOR *)&arg0->bodyPosition;
        v198[1].vx = arg0->ir70[0] + arg0->ir60[0];
        v198[1].vy = arg0->ir70[1] + arg0->ir60[1];
        v198[1].vz = arg0->ir70[2] + arg0->ir60[2];
        BuildRotMatrixX(&mD8[1], -arg0->ang80);
        BuildRotMatrixZ(&m50, -arg0->ang84);
        MulMatrix(&mD8[1], &m50);
        MulMatrix2(&m10, &mD8[1]);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[0], mD8[0].w);
        a1 = 1;
        G->draw74.draw.counter = 0;
        if (ModelBankFormat >= 4) {
            a1 = 3;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)&v198[1], mD8[1].w);
        G->draw74.draw.counter = 0;
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, 1, arg6, s5);
    } else if (i < arg5) {
        s5 = 0;
        if (G->renderFlags64 != 0) {
            s5 = -0x7F;
        }
        BuildRotMatrixY(&m50, -arg0->rot54);
        BuildRotMatrixX(&m90, -arg0->rot50);
        MulMatrix2(&m50, &m90);
        MulMatrix0(&LightMatrix.matrix, &m90, &mB0);
        SetLightMatrix(&mB0);
        BuildRotMatrixZ(&m50, -arg0->rot58);
        MulMatrix2(&m90, &m50);
        MulMatrix2((MatrixWords32 *)SCRATCH_ROT_MATRIX, &m50);
        SetGteObjectTransform((ObjectTransform38 *)(SCRATCHPAD + 0x7C), (const s16 *)arg0->position20.words, m50.w);
        a1 = 1;
        G->draw74.draw.counter = 0;
        if (ModelBankFormat >= 3) {
            a1 = 2;
        }
        SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, a1, 0, s5);
    }
    return dist;
}

void RelocateModelTable(RroModelTableWire *table) {
    u32 *entry = table->words;
    int i = 0;
    int count = table->count;
    int limit;
    ((RroSceneScratchPrefix *)SCRATCHPAD)->modelEntries48 = entry;
    ActiveModelVertexCount = count;
    if (count > 0) {
        limit = count;
        do {
            i++;
            entry[RRO_MODEL_VERTEX_WORD] += (unsigned long)table;
            entry[RRO_MODEL_COMMAND_WORD] += (unsigned long)table;
            entry += RRO_MODEL_ENTRY_WORDS;
        } while (i < limit);
    }
}

void RegisterModelBank(RroModelBank *bank, s32 slot) {
    RroBankCommand *stream = bank->streams;
    unsigned i;
    ModelBankTable[slot] = bank;
    i = 0;
    bank->vertices.pointer = (const Vec3s8 *)(RroModelBankAddress(bank,bank->vertices.relative));
    bank->normals.pointer = (const Vec3s8 *)(RroModelBankAddress(bank,bank->normals.relative));
    if (bank->streamCount != 0) {
        do {
            i++;
            stream->pointer = (const u32 *)(RroModelBankAddress(bank,stream->relative));
            stream++;
        } while (i < bank->streamCount);
    }
}

void UnrelocateModelBank(RroModelBank *bank, const u8 *base) {
    RroBankCommand *stream = bank->streams;
    unsigned i = 0;
    bank->vertices.relative = ((const u8 *)bank->vertices.pointer - base);
    bank->normals.relative = ((const u8 *)bank->normals.pointer - base);
    if (bank->streamCount != 0) {
        do {
            i++;
            stream->relative = ((const u8 *)stream->pointer - base);
            stream++;
        } while (i < bank->streamCount);
    }
}

void SelectModelBank(int slot) {
    RroSceneScratchPrefix *scene = (RroSceneScratchPrefix *)SCRATCHPAD;
    ModelBankFormat = ModelBankTable[slot]->streamCount;
    scene->vertices54 = ModelBankTable[slot]->vertices.pointer;
    scene->normals58 = ModelBankTable[slot]->normals.pointer;
    scene->commands50 = ModelBankTable[slot]->streams;
}

void SetVramImageSlot(int a0, int a1) {
    ModelBoneMatrixTable[a1] = a0;
}

void LoadVramImageSlot(int a0) {
    LoadImage(OpeningLensFlareSprites, ModelBoneMatrixTable[a0]);
}
