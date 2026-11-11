#include "rr/work_arena.h"
/* RODATA 0x800113f0 0x1c */
#include "types.h"
#include "rr/bios_devices.h"
#include "psyq/libcard/cardirq.h"
#include "psyq/libcard/cardsts.h"
#include "psyq/libcard/init.h"
#include "psyq/libc/printf.h"
#include "psyq/libcard/cardc162.h"
#include "psyq/libgpu/gpuctl.h"
#include "game/data/cardini.h"
#include "rr/bios_events.h"
#include "rr/serial_handles.h"
#include "rr/overlay667_packet.h"
#include "rr/card_format.h"
#include "game/data/globals.h"
#include "rr/spu_tick_control.h"

extern int _bu_init();
extern int HwCardEvIoe;
extern int HwCardEvError;
extern int HwCardEvTimeout;
extern int HwCardEvNew;
extern int SwCardEvIoe;
extern int SwCardEvError;
extern int SwCardEvTimeout;
extern int SwCardEvNew;
extern unsigned char CardPortPollStatus[];
extern char StrCardDirPatternFmt[];
extern char CardDirEntries[];
extern char *nextfile();
extern char CardFilePathFmt[];
extern int CardFileHandle;
extern int FileSeek();

void CardSystemShutdown(void);
int PollHwCardEvents(int arg);
void ClearHwCardEvents(void);
int WaitSwCardEvent(void);
void ClearSwCardEvents(void);
int CountCardFiles(int a, int b);
int CardHasFile(int a0, unsigned char *a1);
void CardFormatSlot(s32 arg0, s32 arg1);

void CardSystemInit(void) {
    InitCARD();
    StartCARD();
    _bu_init();
    _cardStartCom();
    EnterCriticalSection();
    HwCardEvIoe = OpenEvent(0xF4000001, 4, 0x2000, 0);
    HwCardEvError = OpenEvent(0xF4000001, 0x8000, 0x2000, 0);
    HwCardEvTimeout = OpenEvent(0xF4000001, 0x100, 0x2000, 0);
    HwCardEvNew = OpenEvent(0xF4000001, 0x2000, 0x2000, 0);
    SwCardEvIoe = OpenEvent(0xF0000011, 4, 0x2000, 0);
    SwCardEvError = OpenEvent(0xF0000011, 0x8000, 0x2000, 0);
    SwCardEvTimeout = OpenEvent(0xF0000011, 0x100, 0x2000, 0);
    SwCardEvNew = OpenEvent(0xF0000011, 0x2000, 0x2000, 0);
    ExitCriticalSection();
    EnableEvent(HwCardEvIoe);
    EnableEvent(HwCardEvError);
    EnableEvent(HwCardEvTimeout);
    EnableEvent(HwCardEvNew);
    EnableEvent(SwCardEvIoe);
    EnableEvent(SwCardEvError);
    EnableEvent(SwCardEvTimeout);
    EnableEvent(SwCardEvNew);
    CardPortPollStatus[1] = 0x5A;
    CardPortPollStatus[0] = 0x5A;
    SaveBuffer = SharedWorkArena;
}

void CardSystemShutdown(void) {
    EnterCriticalSection();
    CloseEvent(HwCardEvIoe);
    CloseEvent(HwCardEvError);
    CloseEvent(HwCardEvTimeout);
    CloseEvent(HwCardEvNew);
    CloseEvent(SwCardEvIoe);
    CloseEvent(SwCardEvError);
    CloseEvent(SwCardEvTimeout);
    CloseEvent(SwCardEvNew);
    ExitCriticalSection();
    _cardStopCom();
    StopCARD();
}

int PollHwCardEvents(int arg) {
    int r;
    int t;
    r = TestEvent(HwCardEvIoe) == 1;
    if (TestEvent(HwCardEvError) == 1) r = 2;
    if (TestEvent(HwCardEvTimeout) == 1) r = 3;
    if (TestEvent(HwCardEvNew) == 1) r = 4;
    t = CardPortTimer[arg]++;
    if (t >= 0x78) r = 2;
    return r;
}

s32 GetHwCardEvent(void) {
    if (TestEvent(HwCardEvIoe) == 1) {
        return 1;
    }
    if (TestEvent(HwCardEvError) == 1) {
        return 2;
    }
    if (TestEvent(HwCardEvTimeout) != 1) {
        return (TestEvent(HwCardEvNew) == 1) * 4;
    }
    return 3;
}

void ClearHwCardEvents(void) {
    TestEvent(HwCardEvIoe);
    TestEvent(HwCardEvError);
    TestEvent(HwCardEvTimeout);
    TestEvent(HwCardEvNew);
}

int WaitSwCardEvent(void) {
    for (;;) {
        if (TestEvent(SwCardEvIoe) == 1) return 1;
        if (TestEvent(SwCardEvError) == 1) return 2;
        if (TestEvent(SwCardEvTimeout) == 1) return 3;
        if (TestEvent(SwCardEvNew) == 1) return 4;
    }
}

