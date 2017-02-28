#pragma once

#ifndef CPPGUI_RENDERER_HEADER
#define CPPGUI_RENDERER_HEADER <cppgui/builtin_drivers/renderer_opengl.hpp>
#endif

#ifndef CPPGUI_INPUT_ADAPTER_HEADER
// For now, this header imports all the SDL-related code, not just input.
#define CPPGUI_INPUT_ADAPTER_HEADER <cppgui/builtin_drivers/session_sdl2.hpp>
// TODO: make this configurable
#endif
