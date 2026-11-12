/* Host entry point. Mounts the user's PAL CUE/BIN image through PsyZ and
 * runs the original game loop (MainLoop, src/main/system/frmtask.c).
 *
 * Diagnostics for automated runs (use with PSYZ_HEADLESS=1
 * SDL_AUDIODRIVER=dummy):
 *   RR4_FRAME_LIMIT=n  exit after n VSyncs
 *   RR4_CAPTURE=file   write the displayed frame as a PPM when exiting
 *   RR4_VRAM=file      write the whole VRAM as a PPM when exiting
 *   RR4_CAPTURE_AT=l   also write the displayed frame at each VSync of the
 *                      comma list l, as <RR4_CAPTURE or "capture">.<vsync>.ppm;
 *                      an entry a-b/s is every s-th VSync from a to b
 *   RR4_CAPTURE_MID=1  with frame interpolation, also write the midpoint
 *                      drawn for the displayed buffer (the in-between of the
 *                      frame shown and the one before it) at each of those
 *                      VSyncs, as <...>.<vsync>.mid.ppm. When no midpoint
 *                      was presented in the two fields before the VSync
 *                      (the frame before stayed on screen), an empty
 *                      <...>.<vsync>.held marks it.
 *   RR4_GPU_DUMP_AT=l  record the GPU commands of the frame after each
 *                      VSync of the comma list l (to the next VSync) as
 *                      <RR4_GPU_DUMP or "gpudump">.<vsync>.psxgpu, a GPUDUMP
 *                      file DuckStation replays, and the VRAM after it as
 *                      <...>.<vsync>.vram (raw 1024x512 16-bit pixels).
 *                      RR4_GPU_DUMP_LEN=n records n VSyncs instead of one
 *                      (frames that take more than one field).
 *   RR4_RASTERIZER=gpu draw with PsyZ's GPU rasterizer at 1x as well; by
 *                      default 1x frames are drawn by the CPU rasterizer.
 *                      Both match the PS1 GPU pixel for pixel; at Nx the
 *                      GPU rasterizer's first pixel of every NxN block is
 *                      the 1x pixel.
 *   RR4_TRACE=n        print the mode state every n VSyncs (with
 *                      PSYZ_FRAME_LOG also "t ms", the host time of
 *                      the log's present_ms column; with frame
 *                      interpolation "interp p/t/m mid n": the last
 *                      frame's primitives, those tagged for
 *                      interpolation, those paired with the frame
 *                      before, and the midpoints presented so far)
 *   RR4_PAD_SCRIPT=s   test input: hold pad buttons over VSync ranges.
 *                      s is a comma list of [2:]button@vsync[+frames]
 *                      (default 4 frames), e.g. "start@400,cross@900+6,
 *                      2:cross@900+6,up@1200". A "2:" prefix presses the
 *                      button on pad 2, otherwise pad 1.
 *                      Buttons: select l3 r3 start up right down left l2 r2
 *                      l1 r1 triangle circle cross square. The pressed
 *                      buttons replace the pad's receive buffer
 *                      (PadPorts[port]) right after PsyZ refreshes it at
 *                      each VSync; outside the listed ranges the real input
 *                      passes through, or an idle connected digital pad when
 *                      none is present. Any script keeps pad 1 connected;
 *                      pad 2 only when some step names it.
 *                      Instead of an absolute vsync a step can be anchored
 *                      to a mode: button@mG/I:d presses d VSyncs after each
 *                      entry into ModeGroup G / ModeIndex I (while still in
 *                      it), and a %p suffix repeats the step every p VSyncs,
 *                      e.g. "circle@m1/12:60+4%154" (dialogue) or
 *                      "start@m1/22:120" (skip a replay).
 *   RR4_WATCH=list     with RR4_TRACE, also print globals by name: a comma
 *                      list of name[:size] (size 1, 2 or 4 bytes, default 4,
 *                      signed), e.g. "TitleSceneStage:2,FadeCountdown:2".
 *                      RR4_WATCH and RR4_POKE find the names among the
 *                      executable's exported symbols: macOS builds export
 *                      them, Linux and Windows builds only when configured
 *                      with -DRR4_EXPORT_SYMBOLS=ON (otherwise no name is
 *                      found).
 *                      name+offset reads a member at a byte offset (decimal
 *                      or 0x hex) into the object, e.g. "SoundState+9904".
 *                      RR4_TRACE_RAND=1 also prints the BIOS rand() state
 *                      (PS1: word at 0x80009010; seeded from VSync(1) at
 *                      race load, so poke it on the reference to compare).
 *   RR4_POKE=list      test hook to fast-forward game state: a comma list of
 *                      name[+offset][:size]=value@vsync (store) or
 *                      name[+offset][:size]+=value@vsync (add), applied once
 *                      at that VSync (between game frames). Names and
 *                      offsets are host symbols/layouts as in RR4_WATCH,
 *                      e.g. "PlayerCars+388+=170330@3400" adds a lap's worth
 *                      of distance to the first player car; the value may
 *                      also name a 32-bit global read when applied
 *                      ("PlayerCars+388+=TrackLength@...", or name+offset
 *                      as in "PlayerCars+388=PlayerCars+1204@..."). The name
 *                      "rand" sets the BIOS rand() state instead, e.g.
 *                      "rand=12345@m1/47:600" to align random sequences
 *                      with the reference. The vsync may
 *                      be a mode anchor mG/I:d as in RR4_PAD_SCRIPT (applied
 *                      once per entry into that mode).
 *   RR4_AUTODRIVE=1    test driver for whole races: while a race runs
 *                      (RaceState 2..3) pad 1 holds accelerate (cross) and
 *                      steers left/right towards the course centre line
 *                      from the player car's track heading and lateral
 *                      offset. Combines with RR4_PAD_SCRIPT. With
 *                      RR4_AUTODRIVE_LOG=file it writes one line
 *                      "ModeFrame mask" per VSync it drives, which
 *                      tools/port/duckstation.py replays on the reference
 *                      (same ModeFrame -> same buttons) for comparison;
 *                      RR4_AUTODRIVE_REPLAY=file drives from such a log;
 *                      RR4_AUTODRIVE_HOLD=n only accelerates before
 *                      ModeFrame n (a fixed input up to the replay snapshot).
 *   RR4_AUDIO_CAPTURE=file  write the mixed audio output (44100 Hz 16-bit
 *                      stereo) as a WAV file, finished when exiting. With
 *                      PSYZ_AUDIO_CLOCK=video the samples follow the VSync
 *                      count instead of the wall clock.
 *   PSYZ_HEADLESS runs default to PSYZ_AUDIO_CLOCK=video: unpaced fields
 *                      would otherwise race the wall-clock sequencer tick
 *                      (PSYZ_AUDIO_CLOCK=wall keeps the host clock).
 *   PSYZ_FRAME_STATS=1 log percentiles of the present interval, host work,
 *                      present call and render passes every 500 presents
 *                      (=n every n); PSYZ_FRAME_LOG=file one line per
 *                      present. PSYZ_HEADLESS=gpu PSYZ_HEADLESS_PACED=1
 *                      PSYZ_AUDIO_CLOCK=wall measures the windowed timing
 *                      path (frame limiter, real-time clock) without a
 *                      window.
 *   PSYZ_CD_TIMING=instant  drop the CD drive's mechanical timing (seeks,
 *                      pauses, 150 sectors/s) for instant loading; the
 *                      default is the PS1's timing. PSYZ_CD_TRACE=1 prints
 *                      each drive command with its emulated time.
 *   RR4_INTERNAL_RES=n render at n times the PlayStation resolution (1..8)
 *                      for this run, over the preferences; with
 *                      PSYZ_HEADLESS=gpu the captured frames grow with it.
 *   RR4_SETTINGS_SCRIPT=s  drive the Escape menu in a headless run: s is a
 *                      comma list of key@present (see settings_overlay.h).
 *   RR4_GRAPHICS=l     graphics options for this run, as the Escape menu
 *                      sets them: a comma list of geometry (geometry
 *                      correction), nodither (dithering off), aa
 *                      (antialiasing), filter (texture filtering),
 *                      interpolation (frame interpolation) and wide (the
 *                      Widescreen 16:9 aspect ratio: square pixels, races
 *                      at 16:9). With PSYZ_HEADLESS=gpu the captures show
 *                      them.
 *
 * Interactive runs (no PSYZ_HEADLESS) read the preferences file
 * (src/port/preferences.c) and apply it, and Escape opens the settings menu
 * (src/port/settings_overlay.c). Without a CUE argument or RR4_DISC they
 * start the saved image or ask for one with a file dialog. Headless runs
 * never read or write preferences and never show a dialog. */

