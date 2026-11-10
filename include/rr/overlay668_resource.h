#ifndef RR_OVERLAY668_RESOURCE_H
#define RR_OVERLAY668_RESOURCE_H
#include "rr/overlay668_playback_init.h"
/* View of the full audio allocation cleared by8004EC28. The movie directory
 * fields are live in the previously opaque2634..263B region. Pointer fields
 * keep their host width in native builds; the PAL layout remains unchanged. */
typedef struct {
 u8 reserved00[0x2634];u32 sector2634;u8 *archive2638;
 u8 reserved263C[0xA8D0-0x263C];
} StrArchiveAudioState;
typedef struct {u32 position,sectors;} StrArchiveEntry;
extern StrArchiveAudioState SoundState;
/* Resource addresses an aligned8-byte entry at archive+130+resource*8.
 * CD conversion writes three location bytes through the full file pointer.
 * Preserve the captured entry across the service, then reload its sector
 * count. The archive root may change during that service. Output sectors may
 * alias file.byteSize, context threshold/budget or an archive entry word.
 * Arithmetic for sector offsets, byte size and budgets wraps32 bits. */
RR_ASSERT_SIZE(StrArchiveAudioState,0xA8D0);
RR_ASSERT_OFFSET(StrArchiveAudioState,archive2638,0x2638);
RR_ASSERT_SIZE(StrArchiveEntry,8);
#endif
