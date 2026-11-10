#ifndef RR_TEST_SCENE_CAMERA_LEGACY_INPUTS_H
#define RR_TEST_SCENE_CAMERA_LEGACY_INPUTS_H

/* Frozen input layout used by the original LP64 controller fixture. This is
 * a generator/decoder format, not a production view or a native pointer owner.
 * Keeping it stable preserves every original RNG byte and PAL trace. Include
 * after the fixture's unchanged car/bounds owner declarations. */
typedef union {u32 words[88];u8 bytes[352];} FrozenSceneCameraInput;
typedef struct {
    u32 prefix[0xEC/4];
    FrozenSceneCameraInput camera;
    u64 discardedPointerBits;
    u32 tail[0x314/4];
} FrozenSceneContextInput;
typedef struct {u32 before[4];FrozenSceneContextInput value;u32 after[4];} FrozenContextOwner;
typedef struct {u32 before[4];FrozenSceneCameraInput value;u32 after[4];} FrozenCameraOwner;
typedef struct {
    FrozenContextOwner context;FrozenCameraOwner cameras[2];CarOwner cars;
    BoundsOwner bounds[3];BoundsPointers table;PairS16 curves[25];s32 masks[4];
    s16 index,selector;s32 draw;
} FrozenSceneWorldInput;

_Static_assert(sizeof(FrozenSceneContextInput)==0x570,"original LP64 context input");
_Static_assert(offsetof(FrozenSceneContextInput,camera)==0xEC,"original inline camera input");
_Static_assert(sizeof(FrozenContextOwner)==0x590&&sizeof(FrozenCameraOwner)==0x180,"original generator owners");

#endif
