#ifndef RR_RUNTIME_SETTINGS_H
#define RR_RUNTIME_SETTINGS_H
#include "rr/layout.h"
typedef struct {s16 buttonLayout00,disabled02;} PulseSetting4;
typedef struct {
 u16 center00,offset02,offset04,offset06;
 s16 deadzone08,range0A,buttonLayout0C,reserved0E;
} PadCalibration10;
typedef struct {
 s8 deadzone00,range01,rumble02,buttonLayout03;
} WheelCalibration4;
typedef struct {
    u8 reserved00[0x30];
    u8 bgmVolume30,volume31;
    s16 mono32;
} AudioSettings;
typedef struct {u8 reserved00[0x34];s32 x34,y38;} DisplaySettings3C;
/* Resident5C0C4 initializes both controller settings and the trailing display
 * words. Frame setup reads the low halfwords of those same signed32 words. */
typedef union {
 u8 bytes[0x40];u16 halves[32];s32 words[16];
 AudioSettings audio;
 struct {PulseSetting4 pulse[2];u8 remainder08[0x38];} controls;
 struct {
  PulseSetting4 f0[2];
  PadCalibration10 p[2];
  WheelCalibration4 w[2];
  u8 f30,f31;
  s16 f32;
  s32 f34,f38,f3C;
 } config;
 DisplaySettings3C display;
} RuntimeSettings;
extern RuntimeSettings Options;
RR_ASSERT_SIZE(PulseSetting4,4);
RR_ASSERT_OFFSET(PulseSetting4,buttonLayout00,0);
RR_ASSERT_OFFSET(PulseSetting4,disabled02,2);
RR_ASSERT_SIZE(PadCalibration10,0x10);
RR_ASSERT_OFFSET(PadCalibration10,buttonLayout0C,0xC);
RR_ASSERT_SIZE(WheelCalibration4,4);
RR_ASSERT_OFFSET(WheelCalibration4,buttonLayout03,3);
RR_ASSERT_SIZE(RuntimeSettings,0x40);
RR_ASSERT_SIZE(AudioSettings,0x34);
RR_ASSERT_OFFSET(DisplaySettings3C,x34,0x34);
RR_ASSERT_OFFSET(DisplaySettings3C,y38,0x38);
#endif