#if defined(__linux__) && !defined(_GNU_SOURCE)
#define _GNU_SOURCE /* setenv and RTLD_DEFAULT under -std=c11 */
#endif
/* windows.h first: PsyZ types.h defers to its byte typedef. */
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#else
#include <dlfcn.h>
#endif
#include <psyz.h>
#include <psyz/audio.h>
#include <psyz/cd.h>
#include <psyz/file.h>
#include <psyz/gpu.h>
#include <psyz/input.h>
#include <psyz/system.h>
#include <psyz/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#ifdef __APPLE__
#include <unistd.h>
#endif
#include "game/system/pad.h"
#include "rr/video_timing.h"
#include "preferences.h"
#include "settings_overlay.h"

#include <SDL3/SDL.h>

void MainLoop(void);
void Psyz_Srand(unsigned int seed);

/* Mode state of the original dispatcher, read for RR4_TRACE. */
extern unsigned short ModeGroup;
extern short ModeIndex;
extern unsigned short SubmodeState;
extern unsigned int FrameCounter;

static long trace_every;
static int frame_log; /* PSYZ_FRAME_LOG is set */

/* Address of a global for RR4_WATCH / RR4_POKE. The executable exports its
 * symbols on macOS; on Linux and Windows only a build configured with
 * -DRR4_EXPORT_SYMBOLS=ON (-rdynamic, --export-all-symbols) has them. */
static void *find_symbol(const char *name) {
#ifdef _WIN32
    return (void *)(uintptr_t)GetProcAddress(GetModuleHandleW(NULL), name);
#else
    return dlsym(RTLD_DEFAULT, name);
#endif
}

