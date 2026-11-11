/* Host entry point. Mounts the user's PAL CUE/BIN image through PsyZ and
 * runs the original game loop (MainLoop, src/main/system/frmtask.c).
 *
 * Diagnostics for automated runs (use with PSYZ_HEADLESS=1
 * SDL_AUDIODRIVER=dummy):
 *   RR4_FRAME_LIMIT=n  exit after n VSyncs
 *   RR4_CAPTURE=file   write the displayed frame as a PPM when exiting
 *   RR4_VRAM=file      write the whole VRAM as a PPM when exiting
 *   RR4_TRACE=n        print the mode state every n VSyncs
 *   RR4_PAD_SCRIPT=s   test input: hold pad 1 buttons over VSync ranges.
 *                      s is a comma list of button@vsync[+frames] (default 4
 *                      frames), e.g. "start@400,cross@900+6,up@1200".
 *                      Buttons: select l3 r3 start up right down left l2 r2
 *                      l1 r1 triangle circle cross square. The pressed
 *                      buttons replace pad 1's receive buffer (PadPorts[0])
 *                      right after PsyZ refreshes it at each VSync; outside
 *                      the listed ranges the real input passes through. */

#include <psyz.h>
#include <psyz/cd.h>
#include <psyz/system.h>
#include <psyz/video.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game/system/pad.h"

void MainLoop(void);

/* Mode state of the original dispatcher, read for RR4_TRACE. */
extern unsigned short ModeGroup;
extern short ModeIndex;
extern unsigned short SubmodeState;
extern unsigned int FrameCounter;

static long trace_every;

static long frame_limit;
static long frame_count;
static PsyzVSyncCb chained_vsync;

/* RR4_PAD_SCRIPT: masks use the game's button word,
 * ~((PadPorts[].data02 << 8) | data03). */
typedef struct {
    unsigned short mask;
    long first, count;
} PadScriptStep;
static PadScriptStep pad_script[64];
static int pad_script_len;

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
        step.mask = pad_button_mask(script, (size_t)(at - script));
        step.first = strtol(at + 1, &end, 10);
        step.count = *end == '+' ? strtol(end + 1, &end, 10) : 4;
        if (step.mask)
            pad_script[pad_script_len++] = step;
        else
            fprintf(stderr, "RR4_PAD_SCRIPT: unknown button in \"%.*s\"\n",
                    (int)(at - script), script);
        script = next ? next + 1 : NULL;
    }
}

static void apply_pad_script(long vsync) {
    unsigned short mask = 0;
    for (int i = 0; i < pad_script_len; i++) {
        if (vsync >= pad_script[i].first && vsync < pad_script[i].first + pad_script[i].count)
            mask |= pad_script[i].mask;
    }
    if (!mask)
        return;
    PadPorts[0].status00 = 0;
    PadPorts[0].type01 = 0x41; /* digital pad */
    PadPorts[0].data02 = (unsigned char)~(mask >> 8);
    PadPorts[0].data03 = (unsigned char)~mask;
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

static void host_exit(void) {
    const char *path;
    int w, h;

    path = getenv("RR4_CAPTURE");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocRenderedFrame(&w, &h), w, h);
    path = getenv("RR4_VRAM");
    if (path && *path)
        write_ppm(path, Psyz_VideoAllocVramDump(&w, &h), w, h);
    fprintf(stderr, "ridge-racer-type-4-pc: exit after %ld vsyncs\n", frame_count);
    exit(0);
}

/* The original loop never returns; the host ends it at a VSync. */
static void host_vsync(void) {
    frame_count++;
    apply_pad_script(frame_count);
    if (trace_every > 0 && frame_count % trace_every == 0)
        fprintf(stderr, "vsync %ld frame %u mode %u/%d sub %u\n", frame_count,
                FrameCounter, ModeGroup, ModeIndex, SubmodeState);
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
        parse_pad_script(getenv("RR4_PAD_SCRIPT"));
    }
    chained_vsync = Psyz_SetVSyncCb(host_vsync);
    MainLoop();
    return 0;
}
