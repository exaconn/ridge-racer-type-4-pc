#include "game/camera/camview.h"
#include "game/camera/camfx.h"
#include "rr/scene_camera_state.h"
#include "rr/player_cars.h"

void UpdateCarShowcaseCamera(int time)
{
  int curveValue;
  int value;
  u32 distance;
  SceneCameraState160 *depthCamera;
  int depthQuarter;
  int steering;
  RaceVehicle *car;
  SceneCameraState160 *heightCamera;
  PairS16 *curve;
  value = InterpCurveEase(time);
  RaceContext.camera24C->parts18[0].rotation10.y = value;
  RaceContext.camera24C->parts18[1].rotation10.y = -(u32)value;
  curve = D_800A0D70;
  RaceContext.camera24C->parts18[3].rotation10.x = InterpCurveLinear(curve, FindCurveSegment(curve, time), time);
  curve = ShowcaseCamDepthCurve;
  curveValue = InterpCurveLinear(curve, FindCurveSegment(curve, time), time);
  curve = D_800A0D88;
  value = InterpCurveLinear(curve, FindCurveSegment(curve, time), time);
  RaceContext.camera24C->parts18[3].position00.xyz.z = curveValue - RaceContext.camera24C->extentA0 * 4U - 0x10U;
  RaceContext.camera24C->parts18[3].position00.xyz.y = value + 0xCU;
  RaceContext.camera24C->parts18[2].position00.xyz.z = curveValue;
  RaceContext.camera24C->parts18[2].position00.xyz.y = value;
  depthQuarter = curveValue;
  depthCamera = RaceContext.camera24C;
  if (curveValue < 0)
  {
    depthQuarter = curveValue + 3;
  }
  distance = depthCamera->distance100;
  depthCamera->offsetB0 = (depthQuarter >> 2) - distance;
  heightCamera = RaceContext.camera24C;
  if (value < 0)
  {
    value += 3;
  }
  heightCamera->offsetAC = value >> 2;
  curve = D_800A0D94;
  curveValue = InterpCurveLinear(curve, FindCurveSegment(curve, time), time);
  steering = -(curveValue * 32U);
  car = PlayerCars;
  RaceContext.camera24C->offsetC8 = curveValue;
  car->wheelSteering17C = steering % 4096;
  car->position20.words[2] = (u32)RaceContext.camera24C->offsetB0 + RaceContext.camera24C->offsetC8;
  car->position20.words[1] = (u32)RaceContext.camera24C->offsetAC + RaceContext.camera24C->offsetC4;
}
