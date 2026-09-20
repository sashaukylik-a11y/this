// MonoClient | @monobrowser
// Standalone Win32 x64 Minecraft 1.21.11 external client. Full v25 reliable menu + resilient HotSpot recovery build.
// One-EXE distribution: dynamic Pulse + Java PID discovery, read-only HotSpot/JVM inspection and SendInput.
// v25 stays external/read-only. It keeps v24 JVM recovery and hardens global menu hotkey/topmost/focus behavior.
// Nothing is written into the Minecraft directory.

#define WIN32_LEAN_AND_MEAN

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef long LONG;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef unsigned long long SIZE_T;
typedef int BOOL;
typedef unsigned int UINT;
typedef unsigned long ULONG;
typedef unsigned long long ULONG_PTR;
typedef long long LONG_PTR;
typedef unsigned long long UINT_PTR;
typedef unsigned long long WPARAM;
typedef long long LPARAM;
typedef long long LRESULT;
typedef void* HANDLE;
typedef void* HWND;
typedef void* HINSTANCE;
typedef void* HMODULE;
typedef void* HICON;
typedef void* HCURSOR;
typedef void* HBRUSH;
typedef void* HBITMAP;
typedef void* HFONT;
typedef void* HPEN;
typedef void* HGDIOBJ;
typedef void* HDC;
typedef void* HRGN;
typedef const wchar_t* LPCWSTR;
typedef const char* LPCSTR;
typedef char* LPSTR;
typedef wchar_t* LPWSTR;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef unsigned long COLORREF;
typedef unsigned short ATOM;
typedef unsigned long long DWORD_PTR;

#define WINAPI __stdcall
#define CALLBACK __stdcall
#define TRUE 1
#define FALSE 0
#define NULL 0
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

struct POINT { LONG x; LONG y; };
struct RECT { LONG left; LONG top; LONG right; LONG bottom; };
struct FILETIME { DWORD dwLowDateTime; DWORD dwHighDateTime; };
struct PROCESSENTRY32W {
    DWORD dwSize; DWORD cntUsage; DWORD th32ProcessID; ULONG_PTR th32DefaultHeapID;
    DWORD th32ModuleID; DWORD cntThreads; DWORD th32ParentProcessID; LONG pcPriClassBase;
    DWORD dwFlags; wchar_t szExeFile[260];
};
struct MODULEENTRY32W {
    DWORD dwSize; DWORD th32ModuleID; DWORD th32ProcessID; DWORD GlblcntUsage; DWORD ProccntUsage;
    BYTE* modBaseAddr; DWORD modBaseSize; HMODULE hModule; wchar_t szModule[256]; wchar_t szExePath[260];
};
struct MOUSEINPUT { LONG dx; LONG dy; DWORD mouseData; DWORD dwFlags; DWORD time; ULONG_PTR dwExtraInfo; };
struct INPUT { DWORD type; DWORD pad; MOUSEINPUT mi; };
struct PROCESS_MEMORY_COUNTERS {
    DWORD cb; DWORD PageFaultCount; SIZE_T PeakWorkingSetSize; SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage; SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage; SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage; SIZE_T PeakPagefileUsage;
};
struct MEMORY_BASIC_INFORMATION_X64 {
    LPVOID BaseAddress;
    LPVOID AllocationBase;
    DWORD AllocationProtect;
    DWORD Alignment1;
    SIZE_T RegionSize;
    DWORD State;
    DWORD Protect;
    DWORD Type;
    DWORD Alignment2;
};
struct PAINTSTRUCT {
    HDC hdc; BOOL fErase; RECT rcPaint; BOOL fRestore; BOOL fIncUpdate; BYTE rgbReserved[32];
};
struct MSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; DWORD lPrivate; };
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
struct WNDCLASSEXW {
    UINT cbSize; UINT style; WNDPROC lpfnWndProc; int cbClsExtra; int cbWndExtra;
    HINSTANCE hInstance; HICON hIcon; HCURSOR hCursor; HBRUSH hbrBackground;
    LPCWSTR lpszMenuName; LPCWSTR lpszClassName; HICON hIconSm;
};

