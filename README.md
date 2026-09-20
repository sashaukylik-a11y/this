# MonoClient Full v25

Minecraft 1.21.11 external Win32 client. Author: `MonoClient | @monobrowser`.

## v25 menu reliability fixes

- Right Shift is registered with `RegisterHotKey` and also has a `GetAsyncKeyState` fallback.
- A short tap can be caught by the transition bit; a held key is caught by the high bit.
- Opening the menu always uses a topmost `SetWindowPos(..., SWP_SHOWWINDOW)` path so borderless Minecraft cannot leave it behind the game window.
- Closing releases mouse capture, hides the menu, restores the configured topmost state, and returns focus to Minecraft.
- Existing v2/v3 configs migrate to v4 and reset only UI visibility defaults (`HUD=ON`, `Always on top=ON`). Combat settings are preserved.
- The HUD remains `WS_EX_NOACTIVATE`, so it does not steal mouse focus.

## Build

Use an x64 Visual Studio/LLVM developer shell with Windows SDK import libraries available:

```bat
clang-cl /nologo /c /O2 /std:c++17 /W4 /WX /GS- /Gs9999999 /GR- /EHs-c- /DUNICODE /D_UNICODE /Fo:MonoClient.obj MonoClient.cpp
lld-link /nologo /brepro /entry:wWinMainCRTStartup /subsystem:windows /nodefaultlib /machine:x64 /out:MonoClient-Full-v25.exe MonoClient.obj kernel32.lib user32.lib gdi32.lib
```

## Controls

- `Right Shift`: open/close menu globally.
- `Esc`: close menu when it has focus.
- The menu is forced topmost while visible.

## Config reset

To completely reset settings, close MonoClient and delete `MonoClient.cfg` next to the EXE. v25 automatically migrates older configs without deleting combat toggles.

## Notes

The JVM/HotSpot reader remains external/read-only. v25 does not add `WriteProcessMemory`, remote threads, DLL injection, or Windows hooks.