void ClearSwCardEvents(void) {
    TestEvent(SwCardEvIoe);
    TestEvent(SwCardEvError);
    TestEvent(SwCardEvTimeout);
    TestEvent(SwCardEvNew);
}

int CountCardFiles(int a, int b) {
    char sp[32];
    char *s0;
    int n = 0;
    PsyqSprintf((s8 *)sp, (const s8 *)StrCardDirPatternFmt, a, b);
    s0 = CardDirEntries;
    if (firstfile(sp, s0) == s0) {
        do { n++; s0 += 0x28; } while (nextfile(s0) == s0);
        n = n;
    }
    return n;
}

int CardFreeBlocks(int a0) {
    int a2 = 0, a1 = 0;
    char *v1;
    char *p = CardDirEntries;
    int v;
    if (a0 > 0) {
        v1 = p;
        do {
            a1 += ((int *)v1)[6];
            v1 += 0x28;
        } while (++a2 < a0);
    }
    v = a1;
    if (v < 0) v += 0x1FFF;
    a1 = v >> 13;
    return 15 - a1;
}

int CardHasFile(int a0, unsigned char *a1) {
    int t1 = 0, t2 = 0;
    char *t0;
    int a3, v1;
    unsigned char a2;
    char *b = CardDirEntries;
    if (a0 > 0) {
        t0 = b;
        do {
            a3 = 0;
            v1 = a3;
            do {
                a2 = a1[v1];
                if (a2 == 0) goto l64;
                if (a2 != (unsigned char)t0[v1]) goto l74;
                v1++;
            } while (v1 < 0x15);
l64:
            t1++;
            if (a3 == 0) {
                t2 = 1;
                break;
            }
            goto next_entry;
l74:
            a3++;
            goto l64;
next_entry:
            t0 += 0x28;
        } while (t1 < a0);
    }
    return t2;
}

int CardReadFile(int port, int slot, const char *name, void *buffer, int blocks) {
    char path[32];
    int descriptor;
    int length;
    int result;
    PsyqSprintf((s8 *)path, (const s8 *)CardFilePathFmt, port, slot, name);
    length = blocks << 13;
    descriptor = FileOpen(path, 0x8001);
    if (descriptor != -1) {
        CardFileHandle = descriptor;
        ClearHwCardEvents();
        while (FileRead(descriptor, buffer, length) == 0) {
        }
        result = 0;
        FileClose(descriptor);
    } else {
        result = -1;
    }
    return result;
}

int CardReadFileAt(int port, int slot, const char *name, void *buffer, int bytes, int offset) {
    char path[32];
    int descriptor;
    /* ASM_BARRIER: pin return status result to $s1 ($17) */
    register int result asm("$17");
    int length;
    PsyqSprintf((s8 *)path, (const s8 *)CardFilePathFmt, port, slot, name);
    length = bytes;
    descriptor = FileOpen(path, 0x8001);
    /* ASM_BARRIER: loads -1 into result after the call, not hoisted before it */
    __asm__ volatile("" : "=r"(result) : "0"(-1));
    if (descriptor != result) {
        if (FileSeek(descriptor, offset, 0) != result) {
            CardFileHandle = descriptor;
            ClearHwCardEvents();
            while (FileRead(descriptor, buffer, length) == 0) ;
            result = 0;
        }
        FileClose(descriptor);
        return result;
    }
    /* ASM_BARRIER: re-launders result so the -1 return uses s1 (not the equal descriptor in s0) */
    __asm__ ("" : "=r"(result) : "0"(result));
    return result;
}

int CardWriteFile(int port, int slot, const char *name, const void *buffer, int blocks, int create) {
    char path[32];
    int descriptor;
    int length;
    int result;
    PsyqSprintf((s8 *)path, (const s8 *)CardFilePathFmt, port, slot, name);
    if (create == 1) {
        descriptor = FileOpen(path, (blocks << 16) | 0x200);
        if (descriptor == -1) {
            return -1;
        }
        FileClose(descriptor);
    }
    length = blocks << 13;
    descriptor = FileOpen(path, 0x8002);
    if (descriptor != -1) {
        CardFileHandle = descriptor;
        ClearHwCardEvents();
        while (FileWrite(descriptor, buffer, length) == 0) {
        }
        result = 0;
        FileClose(descriptor);
    } else {
        result = -1;
    }
    return result;
}

int CardWriteFileAt(int port, int slot, const char *name, const void *buffer, int bytes, int offset) {
    char path[32];
    int descriptor;
    /* ASM_BARRIER: pin return status result to $s1 ($17) */
    register int result asm("$17");
    int length;
    PsyqSprintf((s8 *)path, (const s8 *)CardFilePathFmt, port, slot, name);
    length = bytes;
    descriptor = FileOpen(path, 0x8002);
    /* ASM_BARRIER: loads -1 into result after the call, not hoisted before it */
    __asm__ volatile("" : "=r"(result) : "0"(-1));
    if (descriptor != result) {
        if (FileSeek(descriptor, offset, 0) != result) {
            CardFileHandle = descriptor;
            ClearHwCardEvents();
            while (FileWrite(descriptor, buffer, length) == 0) ;
            result = 0;
        }
        FileClose(descriptor);
        return result;
    }
    /* ASM_BARRIER: re-launders result so the -1 return uses s1 (not the equal descriptor in s0) */
    __asm__ ("" : "=r"(result) : "0"(result));
    return result;
}

