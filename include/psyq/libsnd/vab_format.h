#ifndef RR_PSYQ_LIBSND_VAB_FORMAT_H
#define RR_PSYQ_LIBSND_VAB_FORMAT_H
#include "rr/layout.h"

/* VAB header followed by 64 or 128 program records and the tone tables. */
typedef struct {
    u32 form;
    s32 ver, id;
    u32 fsize;
    u16 reserved0, ps, ts, vs;
    u8 mvol, pan, attr1, attr2;
    u32 reserved1;
} VabHdr;

typedef struct {
    u8 tones, mvol, prior, mode, mpan, reserved0;
    u16 attr;
} ProgAtrAttributes;

/* Opening a VAB fills the tone-block index and the paired SPU addresses.
 * The complete parser record needs word alignment. Byte/halfword-only SDK
 * consumers use resource buffers with the alignment of the accessed field. */
typedef struct {
    ProgAtrAttributes attributes;
    u32 toneBlockIndex;
    u16 sampleAddresses[2];
} ProgAtr;

/* Tone records carry 14 byte attributes, two reserved bytes, four halfword
 * attributes and four reserved halfwords. Getter/setter transfer 24 bytes but
 * deliberately preserve the two reserved bytes at offsets 14 and 15. */
typedef struct {
    u8 prior, mode, vol, pan, center, shift, min, max;
    u8 vibW, vibT, porW, porT, pbmin, pbmax, reserved1, reserved2;
    u16 adsr1, adsr2, prog, vag;
    u16 reserved[4];
} VagAtr;
typedef u8 VagAtrBuffer[sizeof(VagAtr)];

/* Resource records remain byte-addressable when they alias other SDK owners. */
typedef u8 ProgAtrBuffer[sizeof(ProgAtr)];

RR_ASSERT_SIZE(VagAtr, 0x20);
RR_ASSERT_OFFSET(VagAtr, adsr1, 0x10);
RR_ASSERT_OFFSET(VagAtr, adsr2, 0x12);
RR_ASSERT_OFFSET(VagAtr, prog, 0x14);
RR_ASSERT_OFFSET(VagAtr, vag, 0x16);
RR_ASSERT_OFFSET(VagAtr, reserved, 0x18);
RR_ASSERT_SIZE(VabHdr, 0x20);
RR_ASSERT_OFFSET(VabHdr, ps, 0x12);
RR_ASSERT_OFFSET(VabHdr, vs, 0x16);
RR_ASSERT_SIZE(ProgAtrAttributes, 8);
RR_ASSERT_OFFSET(ProgAtrAttributes, attr, 6);
RR_ASSERT_SIZE(ProgAtr, 0x10);
RR_ASSERT_OFFSET(ProgAtr, attributes, 0);
RR_ASSERT_OFFSET(ProgAtr, toneBlockIndex, 8);
RR_ASSERT_OFFSET(ProgAtr, sampleAddresses, 0xC);

/* Wire offsets derive from the single canonical format. A byte cursor can
 * alias selectors/pointer cells; the flags and sample-address views require
 * only the accessed halfword to be aligned. */
enum {
    VAB_PROGRAM_TONE_COUNT = RR_FIELD_OFFSET(ProgAtrAttributes, tones),
    VAB_PROGRAM_VOLUME = RR_FIELD_OFFSET(ProgAtrAttributes, mvol),
    VAB_PROGRAM_PRIORITY = RR_FIELD_OFFSET(ProgAtrAttributes, prior),
    VAB_PROGRAM_MODE = RR_FIELD_OFFSET(ProgAtrAttributes, mode),
    VAB_PROGRAM_PAN = RR_FIELD_OFFSET(ProgAtrAttributes, mpan),
    VAB_PROGRAM_ATTRIBUTE_FLAGS = RR_FIELD_OFFSET(ProgAtrAttributes, attr),
    VAB_PROGRAM_TONE_BLOCK_INDEX = RR_FIELD_OFFSET(ProgAtr, toneBlockIndex),
    VAB_PROGRAM_SAMPLE_ADDRESSES = RR_FIELD_OFFSET(ProgAtr, sampleAddresses)
};

enum {
    VAB_TONE_PRIORITY = RR_FIELD_OFFSET(VagAtr, prior),
    VAB_TONE_MODE = RR_FIELD_OFFSET(VagAtr, mode),
    VAB_TONE_VOLUME = RR_FIELD_OFFSET(VagAtr, vol),
    VAB_TONE_PAN = RR_FIELD_OFFSET(VagAtr, pan),
    VAB_TONE_CENTER = RR_FIELD_OFFSET(VagAtr, center),
    VAB_TONE_SHIFT = RR_FIELD_OFFSET(VagAtr, shift),
    VAB_TONE_MIN_NOTE = RR_FIELD_OFFSET(VagAtr, min),
    VAB_TONE_MAX_NOTE = RR_FIELD_OFFSET(VagAtr, max),
    VAB_TONE_VIBRATO_DEPTH = RR_FIELD_OFFSET(VagAtr, vibW),
    VAB_TONE_VIBRATO_TIME = RR_FIELD_OFFSET(VagAtr, vibT),
    VAB_TONE_PORTAMENTO_DEPTH = RR_FIELD_OFFSET(VagAtr, porW),
    VAB_TONE_PORTAMENTO_TIME = RR_FIELD_OFFSET(VagAtr, porT),
    VAB_TONE_BEND_MIN = RR_FIELD_OFFSET(VagAtr, pbmin),
    VAB_TONE_BEND_MAX = RR_FIELD_OFFSET(VagAtr, pbmax),
    VAB_TONE_ADSR1 = RR_FIELD_OFFSET(VagAtr, adsr1),
    VAB_TONE_ADSR2 = RR_FIELD_OFFSET(VagAtr, adsr2),
    VAB_TONE_PROGRAM = RR_FIELD_OFFSET(VagAtr, prog),
    VAB_TONE_SAMPLE = RR_FIELD_OFFSET(VagAtr, vag)
};
#endif
