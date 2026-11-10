#include "game/camera/viewcam.h"
#include "scratchpad.h"
#include "rr/ring_renderer.h"
#include "rr/game_motion.h"
#include "game/gfx/ring.h"
#include "rr_asm.h"
#include "rr/menu_overlay_runtime.h"
#include "rr/menu_text_rows.h"
#include "game/gfx/scene1.h"

s32 *ScaleMatrix(s32 *matrix, s32 *scale);
s32 RotTransPers(s32 *point, s32 *screen, s32 *interpolation, s32 *flags);

void DrawRing(s32 *position, s32 radius);

static __inline__ s32 project_ring_origin(s32 *position, s32 radius,
    RingScratch48 *scratch, RingVertex8 *point, s32 *output)
{
    s32 depth, innerOffset;
  point->x = position[0];
  point->y = position[1];
  point->z = position[2];
  depth = RotTransPers((s32 *)point, output, output, output) >> 1;
  scratch->geometry.vectors[0].x = 0;
  scratch->geometry.vectors[0].y = radius;
  scratch->geometry.vectors[0].z = position[2];
  RR_GTE_RTPS_LDV(&scratch->geometry.vectors[0]);
  RR_GTE_SWC2_SXY2(&scratch->geometry.screen[0]);
  scratch->geometry.vectors[1].x = 0;
  innerOffset = radius - 128;
  scratch->geometry.vectors[1].y = innerOffset;
  scratch->geometry.vectors[1].z = position[2];
  RR_GTE_RTPS_LDV(&scratch->geometry.vectors[1]);
  RR_GTE_SWC2_SXY2(&scratch->geometry.screen[1]);
  return depth;
}

void DrawRing(s32 *position, s32 radius)
{
  s32 scale[3];
  RingTransform24 transform;
  MatrixWords32 rotation;
  MatrixWords32 camera;
  RingVertex8 point;
  s32 transformed;
  RingScratch48 *scratch = (RingScratch48 *) (SCRATCHPAD + 0x7C);
  RingPacket24 *packet;
  s32 segment;
  s32 angle;
  s32 depth;
  s32 orderOffset;
  s32 inner;
  u32 *head;
  u32 *gteWords;
  transform.x = position[0];
  transform.y = position[1];
  transform.z = position[2];
  transform.rz = 0;
  transform.ry = 0;
  transform.rx = 0;
  SetupObjectLightMatrices((char *)&transform, (char *)&RaceContext.controller3D8->lighting20, &rotation, &camera);
  scale[2] = 1024;
  scale[1] = 1024;
  scale[0] = 1024;
  ScaleMatrix(rotation.w, scale);
  SetGteObjectTransform(&scratch->transform, (s16 *)&transform, rotation.w);
  scratch->geometry.red = 220;
  scratch->geometry.green = 160;
  scratch->geometry.blue = 0;
  depth = project_ring_origin(position, radius, scratch, &point, &transformed);
  segment = 0;
  inner = radius - 128;
  orderOffset = depth * 4;
  packet = *((RingPacket24 **) SCRATCHPAD);
  do
  {
    angle = segment << 7;
    scratch->geometry.vectors[2].x = (rsin(angle) * radius) / 4096;
    scratch->geometry.vectors[2].y = (rcos(angle) * radius) / 4096;
    scratch->geometry.vectors[2].z = position[2];
    scratch->geometry.vectors[3].x = (rsin(angle) * inner) / 4096;
    scratch->geometry.vectors[3].y = (rcos(angle) * inner) / 4096;
    /* One word cursor serves the GTE input and then its projected output. */
    gteWords = (u32 *)&scratch->geometry.vectors[2];
    scratch->geometry.vectors[3].z = position[2];
    RR_GTE_RTPT_LDV(gteWords, &scratch->geometry.vectors[3], &scratch->geometry.vectors[3]);
    gteWords = (u32 *)&scratch->geometry.screen[2];
    RING_STORE_SXY_PAIR(gteWords, &scratch->geometry.screen[3]);
    packet->tag.bits.len = 5;
    packet->code = 0x28;
    packet->xy[0][0] = scratch->geometry.screen[0].x;
    packet->xy[0][1] = scratch->geometry.screen[0].y;
    packet->xy[1][0] = scratch->geometry.screen[1].x;
    packet->xy[1][1] = scratch->geometry.screen[1].y;
    packet->xy[2][0] = scratch->geometry.screen[2].x;
    packet->xy[2][1] = scratch->geometry.screen[2].y;
    packet->xy[3][0] = scratch->geometry.screen[3].x;
    packet->xy[3][1] = scratch->geometry.screen[3].y;
    segment++;
    packet->red = scratch->geometry.red;
    packet->green = scratch->geometry.green;
    packet->blue = scratch->geometry.blue;
    packet->tag.raw = (packet->tag.raw & 0xFF000000) |
        ((RingGpuTag *)(orderOffset +
            (unsigned long)((RingGpuSlots *)SCRATCHPAD)->order.pointer))->bits.addr;
    head = (u32 *)(orderOffset +
        (unsigned long)((RingGpuSlots *)SCRATCHPAD)->order.pointer);
    ((RingGpuTag *)head)->bits.addr = (unsigned long)packet & 0xFFFFFF;
    scratch->geometry.screen[0] = scratch->geometry.screen[2];
    scratch->geometry.screen[1] = scratch->geometry.screen[3];
    packet++;
  }
  while (segment < 33);
  *((RingPacket24 **) SCRATCHPAD) = packet;
}

void InitScrollTrack(int selection) {
    ScrollTrack40 *scroll = &RaceContext.controller3D8->scroll;
    PsyqBzero(scroll, sizeof(*scroll));
    scroll->disabled = 0;
    scroll->keys = D_800A1CD0[selection];
}
