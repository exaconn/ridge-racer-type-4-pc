#ifndef RR_GAME_GFX_SKYROT1_H
#define RR_GAME_GFX_SKYROT1_H

#include "rr/matrix_words.h"
#include "rr/geometry.h"

typedef struct IntroSceneState {
    MatrixWords32 mat00;
    MatrixWords32 mat20;
    s16 *p40;
    s16 s44, s46, s48, s4A, s4C, s4E, s50, s52, s54, s56, s58, s5A, s5C, s5E;
    s16 s60, s62, s64, s66, s68, s6A, s6C, s6E, s70, s72, s74, s76, s78, s7A, s7C, s7E;
    s16 s80, s82, s84, s86, s88, s8A, s8C, s8E, s90, s92, s94, s96, s98, s9A, s9C, s9E;
    s16 sA0;
    u16 uA2;
    s16 sA4, sA6;
    u16 uA8;
    s16 objectYawAA[8];
} IntroSceneState;

/* The pointer at +40 retains its native width; subsequent fields follow it.
 * The eight yaw cells are selected by the intro object index, 0..7. */
#if defined(mips)
RR_ASSERT_SIZE(IntroSceneState, 0xBC);
RR_ASSERT_OFFSET(IntroSceneState, p40, 0x40);
RR_ASSERT_OFFSET(IntroSceneState, s44, 0x44);
RR_ASSERT_OFFSET(IntroSceneState, objectYawAA, 0xAA);
#endif

extern IntroSceneState *IntroSceneStatePtr;
extern int TitleSceneStage;
int DrawRotatingTitleModel(int objectIndex, int animate);
void DrawRotatingSky(void);
void DrawTransformedTitleModel(int modelIndex, const Vec3s8 *angles,
                               const VectorWords16 *position);
void IntroSceneModeEnter(void);


#endif /* RR_GAME_GFX_SKYROT1_H */