/* Host paths are UTF-8 (PsyZ opens them with Psyz_FileOpen, wide APIs on
 * Windows). The CRT's getenv and main's argv are in the ANSI code page on
 * Windows; SDL_getenv reads the UTF-16 environment and returns UTF-8. */
static const char *path_env(const char *name) {
    return SDL_getenv(name);
}

#ifdef _WIN32
/* main's argv converted from the UTF-16 command line, so that a CUE path
 * with characters outside the ANSI code page survives. */
static char **utf8_argv(int *argc) {
    int count = 0;
    wchar_t **wide = CommandLineToArgvW(GetCommandLineW(), &count);
    char **argv;

    if (!wide)
        return NULL;
    argv = (char **)calloc((size_t)count + 1, sizeof(*argv));
    for (int i = 0; argv && i < count; i++) {
        int bytes = WideCharToMultiByte(CP_UTF8, 0, wide[i], -1, NULL, 0, NULL, NULL);
        argv[i] = bytes > 0 ? (char *)malloc((size_t)bytes) : NULL;
        if (!argv[i] ||
            !WideCharToMultiByte(CP_UTF8, 0, wide[i], -1, argv[i], bytes, NULL, NULL)) {
            argv = NULL; /* keep the CRT's argv; the copies are left */
            break;
        }
    }
    LocalFree(wide);
    if (argv)
        *argc = count;
    return argv;
}
#endif

static void set_env(const char *name, const char *value) {
#ifdef _WIN32
    _putenv_s(name, value);
#else
    setenv(name, value, 1);
#endif
}

#define WATCH_MAX 16
static struct {
    char name[64];
    const void *addr;
    int size;
} watches[WATCH_MAX];
static int watch_count;

static void parse_watch(const char *list) {
    while (list && *list && watch_count < WATCH_MAX) {
        const char *end = strchr(list, ',');
        size_t len = end ? (size_t)(end - list) : strlen(list);
        char item[80];
        char *colon;

        if (len >= sizeof(item))
            len = sizeof(item) - 1;
        memcpy(item, list, len);
        item[len] = 0;
        colon = strchr(item, ':');
        watches[watch_count].size = 4;
        if (colon) {
            *colon = 0;
            watches[watch_count].size = atoi(colon + 1);
        }
        snprintf(watches[watch_count].name, sizeof(watches[0].name), "%s", item);
        {
            char *plus = strchr(item, '+');
            long offset = 0;
            if (plus) {
                *plus = 0;
                offset = strtol(plus + 1, NULL, 0);
            }
            watches[watch_count].addr = find_symbol(item);
            if (watches[watch_count].addr)
                watches[watch_count].addr = (const char *)watches[watch_count].addr + offset;
        }
        if (!watches[watch_count].addr)
            fprintf(stderr, "RR4_WATCH: no symbol %s\n", item);
        else
            watch_count++;
        list = end ? end + 1 : NULL;
    }
}

static void print_watches(void) {
    for (int i = 0; i < watch_count; i++) {
        long v = 0;
        switch (watches[i].size) {
        case 1: v = *(const signed char *)watches[i].addr; break;
        case 2: v = *(const short *)watches[i].addr; break;
        default: v = *(const int *)watches[i].addr; break;
        }
        fprintf(stderr, " %s=%ld", watches[i].name, v);
    }
    if (getenv("RR4_TRACE_RAND"))
        fprintf(stderr, " rand=%08x", Psyz_RandState());
}

/* Mode anchors for RR4_PAD_SCRIPT / RR4_POKE: "mG/I:d" counts VSyncs from
 * the latest entry into mode G/I; a plain number is an absolute VSync. */
static int anchor_mode = -1;
static long anchor_entry;

static void update_anchor(long vsync) {
    int mode = (ModeGroup << 8) | (ModeIndex & 0xFF);
    if (mode != anchor_mode) {
        anchor_mode = mode;
        anchor_entry = vsync;
    }
}

static long parse_anchor(const char *text, char **end, int *mode) {
    *mode = -1;
    if (*text == 'm') {
        long group = strtol(text + 1, end, 10);
        long index = **end == '/' ? strtol(*end + 1, end, 10) : 0;
        *mode = (int)((group << 8) | (index & 0xFF));
        return **end == ':' ? strtol(*end + 1, end, 10) : 0;
    }
    return strtol(text, end, 10);
}

/* VSyncs on the anchor's clock, or -1 while outside the anchored mode. */
static long anchor_time(int mode, long vsync) {
    if (mode < 0)
        return vsync;
    return mode == anchor_mode ? vsync - anchor_entry : -1;
}

#define POKE_MAX 32
static struct {
    void *addr;
    int size, add, mode;
    long value, vsync;
    const int *value_from; /* value read from this global when applied */
} pokes[POKE_MAX];
static int poke_count;

