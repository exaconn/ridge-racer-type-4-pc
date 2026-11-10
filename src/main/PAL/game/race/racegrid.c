#include "game/sound/sndque.h"
#include "game/ui/badgeprim.h"
#include "game/ui/badge1.h"
#include "rr/gpu_scratch_types.h"
#include "game/car/carbound.h"
#include "psyq/libgte/gtemat5.h"
#include "rr/frame_storage_types.h"
#include "psyq/libgte/gtemat4.h"
#include "psyq/libgte/gtemat3.h"
/* RODATA 0x800105d0 0x30 */
#include "types.h"
#include "game/car/car.h"
#include "scratchpad.h"
#include "game/system/pad.h"
#include "game/ui/hudmap.h"
#include "game/race/raceai2.h"
#include "game/race/racegrid.h"
#include "rr/race.h"
#include "rr/menu_selection.h"
#include "rr/matrix_workspace.h"
#include "rr/audio_loop.h"
#include "rr/driver_control.h"
#include "rr/bios_events.h"
#include "rr/serial_handles.h"
#include "game/gfx/gfxotag.h"
#include "game/math/rotmat.h"
#include "game/system/pad3.h"

typedef struct { int a, b, c, d; } w3g2V;

typedef struct {
    short x, y;
} V;

typedef struct {
    short x, y, z;
} Vec3s6;

extern Vec3s6 D_8009F694[];
extern Vec3s6 D_8009F6A0[];
extern short AngleDiffSigned();
extern int ViewUsesCarTrackNode;
extern s32 RaceState;
extern int RaceVoiceFlags;
extern short FadeCountdown;
extern void PlaySfx();
extern unsigned char D_8009F6DC[];
extern signed char PauseCursor;
extern int D_8009F6E4[];
extern char SelectionState[];
extern int RankEntries;
extern int TrackLength;
extern short ModeIndex;
extern int TotalRaceTime;
extern short ModeGroup;
extern short SubmodeState;
extern int GameQueueSprite();
extern int EmitPolyF3();
extern unsigned char D_8009F71C[][4];
extern int SioDeviceName[];
extern int LinkWriteHandle;
extern int LinkReadHandle;
extern s16 LinkState;
extern int D_8009F728;
extern signed char LinkEntryState[];
extern signed char D_800ABEBE;
extern int RaceTimeRemaining;
extern short LinkHostFlag;
extern int D_800F3A64;

void SetupMenuCarCamera(int a);
void UpdateRaceChaseCamera(RaceVehicle *p);
void PlayStartCountdownVoice(int a0);
void InitPlayerRumble(void);
void UpdatePauseMenu(int a0);
void ApplyPauseMenuChoice(int arg0);
void DrawPauseMenu(u32 *a0, int a1);
int LinkTimeoutCallback(int a0, unsigned int a1);
void BeginLinkSync(void);
int ExitOnLinkLost(void);
int UpdateLinkHandshake(int a0);
int PollLinkStatus(void);

void SetupMenuCarCamera(int a)
{
  int *g = (int *) SCRATCHPAD;
  int i;
  g[2] = D_8009F6A0[a].x;
  g[3] = D_8009F6A0[a].y;
  g[4] = D_8009F6A0[a].z;
  g[6] = D_8009F694[a].x;
  g[7] = D_8009F694[a].y;
  g[8] = D_8009F694[a].z;
  BuildCameraMatrices();
  ScaleMatrixRow1(2, (MatrixWords32 *)SCRATCH_ROT_MATRIX);
  if (PlayerControllers[ActivePlayerIndex & 1].input00.fields.buttons & 4)
  {
    PlayerCars[ActivePlayerIndex & 1].lateralMotion178 += 0xC0;
  }
  else
    if (PlayerControllers[ActivePlayerIndex & 1].input00.fields.buttons & 8)
  {
    PlayerCars[ActivePlayerIndex & 1].lateralMotion178 -= 0xC0;
  }
  i = PlayerCars[ActivePlayerIndex & 1].lateralMotion178;
  if (i > 0xF00)
  {
    PlayerCars[ActivePlayerIndex & 1].lateralMotion178 = 0xF00;
  }
  else
    if (i < (-0xF00))
  {
    PlayerCars[ActivePlayerIndex & 1].lateralMotion178 = -0xF00;
  }
}

