/* GCC_VERSION: 970318 */
/* MASPSX_FLAGS: --assembler-profile=gnu */
#include "rr/bios_devices.h"

void *firstfile(char *path, void *directory);
s32 _firstfile_hook(s32 *a0,s32 a1,s32 a2);

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

s32 _firstfile_hook(s32 *a0,s32 a1,s32 a2) {
    BiosDeviceHandler handler;
    unsigned long cursor,end,limit;
    if(*a0==0) *a0=1;
    cursor=(unsigned long)*(BiosDevice **)0x150;
    handler=CardFirstFileHandler;
    end=cursor+(*(u32 *)0x154/sizeof(BiosDevice))*sizeof(BiosDevice);
    if(cursor<end) {
        /* ASM_BARRIER: Keep the loop bound assignment after the initial table-range gate. */
        __asm__ volatile("" : : : "$17");
        limit=end;
        do {
            BiosDevice *device=(BiosDevice *)cursor;
            if(device->name) {
                char *page;
                /* ASM_BARRIER: Keep page materialization inside the nonnull-name branch. */
                __asm__("" : : : "$5");
                /* ASM_BARRIER: Materialize the name page inside the loop before its call-delay offset. */
                __asm__ volatile("" : "=r"(page) : "0"((char *)0x80110000U));
                if(PsyqStrcmp(device->name,page+0x43C0)==0) {
                    device->handler=handler;
                    break;
                }
            }
            cursor+=sizeof(BiosDevice);
        } while(cursor<limit);
    }
    return CardFirstFileHandler(a0,a1,a2);
}