static void parse_poke(const char *list) {
    while (list && *list && poke_count < POKE_MAX) {
        const char *end = strchr(list, ',');
        size_t len = end ? (size_t)(end - list) : strlen(list);
        char item[96], *eq, *at, *colon, *plus;
        long offset = 0;

        if (len >= sizeof(item))
            len = sizeof(item) - 1;
        memcpy(item, list, len);
        item[len] = 0;
        list = end ? end + 1 : NULL;
        eq = strchr(item, '=');
        at = strrchr(item, '@');
        if (!eq || !at || at < eq) {
            fprintf(stderr, "RR4_POKE: bad item %s\n", item);
            continue;
        }
        *at = 0;
        {
            char *rest;
            pokes[poke_count].vsync = parse_anchor(at + 1, &rest, &pokes[poke_count].mode);
        }
        {
            char *num_end;
            pokes[poke_count].value = strtol(eq + 1, &num_end, 0);
            pokes[poke_count].value_from = NULL;
            if (num_end == eq + 1 || *num_end) {
                char *off = strchr(eq + 1, '+');
                long from_offset = off ? strtol(off + 1, NULL, 0) : 0;
                if (off)
                    *off = 0;
                pokes[poke_count].value_from = find_symbol(eq + 1);
                if (pokes[poke_count].value_from)
                    pokes[poke_count].value_from = (const int *)((const char *)pokes[poke_count].value_from + from_offset);
                if (!pokes[poke_count].value_from) {
                    fprintf(stderr, "RR4_POKE: no symbol %s\n", eq + 1);
                    continue;
                }
            }
        }
        pokes[poke_count].add = eq > item && eq[-1] == '+';
        eq[pokes[poke_count].add ? -1 : 0] = 0;
        pokes[poke_count].size = 4;
        colon = strchr(item, ':');
        if (colon) {
            *colon = 0;
            pokes[poke_count].size = atoi(colon + 1);
        }
        plus = strchr(item, '+');
        if (plus) {
            *plus = 0;
            offset = strtol(plus + 1, NULL, 0);
        }
        if (!strcmp(item, "rand")) {
            /* The BIOS rand() state (PsyZ keeps it private): store only. */
            pokes[poke_count].addr = NULL;
            pokes[poke_count].add = 0;
            poke_count++;
            continue;
        }
        pokes[poke_count].addr = find_symbol(item);
        if (!pokes[poke_count].addr) {
            fprintf(stderr, "RR4_POKE: no symbol %s\n", item);
            continue;
        }
        pokes[poke_count].addr = (char *)pokes[poke_count].addr + offset;
        poke_count++;
    }
}

static void apply_pokes(long vsync) {
    for (int i = 0; i < poke_count; i++) {
        long v = pokes[i].value_from ? *pokes[i].value_from : pokes[i].value;
        if (anchor_time(pokes[i].mode, vsync) != pokes[i].vsync)
            continue;
        if (!pokes[i].addr) {
            Psyz_Srand((unsigned int)v);
            continue;
        }
        switch (pokes[i].size) {
        case 1:
            *(signed char *)pokes[i].addr = (signed char)(pokes[i].add ? *(signed char *)pokes[i].addr + v : v);
            break;
        case 2:
            *(short *)pokes[i].addr = (short)(pokes[i].add ? *(short *)pokes[i].addr + v : v);
            break;
        default:
            *(int *)pokes[i].addr = (int)(pokes[i].add ? *(int *)pokes[i].addr + v : v);
            break;
        }
    }
}

static long frame_limit;
static long frame_count;
static PsyzVSyncCb chained_vsync;

/* RR4_PAD_SCRIPT: masks use the game's button word,
 * ~((PadPorts[].data02 << 8) | data03). */
typedef struct {
    unsigned short mask;
    int port, mode;
    long first, count, period;
} PadScriptStep;
static PadScriptStep pad_script[128];
static int pad_script_len;
static int pad_script_ports[2];

static unsigned short pad_button_mask(const char *name, size_t len) {
    static const struct {
        const char *name;
        unsigned short mask;
    } buttons[] = {
        {"select", 0x0100}, {"l3", 0x0200}, {"r3", 0x0400}, {"start", 0x0800},
        {"up", 0x1000}, {"right", 0x2000}, {"down", 0x4000}, {"left", 0x8000},
        {"l2", 0x0001}, {"r2", 0x0002}, {"l1", 0x0004}, {"r1", 0x0008},
        {"triangle", 0x0010}, {"circle", 0x0020}, {"cross", 0x0040},
        {"square", 0x0080},
    };
    for (size_t i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++) {
        if (strlen(buttons[i].name) == len && !strncmp(buttons[i].name, name, len))
            return buttons[i].mask;
    }
    return 0;
}

static void parse_pad_script(const char *script) {
    while (script && *script && pad_script_len < (int)(sizeof(pad_script) / sizeof(pad_script[0]))) {
        const char *at = strchr(script, '@');
        const char *next = strchr(script, ',');
        char *end;
        PadScriptStep step;

        if (!at || (next && next < at))
            break;
        step.port = 0;
        if (script[0] == '2' && script[1] == ':') {
            step.port = 1;
            script += 2;
        } else if (script[0] == '1' && script[1] == ':') {
            script += 2;
        }
        step.mask = pad_button_mask(script, (size_t)(at - script));
        step.first = parse_anchor(at + 1, &end, &step.mode);
        step.count = *end == '+' ? strtol(end + 1, &end, 10) : 4;
        step.period = *end == '%' ? strtol(end + 1, &end, 10) : 0;
        if (step.mask) {
            pad_script[pad_script_len++] = step;
            pad_script_ports[step.port] = 1;
        }
        else
            fprintf(stderr, "RR4_PAD_SCRIPT: unknown button in \"%.*s\"\n",
                    (int)(at - script), script);
        script = next ? next + 1 : NULL;
    }
}