void UpdateRaceChaseCamera(RaceVehicle *p) {
    struct { char a[0x20]; char b[0x20]; short v[3]; short reserved46; int w[3]; } s;
    int *g = (int *)SCRATCHPAD;
    int r;
    short t;
    unsigned short a1, a2;
    ViewUsesCarTrackNode = 1;
    if ((u16)p->flags1E4 & 0x100) {
        BuildRotMatrixY(s.a, 0x800 - p->rot54);
        BuildRotMatrixX(s.b, p->rot50);
        MulMatrix((MatrixWords32 *)(s.a), (const MatrixWords32 *)(s.b));
        BuildRotMatrixZ(s.b, p->rot58);
        MulMatrix2((const MatrixWords32 *)(s.a), (MatrixWords32 *)(s.b));
        s.v[0] = 0;
        s.v[1] = -0x64;
        if (p->height108 < 0x28) s.v[1] = (unsigned short)p->height108 - 0x8C;
        s.v[2] = 0;
        ApplyMatrix((const MatrixWords32 *)(s.b), (const Vec3s8 *)(s.v), (Vec3i *)(s.w));
        *(w3g2V *)&g[2] = *(w3g2V *)&p->pos10[0];
        g[2] -= p->ir40[0];
        g[4] -= p->ir40[2];
        *(w3g2V *)&g[6] = *(w3g2V *)&p->rot50;
        g[6] += p->renderAngleX1AC * 2;
        a1 = (u16)p->shakeRollOffset1B6;
        a2 = (u16)p->renderAngleZ1AE;
        g[2] += s.w[0];
        g[3] += s.w[1];
        g[4] += s.w[2];
        g[8] += ((short)a1 / 2) + ((short)a2 / 2);
        return;
    }
    s.v[0] = AngleDiffSigned(g[6], p->rot50) / 8;
    r = AngleDiffSigned(g[7], p->rot54);
    t = r / 6;
    g[6] += s.v[0];
    s.v[1] = t;
    g[7] += t;
    BuildRotMatrixY(s.a, 0x800 - g[7], g[7]);
    BuildRotMatrixX(s.b, g[6]);
    MulMatrix((MatrixWords32 *)(s.a), (const MatrixWords32 *)(s.b));
    BuildRotMatrixZ(s.b, g[8]);
    MulMatrix2((const MatrixWords32 *)(s.a), (MatrixWords32 *)(s.b));
    s.v[0] = 0;
    s.v[1] = -0xFA;
    if (p->height108 < 0x28) s.v[1] = (unsigned short)p->height108 - 0x122;
    s.v[2] = 0x2D0;
    ApplyMatrix((const MatrixWords32 *)(s.b), (const Vec3s8 *)(s.v), (Vec3i *)(s.w));
    *(w3g2V *)&g[2] = *(w3g2V *)&p->pos10[0];
    g[8] = p->rot58 - p->wheelRotZ1AA;
    g[2] += s.w[0];
    g[3] += s.w[1];
    g[4] += s.w[2];
}

