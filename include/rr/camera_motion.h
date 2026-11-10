#ifndef RR_CAMERA_MOTION_H
#define RR_CAMERA_MOTION_H
#include "rr/presentation.h"
#include "game/camera/cammove.h"
/* Initial key08 selects an accessible key; the cyclic mode uses three keys.
 * Input, readonly keys and mutable camera storage are disjoint. Active setup
 * requires a nonzero wrapped signed divisor and excludes INT_MIN/-1. Every
 * word product wraps before division. The horizon uses unsigned division;
 * counter comparison narrows through signed16. Modes0/1 retain their distinct
 * stepping and key-wrap rules. The object pointer is captured before updates. */
RR_ASSERT_SIZE(CameraKey16,0x10);
RR_ASSERT_SIZE(CameraMotion70,0x70);
RR_ASSERT_OFFSET(CameraMotion70,keys0C,0xC);
RR_ASSERT_OFFSET(CameraMotion70,current40,0x40);
RR_ASSERT_OFFSET(CameraMotion70,counter50,0x50);
RR_ASSERT_OFFSET(CameraMotion70,horizon52,0x52);
RR_ASSERT_OFFSET(CameraMotion70,start10,0x10);
RR_ASSERT_OFFSET(CameraMotion70,angles64,0x64);
RR_ASSERT_OFFSET(CameraMotion70,modelPosition54,0x54);
RR_ASSERT_OFFSET(CameraMotion70,modelScale6C,0x6C);
RR_ASSERT_OFFSET(PresentationController,motion74,0x74);
RR_ASSERT_SIZE(CameraScriptEntry8,8);
RR_ASSERT_OFFSET(PresentationController,script64,0x64);
RR_ASSERT_OFFSET(PresentationController,scriptArgument70,0x70);
#endif
