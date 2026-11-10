#ifndef RR_SCENE_CAMERA_TYPES_H
#define RR_SCENE_CAMERA_TYPES_H

#include "rr/geometry.h"
#include "rr/lighting_types.h"

/* A word-component position followed by the SDK short-angle vector. */
typedef struct {
    VectorWords16 position00;
    Vec3sFields8 rotation10;
} SceneCameraPose18;

typedef struct {
    SceneCameraPose18 pose00;
    s32 absolute18, distance1C, projection20;
} SceneCameraView24;

typedef struct {
    SceneCameraView24 view00;
    s32 time24, restart28;
} SceneCameraKeyframe2C;

typedef SceneCameraKeyframe2C SceneCameraTrack24[24];

/* Shared storage initialized by InitCameraState and consumed by both the
 * showcase controller and the reflection renderer. Unknown words stay
 * explicit; they are not separate, incompatible views of the same camera. */
typedef struct {
    SceneCameraPose18 pose00;
    SceneCameraPose18 parts18[4];
    PreviewLighting40 lighting78;
    s32 extentA0, mirrorA4;
    s32 reservedA8;
    s32 offsetAC, offsetB0;
    s32 reservedB4[4];
    s32 offsetC4, offsetC8;
    s32 reservedCC[13];
    s32 distance100;
    s32 frame104, phase108, segment10C;
    VectorWords16 position110;
    Vec3s8 rotation120;
    s32 reserved128, value12C, value130, reserved134;
    SceneCameraTrack24 *tracks138;
    s32 mode13C, mode140;
    s32 reserved144[7];
} SceneCameraState160;

typedef struct {
    s32 reserved00[0x60 / 4];
    SceneCameraView24 view60;
    s32 reserved84[(0xD4 - 0x84) / 4];
    s32 overlayEnabledD4, overlayStyleD8, overlayModeDC, overlayBlendE0;
    u8 overlayColorE4[4];
    s32 reservedE8;
    SceneCameraState160 cameraEC;
    SceneCameraState160 *camera24C;
    s32 reserved250[99];
    s32 overlayToggle3DC;
    u8 reserved3E0[0x560 - 0x3E0];
    /* The legacy mesh API still accepts this slot as an integer. Its pointer
     * migration belongs with DrawMeshPrimList and the reflection packet path. */
    s32 model560;
} SceneRaceContext;

RR_ASSERT_SIZE(SceneCameraPose18, 0x18);
RR_ASSERT_OFFSET(SceneCameraPose18, rotation10, 0x10);
RR_ASSERT_SIZE(SceneCameraView24, 0x24);
RR_ASSERT_OFFSET(SceneCameraView24, projection20, 0x20);
RR_ASSERT_SIZE(SceneCameraKeyframe2C, 0x2C);
RR_ASSERT_OFFSET(SceneCameraKeyframe2C, time24, 0x24);
RR_ASSERT_SIZE(SceneCameraTrack24, 0x420);
RR_ASSERT_OFFSET(SceneCameraState160, parts18, 0x18);
RR_ASSERT_OFFSET(SceneCameraState160, lighting78, 0x78);
RR_ASSERT_OFFSET(SceneCameraState160, extentA0, 0xA0);
RR_ASSERT_OFFSET(SceneCameraState160, offsetAC, 0xAC);
RR_ASSERT_OFFSET(SceneCameraState160, distance100, 0x100);
RR_ASSERT_OFFSET(SceneCameraState160, position110, 0x110);
RR_ASSERT_OFFSET(SceneCameraState160, rotation120, 0x120);
RR_ASSERT_OFFSET(SceneCameraState160, tracks138, 0x138);
RR_ASSERT_OFFSET(SceneRaceContext, view60, 0x60);
#ifdef __mips__
RR_ASSERT_SIZE(SceneCameraState160, 0x160);
RR_ASSERT_OFFSET(SceneCameraState160, mode13C, 0x13C);
RR_ASSERT_OFFSET(SceneRaceContext, cameraEC, 0xEC);
RR_ASSERT_SIZE(SceneRaceContext, 0x564);
RR_ASSERT_OFFSET(SceneRaceContext, camera24C, 0x24C);
RR_ASSERT_OFFSET(SceneRaceContext, overlayToggle3DC, 0x3DC);
RR_ASSERT_OFFSET(SceneRaceContext, model560, 0x560);
#endif

#endif
