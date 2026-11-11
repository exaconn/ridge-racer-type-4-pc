/* BIOS file services (A(00h)..A(04h): open, lseek, read, write, close) under
 * the game's names. Memory card files ("buXY:") are PsyZ host files. Host
 * I/O completes inside the call, so an FASYNC card transfer delivers its
 * SwCARD end-of-I/O event before returning, as the BIOS does once the last
 * sector is done. The "sio:" link cable device has no peer on the host: it
 * opens, but nothing is ever sent or received. */
#include <kernel.h>
#include <romio.h>
#include <psyz/system.h>
#include "rr/bios_events.h"
#include "rr/serial_handles.h"

enum { FILE_HANDLE_COUNT = 16 };

typedef enum {
    FILE_KIND_FREE,
    FILE_KIND_HOST,
    FILE_KIND_LINK,
} FileKind;

typedef struct {
    FileKind kind;
    int host;
    int async;
} FileHandle;

static FileHandle FileHandles[FILE_HANDLE_COUNT];

static FileHandle *FileLookup(s32 descriptor) {
    if (descriptor < 0 || descriptor >= FILE_HANDLE_COUNT ||
        FileHandles[descriptor].kind == FILE_KIND_FREE) {
        return NULL;
    }
    return &FileHandles[descriptor];
}

static int FileIsDevice(const char *name, const char *device) {
    while (*device) {
        if (*name++ != *device++) {
            return 0;
        }
    }
    return 1;
}

static s32 FileTransferDone(FileHandle *file, long count) {
    if (count <= 0) {
        return -1; /* end of file or host error: no data was transferred */
    }
    if (file->async) {
        DeliverEvent(SwCARD, EvSpIOE);
    }
    return (s32)count;
}

s32 FileOpen(const char *name, s32 mode) {
    s32 descriptor;
    for (descriptor = 0; descriptor < FILE_HANDLE_COUNT; descriptor++) {
        if (FileHandles[descriptor].kind == FILE_KIND_FREE) {
            break;
        }
    }
    if (!name || descriptor == FILE_HANDLE_COUNT) {
        return -1;
    }
    if (FileIsDevice(name, "sio:")) {
        FileHandles[descriptor] = (FileHandle){FILE_KIND_LINK, -1, 0};
        return descriptor;
    }
    int host = psyz_open(name, mode);
    if (host < 0) {
        return -1;
    }
    FileHandles[descriptor] = (FileHandle){
        FILE_KIND_HOST, host, FileIsDevice(name, "bu") && (mode & FASYNC)};
    return descriptor;
}

s32 FileSeek(s32 descriptor, s32 offset, s32 whence) {
    FileHandle *file = FileLookup(descriptor);
    if (!file || file->kind != FILE_KIND_HOST) {
        return -1;
    }
    return (s32)psyz_lseek(file->host, (unsigned int)offset, whence);
}

s32 FileRead(s32 descriptor, void *buffer, s32 count) {
    FileHandle *file = FileLookup(descriptor);
    if (!file || !buffer || count < 0) {
        return -1;
    }
    if (file->kind == FILE_KIND_LINK) {
        return 0;
    }
    return FileTransferDone(file, psyz_read(file->host, buffer, count));
}

s32 FileWrite(s32 descriptor, const void *buffer, s32 count) {
    FileHandle *file = FileLookup(descriptor);
    if (!file || !buffer || count < 0) {
        return -1;
    }
    if (file->kind == FILE_KIND_LINK) {
        return 0;
    }
    return FileTransferDone(
        file, psyz_write(file->host, (char *)buffer, count));
}

s32 FileClose(s32 descriptor) {
    FileHandle *file = FileLookup(descriptor);
    s32 result = descriptor;
    if (!file) {
        return -1;
    }
    if (file->kind == FILE_KIND_HOST && psyz_close(file->host) != 0) {
        result = -1;
    }
    file->kind = FILE_KIND_FREE;
    return result;
}