int CardUpdate(int port, int slot) {
    int channel;
    int status;
    int sync[2];
    int previousResult;
    channel = (port << 4) + slot;
    switch (CardPortState[port]) {
    case RR_CARD_PORT_START_INFO:
        _card_info(channel);
        CardPortState[port] = RR_CARD_PORT_WAIT_INFO;
        CardPortTimer[port] = 0;
        CardPortLastResult[port] = 0;
        break;
    case RR_CARD_PORT_WAIT_INFO:
        status = PollHwCardEvents(port);
        if (status == 0) break;
        switch (status) {
        case 1:
            CardPortResult[port] = status;
            CardPortState[port] = RR_CARD_PORT_START_READ;
            CardPortTimer[port] = 0;
            break;
        case 3:
            CardPortResult[port] = -1;
            CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
            CardPortLoaded[port] = 0;
            break;
        case 4:
            CardPortResult[port] = 2;
            CardPortState[port] = RR_CARD_PORT_START_READ;
            CardPortTimer[port] = 0;
            break;
        case 2:
        default:
            CardPortResult[port] = -4;
            CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
            CardPortLoaded[port] = 0;
            break;
        }
        break;
    case RR_CARD_PORT_START_READ:
        if (_cardCmd5ABegin(channel, port * RR_CARD_RAW_REPLY_BYTES + CardRawSectorWorkBuf)) {
            CardPortState[port] = RR_CARD_PORT_WAIT_READ;
            CardPortTimer[port] = 0;
        } else {
            if (CardPortTimer[port]++ >= 0x78) {
                CardPortResult[port] = -4;
                CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                CardPortLoaded[port] = 0;
            }
        }
        break;
    case RR_CARD_PORT_WAIT_READ:
        status = MemCardSync(1, &sync[0], &sync[1]);
        if (status != 0) {
            if (status > 0) {
                status = sync[1];
                switch (status) {
                case 0:
                    CardPortResult[port] = 3;
                    if (CardPortLoaded[port] == 1) {
                        CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                    } else {
                        CardPortState[port] = RR_CARD_PORT_START_LOAD;
                    }
                    break;
                case 2:
                    if (CardPortResult[port] == 1) {
                        if (CardPortLoaded[port] == 1) {
                            CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                        } else {
                            CardPortState[port] = RR_CARD_PORT_START_LOAD;
                        }
                    } else {
                        CardPortResult[port] = 2;
                        ClearSwCardEvents();
                        _card_clear(channel);
                        WaitSwCardEvent();
                        CardPortState[port] = RR_CARD_PORT_START_LOAD;
                        CardPortLoaded[port] = 0;
                    }
                    break;
                case 1:
                    CardPortResult[port] = -1;
                    CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                    CardPortLoaded[port] = 0;
                    break;
                case 3:
                    CardPortResult[port] = 4;
                    ClearSwCardEvents();
                    _card_clear(channel);
                    WaitSwCardEvent();
                    CardPortState[port] = RR_CARD_PORT_START_LOAD;
                    CardPortLoaded[port] = 0;
                    break;
                }
            } else {
                CardPortResult[port] = -4;
                CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                CardPortLoaded[port] = 0;
            }
        } else {
            if (CardPortTimer[port]++ >= 0x78) {
                CardPortResult[port] = -4;
                CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
                CardPortLoaded[port] = 0;
            }
        }
        break;
    case RR_CARD_PORT_START_LOAD:
        ClearHwCardEvents();
        _card_load(channel);
        CardPortState[port] = RR_CARD_PORT_WAIT_LOAD;
        CardPortTimer[port] = 0;
        break;
    case RR_CARD_PORT_WAIT_LOAD:
        status = PollHwCardEvents(port);
        if (status == 0) break;
        CardPortState[port] = RR_CARD_PORT_PUBLISH_RESULT;
        switch (status) {
        case 1:
            CardPortLoaded[port] = status;
            break;
        case 3:
            CardPortResult[port] = -1;
            CardPortLoaded[port] = 0;
            break;
        case 4:
            previousResult = CardPortResult[port];
            if (((u32)previousResult - 1U) < 2U) {
                CardPortResult[port] = -2;
            } else if (((u32)previousResult - 3U) < 2U) {
                CardPortResult[port] = -3;
            }
            CardPortLoaded[port] = 0;
            break;
        case 2:
        default:
            CardPortResult[port] = -4;
            CardPortLoaded[port] = 0;
            break;
        }
        break;
    case RR_CARD_PORT_PUBLISH_RESULT:
        CardPortLastResult[port] = CardPortResult[port];
        CardPortState[port] = RR_CARD_PORT_START_INFO;
        break;
    }
    return CardPortLastResult[port];
}

void CardFormatSlot(s32 arg0, s32 arg1) {
    _card_format((arg0 * 0x10) + arg1);
}
