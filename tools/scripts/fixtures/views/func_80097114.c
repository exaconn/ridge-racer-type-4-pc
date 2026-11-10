/* GCC_VERSION: 970318 */

/* MASPSX_FLAGS: --assembler-profile=gnu */

#include "rr/bios_devices.h"

static __inline__ s32 save_first_file_handler(void) {
    BiosDevice *device, *end, *limit;

    device = *(BiosDevice **)0x150;
    end = device + (*(u32 *)0x154 / sizeof(BiosDevice));
    if (device < end) {
        limit = end;
        do {
            if (device->name && PsyqStrcmp(device->name, D_801143C0) == 0) {
                CardFirstFileHandler = device->handler;
                return 1;
            }
            device++;
        } while (device < limit);
    }
    return 0;
}

void *firstfile(char *path, void *directory) {
    char *output;
    s8 *input;
    BiosDevice *device, *end, *limit;

    input = (s8 *)path;
    output = D_801143C0;
    /* The BIOS prefix ends at ':' or any lower signed character. */
    while (*input > ':') {
        *output++ = (u8)*input++;
    }
    *output = 0;

    if (!save_first_file_handler()) {
        return 0;
    }

    /* Re-read the descriptor: comparison callbacks may replace the table. */
    device = *(BiosDevice **)0x150;
    end = device + (*(u32 *)0x154 / sizeof(BiosDevice));
    if (device < end) {
        limit = end;
        do {
            if (device->name && PsyqStrcmp(device->name, D_801143C0) == 0) {
                device->handler = _firstfile_hook;
                break;
            }
            device++;
        } while (device < limit);
    }
    return firstfile2(path, directory);
}