/* RR4_AUTODRIVE: decided from PS1-layout fields of PlayerCars[0] (offsets
 * below the host-only control block are those of the original). */
extern unsigned char PlayerCars[];
extern int RaceState;
extern int ModeFrame;
static int autodrive;
static FILE *autodrive_log;

static unsigned short autodrive_mask(void) {
    short lateral, track, heading;
    int error;
    unsigned short mask;

    if (!autodrive || RaceState < 2 || RaceState > 3)
        return 0;
    /* Only in the race update modes (RaceState is stale in menus after a
     * replay): VS 1/38, GP 1/47, TA 1/53, link 1/26, Extra trial 1/82. */
    if (ModeGroup != 1 || (ModeIndex != 26 && ModeIndex != 38 && ModeIndex != 47 &&
                           ModeIndex != 53 && ModeIndex != 82))
        return 0;
    /* RR4_AUTODRIVE_REPLAY=file: drive from a RR4_AUTODRIVE_LOG instead. */
    {
        static unsigned short *replay;
        static int replay_state = -1;
        if (replay_state < 0) {
            const char *path = path_env("RR4_AUTODRIVE_REPLAY");
            FILE *in = path ? Psyz_FileOpen(path, "r") : NULL;
            int frame;
            unsigned m;
            replay_state = 0;
            if (in) {
                replay = calloc(65536, sizeof(*replay));
                replay_state = 1;
                while (fscanf(in, "%d %u", &frame, &m) == 2)
                    if (frame >= 0 && frame < 65536)
                        replay[frame] = (unsigned short)m;
                fclose(in);
            }
        }
        if (replay_state > 0)
            return ModeFrame >= 0 && ModeFrame < 65536 ? replay[ModeFrame] : 0;
    }
    lateral = *(const short *)(PlayerCars + 0x172);
    track = *(const short *)(PlayerCars + 0x1EC);
    heading = (short)*(const int *)(PlayerCars + 0x54);
    error = lateral / 2;
    if (error > 400)
        error = 400;
    if (error < -400)
        error = -400;
    /* The track heading runs mirrored to the car's yaw (0x800 - yaw). */
    error = ((0x800 - track - error - heading + 0x800) & 0xFFF) - 0x800;
    mask = error > 300 || error < -300 ? 0 : 0x0040; /* cross: accelerate */
    if (error > 24)
        mask |= 0x2000; /* right */
    else if (error < -24)
        mask |= 0x8000; /* left */
    if (getenv("RR4_AUTODRIVE_HOLD") && ModeFrame < atoi(getenv("RR4_AUTODRIVE_HOLD")))
        mask = 0x0040; /* straight on until that ModeFrame */
    if (autodrive_log)
        fprintf(autodrive_log, "%d %u\n", ModeFrame, mask);
    return mask;
}

static void apply_pad_port(int port, long vsync) {
    unsigned short mask = port == 0 ? autodrive_mask() : 0;
    for (int i = 0; i < pad_script_len; i++) {
        long t = anchor_time(pad_script[i].mode, vsync) - pad_script[i].first;
        if (pad_script[i].port != port || t < 0)
            continue;
        if (pad_script[i].period > 0)
            t %= pad_script[i].period;
        if (t < pad_script[i].count)
            mask |= pad_script[i].mask;
    }
    /* Any script keeps pad 1 connected; pad 2 only when a step names it. */
    int scripted = port == 0 ? pad_script_len > 0 : pad_script_ports[1];
    if (!mask && (!scripted || PadPorts[port].status00 == 0))
        return;
    /* A scripted run always has a digital pad in each scripted port:
     * between presses an idle pad stays connected, as on the console,
     * instead of the host's disconnected port (the game re-detects the
     * controller type after every reconnect and ignores input meanwhile).
     * The frame also goes to PsyZ so that libpad (PadGetState) sees the
     * same pad until the next VSync. */
    {
        char frame[4];
        frame[0] = 0;
        frame[1] = 0x41; /* digital pad */
        frame[2] = (char)~(mask >> 8);
        frame[3] = (char)~mask;
        Psyz_PadsSet(port, frame, sizeof(frame));
        memcpy(&PadPorts[port], frame, sizeof(frame));
    }
}

static void apply_pad_script(long vsync) {
    apply_pad_port(0, vsync);
    apply_pad_port(1, vsync);
}

static void write_ppm(const char *path, unsigned char *rgb, int w, int h) {
    FILE *f;

    if (!rgb || w <= 0 || h <= 0)
        return;
    f = Psyz_FileOpen(path, "wb");
    if (f) {
        fprintf(f, "P6\n%d %d\n255\n", w, h);
        fwrite(rgb, 3, (size_t)w * (size_t)h, f);
        fclose(f);
    }
    free(rgb);
}