void DrawStartCountdown(u32 *ordering, s32 frame) {
    UiUvRect glyph;
    s32 y;
    s32 index;
    s32 xOffset;
    s32 phase;
    s32 top;
    s32 tick;
    s32 palette;
    s32 intensity;
    void *cursor;

    if (RaceState != 0 && (s32)(frame - 0x4BU) < 0xAF) {
        tick = frame % 25;
        phase = (frame - 0x4B) / 25;
        if ((&CurrentDrawBuf->buffer.ordering70.words[0]) == ordering) {
            top = 0;
            if (phase == 6) {
                top = -(tick * 128) / 25;
            }
        } else {
            ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawAreaPrim(ordering, ((GpuScratchPointers *)SCRATCHPAD)->cursor, 0, 0, 0x140, 0x100);
            top = 0x40;
        }
        cursor = AddDrawModePrim(ordering, (GpuDrawModePacket *)((GpuScratchPointers *)SCRATCHPAD)->cursor, 5);
        if (phase == 1) {
            intensity = ((tick * 64) / 25) + 0x40;
        } else if (phase >= 4) {
            intensity = 0xA4;
            if (phase >= 5) {
                intensity = 0x80;
            }
        } else {
            intensity = 0x80;
            if (phase <= 0) {
                intensity = 1;
            }
        }
        if (GameModeType == 1) {
            if (phase >= 4) {
                palette = ((frame >> 2) & 1) + 2;
                glyph = D_8009F6AC[7];
                cursor = AddSpritePrim(ordering, cursor, D_8009F6D8[3] + 0x60, top + 0x39, &glyph, D_8009F6D0[palette], 0);
            } else if (phase > 0) {
                xOffset = 0;
                for (index = 0; index < 0x10;) {
                    s32 b, c;

                    if (index > tick - 7) {
                        break;
                    }
                    b = index & 1;
                    c = b << 1;
                    cursor = AddSpritePrim(ordering, cursor, xOffset + 0x78, top + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    index++;
                    xOffset += 1 << (c + 1);
                }
                palette = 0;
                glyph = D_8009F6AC[5];
                glyph = D_8009F6AC[6];
                glyph.u += (phase - 1) * 12;
                cursor = AddSpritePrim(ordering, cursor, D_8009F6D8[2] + 0x60, top + 0x39, &glyph, D_8009F6D0[0], 0);
            }
        } else {
            if (phase == 0) {
                xOffset = 0;
                for (index = 0; index < 0x10;) {
                    s32 b, c;

                    if (index > tick - 6) {
                        break;
                    }
                    b = index & 1;
                    c = b << 1;
                    cursor = AddSpritePrim(ordering, cursor, xOffset + 0x78, top + 0x39, &D_8009F6AC[2 + b], D_8009F6D0[b * 2], 0);
                    index++;
                    xOffset += 1 << (c + 1);
                }
            } else if (phase >= 4) {
                palette = ((frame >> 2) & 1) + 2;
                glyph = D_8009F6AC[5];
                glyph.u += 0x60;
                for (index = 0; index < 3; index++) {
                    if (index == 2) {
                        glyph = D_8009F6AC[7];
                        index = 3;
                    }
                    cursor = AddSpritePrim(ordering, cursor, D_8009F6D8[index] + 0x60, top + 0x39, &glyph, D_8009F6D0[palette], 0);
                }
            } else {
                palette = 0;
                glyph = D_8009F6AC[5];
                for (index = 0; index < 3; index++) {
                    if (index == 0) {
                        glyph.u += ((tick * 9) / 25) * 12;
                    } else if (index == 1) {
                        glyph.u = D_8009F6AC[5].u + phase * 0x18;
                    } else {
                        glyph = D_8009F6AC[6];
                        glyph.u += (phase - 1) * 12;
                    }
                    cursor = AddSpritePrim(ordering, cursor, D_8009F6D8[index] + 0x60, top + 0x39, &glyph, D_8009F6D0[palette], 0);
                }
            }
        }
        if (phase > 0) {
            y = top + 0x39;
            xOffset = 0;
            for (index = 0; index < 10; index++) {
                cursor = AddSpritePrim(ordering, cursor, xOffset + 0x78, y, &D_8009F6AC[4], D_8009F6D0[(index - 3U) < 4U ? 3 : palette], 0);
                xOffset += 8;
            }
        }
        cursor = AddSpritePrim(ordering, cursor, 0x68, top + 0x1A, &D_8009F6AC[1], D_8009F6CC[phase >= 4], (intensity << 1) | 1);
        cursor = AddSpritePrim(ordering, cursor, 0x60, top, &D_8009F6AC[0], 0x797C, 0);
        ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawModePrim(ordering, cursor, 0x3F);
        if (ordering == &CurrentDrawBuf->buffer.secondaryOrderingB70.words[0]) {
            s32 y;
            if (phase == 6) {
                intensity = ((0x19 - tick) * 0x18) / 25;
            } else {
                intensity = 0x18;
            }
            y = (0x18 - intensity) / 2 + 0x34;
            ((GpuScratchPointers *)SCRATCHPAD)->cursor = AddDrawAreaPrim(ordering, ((GpuScratchPointers *)SCRATCHPAD)->cursor, 0x70, top + y, 0x60, intensity);
        }
    }
}

void PlayStartCountdownVoice(int a0) {
    if (RaceState != 0 && a0 < 0x115) {
        if (a0 == 0xC3) {
            FadeCountdown = 0x1E;
        }
        if (a0 == 0x65) {
            if (!(RaceVoiceFlags & 1)) {
                SoundQueueEnqueue(0, 0);
                RaceVoiceFlags |= 1;
                return;
            }
        }
        if (a0 == 0x7E) {
            if (!(RaceVoiceFlags & 2)) {
                SoundQueueEnqueue(1, 0);
                RaceVoiceFlags |= 2;
                return;
            }
        }
        if (a0 == 0x97) {
            SoundQueueEnqueue(2, 0);
            return;
        }
        if (a0 == 0xB0) {
            if (!(RaceVoiceFlags & 4)) {
                SoundQueueEnqueue(3, 0);
                RaceVoiceFlags |= 4;
                return;
            }
        }
        if (a0 == 0xE2) {
            if (!(RaceVoiceFlags & 8)) {
                SoundQueueEnqueue(0x17, 0);
                RaceVoiceFlags |= 8;
                return;
            }
        }
        if (a0 == 0x114) {
            if (!(RaceVoiceFlags & 0x10)) {
                SoundQueueEnqueue(0x18, 0);
                RaceVoiceFlags |= 0x10;
            }
        }
    }
}

void InitPlayerRumble(void) {
    int i;
    PadClearPulse(0);
    PadClearPulse(1);
    for (i = 0; i < 2; i++) {
        if ((s16)PlayerCars[i].flags1E4 != 0) {
            PlayerControllers[i].rumbleActive50 = 1;
            PadSetRumbleParams(i, 0, 0xF);
        }
    }
}

void UpdatePauseMenu(int a0) {
    if (Pad0Buttons & 0x1000) {
        if (PauseCursor > 0) {
            PauseCursor = PauseCursor - 1;
            PlaySfx(0);
        }
    }
    if (Pad0Buttons & 0x4000) {
        if (PauseCursor < D_8009F6DC[a0]) {
            PauseCursor = PauseCursor + 1;
            PlaySfx(0);
        }
    }
    InitPlayerRumble();
}

void ApplyPauseMenuChoice(int arg0) {
    int *t;
    int b;
    if (PauseCursor == 0) {
        FadeCountdown = 0x1E;
        return;
    }
    SelectionState[0x19] = 0;
    TotalRaceTime = -1;
    if (arg0 == 1) {
        SelectionState[0x19] = 9;
        RaceState = 6;
        RankEntries = -TrackLength;
        if (PauseCursor == 2) {
            SubmodeState = 0;
            ModeGroup = D_8009F6E4[3] >> 8;
            ModeIndex = *(unsigned char *)&D_8009F6E4[3];
            return;
        }
        FinishRace(SelectionState, PauseCursor);
        return;
    }
    b = D_8009F6E4;
    RaceState = 6;
    t = (int *)((PauseCursor - 1) * 4 + arg0 * 8 + b);
    ModeGroup = *t >> 8;
    {
        int v = *(unsigned char *)t;
        SubmodeState = 0;
        ModeIndex = v;
    }
}

void DrawPauseMenu(u32 *a0, int a1) {
    V v[4];
    unsigned char rgb[4];
    int p;
    int i, j;
    unsigned char (*base)[4];
    unsigned char *t;

    p = GameQueueSprite(a0, *(int *)SCRATCHPAD, 0x90, 0x5C, 0x1C, 8, 0x60, 0xA0, 0x7FF8);
    p = GameQueueSprite(a0, p, 0x90, 0x6A, 0x20, 8, 0xC4, 0xA0, 0x7FF8);
    if (GameModeType == 1) {
        p = GameQueueSprite(a0, p, 0x90, 0x74, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    } else {
        p = GameQueueSprite(a0, p, 0x90, 0x74, 0x24, 8, 0x9C, 0xA0, 0x7FF8);
        p = GameQueueSprite(a0, p, 0x90, 0x7E, 0x1C, 8, 0x7C, 0xA0, 0x7FF8);
    }
    p = GameQueueSprite(a0, p, 0x88, a1 * 10 + 0x6A, 8, 8, 0xE4, 0xA0, 0x7FF8);

    for (i = 0; i < 9; i++) {
        for (j = 0; j < 4; j++) {
            int kx = ((j & 1) << 5) + 0x70;
            v[j].x = ((i % 3) << 5) + kx;
            {
                int ky = ((j >> 1) << 4) + 0x56;
                v[j].y = ((i / 3) << 4) + ky;
            }
        }
        rgb[0] = 0xFF;
        rgb[2] = 0x20;
        rgb[1] = 0x20;
        j = i / 3;
        j = j == 1;
        base = D_8009F71C;
        t = base[j];
        p = EmitPolyF3(a0, p, v[t[0]], v[t[1]], v[t[2]], rgb, 1);
        rgb[0] = 0xFF;
        rgb[2] = 0xFF;
        rgb[1] = 0xFF;
        p = EmitPolyF3(a0, p, v[t[1]], v[t[2]], v[t[3]], rgb, 1);
    }
    *(int *)SCRATCHPAD = AddDrawModePrim(a0, p, 5);

    for (j = 0; j < 4; j++) {
        if (j == 1 || j == 2) {
            v[j].x = 0xD0;
        } else {
            v[j].x = 0x70;
        }
        if (j >= 2) {
            v[j].y = 0x86;
        } else {
            v[j].y = 0x56;
        }
    }

    for (i = 0; i < 3; i++) {
        j = 0x40;
        AddPartialLine(a0, v[i].x, v[i].y, v[i + 1].x, v[i + 1].y, 0x100, 0, j, j, j, -1);
    }
}

void ResetLinkSioControl(void) {
    _sio_control(1, 1, 1);
}

int IsLinkPeerPresent(void) {
    return (((unsigned)_sio_control(0, 0, 0) >> 7) & 3) - 1 < 2;
}

int LinkTimeoutCallback(int a0, unsigned int a1) {
    return a1 <= 0x1869E;
}

void OpenLinkSerial(void) {
    _sio_control(1, 3, 0x7E900);
    _sio_control(1, 4, 1);
    LinkWriteHandle = FileOpen(SioDeviceName, 2);
    LinkReadHandle = FileOpen(SioDeviceName, 1);
    _sio_control(4, 0, LinkTimeoutCallback);
    _sio_control(2, 0, 0);
}

void CloseLinkSerial(void) {
    _sio_control(2, 3, 0);
    _sio_control(2, 0, 0);
    FileClose(LinkWriteHandle);
    FileClose(LinkReadHandle);
    _sio_control(4, 0, 0);
}

void BeginLinkSync(void) {
    if (LinkState == 2) {
        LinkState = 3;
        _sio_control(1, 1, 2);
    }
}

int ExitOnLinkLost(void) {
    if (LinkState == -1) {
        ModeGroup = 1;
        ModeIndex = 0x2D;
        SubmodeState = 0;
        _sio_control(1, 1, 0);
        return 1;
    }
    return 0;
}

int UpdateLinkHandshake(int a0)
{
  int r = 0;
  long new_var2;
  int st;
  int mm;
  signed char *t;
  signed char *bp;
  short z;
  int xx;
  int q3;
  ControllerState5C (*e)[2];
  signed char *new_var;
  unsigned int k = ((unsigned int)_sio_control(0, 0, 0) >> 7) & 3;
  short new_var3;
  st = (short) (LinkState + 1);
  switch (st)
  {
    case 1:
      if (_sio_control(3, 1, 0) != 0)
    {
      _sio_control(1, 1, 2);
      if (a0 == 0)
      {
        LinkHostFlag = 0;
        D_800ABEBE = 1;
        D_8009F728 = 3;
        LinkState = 10;
      }
      else
      {
        goto s2;
      }
      break;
    }
      _sio_control(1, 1, 2);
      if (a0 == 0)
    {
      LinkHostFlag = 1;
      LinkEntryState[0] = 1;
      LinkState = 1;
    }
    else
      if (k != 1)
    {
      LinkState = 1;
    }
    else
    {
      goto s2;
    }
      break;

    case 11:
      if (D_8009F728 > 0)
    {
      _sio_control(1, 1, 2);
      D_8009F728 -= 1;
      if (k == 1)
      {
        LinkState = 2;
        D_8009F728 = 0;
      }
      break;
    }
      s2:
    LinkState = 2;

      break;

    case 2:
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
    else
      if (k == 2)
    {
      LinkState = k;
      D_8009F728 = 4;
    }
      break;

    case 3:
      _sio_control(1, 1, 1);
      if (D_8009F728 > 0)
    {
      if (a0 == 0)
      {
        D_8009F728 -= 1;
        break;
      }
    }
    else
      if (a0 == 0)
    {
      new_var = LinkEntryState;
      xx = LinkHostFlag;
      z = 3;
      new_var2 = xx * 2;
      bp = new_var;
      t = &bp[z - new_var2];
      if ((*t) == 0)
      {
        e = &PlayerControllers;
        new_var3 = z;
        if ((*e)[1].action28 != 0)
        {
          PlaySfx(1);
          bp[new_var3 - (LinkHostFlag * 2)] = 1;
        }
        else
          if ((*e)[1].action2C != 0)
        {
          *t = -1;
        }
        if (D_800F3A64 != 0)
        {
          LinkEntryState[1 - (q3 = LinkHostFlag)] = -1;
          ((void (*)())_sio_control)(1, 1, 0, q3);
        }
      }
    }
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
    else
      if (a0 == 0)
    {
      int v = RaceTimeRemaining - 1;
      RaceTimeRemaining = v;
      if (v <= 0)
      {
        LinkState = 4;
        _sio_control(1, 1, 2);
      }
      else
        if (LinkEntryState[3 - (LinkHostFlag * 2)] != 0)
      {
        LinkState = 3;
        _sio_control(1, 1, 2);
      }
    }
      break;

    case 4:
      if (a0 == 0)
    {
      if (RaceTimeRemaining > 0)
      {
        RaceTimeRemaining -= 1;
      }
    }
      if ((k == 0) || ((k == 3) && (a0 == 0)))
    {
      goto m1;
    }
      if (k == 1)
    {
      break;
    }

    case 5:
    if (a0 != 2)
    {
      LinkState = 0;
    }

      r = 1;
      break;

    case 0:
      r = -1;
      break;
      m1:
    *((short *) (&LinkState)) = -1;

      break;

  }

  return r;
}

int PollLinkStatus(void) {
    int r = 0;
    int t = ((unsigned int)_sio_control(0, 0, 0) >> 7) & 3;
    if (t == 0 || t == 3) {
        LinkState = -1;
        r = -1;
    } else if (t == 1) {
        LinkState = t;
    } else if (t == 2) {
        if (LinkState == 3) r = 1;
    }
    return r;
}
