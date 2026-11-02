#ifndef RR_RUNTIME_SETTINGS_H
#define RR_RUNTIME_SETTINGS_H
#include "rr/layout.h"
typedef struct {s16 unused00,disabled02;} RrPulseSetting4;
typedef struct {u8 pad00[0x31];u8 volume31;s16 mono32;} RrAudioSettings;
typedef struct {u8 reserved00[0x34];s32 x34,y38;} RrDisplaySettings3C;
/* Resident5C0C4 initializes both controller settings and the trailing display
 * words. Frame setup reads the low halfwords of those same signed32 words. */
typedef union {
 u8 bytes[0x3C];u16 halves[30];
 RrAudioSettings audio;
 struct {RrPulseSetting4 pulse[2];u8 remainder08[0x34];} controls;
 RrDisplaySettings3C display;
} RrRuntimeSettings;
extern RrRuntimeSettings D_800AD528;
RR_ASSERT_SIZE(RrRuntimeSettings,0x3C);
RR_ASSERT_SIZE(RrAudioSettings,0x34);
RR_ASSERT_OFFSET(RrDisplaySettings3C,x34,0x34);
RR_ASSERT_OFFSET(RrDisplaySettings3C,y38,0x38);
#endif
