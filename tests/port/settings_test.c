/* Headless checks of the Escape settings menu: preferences file round trip
 * and precedence, the model's input state machine and option application,
 * and the panel raster (written as PPMs into the directory argv[1]).
 * Run with PSYZ_HEADLESS=1 SDL_AUDIODRIVER=dummy. */

#include <SDL3/SDL.h>
#include <psyz.h>
#include <psyz/audio.h>
#include <psyz/overlay_sdl3.h>
#include <psyz/video.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "preferences.h"
#include "settings_model.h"
#include "settings_raster.h"

static int failures;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, __LINE__, #cond); \
            failures++;                                                      \
        }                                                                    \
    } while (0)

enum { BTN_L2 = 0, BTN_CROSS = 6, BTN_UP = 12 };

static void Do(SettingsModel *m, SettingsAction a, int times) {
    while (times-- > 0)
        SettingsModelDo(m, a);
}

static void TestDefaults(void) {
    const Rr4Prefs *d = Rr4PrefsDefaults();
    CHECK(d->scale == 1);
    CHECK(d->volume == 100);
    CHECK(!d->mute && !d->fullscreen && !d->frame_interpolation &&
          d->aspect_ratio == RR4_ASPECT_CLASSIC);
    CHECK(d->vsync == RR4_VSYNC_AUTO);
    CHECK(d->video_timing == RR4_VIDEO_PAL);
    for (int i = 0; i < RR4_PREFS_BUTTONS; i++) {
        CHECK(d->keys[i] == Psyz_GetKeyboardBinding_SDL3((unsigned)i));
        CHECK(d->buttons[i] == Psyz_GetGamepadBinding_SDL3((unsigned)i));
    }
}

static void TestGraphicsPage(void) {
    SettingsModel m;
    SettingsModelOpen(&m, Rr4PrefsDefaults(), "/discs/rr4.cue");
    CHECK(m.open && m.page == SETTINGS_PAGE_GRAPHICS && m.row == 0);
    Do(&m, SETTINGS_RIGHT, 3); /* 1x -> 2x -> 4x -> 6x */
    CHECK(m.prefs.scale == 6);
    CHECK(SettingsModelTakeChanged(&m) == 1);
    CHECK(SettingsModelTakeChanged(&m) == 0);
    Do(&m, SETTINGS_RIGHT, 2); /* 8x, wraps to 1x */
    CHECK(m.prefs.scale == 1);
    Do(&m, SETTINGS_LEFT, 1);
    CHECK(m.prefs.scale == 8);
    Do(&m, SETTINGS_DOWN, 1);
    Do(&m, SETTINGS_ACTIVATE, 1); /* display mode */
    CHECK(m.prefs.fullscreen == 1);
    Do(&m, SETTINGS_DOWN, 1);
    Do(&m, SETTINGS_RIGHT, 2);
    CHECK(m.prefs.vsync == RR4_VSYNC_OFF);
    /* Video timing: used from the next start, the status says so. */
    Do(&m, SETTINGS_DOWN, 1);
    Do(&m, SETTINGS_RIGHT, 1);
    CHECK(m.prefs.video_timing == RR4_VIDEO_NTSC);
    CHECK(strstr(SettingsModelHelp(&m), "next start") != NULL);
    Do(&m, SETTINGS_UP, 1);
    /* Up from the first row wraps to Quit game. */
    Do(&m, SETTINGS_UP, 3);
    CHECK(m.row == SettingsRowCount(&m) - 1);
    {
        SettingsRowView v;
        SettingsModelRow(&m, m.row, &v);
        CHECK(v.danger && !strcmp(v.label, "Quit game"));
    }
    /* Restore page defaults. */
    SettingsModelSelect(&m, SettingsContentRows(&m));
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(m.prefs.scale == 1 && !m.prefs.fullscreen && m.prefs.vsync == RR4_VSYNC_AUTO &&
          m.prefs.video_timing == RR4_VIDEO_PAL);
    CHECK(m.open);
    /* Back closes. */
    Do(&m, SETTINGS_BACK, 1);
    CHECK(!m.open && !m.quit);
}

