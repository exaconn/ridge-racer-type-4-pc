#include "psyq/libgte/rotmat.h"
#include "rr/rro_model.h"
#include "game/gfx/model.h"
#include "psyq/libgte/rotmatz.h"
#include "game/camera/viewcam.h"
#include "types.h"
#include "scratchpad.h"
#include "rr_asm.h"
#include "game/gfx/sky.h"
#include "psyq/libgte/gtemat2.h"
#include "game/gfx/skyrot1.h"
#include "rr/driver_control.h"
#include "rr/race_setup.h"

struct IntroColorBytes8 { unsigned char c[8]; };

extern char D_8001013C[];
extern struct IntroColorBytes8 D_800101E4;
extern int RotMatrixX();
extern int RotMatrixY();
extern char D_8009E88C[];
extern s16 FadeCountdown;
extern s16 ModeIndex;
extern s16 ModeGroup;
extern s16 SubmodeState;

void IntroSceneModeEnter(void);

int DrawRotatingTitleModel(int objectIndex, int animate)
{
    MatrixWords32 rotation;
    RroScratchWork *work = &((RroModelScratch *)SCRATCHPAD)->work7C;
    int yaw;
    SelectModelBank(4);
    yaw = IntroSceneStatePtr->objectYawAA[objectIndex];
    if (animate || (yaw && (yaw & 0x3FF) > 0x20)) yaw += 0x20;
    else if (yaw) yaw = 0;
    yaw &= 0xFFF;
    IntroSceneStatePtr->objectYawAA[objectIndex] = yaw;
    work->setup.angles.fields.x = 0;
    work->setup.angles.fields.z = 0;
    work->setup.angles.fields.y = -yaw;
    RotMatrix(&work->setup.angles.fields, &work->setup.rotation);
    RR_GTE_SETROT(IntroSceneStatePtr->mat00.w);
    MulRotMatrix0(&work->setup.rotation, &work->setup.light);
    RR_GTE_SETLLM(work->setup.light.w);
    RR_GTE_SETROT(((RroModelScratch *)SCRATCHPAD)->scene.view.matrix28.w);
    MulRotMatrix0(&work->setup.rotation, &rotation);
    SetGteObjectTransform(&((RroModelScratch *)SCRATCHPAD)->work7C.transform, (const s16 *)(D_8001013C + objectIndex * 16), rotation.w);
    ((RroModelScratch *)SCRATCHPAD)->scene.draw74.draw.counter = 0;
    SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD, ModelBankFormat < 1, 0, 0);
    return yaw;
}

void DrawRotatingSky(void)
{
    struct IntroColorBytes8 sp10;
    MatrixWords32 sp18;
    int t;
    sp10 = D_800101E4;
    IntroSceneStatePtr->uA8 += 0x20;
    sp18.w[4] = 0x1000;
    sp18.w[2] = 0x1000;
    sp18.w[3] = 0;
    sp18.w[1] = 0;
    t = (short)IntroSceneStatePtr->uA8;
    sp18.w[0] = 0x1000;
    RotMatrixY(t, &sp18);
    RotMatrixX(0x100, &sp18);
    RotMatrixZ(0xAA, &sp18);
    RenderSkyToRect(D_8009E88C, &sp18, &sp10, 0xC8);
}

void DrawTransformedTitleModel(int modelIndex, const Vec3s8 *angles,
                               const VectorWords16 *position)
{
    MatrixWords32 rotation;
    SelectModelBank(4);
    RotMatrix(&angles->fields, &((RroModelScratch *)SCRATCHPAD)->work7C.setup.rotation);
    RR_GTE_SETROT(IntroSceneStatePtr->mat20.w);
    MulRotMatrix0(&((RroModelScratch *)SCRATCHPAD)->work7C.setup.rotation,
                  &((RroModelScratch *)SCRATCHPAD)->work7C.setup.light);
    RR_GTE_SETLLM(((RroModelScratch *)SCRATCHPAD)->work7C.setup.light.w);
    RR_GTE_SETROT(((RroModelScratch *)SCRATCHPAD)->scene.view.matrix28.w);
    MulRotMatrix0(&((RroModelScratch *)SCRATCHPAD)->work7C.setup.rotation, &rotation);
    SetGteObjectTransform(&((RroModelScratch *)SCRATCHPAD)->work7C.transform,
                          (const s16 *)position->words, rotation.w);
    ((RroModelScratch *)SCRATCHPAD)->scene.draw74.draw.counter = 0;
    SubmitModel((RroSceneScratchPrefix *)SCRATCHPAD,
                modelIndex < ModelBankFormat ? modelIndex : 1, 0, 0);
}

void IntroSceneModeEnter(void) {
    VSyncThreshold = 0x80;
    FadeCountdown = 0;
    TitleSceneStage = 0;
    SetDispMask(1);
    ModeGroup = 0;
    ModeIndex = 2;
    SubmodeState = 0;
}
