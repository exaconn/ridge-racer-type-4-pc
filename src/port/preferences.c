#include "preferences.h"

#include <SDL3/SDL.h>
#include <psyz.h>
#include <psyz/audio.h>
#include <psyz/gpu.h>
#include <psyz/overlay_sdl3.h>
#include <psyz/video.h>

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREFS_FILE "ridge-racer-type-4.toml"
#define PREFS_APP "Ridge Racer Type 4"

static const char *const kButtonKeys[RR4_PREFS_BUTTONS] = {
    "l2", "r2", "l1", "r1", "triangle", "circle", "cross", "square",
    "select", "l3", "r3", "start", "up", "right", "down", "left"};
static const char *const kButtonLabels[RR4_PREFS_BUTTONS] = {
    "L2", "R2", "L1", "R1", "Triangle", "Circle", "Cross", "Square",
    "Select", "L3", "R3", "Start", "Up", "Right", "Down", "Left"};

const char *Rr4PrefsButtonKey(int button) {
    return button >= 0 && button < RR4_PREFS_BUTTONS ? kButtonKeys[button] : "";
}

const char *Rr4PrefsButtonLabel(int button) {
    return button >= 0 && button < RR4_PREFS_BUTTONS ? kButtonLabels[button] : "";
}

/* ---- defaults ---- */

static Rr4Prefs s_defaults;
static int s_defaults_ready;

void Rr4PrefsCaptureDefaults(void) {
    if (s_defaults_ready)
        return;
    memset(&s_defaults, 0, sizeof(s_defaults));
    s_defaults.scale = 1;
    s_defaults.vsync = RR4_VSYNC_AUTO;
    s_defaults.volume = 100;
    for (int i = 0; i < RR4_PREFS_BUTTONS; i++) {
        s_defaults.keys[i] = Psyz_GetKeyboardBinding_SDL3((unsigned)i);
        s_defaults.buttons[i] = Psyz_GetGamepadBinding_SDL3((unsigned)i);
    }
    s_defaults_ready = 1;
}

const Rr4Prefs *Rr4PrefsDefaults(void) {
    Rr4PrefsCaptureDefaults();
    return &s_defaults;
}

/* ---- TOML subset: tables, integers, booleans, single-line strings ---- */

static char *Trim(char *s) {
    char *end;

    while (isspace((unsigned char)*s))
        ++s;
    end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1]))
        --end;
    *end = '\0';
    return s;
}

static int ParseString(const char *text, char *out, size_t size) {
    char quote = *text++;
    size_t n = 0;

    if (quote != '"' && quote != '\'')
        return 0;
    while (*text && *text != quote) {
        unsigned char c = (unsigned char)*text++;
        if (c < 32)
            return 0;
        if (quote == '"' && c == '\\') {
            c = (unsigned char)*text++;
            if (c == 'n')
                c = '\n';
            else if (c == 'r')
                c = '\r';
            else if (c == 't')
                c = '\t';
            else if (c != '"' && c != '\\')
                return 0;
        }
        if (n + 1 >= size)
            return 0;
        out[n++] = (char)c;
    }
    if (*text++ != quote)
        return 0;
    while (isspace((unsigned char)*text))
        ++text;
    if (*text && *text != '#')
        return 0;
    out[n] = '\0';
    return 1;
}

static int ParseInt(char *value, int *out) {
    char *end = NULL;
    long parsed;

    if (!strncmp(value, "true", 4) &&
        (!value[4] || isspace((unsigned char)value[4]) || value[4] == '#')) {
        *out = 1;
        return 1;
    }
    if (!strncmp(value, "false", 5) &&
        (!value[5] || isspace((unsigned char)value[5]) || value[5] == '#')) {
        *out = 0;
        return 1;
    }
    errno = 0;
    parsed = strtol(value, &end, 10);
    end = Trim(end);
    if (errno || end == value || (*end && *end != '#') || parsed < 0 || parsed > INT_MAX)
        return 0;
    *out = (int)parsed;
    return 1;
}

static int ParseFlag(char *value, int *out) {
    int v;
    if (!ParseInt(value, &v) || v > 1)
        return 0;
    *out = v;
    return 1;
}

