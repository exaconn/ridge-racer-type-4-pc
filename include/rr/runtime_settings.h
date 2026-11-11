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
/* Saved options block (0x800AD528, 0x40 bytes; copied whole to and from the
 * save file). The PS1 map names the tail separately: CdMusicMasterVolume
 * (+0x30), CdAudioMixChannelMode (+0x32) and SelectedStoryBranchId (+0x3C). */
typedef struct {
 PulseSetting4 pulse[2];      /* +0x00 digital pad layout, vibration off */
 PadCalibration10 pad[2];     /* +0x08 analogue pad calibration */
 WheelCalibration4 wheel[2];  /* +0x28 wheel/jogcon calibration */
 u8 bgmVolume30,volume31;     /* +0x30 music and effect volume, 0..16 */
 s16 mono32;                  /* +0x32 non-zero: mono output */
 s32 screenX34,screenY38;     /* +0x34 display offset */
 s32 storyBranch3C;           /* +0x3C epilogue story branch 0..4 */
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
RR_ASSERT_OFFSET(RuntimeSettings,pad,0x8);
RR_ASSERT_OFFSET(RuntimeSettings,wheel,0x28);
RR_ASSERT_OFFSET(RuntimeSettings,bgmVolume30,0x30);
RR_ASSERT_OFFSET(RuntimeSettings,volume31,0x31);
RR_ASSERT_OFFSET(RuntimeSettings,mono32,0x32);
RR_ASSERT_OFFSET(RuntimeSettings,screenX34,0x34);
RR_ASSERT_OFFSET(RuntimeSettings,storyBranch3C,0x3C);
#endif