static_assert(sizeof(DWORD)==4, "Win32 DWORD ABI mismatch");
static_assert(sizeof(SIZE_T)==8, "x64 SIZE_T ABI mismatch");
static_assert(sizeof(PROCESSENTRY32W)==568, "PROCESSENTRY32W ABI mismatch");
static_assert(sizeof(PROCESS_MEMORY_COUNTERS)==72, "PROCESS_MEMORY_COUNTERS ABI mismatch");
static_assert(sizeof(MEMORY_BASIC_INFORMATION_X64)==48, "MEMORY_BASIC_INFORMATION x64 ABI mismatch");
static_assert(sizeof(MODULEENTRY32W)==1080, "MODULEENTRY32W ABI mismatch");
static_assert(sizeof(INPUT)==40, "INPUT ABI mismatch");
static_assert(sizeof(PAINTSTRUCT)==72, "PAINTSTRUCT ABI mismatch");
static_assert(sizeof(MSG)==48, "MSG ABI mismatch");
static_assert(sizeof(WNDCLASSEXW)==80, "WNDCLASSEXW ABI mismatch");

extern "C" {
__declspec(dllimport) HANDLE WINAPI CreateToolhelp32Snapshot(DWORD,DWORD);
__declspec(dllimport) BOOL WINAPI Process32FirstW(HANDLE,PROCESSENTRY32W*);
__declspec(dllimport) BOOL WINAPI Process32NextW(HANDLE,PROCESSENTRY32W*);
__declspec(dllimport) BOOL WINAPI Module32FirstW(HANDLE,MODULEENTRY32W*);
__declspec(dllimport) BOOL WINAPI Module32NextW(HANDLE,MODULEENTRY32W*);
__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE);
__declspec(dllimport) HANDLE WINAPI OpenProcess(DWORD,BOOL,DWORD);
__declspec(dllimport) BOOL WINAPI QueryFullProcessImageNameW(HANDLE,DWORD,LPWSTR,DWORD*);
__declspec(dllimport) BOOL WINAPI ReadProcessMemory(HANDLE,LPCVOID,LPVOID,SIZE_T,SIZE_T*);
__declspec(dllimport) SIZE_T WINAPI VirtualQueryEx(HANDLE,LPCVOID,MEMORY_BASIC_INFORMATION_X64*,SIZE_T);
__declspec(dllimport) BOOL WINAPI GetProcessTimes(HANDLE,FILETIME*,FILETIME*,FILETIME*,FILETIME*);
__declspec(dllimport) BOOL WINAPI K32GetProcessMemoryInfo(HANDLE,PROCESS_MEMORY_COUNTERS*,DWORD);
__declspec(dllimport) DWORD WINAPI GetCurrentProcessId();
__declspec(dllimport) ULONGLONG WINAPI GetTickCount64();
__declspec(dllimport) void WINAPI Sleep(DWORD);
__declspec(dllimport) HANDLE WINAPI GetCurrentThread();
__declspec(dllimport) BOOL WINAPI SetThreadPriority(HANDLE,int);
__declspec(dllimport) HANDLE WINAPI CreateFileW(LPCWSTR,DWORD,DWORD,LPVOID,DWORD,DWORD,HANDLE);
__declspec(dllimport) BOOL WINAPI ReadFile(HANDLE,LPVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) BOOL WINAPI WriteFile(HANDLE,LPCVOID,DWORD,DWORD*,LPVOID);
__declspec(dllimport) DWORD WINAPI GetModuleFileNameW(HINSTANCE,LPWSTR,DWORD);
__declspec(dllimport) HINSTANCE WINAPI GetModuleHandleW(LPCWSTR);
__declspec(dllimport) void WINAPI ExitProcess(UINT);
__declspec(dllimport) HANDLE WINAPI CreateThread(LPVOID,SIZE_T,DWORD (WINAPI*)(LPVOID),LPVOID,DWORD,DWORD*);
__declspec(dllimport) DWORD WINAPI WaitForSingleObject(HANDLE,DWORD);

