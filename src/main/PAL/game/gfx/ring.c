#include "scratchpad.h"
#include "rr/ring_renderer.h"
#include "rr/game_motion.h"
#include "rr_asm.h"

/* RTPT projects the inner endpoint twice; the last two SXY stores share a slot. */
#define RING_STORE_SXY_PAIR(outer, inner) \
    __asm__ volatile( \
        "swc2 $12,0(%0); swc2 $13,0(%1); swc2 $14,0(%1)" \
        : : "r"(outer), "r"(inner) : "memory")

void func_8005599C(char *view, char *lighting, RrMatrixWords32 *first, RrMatrixWords32 *second);
s32 *func_80090F74(s32 *matrix, s32 *scale);
void func_8006EE3C(s16 *output, s16 *view, s32 *matrix);
s32 func_800916D4(s32 *point, s32 *screen, s32 *interpolation, s32 *flags);
s32 func_80090424(s32 angle);
s32 func_800904F4(s32 angle);
extern int D_800A1CD0[];
extern void func_80096434();

void func_8005E900(s32 *position, s32 radius);
void func_8005ECD4(int a0);

static __inline__ s32 project_ring_origin(s32 *position, s32 radius,
    RrRingScratch48 *scratch, RrRingVertex8 *point, s32 *output)
{
    s32 depth, innerOffset;
  point->x = position[0];
  point->y = position[1];
  point->z = position[2];
  depth = func_800916D4((s32 *)point, output, output, output) >> 1;
  scratch->vectors[0].x = 0;
  scratch->vectors[0].y = radius;
  scratch->vectors[0].z = position[2];
  RR_GTE_RTPS_LDV(&scratch->vectors[0]);
  RR_GTE_SWC2_SXY2(&scratch->screen[0]);
  scratch->vectors[1].x = 0;
  innerOffset = radius - 128;
  scratch->vectors[1].y = innerOffset;
  scratch->vectors[1].z = position[2];
  RR_GTE_RTPS_LDV(&scratch->vectors[1]);
  RR_GTE_SWC2_SXY2(&scratch->screen[1]);
  return depth;
}

void func_8005E900(s32 *position, s32 radius)
{
  s32 scale[3];
  RrRingTransform24 transform;
  RrMatrixWords32 rotation;
  RrMatrixWords32 camera;
  RrRingVertex8 point;
  s32 transformed;
  RrRingScratch48 *scratch = (RrRingScratch48 *) (SCRATCHPAD + 0x7C);
  RrRingPacket24 *packet;
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
  func_8005599C((char *)&transform, (char *)D_800ACA28 + 0x20, &rotation, &camera);
  scale[2] = 1024;
  scale[1] = 1024;
  scale[0] = 1024;
  func_80090F74(rotation.w, scale);
  func_8006EE3C(scratch->origin, (s16 *)&transform, rotation.w);
  scratch->red = 220;
  scratch->green = 160;
  scratch->blue = 0;
  depth = project_ring_origin(position, radius, scratch, &point, &transformed);
  segment = 0;
  inner = radius - 128;
  orderOffset = depth * 4;
  packet = *((RrRingPacket24 **) SCRATCHPAD);
  do
  {
    angle = segment << 7;
    scratch->vectors[2].x = (func_80090424(angle) * radius) / 4096;
    scratch->vectors[2].y = (func_800904F4(angle) * radius) / 4096;
    scratch->vectors[2].z = position[2];
    scratch->vectors[3].x = (func_80090424(angle) * inner) / 4096;
    scratch->vectors[3].y = (func_800904F4(angle) * inner) / 4096;
    /* One word cursor serves the GTE input and then its projected output. */
    gteWords = (u32 *)&scratch->vectors[2];
    scratch->vectors[3].z = position[2];
    RR_GTE_RTPT_LDV(gteWords, &scratch->vectors[3], &scratch->vectors[3]);
    gteWords = (u32 *)&scratch->screen[2];
    RING_STORE_SXY_PAIR(gteWords, &scratch->screen[3]);
    packet->tag.bits.len = 5;
    packet->code = 0x28;
    packet->xy[0][0] = scratch->screen[0].x;
    packet->xy[0][1] = scratch->screen[0].y;
    packet->xy[1][0] = scratch->screen[1].x;
    packet->xy[1][1] = scratch->screen[1].y;
    packet->xy[2][0] = scratch->screen[2].x;
    packet->xy[2][1] = scratch->screen[2].y;
    packet->xy[3][0] = scratch->screen[3].x;
    packet->xy[3][1] = scratch->screen[3].y;
    segment++;
    packet->red = scratch->red;
    packet->green = scratch->green;
    packet->blue = scratch->blue;
    packet->tag.raw = (packet->tag.raw & 0xFF000000) |
        ((RrRingGpuTag *)(orderOffset +
            (unsigned long)((RrRingGpuSlots *)SCRATCHPAD)->order.pointer))->bits.addr;
    head = (u32 *)(orderOffset +
        (unsigned long)((RrRingGpuSlots *)SCRATCHPAD)->order.pointer);
    ((RrRingGpuTag *)head)->bits.addr = (unsigned long)packet & 0xFFFFFF;
    scratch->screen[0] = scratch->screen[2];
    scratch->screen[1] = scratch->screen[3];
    packet++;
  }
  while (segment < 33);
  *((RrRingPacket24 **) SCRATCHPAD) = packet;
}

/* legacy typing of shared globals in this part of the unit */
#define D_800ACA28 (*(int (*))&D_800ACA28)

void func_8005ECD4(int a0) {
    int *p = (int *)(D_800ACA28 + 0x134);
    func_80096434(p, 0x28);
    p[1] = 0;
    p[5] = D_800A1CD0[a0];
}

#undef D_800ACA28