/* RR4_AUDIO_CAPTURE: the PCM data follows a 44-byte WAV header that
 * finish_audio_capture fills in once the length is known. */
static FILE *audio_capture;
static unsigned long audio_capture_frames;

static void capture_audio(const short *samples, int frames, void *userdata) {
    (void)userdata;
    if (audio_capture && frames > 0) {
        fwrite(samples, 4, (size_t)frames, audio_capture);
        audio_capture_frames += (unsigned long)frames;
    }
}

static void put_le(unsigned char *p, unsigned long v, int n) {
    for (int i = 0; i < n; i++)
        p[i] = (unsigned char)(v >> (8 * i));
}

static void start_audio_capture(const char *path) {
    unsigned char header[44] = {0};

    audio_capture = Psyz_FileOpen(path, "wb");
    if (!audio_capture) {
        fprintf(stderr, "ridge-racer-type-4-pc: cannot write %s\n", path);
        return;
    }
    fwrite(header, 1, sizeof(header), audio_capture);
    Psyz_AudioSetCaptureCallback(capture_audio, NULL);
}

static void finish_audio_capture(void) {
    unsigned char h[44];
    unsigned long bytes;

    if (!audio_capture)
        return;
    Psyz_AudioSetCaptureCallback(NULL, NULL);
    bytes = audio_capture_frames * 4;
    memcpy(h, "RIFF", 4);
    put_le(h + 4, 36 + bytes, 4);
    memcpy(h + 8, "WAVEfmt ", 8);
    put_le(h + 16, 16, 4);
    put_le(h + 20, 1, 2);         /* PCM */
    put_le(h + 22, 2, 2);         /* stereo */
    put_le(h + 24, 44100, 4);
    put_le(h + 28, 44100 * 4, 4); /* bytes per second */
    put_le(h + 32, 4, 2);         /* bytes per frame */
    put_le(h + 34, 16, 2);
    memcpy(h + 36, "data", 4);
    put_le(h + 40, bytes, 4);
    fseek(audio_capture, 0, SEEK_SET);
    fwrite(h, 1, sizeof(h), audio_capture);
    fclose(audio_capture);
    audio_capture = NULL;
}

static void host_exit(void) {
    const char *path;
    int w, h;

    finish_audio_capture();

    path = path_env("RR4_CAPTURE");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocRenderedFrame(&w, &h), w, h);
    path = path_env("RR4_VRAM");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocVramDump(&w, &h), w, h);
    fprintf(stderr, "ridge-racer-type-4-pc: exit after %ld vsyncs\n", frame_count);
    exit(0);
}

/* RR4_CAPTURE_AT entries: VSyncs first..last, every step-th. */
static struct {
    long first, last, step;
} capture_at[64];
static int capture_at_len;

static void capture_at_vsync(long vsync) {
    /* midpoints presented by each of the last VSyncs */
    static unsigned long long presented[4];
    presented[vsync & 3] = Psyz_GpuGetInterpolatedPresentCount();
    for (int i = 0; i < capture_at_len; i++) {
        if (vsync >= capture_at[i].first && vsync <= capture_at[i].last &&
            (vsync - capture_at[i].first) % capture_at[i].step == 0) {
            const char *base = path_env("RR4_CAPTURE");
            char path[512];
            int w, h;
            snprintf(path, sizeof(path), "%s.%ld.ppm", base && *base ? base : "capture", vsync);
            write_ppm(path, Psyz_VideoAllocRenderedFrame(&w, &h), w, h);
            if (getenv("RR4_CAPTURE_MID") && *getenv("RR4_CAPTURE_MID") == '1') {
                unsigned char *mid = Psyz_VideoAllocMidpointFrame(&w, &h);
                snprintf(path, sizeof(path), "%s.%ld.mid.ppm", base && *base ? base : "capture", vsync);
                if (mid)
                    write_ppm(path, mid, w, h);
                if (mid && presented[vsync & 3] == presented[(vsync - 2) & 3]) {
                    FILE *held;
                    snprintf(path, sizeof(path), "%s.%ld.held", base && *base ? base : "capture", vsync);
                    held = Psyz_FileOpen(path, "w");
                    if (held)
                        fclose(held);
                }
            }
        }
    }
}

static long gpu_dump_at[64];
static int gpu_dump_at_len;
static long gpu_dump_open = -1;

static void gpu_dump_vsync(long vsync) {
    const char *base = path_env("RR4_GPU_DUMP");
    char path[512];

    if (!base || !*base)
        base = "gpudump";
    const char *len = getenv("RR4_GPU_DUMP_LEN");

    if (gpu_dump_open >= 0 && vsync - gpu_dump_open >= (len ? atol(len) : 1)) {
        snprintf(path, sizeof(path), "%s.%ld.vram", base, gpu_dump_open);
        Psyz_GpuDumpEnd(path);
        gpu_dump_open = -1;
    }
    for (int i = 0; i < gpu_dump_at_len; i++) {
        if (gpu_dump_at[i] == vsync && gpu_dump_open < 0) {
            snprintf(path, sizeof(path), "%s.%ld.psxgpu", base, vsync);
            if (Psyz_GpuDumpBegin(path) == 0)
                gpu_dump_open = vsync;
        }
    }
}