static int ParseValue(Rr4Prefs *p, const char *section, const char *key, char *value) {
    int v = 0;

    if (!strcmp(section, "disc")) {
        if (!strcmp(key, "image"))
            return ParseString(value, p->disc, sizeof(p->disc));
        return 1;
    }
    if (!strcmp(section, "graphics") && !strcmp(key, "vsync")) {
        char mode[16];
        if (!ParseString(value, mode, sizeof(mode)))
            return 0;
        if (!strcmp(mode, "auto"))
            p->vsync = RR4_VSYNC_AUTO;
        else if (!strcmp(mode, "on"))
            p->vsync = RR4_VSYNC_ON;
        else if (!strcmp(mode, "off"))
            p->vsync = RR4_VSYNC_OFF;
        else
            return 0;
        return 1;
    }
    if (!strcmp(section, "graphics")) {
        if (!strcmp(key, "internal_resolution")) {
            if (!ParseInt(value, &v) || v < 1 || v > PSYZ_INTERNAL_RES_MAX)
                return 0;
            if (v > 1 && (v & 1))
                ++v; /* odd scales are not offered */
            p->scale = (unsigned)v;
            return 1;
        }
        if (!strcmp(key, "fullscreen"))
            return ParseFlag(value, &p->fullscreen);
        if (!strcmp(key, "fullscreen_field_rate"))
            return ParseFlag(value, &p->fullscreen_field_rate);
        if (!strcmp(key, "video_timing")) {
            char mode[16];
            if (!ParseString(value, mode, sizeof(mode)))
                return 0;
            if (!strcmp(mode, "pal"))
                p->video_timing = RR4_VIDEO_PAL;
            else if (!strcmp(mode, "ntsc"))
                p->video_timing = RR4_VIDEO_NTSC;
            else
                return 0;
            return 1;
        }
        if (!strcmp(key, "aspect_ratio")) {
            char mode[16];
            if (!ParseString(value, mode, sizeof(mode)))
                return 0;
            p->aspect_ratio = !strcmp(mode, "widescreen") ? RR4_ASPECT_WIDESCREEN : RR4_ASPECT_CLASSIC;
            return 1;
        }
        /* Earlier files: square_pixels alone is no longer offered (classic);
         * widescreen = true was 16:9. */
        if (!strcmp(key, "square_pixels"))
            return 1;
        if (!strcmp(key, "disable_dithering"))
            return ParseFlag(value, &p->disable_dithering);
        if (!strcmp(key, "antialiasing"))
            return ParseFlag(value, &p->antialiasing);
        if (!strcmp(key, "texture_filtering"))
            return ParseFlag(value, &p->texture_filtering);
        if (!strcmp(key, "geometry_correction"))
            return ParseFlag(value, &p->geometry_correction);
        if (!strcmp(key, "frame_interpolation"))
            return ParseFlag(value, &p->frame_interpolation);
        if (!strcmp(key, "widescreen")) {
            if (!ParseFlag(value, &v))
                return 0;
            p->aspect_ratio = v ? RR4_ASPECT_WIDESCREEN : RR4_ASPECT_CLASSIC;
            return 1;
        }
        return 1;
    }
    if (!strcmp(section, "audio")) {
        if (!strcmp(key, "volume")) {
            if (!ParseInt(value, &v) || v > 100)
                return 0;
            p->volume = (unsigned)v;
            return 1;
        }
        if (!strcmp(key, "mute"))
            return ParseFlag(value, &p->mute);
        return 1;
    }
    if (!strcmp(section, "keyboard") || !strcmp(section, "controller")) {
        int keyboard = !strcmp(section, "keyboard");
        for (int i = 0; i < RR4_PREFS_BUTTONS; i++) {
            if (strcmp(key, kButtonKeys[i]))
                continue;
            if (!ParseInt(value, &v))
                return 0;
            if (keyboard) {
                if (v <= SDL_SCANCODE_UNKNOWN || v >= SDL_SCANCODE_COUNT ||
                    v == SDL_SCANCODE_ESCAPE)
                    return 0;
                p->keys[i] = v;
            } else {
                if (v > SDL_GAMEPAD_BUTTON_COUNT + 1)
                    return 0;
                p->buttons[i] = v;
            }
            return 1;
        }
        return 1;
    }
    return 1; /* unknown sections stay harmless */
}