static void TestControlsPage(void) {
    SettingsModel m;
    const Rr4Prefs *d = Rr4PrefsDefaults();
    SettingsRowView v;

    SettingsModelOpen(&m, d, NULL);
    Do(&m, SETTINGS_PAGE_NEXT, 1);
    CHECK(m.page == SETTINGS_PAGE_CONTROLS);
    CHECK(SettingsContentRows(&m) == 1 + RR4_PREFS_BUTTONS);
    Do(&m, SETTINGS_DOWN, 1); /* Up binding */
    SettingsModelRow(&m, m.row, &v);
    CHECK(v.kind == 2 && !strcmp(v.label, "Up"));
    CHECK(strstr(v.help, "Default:") != NULL);
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(m.capturing);
    /* Navigation is ignored while capturing; gamepad input for the keyboard
     * list too. */
    Do(&m, SETTINGS_DOWN, 1);
    CHECK(m.row == 1 && m.capturing);
    CHECK(SettingsModelCaptureButton(&m, SDL_GAMEPAD_BUTTON_SOUTH) == 0);
    /* Escape cancels the capture, not the menu. */
    Do(&m, SETTINGS_BACK, 1);
    CHECK(!m.capturing && m.open);
    /* Rebind Up to the key of L2: the two swap. */
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(SettingsModelCaptureKey(&m, SDL_SCANCODE_ESCAPE) == 0);
    CHECK(m.capturing);
    CHECK(SettingsModelCaptureKey(&m, d->keys[BTN_L2]) == 1);
    CHECK(!m.capturing);
    CHECK(m.prefs.keys[BTN_UP] == d->keys[BTN_L2]);
    CHECK(m.prefs.keys[BTN_L2] == d->keys[BTN_UP]);
    CHECK(SettingsModelTakeChanged(&m));
    /* Backspace restores the default, swapping back. */
    Do(&m, SETTINGS_CLEAR, 1);
    CHECK(m.prefs.keys[BTN_UP] == d->keys[BTN_UP]);
    CHECK(m.prefs.keys[BTN_L2] == d->keys[BTN_L2]);
    /* Controller list: bind Cross (row 5) to the right trigger. */
    SettingsModelSelect(&m, 0);
    Do(&m, SETTINGS_RIGHT, 1);
    CHECK(m.device == SETTINGS_DEVICE_CONTROLLER);
    SettingsModelSelect(&m, 5);
    SettingsModelRow(&m, 5, &v);
    CHECK(!strcmp(v.label, "Cross"));
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(SettingsModelCaptureKey(&m, SDL_SCANCODE_A) == 0);
    CHECK(SettingsModelCaptureButton(&m, SDL_GAMEPAD_BUTTON_COUNT + 1) == 1);
    CHECK(m.prefs.buttons[BTN_CROSS] == SDL_GAMEPAD_BUTTON_COUNT + 1);
    SettingsModelRow(&m, 5, &v);
    CHECK(!strcmp(v.value, "Right trigger"));
    /* Scrolling keeps the selection visible. */
    SettingsModelSelect(&m, RR4_PREFS_BUTTONS);
    CHECK(SettingsRowTop(&m, m.row) >= 0.0f);
    CHECK(SettingsRowTop(&m, 0) < 0.0f);
    /* Restore page defaults resets the listed device only. */
    m.prefs.keys[BTN_UP] = SDL_SCANCODE_I;
    SettingsModelSelect(&m, SettingsContentRows(&m));
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(m.prefs.buttons[BTN_CROSS] == d->buttons[BTN_CROSS]);
    CHECK(m.prefs.keys[BTN_UP] == SDL_SCANCODE_I);
}

static void TestAudioAndDisc(void) {
    SettingsModel m;
    SettingsRowView v;

    SettingsModelOpen(&m, Rr4PrefsDefaults(), "/discs/Ridge Racer Type 4 (Europe).cue");
    SettingsModelSwitchPage(&m, SETTINGS_PAGE_AUDIO);
    Do(&m, SETTINGS_RIGHT, 1); /* clamps at 100% */
    CHECK(m.prefs.volume == 100);
    Do(&m, SETTINGS_LEFT, 3);
    CHECK(m.prefs.volume == 85);
    Do(&m, SETTINGS_LEFT, 30);
    CHECK(m.prefs.volume == 0);
    Do(&m, SETTINGS_DOWN, 1);
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(m.prefs.mute == 1);

    Do(&m, SETTINGS_PAGE_NEXT, 1);
    CHECK(m.page == SETTINGS_PAGE_DISC);
    SettingsModelRow(&m, 0, &v);
    CHECK(!strcmp(v.value, "Ridge Racer Type 4 (Europe).cue"));
    Do(&m, SETTINGS_DOWN, 1);
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(m.choose_disc);
    SettingsModelDiscChosen(&m, "/x/track.bin");
    CHECK(!m.choose_disc && m.prefs.disc[0] == '\0');
    SettingsModelDiscChosen(&m, NULL);
    CHECK(m.prefs.disc[0] == '\0');
    SettingsModelDiscChosen(&m, "/y/RR4.CUE");
    CHECK(!strcmp(m.prefs.disc, "/y/RR4.CUE"));
    SettingsModelRow(&m, 1, &v);
    CHECK(!strcmp(v.value, "RR4.CUE"));
    /* The mounted image is unaffected. */
    SettingsModelRow(&m, 0, &v);
    CHECK(!strcmp(v.value, "Ridge Racer Type 4 (Europe).cue"));
    /* Next page wraps to Graphics; Quit game closes and quits. */
    Do(&m, SETTINGS_PAGE_NEXT, 1);
    CHECK(m.page == SETTINGS_PAGE_GRAPHICS);
    SettingsModelSelect(&m, SettingsRowCount(&m) - 1);
    Do(&m, SETTINGS_ACTIVATE, 1);
    CHECK(!m.open && m.quit && m.edited);
}