/* The original loop never returns; the host ends it at a VSync. */
static void host_vsync(void) {
    frame_count++;
    capture_at_vsync(frame_count);
    gpu_dump_vsync(frame_count);
    update_anchor(frame_count);
    apply_pad_script(frame_count);
    apply_pokes(frame_count);
    if (trace_every > 0 && frame_count % trace_every == 0)
    {
        fprintf(stderr, "vsync %ld frame %u mode %u/%d sub %u", frame_count,
                FrameCounter, ModeGroup, ModeIndex, SubmodeState);
        print_watches();
        if (Psyz_GpuGetFrameInterpolation()) {
            unsigned prims, tagged, paired;
            Psyz_GpuGetInterpolationStats(&prims, &tagged, &paired);
            fprintf(stderr, " interp %u/%u/%u mid %llu", prims, tagged, paired,
                    Psyz_GpuGetInterpolatedPresentCount());
        }
        /* host time on the clock of PSYZ_FRAME_LOG's present_ms */
        if (frame_log)
            fprintf(stderr, " t %.3f",
                    (double)SDL_GetPerformanceCounter() * 1000.0 /
                        (double)SDL_GetPerformanceFrequency());
        fputc('\n', stderr);
    }
    if ((frame_limit > 0 && frame_count >= frame_limit) || Psyz_QuitRequested())
        host_exit();
    if (chained_vsync)
        chained_vsync();
}

static const char *disc_path(int argc, char **argv) {
    const char *env;

    if (argc > 1)
        return argv[1];
    env = path_env("RR4_DISC");
    if (env && *env)
        return env;
    return NULL;
}

#ifdef __APPLE__
/* Finder starts an application bundle in "/", where the memory card
 * directories (bu00, bu10, created in the working directory) cannot be
 * written: keep them beside the preferences instead. */
static void enter_preferences_directory(void) {
    char cwd[RR4_PREFS_PATH_MAX], dir[RR4_PREFS_PATH_MAX];
    char *slash;

    if (!getcwd(cwd, sizeof(cwd)) || strcmp(cwd, "/") || !Rr4PrefsPath()[0])
        return;
    snprintf(dir, sizeof(dir), "%s", Rr4PrefsPath());
    slash = strrchr(dir, '/');
    if (!slash || slash == dir)
        return;
    *slash = '\0';
    if (chdir(dir))
        fprintf(stderr, "ridge-racer-type-4-pc: cannot enter %s\n", dir);
}
#endif

static int file_exists(const char *path) {
    FILE *f = path && *path ? Psyz_FileOpen(path, "rb") : NULL;
    if (f)
        fclose(f);
    return f != NULL;
}