int Rr4PrefsLoadFile(Rr4Prefs *prefs, const char *path) {
    FILE *file = Psyz_FileOpen(path, "r");
    char line[RR4_PREFS_PATH_MAX + 256];
    char section[64] = "";
    unsigned number = 0;

    if (!file)
        return 0;
    while (fgets(line, sizeof(line), file)) {
        char *s = Trim(line), *eq;
        ++number;
        if (!*s || *s == '#')
            continue;
        if (*s == '[') {
            char *end = strchr(s, ']');
            if (!end || (size_t)(end - s - 1) >= sizeof(section)) {
                fprintf(stderr, "%s:%u: invalid preference\n", path, number);
                continue;
            }
            *end = '\0';
            snprintf(section, sizeof(section), "%s", Trim(s + 1));
            continue;
        }
        eq = strchr(s, '=');
        if (!eq) {
            fprintf(stderr, "%s:%u: invalid preference\n", path, number);
            continue;
        }
        *eq = '\0';
        if (!ParseValue(prefs, section, Trim(s), Trim(eq + 1)))
            fprintf(stderr, "%s:%u: invalid preference\n", path, number);
    }
    fclose(file);
    return 1;
}

static void WriteString(FILE *file, const char *value) {
    fputc('"', file);
    for (const unsigned char *s = (const unsigned char *)value; *s; ++s) {
        if (*s == '\\' || *s == '"')
            fputc('\\', file);
        if (*s == '\n')
            fputs("\\n", file);
        else if (*s == '\r')
            fputs("\\r", file);
        else if (*s == '\t')
            fputs("\\t", file);
        else
            fputc(*s, file);
    }
    fputc('"', file);
}

static const char *Bool(int v) { return v ? "true" : "false"; }

int Rr4PrefsSaveFile(const Rr4Prefs *p, const char *path) {
    static const char *const kVsync[] = {"auto", "on", "off"};
    char temporary[RR4_PREFS_PATH_MAX + 8];
    FILE *file;
    int failed;

    if (!path || !*path)
        return 0;
    snprintf(temporary, sizeof(temporary), "%s.tmp", path);
    file = Psyz_FileOpen(temporary, "w");
    if (!file)
        return 0;
    fprintf(file,
            "# Ridge Racer Type 4 preferences (Escape opens the settings menu)\n"
            "[graphics]\n"
            "internal_resolution = %u\n"
            "fullscreen = %s\n"
            "fullscreen_field_rate = %s\n"
            "vsync = \"%s\"\n"
            "video_timing = \"%s\"\n"
            "aspect_ratio = \"%s\"\n"
            "disable_dithering = %s\n"
            "antialiasing = %s\n"
            "texture_filtering = %s\n"
            "geometry_correction = %s\n"
            "frame_interpolation = %s\n\n"
            "[audio]\n"
            "volume = %u\n"
            "mute = %s\n\n"
            "[disc]\n"
            "image = ",
            p->scale, Bool(p->fullscreen), Bool(p->fullscreen_field_rate),
            kVsync[p->vsync >= 0 && p->vsync <= 2 ? p->vsync : 0],
            p->video_timing == RR4_VIDEO_NTSC ? "ntsc" : "pal",
            p->aspect_ratio == RR4_ASPECT_WIDESCREEN ? "widescreen" : "classic",
            Bool(p->disable_dithering),
            Bool(p->antialiasing), Bool(p->texture_filtering),
            Bool(p->geometry_correction), Bool(p->frame_interpolation),
            p->volume, Bool(p->mute));
    WriteString(file, p->disc);
    fputs("\n\n# SDL scancodes\n[keyboard]\n", file);
    for (int i = 0; i < RR4_PREFS_BUTTONS; i++)
        fprintf(file, "%s = %d\n", kButtonKeys[i], p->keys[i]);
    fprintf(file, "\n# SDL gamepad buttons; %d and %d are the left and right trigger\n"
                  "[controller]\n",
            SDL_GAMEPAD_BUTTON_COUNT, SDL_GAMEPAD_BUTTON_COUNT + 1);
    for (int i = 0; i < RR4_PREFS_BUTTONS; i++)
        fprintf(file, "%s = %d\n", kButtonKeys[i], p->buttons[i]);
    failed = ferror(file);
    if (fclose(file))
        failed = 1;
    if (failed || !SDL_RenamePath(temporary, path)) {
        SDL_RemovePath(temporary);
        return 0;
    }
    return 1;
}

/* ---- locations ---- */

static char s_path[RR4_PREFS_PATH_MAX];

const char *Rr4PrefsPath(void) { return s_path; }

static int Exists(const char *path) {
    SDL_PathInfo info;
    return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_FILE;
}

