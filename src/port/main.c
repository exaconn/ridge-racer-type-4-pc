/* Host entry point. Mounts the user's PAL CUE/BIN image through PsyZ and
 * runs the original game loop (MainLoop, src/main/system/frmtask.c).
 *
 * Diagnostics for automated runs (use with PSYZ_HEADLESS=1
 * SDL_AUDIODRIVER=dummy):
 *   RR4_FRAME_LIMIT=n  exit after n VSyncs
 *   RR4_CAPTURE=file   write the displayed frame as a PPM when exiting
 *   RR4_VRAM=file      write the whole VRAM as a PPM when exiting
 *   RR4_CAPTURE_AT=l   also write the displayed frame at each VSync of the
 *                      comma list l, as <RR4_CAPTURE or "capture">.<vsync>.ppm
 *   RR4_TRACE=n        print the mode state every n VSyncs
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
 *                      name+offset reads a member at a byte offset (decimal
 *                      or 0x hex) into the object, e.g. "SoundState+9904".
 *   RR4_POKE=list      test hook to fast-forward game state: a comma list of
 *                      name[+offset][:size]=value@vsync (store) or
 *                      name[+offset][:size]+=value@vsync (add), applied once
 *                      at that VSync (between game frames). Names and
 *                      offsets are host symbols/layouts as in RR4_WATCH,
 *                      e.g. "PlayerCars+388+=170330@3400" adds a lap's worth
 *                      of distance to the first player car; the value may
 *                      also name a 32-bit global read when applied
 *                      ("PlayerCars+388+=TrackLength@..."). The vsync may
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
 *                      RR4_AUTODRIVE_REPLAY=file drives from such a log.
 *   RR4_AUDIO_CAPTURE=file  write the mixed audio output (44100 Hz 16-bit
 *                      stereo) as a WAV file, finished when exiting. With
 *                      PSYZ_AUDIO_CLOCK=video the samples follow the VSync
 *                      count instead of the wall clock. */

#include <psyz.h>
#include <psyz/audio.h>
#include <psyz/cd.h>
#include <psyz/input.h>
#include <psyz/system.h>
#include <psyz/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "game/system/pad.h"

void MainLoop(void);

/* Mode state of the original dispatcher, read for RR4_TRACE. */
extern unsigned short ModeGroup;
extern short ModeIndex;
extern unsigned short SubmodeState;
extern unsigned int FrameCounter;

static long trace_every;

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
            watches[watch_count].addr = dlsym(RTLD_DEFAULT, item);
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
                pokes[poke_count].value_from = dlsym(RTLD_DEFAULT, eq + 1);
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
        pokes[poke_count].addr = dlsym(RTLD_DEFAULT, item);
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
    /* RR4_AUTODRIVE_REPLAY=file: drive from a RR4_AUTODRIVE_LOG instead. */
    {
        static unsigned short *replay;
        static int replay_state = -1;
        if (replay_state < 0) {
            const char *path = getenv("RR4_AUTODRIVE_REPLAY");
            FILE *in = path ? fopen(path, "r") : NULL;
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
    f = fopen(path, "wb");
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

    audio_capture = fopen(path, "wb");
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

    path = getenv("RR4_CAPTURE");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocRenderedFrame(&w, &h), w, h);
    path = getenv("RR4_VRAM");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocVramDump(&w, &h), w, h);
    fprintf(stderr, "ridge-racer-type-4-pc: exit after %ld vsyncs\n", frame_count);
    exit(0);
}

static long capture_at[64];
static int capture_at_len;

static void capture_at_vsync(long vsync) {
    for (int i = 0; i < capture_at_len; i++) {
        if (capture_at[i] == vsync) {
            const char *base = getenv("RR4_CAPTURE");
            char path[512];
            int w, h;
            snprintf(path, sizeof(path), "%s.%ld.ppm", base && *base ? base : "capture", vsync);
            write_ppm(path, Psyz_VideoAllocRenderedFrame(&w, &h), w, h);
        }
    }
}

/* The original loop never returns; the host ends it at a VSync. */
static void host_vsync(void) {
    frame_count++;
    capture_at_vsync(frame_count);
    update_anchor(frame_count);
    apply_pad_script(frame_count);
    apply_pokes(frame_count);
    if (trace_every > 0 && frame_count % trace_every == 0)
    {
        fprintf(stderr, "vsync %ld frame %u mode %u/%d sub %u", frame_count,
                FrameCounter, ModeGroup, ModeIndex, SubmodeState);
        print_watches();
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
    env = getenv("RR4_DISC");
    if (env && *env)
        return env;
    return NULL;
}

int main(int argc, char **argv) {
    const char *cue = disc_path(argc, argv);

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
    {
        const char *limit = getenv("RR4_FRAME_LIMIT");
        const char *trace = getenv("RR4_TRACE");
        frame_limit = limit ? strtol(limit, NULL, 10) : 0;
        trace_every = trace ? strtol(trace, NULL, 10) : 0;
        parse_watch(getenv("RR4_WATCH"));
        parse_pad_script(getenv("RR4_PAD_SCRIPT"));
        parse_poke(getenv("RR4_POKE"));
        if (getenv("RR4_AUDIO_CAPTURE") && *getenv("RR4_AUDIO_CAPTURE"))
            start_audio_capture(getenv("RR4_AUDIO_CAPTURE"));
        autodrive = getenv("RR4_AUTODRIVE") && atoi(getenv("RR4_AUTODRIVE"));
        if (autodrive && getenv("RR4_AUTODRIVE_LOG"))
            autodrive_log = fopen(getenv("RR4_AUTODRIVE_LOG"), "w");
        for (const char *l = getenv("RR4_CAPTURE_AT"); l && *l && capture_at_len < 64;) {
            char *end;
            capture_at[capture_at_len++] = strtol(l, &end, 10);
            l = *end == ',' ? end + 1 : NULL;
        }
    }
    chained_vsync = Psyz_SetVSyncCb(host_vsync);
    MainLoop();
    return 0;
}