static void TestPointer(void) {
    SettingsModel m;
    SettingsRect tab = SettingsTabRect(SETTINGS_PAGE_AUDIO);
    float top;

    SettingsModelOpen(&m, Rr4PrefsDefaults(), NULL);
    SettingsModelPointer(&m, tab.x + 4, tab.y + 4, 1);
    CHECK(m.page == SETTINGS_PAGE_AUDIO);
    top = SettingsRowTop(&m, 0);
    SettingsModelPointer(&m, SettingsValueCentre() - 60, top + 10, 1); /* left of value: down */
    CHECK(m.prefs.volume == 95);
    SettingsModelPointer(&m, -10, -10, 0); /* hover outside: nothing */
    CHECK(m.open);
    SettingsModelPointer(&m, -10, -10, 1);
    CHECK(!m.open);
}

static void TestPrefsFile(const char *dir) {
    char path[1024];
    Rr4Prefs p = *Rr4PrefsDefaults(), q = *Rr4PrefsDefaults();
    FILE *f;

    snprintf(path, sizeof(path), "%s/roundtrip.toml", dir);
    p.scale = 4;
    p.fullscreen = 1;
    p.vsync = RR4_VSYNC_ON;
    p.antialiasing = 1;
    p.aspect_ratio = RR4_ASPECT_WIDESCREEN;
    p.video_timing = RR4_VIDEO_NTSC;
    p.volume = 35;
    p.mute = 1;
    p.keys[BTN_UP] = SDL_SCANCODE_I;
    p.buttons[BTN_CROSS] = SDL_GAMEPAD_BUTTON_COUNT;
    snprintf(p.disc, sizeof(p.disc), "%s", "/games/rr4 \"PAL\"/Ridge Racer Type 4.cue");
    CHECK(Rr4PrefsSaveFile(&p, path));
    CHECK(Rr4PrefsLoadFile(&q, path));
    CHECK(!memcmp(&p, &q, sizeof(p)));

    /* Invalid values are skipped, the rest still loads. */
    snprintf(path, sizeof(path), "%s/partial.toml", dir);
    f = fopen(path, "w");
    CHECK(f != NULL);
    if (f) {
        fputs("[graphics]\ninternal_resolution = 99\nfullscreen = true\nvsync = \"sometimes\"\n"
              "internal_resolution = 3\n[keyboard]\nup = 41\ncross = 7\nnonsense\n[future]\nx = 1\n",
              f);
        fclose(f);
    }
    q = *Rr4PrefsDefaults();
    CHECK(Rr4PrefsLoadFile(&q, path));
    CHECK(q.scale == 4); /* 3 rounds up to an offered scale */
    CHECK(q.fullscreen == 1 && q.vsync == RR4_VSYNC_AUTO);
    CHECK(q.keys[BTN_UP] == Rr4PrefsDefaults()->keys[BTN_UP]); /* Escape refused */
    CHECK(q.keys[BTN_CROSS] == 7);
    CHECK(!Rr4PrefsLoadFile(&q, "/nonexistent/rr4.toml"));

    /* Earlier aspect keys: square pixels alone is classic, widescreen 16:9. */
    snprintf(path, sizeof(path), "%s/aspect.toml", dir);
    f = fopen(path, "w");
    CHECK(f != NULL);
    if (f) {
        fputs("[graphics]\nsquare_pixels = true\n", f);
        fclose(f);
    }
    q = *Rr4PrefsDefaults();
    CHECK(Rr4PrefsLoadFile(&q, path) && q.aspect_ratio == RR4_ASPECT_CLASSIC);
    f = fopen(path, "w");
    CHECK(f != NULL);
    if (f) {
        fputs("[graphics]\nsquare_pixels = true\nwidescreen = true\n", f);
        fclose(f);
    }
    CHECK(Rr4PrefsLoadFile(&q, path) && q.aspect_ratio == RR4_ASPECT_WIDESCREEN);
    f = fopen(path, "w");
    CHECK(f != NULL);
    if (f) {
        fputs("[graphics]\naspect_ratio = \"sideways\"\n", f);
        fclose(f);
    }
    CHECK(Rr4PrefsLoadFile(&q, path) && q.aspect_ratio == RR4_ASPECT_CLASSIC);

    /* RR4_CONFIG_HOME selects the user file and wins over other layers. */
    snprintf(path, sizeof(path), "%s/home/ridge-racer-type-4.toml", dir);
    SDL_RemovePath(path);
    snprintf(path, sizeof(path), "%s/home", dir);
    SDL_setenv_unsafe("RR4_CONFIG_HOME", path, 1);
    CHECK(Rr4PrefsLoad(&q));
    CHECK(strstr(Rr4PrefsPath(), "/home/ridge-racer-type-4.toml") != NULL);
    CHECK(q.scale == 1); /* fresh: defaults */
    q.scale = 2;
    CHECK(Rr4PrefsSave(&q));
    p = *Rr4PrefsDefaults();
    CHECK(Rr4PrefsLoad(&p));
    CHECK(p.scale == 2);
    SDL_unsetenv_unsafe("RR4_CONFIG_HOME");
}