/* A portable file next to the executable (or in the working directory)
 * keeps the preferences there. */
static int PortableDirectory(const char *base, char *out, size_t size) {
    char path[RR4_PREFS_PATH_MAX];

    if (base) {
        snprintf(path, sizeof(path), "%s" PREFS_FILE, base);
        if (Exists(path)) {
            snprintf(out, size, "%s", base);
            return 1;
        }
#ifdef __APPLE__
        if (strstr(base, ".app/Contents/Resources/")) {
            snprintf(path, sizeof(path), "%s../../../" PREFS_FILE, base);
            if (Exists(path)) {
                snprintf(out, size, "%s../../../", base);
                return 1;
            }
        }
#endif
    }
    if (Exists(PREFS_FILE)) {
        char *cwd = SDL_GetCurrentDirectory();
        if (cwd) {
            size_t n = strlen(cwd);
            while (n > 1 && (cwd[n - 1] == '/' || cwd[n - 1] == '\\'))
                cwd[--n] = '\0';
            snprintf(out, size, "%s/", cwd);
            SDL_free(cwd);
            return 1;
        }
    }
    return 0;
}

static int ResolvePath(void) {
    const char *base = SDL_GetBasePath();
    const char *override = SDL_getenv("RR4_CONFIG_HOME");
    char directory[RR4_PREFS_PATH_MAX];

    s_path[0] = '\0';
    if (override && *override) {
        SDL_CreateDirectory(override);
        snprintf(s_path, sizeof(s_path), "%s/" PREFS_FILE, override);
        return 1;
    }
    if (PortableDirectory(base, directory, sizeof(directory))) {
        snprintf(s_path, sizeof(s_path), "%s" PREFS_FILE, directory);
        return 1;
    }
#if defined(__linux__) && !defined(__ANDROID__)
    {
        const char *root = SDL_getenv("XDG_CONFIG_HOME");
        if (root && *root) {
            snprintf(directory, sizeof(directory), "%s/ridge-racer-type-4", root);
        } else {
            root = SDL_getenv("HOME");
            if (!root || !*root)
                return 0;
            snprintf(directory, sizeof(directory), "%s/.config/ridge-racer-type-4", root);
        }
        SDL_CreateDirectory(directory);
        snprintf(s_path, sizeof(s_path), "%s/" PREFS_FILE, directory);
        return 1;
    }
#else
    {
        char *pref = SDL_GetPrefPath("", PREFS_APP);
        if (!pref)
            return 0;
        snprintf(s_path, sizeof(s_path), "%s" PREFS_FILE, pref);
        SDL_free(pref);
        return 1;
    }
#endif
}

int Rr4PrefsLoad(Rr4Prefs *prefs) {
    const char *base = SDL_GetBasePath();
    char path[RR4_PREFS_PATH_MAX];

    *prefs = *Rr4PrefsDefaults();
    if (base) {
        snprintf(path, sizeof(path), "%s" PREFS_FILE, base);
        Rr4PrefsLoadFile(prefs, path);
#ifdef __APPLE__
        if (strstr(base, ".app/Contents/Resources/")) {
            snprintf(path, sizeof(path), "%s../../../" PREFS_FILE, base);
            Rr4PrefsLoadFile(prefs, path);
        }
#endif
    }
    if (!base || strcmp(base, "./"))
        Rr4PrefsLoadFile(prefs, PREFS_FILE);
    if (!ResolvePath())
        return 0;
    Rr4PrefsLoadFile(prefs, s_path);
    return 1;
}

int Rr4PrefsSave(const Rr4Prefs *prefs) { return Rr4PrefsSaveFile(prefs, s_path); }

/* ---- applying ---- */

void Rr4ApplyWidescreen(int enabled) {
    /* The squeeze fits the display mode, so the buffer holds exactly the
     * 16:9 view at square pixels (rr/widescreen.h). Frames shown without it
     * (menus, 2D screens, movies) keep the display aspect, presented exactly
     * as in Classic 4:3 with bars at the sides. Switched on: the buffers
     * drawn so far (RR4's display buffers start at rows 0 and 256,
     * system/frmenv.c) are shown as drawn, 4:3, until the game marks its
     * next frames (camera/viewcam.c WideCommitFrame). Without widescreen
     * support everything stays Classic. */
    Psyz_VideoSetWidescreenSqueeze(0.0f);
    Psyz_VideoSetWidescreenNativeAspect(PSYZ_ASPECT_DISPLAY);
    if (enabled && !Psyz_VideoGetWidescreen() && Psyz_VideoSetWidescreen(1) == 0) {
        Psyz_VideoSetWidescreenNative(0);
        Psyz_VideoSetWidescreenNative(256);
    } else if (!enabled) {
        Psyz_VideoSetWidescreen(0);
    }
    Psyz_VideoSetAspectMode(Psyz_VideoGetWidescreen() ? PSYZ_ASPECT_SQUARE : PSYZ_ASPECT_DISPLAY);
}

