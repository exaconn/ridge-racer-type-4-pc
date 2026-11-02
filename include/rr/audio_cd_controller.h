#ifndef RR_AUDIO_CD_CONTROLLER_H
#define RR_AUDIO_CD_CONTROLLER_H
#include "rr/audio_state_types.h"
#include "rr/cd.h"
extern RrAudioState D_800F4DB8;
extern char D_8001118C[];
/* Physical alias: request26B4. */
extern s32 D_800F746C;
s32 func_8001E84C(const char*,...);
void func_80052AB8(void);void func_80052ADC(u16);void func_80052B48(s32);void func_80052C70(void);
void func_80052DA8(void);void func_80052E6C(void);void func_80052F2C(void);void func_80053120(void);void func_80053254(void);void func_80053300(void);void func_80053384(void);
s32 func_8008BFB4(void);void func_8008C03C(void);s32 func_8008C6A4(const u8*);s32 func_8008F274(s32);
/* CD parameters use a two-byte filter and three defined location bytes.
 * Active TOC indices address one of16 banks and256 complete8-byte entries.
 * Services may mutate controller fields; counters/products wrap32 bits.
 * All software pointers remain full width, including automatic CD buffers. */
void func_800534D8(void);
#endif
