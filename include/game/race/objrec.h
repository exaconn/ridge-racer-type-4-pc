#ifndef RR_GAME_RACE_OBJREC_H
#define RR_GAME_RACE_OBJREC_H

#include "rr/layout.h"

/* Records of the object pools at TrackObjectStates (0x2C bytes) and TrackObjectDefs (0x30 bytes). */

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    unsigned short f1C;
    unsigned short f1E;
    unsigned short f20;
    unsigned short f22;
    int f24;
    int f28;
} S_ECB4;

typedef struct {
    int f0;
    int f4;
    int f8;
    int fC;
    int f10;
    int f14;
    int f18;
    int f1C;
    int f20;
    int f24;
    int f28;
    int f2C;
} S_ECB0;

typedef struct {
    int f00;
    int f04;
    int f08;
    int f0C;
    int f10;
    int f14;
    int f18;
    short f1C;
    short f1E;
    short f20;
    short f22;
} Node24;

/* Counted definitions are followed by Node24 keyframes in the same file. */
typedef struct {
    s32 count;
#if defined(__GNUC__) && __GNUC__ < 3
    S_ECB0 definitions[0];
#else
    S_ECB0 definitions[];
#endif
} TrackObjectFile;

RR_ASSERT_SIZE(S_ECB4, 0x2C);
RR_ASSERT_SIZE(S_ECB0, 0x30);
RR_ASSERT_SIZE(Node24, 0x24);
RR_ASSERT_OFFSET(TrackObjectFile, definitions, 4);

typedef struct {
    int vx, vy, vz, reserved0C;
} ObjectTrailPoint16;

extern Node24 *TrackObjectKeyframes;
extern S_ECB0 *TrackObjectDefs;
extern S_ECB4 *TrackObjectStates;
extern ObjectTrailPoint16 TrackObjectTrailPoints[];
/* Legacy address labels alias the same selected definition/key/state roots. */
extern Node24 *D_8010E8D8;
extern S_ECB0 *D_8010E8DC;
extern S_ECB4 *D_8010E8E0;

#endif