__declspec(dllimport) ATOM WINAPI RegisterClassExW(const WNDCLASSEXW*);
__declspec(dllimport) HCURSOR WINAPI LoadCursorW(HINSTANCE,LPCWSTR);
__declspec(dllimport) HWND WINAPI CreateWindowExW(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,void*,HINSTANCE,LPVOID);
__declspec(dllimport) LRESULT WINAPI DefWindowProcW(HWND,UINT,WPARAM,LPARAM);
__declspec(dllimport) BOOL WINAPI ShowWindow(HWND,int);
__declspec(dllimport) BOOL WINAPI UpdateWindow(HWND);
__declspec(dllimport) BOOL WINAPI GetMessageW(MSG*,HWND,UINT,UINT);
__declspec(dllimport) BOOL WINAPI TranslateMessage(const MSG*);
__declspec(dllimport) LRESULT WINAPI DispatchMessageW(const MSG*);
__declspec(dllimport) void WINAPI PostQuitMessage(int);
__declspec(dllimport) HDC WINAPI BeginPaint(HWND,PAINTSTRUCT*);
__declspec(dllimport) BOOL WINAPI EndPaint(HWND,const PAINTSTRUCT*);
__declspec(dllimport) UINT_PTR WINAPI SetTimer(HWND,UINT_PTR,UINT,void*);
__declspec(dllimport) BOOL WINAPI KillTimer(HWND,UINT_PTR);
__declspec(dllimport) BOOL WINAPI RegisterHotKey(HWND,int,UINT,UINT);
__declspec(dllimport) BOOL WINAPI UnregisterHotKey(HWND,int);
__declspec(dllimport) short WINAPI GetAsyncKeyState(int);
__declspec(dllimport) BOOL WINAPI InvalidateRect(HWND,const RECT*,BOOL);
__declspec(dllimport) int WINAPI GetSystemMetrics(int);
__declspec(dllimport) BOOL WINAPI SetWindowPos(HWND,HWND,int,int,int,int,UINT);
__declspec(dllimport) BOOL WINAPI SetForegroundWindow(HWND);
__declspec(dllimport) HWND WINAPI GetForegroundWindow();
__declspec(dllimport) BOOL WINAPI EnumWindows(BOOL (CALLBACK*)(HWND,LPARAM),LPARAM);
__declspec(dllimport) BOOL WINAPI IsWindowVisible(HWND);
__declspec(dllimport) DWORD WINAPI GetWindowThreadProcessId(HWND,DWORD*);
__declspec(dllimport) int WINAPI GetWindowTextW(HWND,LPWSTR,int);
__declspec(dllimport) int WINAPI GetWindowTextLengthW(HWND);
__declspec(dllimport) int WINAPI DrawTextW(HDC,LPCWSTR,int,RECT*,UINT);
__declspec(dllimport) BOOL WINAPI GetClientRect(HWND,RECT*);
__declspec(dllimport) BOOL WINAPI GetWindowRect(HWND,RECT*);
__declspec(dllimport) BOOL WINAPI ClientToScreen(HWND,POINT*);
__declspec(dllimport) UINT WINAPI SendInput(UINT,INPUT*,int);
__declspec(dllimport) HWND WINAPI SetCapture(HWND);
__declspec(dllimport) BOOL WINAPI ReleaseCapture();
__declspec(dllimport) LRESULT WINAPI SendMessageW(HWND,UINT,WPARAM,LPARAM);
__declspec(dllimport) BOOL WINAPI SetLayeredWindowAttributes(HWND,COLORREF,BYTE,DWORD);
__declspec(dllimport) int WINAPI SetWindowRgn(HWND,HRGN,BOOL);

