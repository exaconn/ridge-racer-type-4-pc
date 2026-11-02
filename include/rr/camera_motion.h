#ifndef RR_CAMERA_MOTION_H
#define RR_CAMERA_MOTION_H
#include "types.h"
#include "rr/layout.h"
typedef struct {s32 x,y,z,reservedC;} RrCameraKey16;
typedef struct {
 s32 phase00,mode04,key08;RrCameraKey16 *keys0C;
 s32 start10[3],reserved1C,target20[3],reserved2C;
 s32 delta30[3],reserved3C,current40[3],reserved4C;
 u16 counter50;s16 horizon52;u8 reserved54[16];s16 angles64[3];u8 reserved6A[6];
} RrCameraMotion70;
typedef struct {u8 reserved00[0x4C];s32 scale4C;u8 reserved50[0x24];RrCameraMotion70 motion74;} RrCameraObjectPrefix;
typedef struct {u8 reserved00[0x3D8];RrCameraObjectPrefix *camera3D8;} RrCameraRootPrefix;
typedef struct {s16 active00;u8 reserved02[14];s32 timing10;} RrCameraMotionInput14;
/* Initial key08 selects an accessible key; the cyclic mode uses three keys.
 * Input, readonly keys and mutable camera storage are disjoint. Active setup
 * requires a nonzero wrapped signed divisor and excludes INT_MIN/-1. Every
 * word product wraps before division. The horizon uses unsigned division;
 * counter comparison narrows through signed16. Modes0/1 retain their distinct
 * stepping and key-wrap rules. The object pointer is captured before updates. */
extern RrCameraRootPrefix D_800AC650;
void func_8005B9F4(RrCameraMotionInput14 *input);
RR_ASSERT_SIZE(RrCameraKey16,0x10);
RR_ASSERT_SIZE(RrCameraMotion70,0x70);
RR_ASSERT_SIZE(RrCameraObjectPrefix,0xE4);
RR_ASSERT_OFFSET(RrCameraMotion70,keys0C,0xC);
RR_ASSERT_OFFSET(RrCameraMotion70,current40,0x40);
RR_ASSERT_OFFSET(RrCameraMotion70,counter50,0x50);
RR_ASSERT_OFFSET(RrCameraMotion70,horizon52,0x52);
RR_ASSERT_OFFSET(RrCameraMotion70,start10,0x10);
RR_ASSERT_OFFSET(RrCameraMotion70,angles64,0x64);
RR_ASSERT_OFFSET(RrCameraObjectPrefix,motion74,0x74);
RR_ASSERT_OFFSET(RrCameraRootPrefix,camera3D8,0x3D8);
RR_ASSERT_SIZE(RrCameraMotionInput14,0x14);
#endif
