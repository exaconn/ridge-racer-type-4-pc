/* Host entry point. Mounts the user's PAL CUE/BIN image through PsyZ and
 * runs the original game loop (MainLoop, src/main/system/frmtask.c).
 *
 * Diagnostics for automated runs (use with PSYZ_HEADLESS=1
 * SDL_AUDIODRIVER=dummy):
 *   RR4_FRAME_LIMIT=n  exit after n VSyncs
 *   RR4_CAPTURE=file   write the displayed frame as a PPM when exiting
 *   RR4_VRAM=file      write the whole VRAM as a PPM when exiting
 *   RR4_TRACE=n        print the mode state every n VSyncs */

#include <psyz.h>
#include <psyz/cd.h>
#include <psyz/system.h>
#include <psyz/video.h>
#include <stdio.h>
#include <stdlib.h>

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
    }
    chained_vsync = Psyz_SetVSyncCb(host_vsync);
    MainLoop();
    return 0;
}