int main(int argc, char **argv) {
#ifdef _WIN32
    char **wide_argv = utf8_argv(&argc);
    if (wide_argv)
        argv = wide_argv;
#endif
    const char *cue = disc_path(argc, argv);
    const char *headless = getenv("PSYZ_HEADLESS");
    int interactive = !(headless && *headless);
    static Rr4Prefs prefs;
    static char mounted[RR4_PREFS_PATH_MAX];

    /* Headless runs are not paced (PsyZ limitless mode), so wall time has no
     * relation to the emulated fields. The root counters (the libsnd
     * sequencer tick, which times the music and the beat-synced front-end
     * scripts) and the SPU mix then follow the counted fields, as with
     * PSYZ_AUDIO_CLOCK=video; another PSYZ_AUDIO_CLOCK value keeps the host
     * clock. Set before PsyZ reads it (kernel and audio initialisation). */
    if (getenv("PSYZ_HEADLESS") && *getenv("PSYZ_HEADLESS") &&
        !getenv("PSYZ_AUDIO_CLOCK"))
        set_env("PSYZ_AUDIO_CLOCK", "video");
    /* Preferences and the image dialog are for players only. */
    Rr4PrefsCaptureDefaults();
    prefs = *Rr4PrefsDefaults();
    if (interactive) {
        if (!Rr4PrefsLoad(&prefs))
            fprintf(stderr, "ridge-racer-type-4-pc: no preferences location, settings are not saved\n");
#ifdef __APPLE__
        enter_preferences_directory();
#endif
        if (!cue && file_exists(prefs.disc))
            cue = prefs.disc;
        if (!cue && SettingsChooseDisc(prefs.disc, sizeof(prefs.disc))) {
            cue = prefs.disc;
            Rr4PrefsSave(&prefs);
        }
    }
    if (!cue) {
        fprintf(stderr,
                "usage: %s <Ridge Racer Type 4 (Europe).cue>\n"
                "       (or set RR4_DISC to the CUE file)\n",
                argv[0]);
        return 2;
    }
    if (Psyz_CdSetDiskPath(cue) != 0) {
        fprintf(stderr, "ridge-racer-type-4-pc: cannot open disc image %s\n", cue);
        return 1;
    }
    snprintf(mounted, sizeof(mounted), "%s", cue);
    /* At 1x without GPU-only enhancements the CPU rasterizer draws the
     * frames bit-exactly as the PS1 GPU; higher internal resolutions and the
     * enhancements use the GPU rasterizer, which samples the same PS1 rules
     * at every target pixel (its native VRAM is the 1x result).
     * RR4_RASTERIZER=gpu forces the GPU rasterizer at 1x too (for
     * comparisons). */
    Psyz_VideoSetNativeRasterizer(!(getenv("RR4_RASTERIZER") &&
                                    !strcmp(getenv("RR4_RASTERIZER"), "gpu")));
    if (interactive)
        Rr4PrefsApply(&prefs, 0);
    if (interactive || (getenv("RR4_SETTINGS_SCRIPT") && *getenv("RR4_SETTINGS_SCRIPT")))
        SettingsOverlayInstall(&prefs, mounted);
    if (getenv("RR4_INTERNAL_RES") && *getenv("RR4_INTERNAL_RES")) {
        unsigned scale = (unsigned)strtoul(getenv("RR4_INTERNAL_RES"), NULL, 10);
        /* The settings menu applies the preferences again (headless with
         * RR4_SETTINGS_SCRIPT too): keep this scale there. */
        if (Psyz_VideoSetInternalResolution(scale) == 0)
            prefs.scale = scale;
    }
    if (getenv("RR4_GRAPHICS") && *getenv("RR4_GRAPHICS")) {
        const char *l = getenv("RR4_GRAPHICS");
        prefs.geometry_correction = strstr(l, "geometry") != NULL;
        prefs.disable_dithering = strstr(l, "nodither") != NULL;
        prefs.antialiasing = strstr(l, "aa") != NULL;
        prefs.texture_filtering = strstr(l, "filter") != NULL;
        prefs.frame_interpolation = strstr(l, "interpolation") != NULL;
        prefs.aspect_ratio = strstr(l, "wide") ? RR4_ASPECT_WIDESCREEN : RR4_ASPECT_CLASSIC;
        Psyz_VideoSetGeometryCorrection(prefs.geometry_correction);
        Psyz_VideoSetDitheringMode(prefs.disable_dithering ? PSYZ_DITHER_OFF : PSYZ_DITHER_AUTO);
        Psyz_VideoSetAntialiasing(prefs.antialiasing);
        Psyz_VideoSetModelFiltering(prefs.texture_filtering);
        Psyz_VideoSetAspectMode(prefs.aspect_ratio == RR4_ASPECT_WIDESCREEN ? PSYZ_ASPECT_SQUARE
                                                                            : PSYZ_ASPECT_DISPLAY);
        Psyz_GpuSetFrameInterpolation(prefs.frame_interpolation);
        Rr4ApplyWidescreen(prefs.aspect_ratio == RR4_ASPECT_WIDESCREEN);
    }
    {
        const char *limit = getenv("RR4_FRAME_LIMIT");
        const char *trace = getenv("RR4_TRACE");
        frame_limit = limit ? strtol(limit, NULL, 10) : 0;
        trace_every = trace ? strtol(trace, NULL, 10) : 0;
        frame_log = SDL_getenv("PSYZ_FRAME_LOG") != NULL;
        parse_watch(getenv("RR4_WATCH"));
        parse_pad_script(getenv("RR4_PAD_SCRIPT"));
        parse_poke(getenv("RR4_POKE"));
        if (path_env("RR4_AUDIO_CAPTURE") && *path_env("RR4_AUDIO_CAPTURE"))
            start_audio_capture(path_env("RR4_AUDIO_CAPTURE"));
        autodrive = getenv("RR4_AUTODRIVE") && atoi(getenv("RR4_AUTODRIVE"));
        if (autodrive && path_env("RR4_AUTODRIVE_LOG"))
            autodrive_log = Psyz_FileOpen(path_env("RR4_AUTODRIVE_LOG"), "w");
        for (const char *l = getenv("RR4_GPU_DUMP_AT"); l && *l && gpu_dump_at_len < 64;) {
            char *end;
            gpu_dump_at[gpu_dump_at_len++] = strtol(l, &end, 10);
            l = *end == ',' ? end + 1 : NULL;
        }
        for (const char *l = getenv("RR4_CAPTURE_AT"); l && *l && capture_at_len < 64;) {
            char *end;
            long first = strtol(l, &end, 10), last = first, step = 1;
            if (*end == '-')
                last = strtol(end + 1, &end, 10);
            if (*end == '/')
                step = strtol(end + 1, &end, 10);
            capture_at[capture_at_len].first = first;
            capture_at[capture_at_len].last = last;
            capture_at[capture_at_len++].step = step > 0 ? step : 1;
            l = *end == ',' ? end + 1 : NULL;
        }
    }
    {
        /* Video timing for the whole run (rr/video_timing.h): the
         * preference, or RR4_VIDEO=pal / ntsc (also headless). NTSC keeps
         * the PAL release's 256-line picture whole at NTSC field timing. */
        const char *video = getenv("RR4_VIDEO");
        int ntsc = prefs.video_timing == RR4_VIDEO_NTSC;
        if (video && *video)
            ntsc = !strcmp(video, "ntsc");
        VideoTimingNtsc = ntsc;
        Psyz_GpuSetNtscPalPicture(ntsc);
    }
    chained_vsync = Psyz_SetVSyncCb(host_vsync);
    MainLoop();
    return 0;
}