void Rr4PrefsApply(const Rr4Prefs *p, int window_ready) {
    static const PsyzVsyncMode kVsync[] = {PSYZ_VSYNC_AUTO, PSYZ_VSYNC_ON, PSYZ_VSYNC_OFF};

    Psyz_VideoSetInternalResolution(p->scale >= 1 ? p->scale : 1);
    Psyz_VideoSetDitheringMode(p->disable_dithering ? PSYZ_DITHER_OFF : PSYZ_DITHER_AUTO);
    Psyz_VideoSetAntialiasing(p->antialiasing != 0);
    Psyz_VideoSetModelFiltering(p->texture_filtering != 0);
    Psyz_VideoSetGeometryCorrection(p->geometry_correction != 0);
    Psyz_GpuSetFrameInterpolation(p->frame_interpolation != 0);
    Rr4ApplyWidescreen(p->aspect_ratio == RR4_ASPECT_WIDESCREEN);
    if (Psyz_VideoGetVsyncMode() != kVsync[p->vsync >= 0 && p->vsync <= 2 ? p->vsync : 0])
        Psyz_VideoSetVsyncMode(kVsync[p->vsync >= 0 && p->vsync <= 2 ? p->vsync : 0]);
    Psyz_AudioSetOutputVolume(p->mute ? 0 : (p->volume <= 100 ? p->volume : 100));
    for (int i = 0; i < RR4_PREFS_BUTTONS; i++) {
        Psyz_SetKeyboardBinding_SDL3((unsigned)i, (SDL_Scancode)p->keys[i]);
        Psyz_SetGamepadBinding_SDL3((unsigned)i, p->buttons[i]);
    }
    Psyz_VideoSetFullscreenFieldRate(p->fullscreen_field_rate != 0);
    if (window_ready)
        Psyz_SetFullscreen_SDL3(p->fullscreen != 0);
}

/* ---- names ---- */

const char *Rr4PrefsKeyName(int scancode) {
    /* Bindings are physical scancodes: label them with the key of the active
     * layout. The bundled font is Latin-only, so other names fall back to
     * the US legend, which those keyboards also print. */
    SDL_Keycode keycode;
    const char *name = NULL;

    if (scancode <= SDL_SCANCODE_UNKNOWN || scancode >= SDL_SCANCODE_COUNT)
        return "Unbound";
    keycode = SDL_GetKeyFromScancode((SDL_Scancode)scancode, SDL_KMOD_NONE, false);
    if (keycode != SDLK_UNKNOWN)
        name = SDL_GetKeyName(keycode);
    for (const char *c = name; c && *c; c++) {
        if ((unsigned char)*c >= 0x80) {
            name = NULL;
            break;
        }
    }
    if (!name || !*name)
        name = SDL_GetScancodeName((SDL_Scancode)scancode);
    return name && *name ? name : "Unbound";
}

const char *Rr4PrefsGamepadName(int binding) {
    static const char *const kNames[] = {
        "A / Cross", "B / Circle", "X / Square", "Y / Triangle", "Back / Select",
        "Guide", "Start", "Left stick", "Right stick", "Left shoulder",
        "Right shoulder", "D-pad up", "D-pad down", "D-pad left", "D-pad right"};

    if (binding == SDL_GAMEPAD_BUTTON_COUNT)
        return "Left trigger";
    if (binding == SDL_GAMEPAD_BUTTON_COUNT + 1)
        return "Right trigger";
    if (binding >= 0 && binding < (int)(sizeof(kNames) / sizeof(kNames[0])))
        return kNames[binding];
    if (binding >= 0 && binding < SDL_GAMEPAD_BUTTON_COUNT) {
        const char *name = SDL_GetGamepadStringForButton((SDL_GamepadButton)binding);
        if (name && *name)
            return name;
    }
    return "Unbound";
}