__declspec(dllimport) HDC WINAPI CreateCompatibleDC(HDC);
__declspec(dllimport) HBITMAP WINAPI CreateCompatibleBitmap(HDC,int,int);
__declspec(dllimport) BOOL WINAPI DeleteDC(HDC);
__declspec(dllimport) BOOL WINAPI BitBlt(HDC,int,int,int,int,HDC,int,int,DWORD);
__declspec(dllimport) BOOL WINAPI StretchBlt(HDC,int,int,int,int,HDC,int,int,int,int,DWORD);
__declspec(dllimport) int WINAPI SetStretchBltMode(HDC,int);
__declspec(dllimport) BOOL WINAPI SetViewportOrgEx(HDC,int,int,POINT*);
__declspec(dllimport) int WINAPI SelectClipRgn(HDC,HRGN);
__declspec(dllimport) HRGN WINAPI CreateRectRgn(int,int,int,int);
__declspec(dllimport) HBRUSH WINAPI CreateSolidBrush(COLORREF);
__declspec(dllimport) BOOL WINAPI DeleteObject(HGDIOBJ);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(HDC,HGDIOBJ);
__declspec(dllimport) COLORREF WINAPI SetTextColor(HDC,COLORREF);
__declspec(dllimport) int WINAPI SetBkMode(HDC,int);
__declspec(dllimport) BOOL WINAPI RoundRect(HDC,int,int,int,int,int,int);
__declspec(dllimport) HFONT WINAPI CreateFontW(int,int,int,int,int,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,LPCWSTR);
__declspec(dllimport) HGDIOBJ WINAPI GetStockObject(int);
__declspec(dllimport) HRGN WINAPI CreateRoundRectRgn(int,int,int,int,int,int);
}

extern "C" int _fltused = 0;
extern "C" void* memset(void* p, int v, SIZE_T n){ BYTE* b=(BYTE*)p; for(SIZE_T i=0;i<n;i++) b[i]=(BYTE)v; return p; }
extern "C" void* memcpy(void* d,const void* s,SIZE_T n){ BYTE* a=(BYTE*)d; const BYTE* b=(const BYTE*)s; for(SIZE_T i=0;i<n;i++) a[i]=b[i]; return d; }
extern "C" void* memmove(void* d,const void* s,SIZE_T n){ BYTE* a=(BYTE*)d; const BYTE* b=(const BYTE*)s; if(a<b){for(SIZE_T i=0;i<n;i++)a[i]=b[i];}else if(a>b){for(SIZE_T i=n;i;i--)a[i-1]=b[i-1];} return d; }

// Win32 constants
#define TH32CS_SNAPPROCESS 0x00000002
#define TH32CS_SNAPMODULE 0x00000008
#define TH32CS_SNAPMODULE32 0x00000010
#define PROCESS_VM_READ 0x0010
#define MEM_COMMIT 0x00001000
#define PAGE_READWRITE 0x00000004
#define WAIT_OBJECT_0 0x00000000
#define MEM_IMAGE 0x01000000
#define MEM_MAPPED 0x00040000
#define MEM_PRIVATE 0x00020000
#define PAGE_EXECUTE 0x00000010
#define PAGE_EXECUTE_READ 0x00000020
#define PAGE_EXECUTE_READWRITE 0x00000040
#define PAGE_EXECUTE_WRITECOPY 0x00000080
#define PAGE_NOACCESS 0x00000001
#define PAGE_GUARD 0x00000100
#define INPUT_MOUSE 0
#define MOUSEEVENTF_LEFTDOWN 0x0002
#define MOUSEEVENTF_LEFTUP 0x0004
#define PROCESS_QUERY_INFORMATION 0x0400
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#define GENERIC_READ 0x80000000UL
