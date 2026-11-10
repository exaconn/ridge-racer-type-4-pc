#ifndef RR_OVERLAY668_RESOURCE_H
#define RR_OVERLAY668_RESOURCE_H
#include "rr/overlay668_playback_init.h"
#include "rr/audio_state_types.h"
typedef AudioStreamEntry StrArchiveEntry;
/* Resource addresses an aligned8-byte entry at archive+130+resource*8.
 * CD conversion writes three location bytes through the full file pointer.
 * Preserve the captured entry across the service, then reload its sector
 * count. The archive root may change during that service. Output sectors may
 * alias file.byteSize, context threshold/budget or an archive entry word.
 * Arithmetic for sector offsets, byte size and budgets wraps32 bits. */
RR_ASSERT_SIZE(StrArchiveEntry,8);
#endif
