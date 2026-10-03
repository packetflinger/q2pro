/*
Copyright (C) 2024 q2pro-wallfly contributors

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

// Null refresh/video backend for the headless client. Implements the
// refresh.h entry points as no-ops and provides a "null" video driver so
// CL_InitRefresh() succeeds without ever opening a window or GL context.
// This lets the client-side game code (console, prediction, entity
// parsing, etc) run unmodified while producing no video output at all.

#include "shared/shared.h"
#include "common/zone.h"
#include "client/client.h"
#include "client/video.h"
#include "refresh/refresh.h"

refcfg_t r_config = {
    .width = 640,
    .height = 480,
};

bool R_Init(bool total)
{
    return true;
}

void R_Shutdown(bool total)
{
}

void R_BeginRegistration(const char *map)
{
}

// Always report success with a fake nonzero handle. Some callers (e.g.
// Con_RegisterMedia() registering the console font/background) treat a
// zero/failed handle for an essential resource as a fatal error, which
// would otherwise make this null backend unable to ever finish client
// startup. The handle value itself is never dereferenced since every
// R_Draw*/R_GetPicSize() call below is a no-op.
#define NULL_HANDLE ((qhandle_t)1)

qhandle_t R_RegisterModel(const char *name)
{
    return NULL_HANDLE;
}

qhandle_t R_RegisterImage(const char *name, imagetype_t type, imageflags_t flags)
{
    return NULL_HANDLE;
}

void R_SetSky(const char *name, float rotate, bool autorotate, const vec3_t axis)
{
}

void R_EndRegistration(void)
{
}

void R_RenderFrame(const refdef_t *fd)
{
}

void R_LightPoint(const vec3_t origin, vec3_t light)
{
    VectorClear(light);
}

void R_ClearColor(void)
{
}

void R_SetAlpha(float alpha)
{
}

void R_SetColor(uint32_t color)
{
}

void R_SetClipRect(const clipRect_t *clip)
{
}

float R_ClampScale(cvar_t *var)
{
    return 1.0f;
}

void R_SetScale(float scale)
{
}

void R_DrawChar(int x, int y, int flags, int ch, qhandle_t font)
{
}

int R_DrawString(int x, int y, int flags, size_t maxChars, const char *string, qhandle_t font)
{
    return x;
}

bool R_GetPicSize(int *w, int *h, qhandle_t pic)
{
    if (w)
        *w = 0;
    if (h)
        *h = 0;
    return false;
}

void R_DrawPic(int x, int y, qhandle_t pic)
{
}

void R_DrawStretchPic(int x, int y, int w, int h, qhandle_t pic)
{
}

void R_DrawKeepAspectPic(int x, int y, int w, int h, qhandle_t pic)
{
}

void R_DrawStretchRaw(int x, int y, int w, int h)
{
}

void R_UpdateRawPic(int pic_w, int pic_h, const uint32_t *pic)
{
}

void R_TileClear(int x, int y, int w, int h, qhandle_t pic)
{
}

void R_DrawFill8(int x, int y, int w, int h, int c)
{
}

void R_DrawFill32(int x, int y, int w, int h, uint32_t color)
{
}

void R_BeginFrame(void)
{
}

void R_EndFrame(void)
{
}

void R_ModeChanged(int width, int height, int flags)
{
}

bool R_VideoSync(void)
{
    return false;
}

r_opengl_config_t R_GetGLConfig(void)
{
    r_opengl_config_t cfg = { 0 };
    return cfg;
}

/*
==========================================================================

NULL VIDEO DRIVER

The headless client has no window and no input device. This driver only
exists to satisfy CL_InitRefresh()'s requirement that some driver claims
success; once selected, the client becomes fully active so it runs at
full speed instead of the throttled "minimized" frame rate.

==========================================================================
*/

static bool vid_null_probe(void)
{
    return true;
}

static void vid_null_pump_events(void)
{
}

static char *vid_null_get_mode_list(void)
{
    return Z_CopyString("");
}

static void vid_null_set_mode(void)
{
    // there is no window to (de)activate, so just force the client to run
    // at full speed instead of the "minimized" 10 Hz throttle
    CL_Activate(ACT_ACTIVATED);
}

const vid_driver_t vid_null = {
    .name = "null",
    .probe = vid_null_probe,
    .pump_events = vid_null_pump_events,
    .get_mode_list = vid_null_get_mode_list,
    .set_mode = vid_null_set_mode,
};
