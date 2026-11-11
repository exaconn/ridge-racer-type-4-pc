/* Compile-time ABI checks: the game's SDK types against the PsyZ (__psyz)
 * declarations they mirror. Only type headers are included on the game side,
 * so no function prototype is declared twice; game types that reuse an SDK
 * name are renamed while their header is read. This unit emits no code. */
#include <stddef.h>
#include <libgpu.h>
#include <libgte.h>
#include <libcd.h>
#include <libspu.h>
#include <libsnd.h>

/* Function names the game headers declare with game types. */
#define MoveImage2 RrMoveImage2
#define Square0 RrSquare0
#define Square12 RrSquare12
#define CdSearchFile RrCdSearchFile
#include "rr/gpu_environment_types.h"
#include "rr/matrix_words.h"
#include "rr/geometry.h"

#define VECTOR RrVECTOR
#define SVECTOR RrSVECTOR
#include "psyq/libgte/libgte.h"
#undef VECTOR
#undef SVECTOR

#define CdlFILE RrCdlFILE
#include "psyq/libcd/cdfile.h"
#undef CdlFILE

#define SpuVolume RrSpuVolume
#define SpuVoiceAttr RrSpuVoiceAttr
#include "psyq/libspu/libspu.h"
#undef SpuVolume
#undef SpuVoiceAttr

#define VabHdr RrVabHdr
#define ProgAtr RrProgAtr
#define VagAtr RrVagAtr
#include "psyq/libsnd/vab_format.h"
#undef VabHdr
#undef ProgAtr
#undef VagAtr

#define SAME_SIZE(game, sdk) \
    _Static_assert(sizeof(game) == sizeof(sdk), #game " size != " #sdk)
#define SAME_FIELD(game, gfield, sdk, sfield) \
    _Static_assert(offsetof(game, gfield) == offsetof(sdk, sfield), \
                   #game "." #gfield " != " #sdk "." #sfield)

/* libgpu: tags, rectangles, state packets and the environments. */
SAME_SIZE(GpuDmaTag, OT_TYPE);
SAME_FIELD(GpuDmaTag, length, P_TAG, len);
SAME_FIELD(GpuPrimitiveHeader, color, P_TAG, r0);
SAME_SIZE(GpuRectangleFields, RECT);
SAME_SIZE(GpuRectangle, RECT);
SAME_SIZE(GpuDrawEnvironmentPacket, DR_ENV);
SAME_FIELD(GpuDrawEnvironmentPacket, code, DR_ENV, code);
SAME_SIZE(GpuDrawModePacket, DR_MODE);
SAME_SIZE(GpuDrawModePacket, DR_STP);
SAME_SIZE(GpuDrawModePacket, DR_AREA);
SAME_FIELD(GpuDrawModePacket, command, DR_MODE, code);
SAME_SIZE(DrawEnvironment, DRAWENV);
SAME_FIELD(DrawEnvironment, clip, DRAWENV, clip);
SAME_FIELD(DrawEnvironment, offset_x, DRAWENV, ofs);
SAME_FIELD(DrawEnvironment, window, DRAWENV, tw);
SAME_FIELD(DrawEnvironment, page, DRAWENV, tpage);
SAME_FIELD(DrawEnvironment, dither, DRAWENV, dtd);
SAME_FIELD(DrawEnvironment, draw_display, DRAWENV, dfe);
SAME_FIELD(DrawEnvironment, background, DRAWENV, isbg);
SAME_FIELD(DrawEnvironment, r, DRAWENV, r0);
SAME_FIELD(DrawEnvironment, b, DRAWENV, b0);
SAME_FIELD(DrawEnvironment, draw_packet1C, DRAWENV, dr_env);
SAME_SIZE(DisplayEnvironment, DISPENV);
SAME_FIELD(DisplayEnvironment, screen, DISPENV, screen);
SAME_FIELD(DisplayEnvironment, flags10, DISPENV, isinter);

/* libgte */
SAME_SIZE(MatrixWords32, MATRIX);
SAME_FIELD(MatrixFields32, translation14, MATRIX, t);
SAME_SIZE(RrVECTOR, VECTOR);
SAME_SIZE(RrSVECTOR, SVECTOR);
SAME_SIZE(Vector, VECTOR);
SAME_SIZE(Vec3s8, SVECTOR);

/* libcd */
SAME_SIZE(RrCdlFILE, CdlFILE);
SAME_FIELD(RrCdlFILE, size, CdlFILE, size);
SAME_FIELD(RrCdlFILE, name, CdlFILE, name);

/* libspu */
SAME_SIZE(RrSpuVoiceAttr, SpuVoiceAttr);
SAME_FIELD(RrSpuVoiceAttr, volume, SpuVoiceAttr, volume);
SAME_FIELD(RrSpuVoiceAttr, addr, SpuVoiceAttr, addr);
SAME_FIELD(RrSpuVoiceAttr, a_mode, SpuVoiceAttr, a_mode);
SAME_FIELD(RrSpuVoiceAttr, adsr2, SpuVoiceAttr, adsr2);

/* libsnd VAB records (disc format, identical on every host) */
SAME_SIZE(RrVabHdr, VabHdr);
SAME_SIZE(RrVagAtr, VagAtr);
SAME_FIELD(RrVagAtr, adsr1, VagAtr, adsr1);
SAME_FIELD(RrVagAtr, vag, VagAtr, vag);
SAME_SIZE(RrProgAtr, ProgAtr);