static void TestApply(void) {
    Rr4Prefs p = *Rr4PrefsDefaults();

    p.volume = 40;
    p.keys[BTN_UP] = SDL_SCANCODE_I;
    p.buttons[BTN_CROSS] = SDL_GAMEPAD_BUTTON_COUNT + 1;
    Rr4PrefsApply(&p, 0);
    CHECK(Psyz_AudioGetOutputVolume() == 40);
    CHECK(Psyz_GetKeyboardBinding_SDL3(BTN_UP) == SDL_SCANCODE_I);
    CHECK(Psyz_GetGamepadBinding_SDL3(BTN_CROSS) == SDL_GAMEPAD_BUTTON_COUNT + 1);
    p.mute = 1;
    Rr4PrefsApply(&p, 0);
    CHECK(Psyz_AudioGetOutputVolume() == 0);
    Rr4PrefsApply(Rr4PrefsDefaults(), 0);
    CHECK(Psyz_AudioGetOutputVolume() == 100);
    CHECK(Psyz_GetKeyboardBinding_SDL3(BTN_UP) == Rr4PrefsDefaults()->keys[BTN_UP]);
}

static void TestRaster(const char *dir) {
    SettingsModel m;
    SettingsSurface surface = {0};
    SettingsLayout layout;
    char path[1024];

    CHECK(SettingsFontReady());
    SettingsModelOpen(&m, Rr4PrefsDefaults(), "/discs/Ridge Racer Type 4 (Europe).cue");
    for (int page = 0; page < SETTINGS_PAGE_COUNT; page++) {
        size_t opaque = 0;
        SettingsModelSwitchPage(&m, page);
        if (page == SETTINGS_PAGE_CONTROLS) {
            SettingsModelSelect(&m, 2);
            SettingsModelDo(&m, SETTINGS_ACTIVATE); /* shows the capture state */
        }
        SettingsComputeLayout(&m, 1280, 960, &layout);
        CHECK(layout.w > 0 && layout.h > 0 && layout.h <= 960 && layout.x >= 0 && layout.y >= 0);
        CHECK(SettingsSurfaceResize(&surface, layout.w, layout.h));
        SettingsPaint(&surface, &m, &layout);
        for (size_t i = 0; i < (size_t)layout.w * (size_t)layout.h; i++)
            opaque += surface.pixels[i * 4 + 3] > 200;
        CHECK(opaque > (size_t)layout.w * (size_t)layout.h / 2);
        snprintf(path, sizeof(path), "%s/panel-%s.ppm", dir, SettingsPageName(page));
        CHECK(SettingsSurfaceDump(&surface, path));
        if (page == SETTINGS_PAGE_CONTROLS) {
            CHECK(m.capturing);
            SettingsModelDo(&m, SETTINGS_BACK); /* ends the capture only */
            CHECK(m.open && !m.capturing);
        }
    }
    SettingsSurfaceRelease(&surface);
}

int main(int argc, char **argv) {
    const char *dir = argc > 1 ? argv[1] : "settings-test";

    SDL_CreateDirectory(dir);
    Rr4PrefsCaptureDefaults();
    TestDefaults();
    TestGraphicsPage();
    TestControlsPage();
    TestAudioAndDisc();
    TestPointer();
    TestPrefsFile(dir);
    TestApply();
    TestRaster(dir);
    if (failures) {
        fprintf(stderr, "settings_test: %d failures\n", failures);
        return 1;
    }
    printf("settings_test: ok (panels in %s)\n", dir);
    return 0;
}
