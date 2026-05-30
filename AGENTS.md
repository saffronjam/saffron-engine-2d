# saffron-engine-2d — engine submodule

2D framework used by fractals: SFML 2.x (window/graphics/audio/input), ImGui (with the engine's **own**
SFML backend in `src/saffron/gui/GuiImpl.cpp` — `BackendPlatformName = "imgui_impl_sfml"`), OpenGL via
Glad (compute + pixel shaders), Box2D (physics), spdlog (logging), jcv (Voronoi). Static library;
the client links it. `git submodule` of `saffronjam/saffron-engine-2d`.

## src/saffron layout

`Core/` (App, Window, Run, Clock, ThreadPool, SIMD.h, Singleton), `Graphics/`, `Gui/` (ImGui panes incl.
`ViewportPane`, `Terminal`), `Input/`, `Lighting/`, `Math/`, `Physics/` (Box2D wrappers — fractals uses
none of it), `Platform/` (per-OS file dialogs), `Log/`, `Resource/`. `entry_point.h` provides `main()`
(not `WinMain`) and calls the client's `saffron::CreateApplication()`.

## Build module pattern

`premake5.lua` is a Lua module exposing `Include()/Link()/AddDefines()/PreBuild()/PostBuild()`, called by
the client. `deps/<dep>/premake5.lua` each expose `Include()/Link()`. Header-only deps (spdlog, jcv)
are `kind "None"`; ImGui builds from source; SFML/Glad/Box2D link prebuilts. `utils.lua` `CopyCmd` is the
asset-copy helper (POSIX-merge-aware). Regenerate after edits.

## Platform abstraction

- `config.h`, `Core/macros.h`, `Core/App.cpp` already branch on `SE_PLATFORM_WINDOWS` / `SE_PLATFORM_LINUX`.
- `Platform/Windows/WindowsFileIOManager.cpp` (commdlg dialogs) and `Platform/Linux/LinuxFileIOManager.cpp`
  (zenity) each implement `Filesystem::OpenFile/SaveFile`; premake compiles only the matching one
  (`removefiles` filters in the engine project).
- `precompiled header` (`saffron_pch.h`) is Windows-only in premake — gcc's force-include double-defines
  `#pragma once` headers, and every `.cpp` includes the PCH explicitly anyway.

## Linux-build specifics

- premake adds `system:linux` link blocks; SFML uses the system package (vendored headers are 2.5.1,
  system lib is 2.6.x — ABI-compatible). Glad rebuilt from `src/glad.c`, Box2D 2.3.1 rebuilt from source,
  both into `deps/<dep>/lib/linux/`.
- The unused ImGui DX11/Win32 backend files are compiled only under `filter "system:windows"`.
- Static-lib `-l` deps don't auto-propagate to the executable in premake gmake2, so the client's
  `premake5.lua` links `ImGui`, SFML, `Box2D`, `Glad`, `GL`, `dl`, `pthread` directly on Linux.
