#ifndef RR_GAME_RACE_OBJREC_H
#define RR_GAME_RACE_OBJREC_H

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

#endif
