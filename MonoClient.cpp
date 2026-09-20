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

#define GENERIC_WRITE 0x40000000UL
#define FILE_SHARE_READ 0x00000001
#define OPEN_EXISTING 3
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define WS_POPUP 0x80000000UL
#define WS_EX_TOOLWINDOW 0x00000080UL
#define WS_EX_TOPMOST 0x00000008UL
#define WS_EX_LAYERED 0x00080000UL
#define WS_EX_TRANSPARENT 0x00000020UL
#define WS_EX_NOACTIVATE 0x08000000UL
#define CS_HREDRAW 0x0002
#define THREAD_PRIORITY_BELOW_NORMAL -1
#define THREAD_PRIORITY_LOWEST -2
#define THREAD_PRIORITY_IDLE -15
#define CS_VREDRAW 0x0001
#define WM_DESTROY 0x0002
#define WM_MOVE 0x0003
#define WM_PAINT 0x000F
#define WM_ERASEBKGND 0x0014
#define WM_TIMER 0x0113
#define WM_HOTKEY 0x0312
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_MOUSEMOVE 0x0200
#define WM_KEYDOWN 0x0100
#define WM_NCLBUTTONDOWN 0x00A1
#define WM_SIZE 0x0005
#define HTCAPTION 2
#define VK_RSHIFT 0xA1
#define VK_ESCAPE 0x1B
#define VK_LBUTTON 0x01
#define SW_HIDE 0
#define SW_SHOW 5
#define SW_SHOWNOACTIVATE 4
#define SW_SHOWNORMAL 1
#define SM_CXSCREEN 0
#define SM_CYSCREEN 1
#define HWND_TOPMOST ((HWND)(LONG_PTR)-1)
#define HWND_NOTOPMOST ((HWND)(LONG_PTR)-2)
#define SWP_NOMOVE 0x0002
#define SWP_NOSIZE 0x0001
#define SWP_NOACTIVATE 0x0010
#define SWP_SHOWWINDOW 0x0040
#define MOD_NOREPEAT 0x4000
#define MENU_HOTKEY_ID 0x4D43
#define LWA_COLORKEY 0x00000001
#define LWA_ALPHA 0x00000002
#define WM_RBUTTONDOWN 0x0204
#define WM_MOUSEWHEEL 0x020A
#define TRANSPARENT 1
#define SRCCOPY 0x00CC0020
#define COLORONCOLOR 3
#define HALFTONE 4
#define PS_SOLID 0
#define NULL_PEN 8
#define BLACK_BRUSH 4
#define FW_NORMAL 400
#define FW_SEMIBOLD 600
#define FW_BOLD 700
#define DEFAULT_CHARSET 1
#define OUT_DEFAULT_PRECIS 0
#define CLIP_DEFAULT_PRECIS 0
#define CLEARTYPE_QUALITY 5
#define DEFAULT_PITCH 0
#define FF_DONTCARE 0
#define DT_LEFT 0x00000000
#define DT_CENTER 0x00000001
#define DT_RIGHT 0x00000002
#define DT_VCENTER 0x00000004
#define DT_SINGLELINE 0x00000020
#define DT_END_ELLIPSIS 0x00008000
#define DT_NOPREFIX 0x00000800
#define COLOR_WINDOW 5

static COLORREF RGBc(int r,int g,int b){ return (COLORREF)(r | (g<<8) | (b<<16)); }
static int LoWord(LPARAM v){ return (int)((WORD)(v & 0xffff)); }
static int HiWord(LPARAM v){ return (int)((WORD)((v>>16)&0xffff)); }
static ULONGLONG FtToU64(const FILETIME& f){ return ((ULONGLONG)f.dwHighDateTime<<32) | f.dwLowDateTime; }

static int WLen(const wchar_t* s){ int n=0; while(s && s[n]) n++; return n; }
static void WCopy(wchar_t* d,const wchar_t* s,int cap){ if(cap<=0)return; int i=0; for(;i<cap-1 && s && s[i];i++) d[i]=s[i]; d[i]=0; }
static wchar_t ToLowerW(wchar_t c){ if(c>=L'A'&&c<=L'Z') return c+32; return c; }
static BOOL WEqualI(const wchar_t* a,const wchar_t* b){ int i=0; for(;;i++){ wchar_t x=ToLowerW(a[i]), y=ToLowerW(b[i]); if(x!=y)return FALSE; if(!x)return TRUE; } }
static BOOL WContainsI(const wchar_t* s,const wchar_t* needle){ int nl=WLen(needle); if(nl==0)return TRUE; for(int i=0;s[i];i++){ int j=0; while(j<nl && s[i+j] && ToLowerW(s[i+j])==ToLowerW(needle[j])) j++; if(j==nl)return TRUE; } return FALSE; }
static void UIntToW(ULONGLONG v,wchar_t* out,int cap){ if(cap<2)return; wchar_t tmp[32]; int n=0; if(v==0)tmp[n++]=L'0'; else while(v&&n<31){tmp[n++]=(wchar_t)(L'0'+(v%10));v/=10;} int k=0; while(n&&k<cap-1)out[k++]=tmp[--n]; out[k]=0; }
static void IntToW(int v,wchar_t* out,int cap){ if(v<0){ if(cap>1){out[0]=L'-'; UIntToW((ULONGLONG)(-v),out+1,cap-1);} } else UIntToW((ULONGLONG)v,out,cap); }
static void WAppend(wchar_t* dst,const wchar_t* src,int cap){ int n=WLen(dst); int i=0; while(n<cap-1 && src && src[i]) dst[n++]=src[i++]; dst[n]=0; }

// v25: no injected helper. JVM discovery is read-only from the external process; menu uses global hotkey + topmost show path.

struct Config {
    DWORD magic; DWORD version;
    int autoDetect,preferVisible,lockTarget,showHud,alwaysOnTop,cpuWeight,memWeight,scanMs,hudAlpha;
    int triggerEnabled,criticalOnly,hitPlayers,hitMobs,useSword,useMace;
    int swordCooldownMs,maceCooldownMs;
    int seeInvisible,showNames,invisAlpha,fov;
    int accentR,accentG,accentB;
};
static Config g_cfg = {0x4D434647,4, 1,1,1,1,1,68,32,1000,92, 0,1,1,1,1,1, 625,1670, 0,0,58,70, 112,126,255};

struct Candidate {
    DWORD pid; DWORD ppid; wchar_t name[260]; ULONGLONG cpuTotal; ULONGLONG createTime; SIZE_T memBytes;
    int cpuX10; int memMB; int visible; int score;
};
#define MAX_TRACKED_PROCESSES 2048
#define MAX_UI_CANDIDATES 64
static Candidate g_candidates[MAX_UI_CANDIDATES]; static int g_candidateCount=0;
static Candidate g_target={};
static DWORD g_visiblePids[MAX_TRACKED_PROCESSES]; static int g_visibleCount=0;
static DWORD g_gameTitlePids[MAX_TRACKED_PROCESSES]; static int g_gameTitleCount=0;
static DWORD g_startForegroundPid=0;
static HWND g_main=0, g_hud=0;
static BOOL g_menuVisible=FALSE, g_rshiftDown=FALSE, g_hotkeyRegistered=FALSE;
static ULONGLONG g_lastMenuToggleTick=0;
static int g_menuAnim=0, g_menuAnimTarget=0;
static int g_menuCenterX=0, g_menuCenterY=0;
static HDC g_menuDC=0; static HBITMAP g_menuBmp=0; static HGDIOBJ g_menuOld=0; static BOOL g_menuCacheValid=FALSE;
static HDC g_hudDC=0; static HBITMAP g_hudBmp=0; static HGDIOBJ g_hudOld=0; static int g_hudBufW=0,g_hudBufH=0;
static UINT g_uiTimerMs=16;
static int g_lastHudX=-100000,g_lastHudY=-100000,g_lastHudW=0,g_lastHudH=0; static BOOL g_hudShown=FALSE; static int g_lastHudAlpha=-1;
static ULONGLONG g_menuAnimStartTick=0; static int g_menuAnimStartValue=0;
static int g_scroll[3]={0,0,0};
static int g_dragSlider=0;
static HFONT g_fontTitle=0,g_fontSub=0,g_fontBody=0,g_fontSmall=0,g_fontMetric=0;
struct BrushCacheEntry { COLORREF color; HBRUSH brush; }; static BrushCacheEntry g_brushCache[24]={}; static int g_brushCacheNext=0;
static HBRUSH BrushFor(COLORREF c){for(int i=0;i<24;i++)if(g_brushCache[i].brush&&g_brushCache[i].color==c)return g_brushCache[i].brush;for(int i=0;i<24;i++)if(!g_brushCache[i].brush){g_brushCache[i].color=c;g_brushCache[i].brush=CreateSolidBrush(c);return g_brushCache[i].brush;}int i=g_brushCacheNext++%24;if(g_brushCache[i].brush)DeleteObject(g_brushCache[i].brush);g_brushCache[i].color=c;g_brushCache[i].brush=CreateSolidBrush(c);return g_brushCache[i].brush;}
static void ClearBrushCache(){for(int i=0;i<24;i++){if(g_brushCache[i].brush)DeleteObject(g_brushCache[i].brush);g_brushCache[i].brush=0;}g_brushCacheNext=0;}
static wchar_t g_cfgPath[520]={0};
static wchar_t g_targetWindow[260]={0};
static HWND g_gameWnd=0;

// Strict Pulse launcher target requested by the operator.
// Minecraft 1.21.11 is hosted directly inside this EXE; there is no java.exe.
static const wchar_t* STRICT_TARGET_NAME = L"pulse_launcher (2).exe";
static const wchar_t* STRICT_TARGET_PATH = L"C:\Users\U10\Downloads\pulse_launcher (2).exe";
static const wchar_t* STRICT_TARGET_TITLE = L"Minecraft";
enum TargetHostMode { HOST_NONE=0, HOST_PULSE=1, HOST_JAVA=2, HOST_GENERIC=3 };
static TargetHostMode g_targetHostMode=HOST_NONE;
static BOOL g_targetNeedsGameTitle=FALSE; // true when selected from a real Minecraft-titled window
static int g_targetTitleMisses=0;
static DWORD g_bridgePidHint=0; static ULONGLONG g_bridgeCreateHint=0;

static BOOL IsExcludedName(const wchar_t* n){
    const wchar_t* ex[] = {L"system",L"system idle process",L"registry",L"smss.exe",L"csrss.exe",L"wininit.exe",L"winlogon.exe",L"services.exe",L"lsass.exe",L"svchost.exe",L"dwm.exe",L"explorer.exe",L"searchhost.exe",L"startmenuexperiencehost.exe",L"shellexperiencehost.exe",L"runtimebroker.exe",L"textinputhost.exe",L"applicationframehost.exe"};
    for(int i=0;i<(int)(sizeof(ex)/sizeof(ex[0]));i++) if(WEqualI(n,ex[i])) return TRUE;
    return FALSE;
}
static BOOL IsVisiblePid(DWORD pid){ for(int i=0;i<g_visibleCount;i++) if(g_visiblePids[i]==pid)return TRUE; return FALSE; }
static BOOL IsGameTitlePid(DWORD pid){ for(int i=0;i<g_gameTitleCount;i++) if(g_gameTitlePids[i]==pid)return TRUE; return FALSE; }
static BOOL CALLBACK EnumVisibleProc(HWND h, LPARAM){
    if(!IsWindowVisible(h)) return TRUE;
    DWORD pid=0; GetWindowThreadProcessId(h,&pid); if(!pid)return TRUE;
    // A fullscreen/custom GLFW window may have an empty title. Keep the PID visible
    // even then so the rare known-host fallback can still recover Pulse/Java clients.
    BOOL seen=FALSE; for(int i=0;i<g_visibleCount;i++) if(g_visiblePids[i]==pid){seen=TRUE;break;}
    if(!seen && g_visibleCount<MAX_TRACKED_PROCESSES)g_visiblePids[g_visibleCount++]=pid;
    int titleLen=GetWindowTextLengthW(h); if(titleLen<=0) return TRUE;
    wchar_t title[260]; title[0]=0; GetWindowTextW(h,title,260);
    if(WContainsI(title,L"minecraft")&&!WContainsI(title,L"Minecraft Launcher")){BOOL gs=FALSE;for(int i=0;i<g_gameTitleCount;i++)if(g_gameTitlePids[i]==pid){gs=TRUE;break;}if(!gs&&g_gameTitleCount<MAX_TRACKED_PROCESSES)g_gameTitlePids[g_gameTitleCount++]=pid;}
    return TRUE;
}
static BOOL IsMinecraftGameTitle(const wchar_t* title){
    return title && WContainsI(title,STRICT_TARGET_TITLE) && !WContainsI(title,L"Minecraft Launcher");
}
static DWORD g_titlePid=0;
static BOOL CALLBACK EnumTargetTitle(HWND h, LPARAM){
    if(!IsWindowVisible(h))return TRUE;DWORD pid=0;GetWindowThreadProcessId(h,&pid);if(pid!=g_titlePid)return TRUE;
    int n=GetWindowTextLengthW(h);if(n<=0)return TRUE;wchar_t title[260];title[0]=0;GetWindowTextW(h,title,260);
    // Prefer an actual Minecraft title. For custom-title embedded clients keep the first visible
    // window as a fallback so detection survives launchers that rewrite the GLFW title.
    if(IsMinecraftGameTitle(title)){WCopy(g_targetWindow,title,260);g_gameWnd=h;return FALSE;}
    if(!g_gameWnd){WCopy(g_targetWindow,title,260);g_gameWnd=h;}
    return TRUE;
}
static BOOL GetProcessPathStrict(DWORD pid,wchar_t*out,int cap){
    if(!out||cap<2)return FALSE;out[0]=0;
    HANDLE ph=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid);if(!ph)ph=OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pid);if(!ph)return FALSE;
    DWORD n=(DWORD)cap;BOOL ok=QueryFullProcessImageNameW(ph,0,out,&n);CloseHandle(ph);
    if(!ok||n==0){out[0]=0;return FALSE;}if(n>=(DWORD)cap)n=(DWORD)cap-1;out[n]=0;return TRUE;
}
static BOOL ProcessHasJvmDll(DWORD pid){
    HANDLE s=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);
    if(s==INVALID_HANDLE_VALUE)return FALSE;
    MODULEENTRY32W me;memset(&me,0,sizeof(me));me.dwSize=sizeof(me);BOOL found=FALSE;
    if(Module32FirstW(s,&me)){do{if(WEqualI(me.szModule,L"jvm.dll")){found=TRUE;break;}}while(Module32NextW(s,&me));}
    CloseHandle(s);return found;
}
static ULONGLONG GetProcCreateTimeValue(DWORD pid){HANDLE ph=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,FALSE,pid);if(!ph)return 0;FILETIME c,e,k,u;ULONGLONG v=0;if(GetProcessTimes(ph,&c,&e,&k,&u))v=FtToU64(c);CloseHandle(ph);return v;}
// ========================= v23 window-first detector =========================
// Fast path is window-first: enumerate top-level windows, map a Minecraft window to its PID,
// then inspect only that PID. A full process snapshot is a rare fallback for custom-title clients.
static const wchar_t* BaseNamePtr(const wchar_t* path){
    if(!path)return L"";const wchar_t* b=path;for(int i=0;path[i];i++)if(path[i]==L'\'||path[i]==L'/')b=path+i+1;return b;
}
static BOOL IsKnownEmbeddedClient(const wchar_t* name,const wchar_t* path){
    const wchar_t* marks[]={L"pulse",L"lunar",L"badlion",L"feather",L"altber",L"aiaber",L"prismlauncher",L"multimc",L"modrinth",L"curseforge",L"atlauncher",L"minecraft"};
    for(int i=0;i<(int)(sizeof(marks)/sizeof(marks[0]));i++)if(WContainsI(name,marks[i])||WContainsI(path,marks[i]))return TRUE;
    return FALSE;
}
static BOOL IsDirectGameHostName(const wchar_t* name,const wchar_t* path){
    // These are known to host the game/JVM directly. Launcher-only shells such as Prism/MultiMC
    // are intentionally excluded from the foreground shortcut to avoid false attachment.
    const wchar_t* marks[]={L"pulse",L"lunar",L"badlion",L"feather",L"altber",L"aiaber"};
    for(int i=0;i<(int)(sizeof(marks)/sizeof(marks[0]));i++)if(WContainsI(name,marks[i])||WContainsI(path,marks[i]))return TRUE;
    return FALSE;
}
static BOOL BuildCandidateFromPid(DWORD pid,Candidate*out,TargetHostMode*mode,BOOL requireGameWindow){
    if(!pid||pid==GetCurrentProcessId()||!out||!mode)return FALSE;
    wchar_t path[520];path[0]=0;if(!GetProcessPathStrict(pid,path,520))return FALSE;
    const wchar_t* base=BaseNamePtr(path);if(!base[0]||IsExcludedName(base))return FALSE;
    BOOL gameTitle=IsGameTitlePid(pid);if(requireGameWindow&&!gameTitle)return FALSE;
    BOOL visible=IsVisiblePid(pid);
    BOOL pulse=WContainsI(base,L"pulse_launcher")||WContainsI(base,L"pulse launcher")||WContainsI(path,L"pulse_launcher")||WContainsI(path,L"pulse launcher");
    BOOL java=WEqualI(base,L"java.exe")||WEqualI(base,L"javaw.exe");
    BOOL known=IsKnownEmbeddedClient(base,path);
    int score=0;TargetHostMode m=HOST_NONE;
    if(pulse){m=HOST_PULSE;score=5000;if(WEqualI(base,STRICT_TARGET_NAME))score+=700;if(WEqualI(path,STRICT_TARGET_PATH))score+=700;}
    else if(java){m=HOST_JAVA;score=4300;if(WEqualI(base,L"javaw.exe"))score+=120;}
    else if(known){m=HOST_GENERIC;score=3600;}

    else if(gameTitle){m=HOST_GENERIC;score=3000;}
    else return FALSE;
    if(gameTitle)score+=1200;if(visible)score+=150;if(pid==g_startForegroundPid)score+=100;
    memset(out,0,sizeof(*out));out->pid=pid;out->createTime=GetProcCreateTimeValue(pid);if(!out->createTime)return FALSE;out->visible=visible?1:0;out->score=score;WCopy(out->name,base,260);*mode=m;return TRUE;
}
static void AddUiCandidate(const Candidate& c){
    if(!c.pid)return;
    for(int i=0;i<g_candidateCount;i++)if(g_candidates[i].pid==c.pid)return;
    int pos=g_candidateCount<MAX_UI_CANDIDATES?g_candidateCount++:MAX_UI_CANDIDATES-1;
    if(pos<0)return;g_candidates[pos]=c;
    while(pos>0&&g_candidates[pos].score>g_candidates[pos-1].score){Candidate t=g_candidates[pos-1];g_candidates[pos-1]=g_candidates[pos];g_candidates[pos]=t;pos--;}
}
static BOOL FallbackKnownProcessScan(Candidate*best,TargetHostMode*bestMode){
    HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(snap==INVALID_HANDLE_VALUE)return FALSE;
    PROCESSENTRY32W pe;memset(&pe,0,sizeof(pe));pe.dwSize=sizeof(pe);BOOL found=FALSE;int bestScore=-1;
    if(Process32FirstW(snap,&pe))do{
        if(!pe.th32ProcessID||pe.th32ProcessID==4||pe.th32ProcessID==GetCurrentProcessId())continue;
        BOOL likely=WContainsI(pe.szExeFile,L"pulse")||WEqualI(pe.szExeFile,L"java.exe")||WEqualI(pe.szExeFile,L"javaw.exe")||WContainsI(pe.szExeFile,L"lunar")||WContainsI(pe.szExeFile,L"badlion")||WContainsI(pe.szExeFile,L"feather")||WContainsI(pe.szExeFile,L"altber")||WContainsI(pe.szExeFile,L"aiaber")||WContainsI(pe.szExeFile,L"prism")||WContainsI(pe.szExeFile,L"multimc")||WContainsI(pe.szExeFile,L"modrinth")||WContainsI(pe.szExeFile,L"atlauncher");
        if(!likely||!IsVisiblePid(pe.th32ProcessID))continue;
        Candidate c={};TargetHostMode m=HOST_NONE;if(!BuildCandidateFromPid(pe.th32ProcessID,&c,&m,FALSE))continue;
        if(m==HOST_JAVA&&!ProcessHasJvmDll(c.pid))continue;
        if(!found||c.score>bestScore){*best=c;*bestMode=m;bestScore=c.score;found=TRUE;}AddUiCandidate(c);
    }while(Process32NextW(snap,&pe));
    CloseHandle(snap);return found;
}
static void GatherCandidates(BOOL useDelta){
    (void)useDelta;
    g_visibleCount=0;g_gameTitleCount=0;EnumWindows(EnumVisibleProc,0);
    g_candidateCount=0;
    Candidate best={};TargetHostMode bestMode=HOST_NONE;int bestScore=-1;BOOL bestFromGameTitle=FALSE;
    for(int i=0;i<g_gameTitleCount;i++){
        Candidate c={};TargetHostMode m=HOST_NONE;if(!BuildCandidateFromPid(g_gameTitlePids[i],&c,&m,TRUE))continue;AddUiCandidate(c);
        if(c.score>bestScore){best=c;bestMode=m;bestScore=c.score;bestFromGameTitle=TRUE;}
    }
    if(!best.pid){HWND fg=GetForegroundWindow();DWORD fp=0;if(fg)GetWindowThreadProcessId(fg,&fp);if(fp&&IsVisiblePid(fp)){Candidate c={};TargetHostMode m=HOST_NONE;if(BuildCandidateFromPid(fp,&c,&m,FALSE)&&IsDirectGameHostName(c.name,L"")){best=c;bestMode=m;bestFromGameTitle=FALSE;AddUiCandidate(c);}}}
    if(!best.pid&&useDelta){if(FallbackKnownProcessScan(&best,&bestMode))bestFromGameTitle=FALSE;}
    if(best.pid){g_target=best;g_targetHostMode=bestMode;g_targetNeedsGameTitle=bestFromGameTitle;g_targetTitleMisses=0;}else{memset(&g_target,0,sizeof(g_target));g_targetHostMode=HOST_NONE;g_targetNeedsGameTitle=FALSE;g_targetTitleMisses=0;}
    g_targetWindow[0]=0;g_gameWnd=0;if(g_target.pid){g_titlePid=g_target.pid;EnumWindows(EnumTargetTitle,0);}
    g_bridgePidHint=g_target.pid;g_bridgeCreateHint=g_target.createTime;
}
static void InitialDetect(){ GatherCandidates(FALSE); }

static void BuildCfgPath(){ DWORD n=GetModuleFileNameW(0,g_cfgPath,520); if(n==0||n>=519){WCopy(g_cfgPath,L"MonoClient.cfg",520);return;} int slash=-1; for(int i=0;g_cfgPath[i];i++)if(g_cfgPath[i]==L'\'||g_cfgPath[i]==L'/')slash=i; if(slash>=0)g_cfgPath[slash+1]=0; else g_cfgPath[0]=0; WAppend(g_cfgPath,L"MonoClient.cfg",520); }
static void LoadConfig(){ BuildCfgPath(); DWORD oldVersion=0; HANDLE h=CreateFileW(g_cfgPath,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0); if(h!=INVALID_HANDLE_VALUE){Config c;DWORD got=0;if(ReadFile(h,&c,sizeof(c),&got,0)&&got==sizeof(c)&&c.magic==0x4D434647&&(c.version==2||c.version==3||c.version==4)){oldVersion=c.version;g_cfg=c;if(c.version==2)g_cfg.hitMobs=1;g_cfg.version=4;g_cfg.preferVisible=!!g_cfg.preferVisible;g_cfg.showHud=!!g_cfg.showHud;g_cfg.alwaysOnTop=!!g_cfg.alwaysOnTop;g_cfg.triggerEnabled=!!g_cfg.triggerEnabled;g_cfg.criticalOnly=!!g_cfg.criticalOnly;g_cfg.hitPlayers=!!g_cfg.hitPlayers;g_cfg.hitMobs=!!g_cfg.hitMobs;g_cfg.useSword=!!g_cfg.useSword;g_cfg.useMace=!!g_cfg.useMace;g_cfg.seeInvisible=!!g_cfg.seeInvisible;g_cfg.showNames=!!g_cfg.showNames;g_cfg.swordCooldownMs=625;g_cfg.maceCooldownMs=1670;g_cfg.seeInvisible=0;g_cfg.showNames=0;if(g_cfg.invisAlpha<20||g_cfg.invisAlpha>90)g_cfg.invisAlpha=58;if(g_cfg.fov<30||g_cfg.fov>120)g_cfg.fov=70;if(g_cfg.accentR<0||g_cfg.accentR>255)g_cfg.accentR=112;if(g_cfg.accentG<0||g_cfg.accentG>255)g_cfg.accentG=126;if(g_cfg.accentB<0||g_cfg.accentB>255)g_cfg.accentB=255;if(g_cfg.scanMs<300||g_cfg.scanMs>3000)g_cfg.scanMs=1000;}CloseHandle(h);}g_cfg.autoDetect=1;g_cfg.lockTarget=1;g_cfg.seeInvisible=0;g_cfg.showNames=0;g_cfg.swordCooldownMs=625;g_cfg.maceCooldownMs=1670;if(oldVersion>0&&oldVersion<4){g_cfg.showHud=1;g_cfg.alwaysOnTop=1;}}
static void SaveConfig(){ HANDLE h=CreateFileW(g_cfgPath,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0); if(h==INVALID_HANDLE_VALUE)return; DWORD w=0;WriteFile(h,&g_cfg,sizeof(g_cfg),&w,0);CloseHandle(h); }

typedef unsigned long long U64;
typedef unsigned int U32;
typedef unsigned short U16;
typedef unsigned char U8;

static BOOL AEq(const char* a,const char* b){int i=0;for(;;i++){if(a[i]!=b[i])return FALSE;if(!a[i])return TRUE;}}
static BOOL AEndsI(const wchar_t* s,const wchar_t* tail){int a=WLen(s),b=WLen(tail);if(b>a)return FALSE;return WEqualI(s+a-b,tail);}
static void ACopy(char* d,const char* s,int cap){if(cap<=0)return;int i=0;for(;i<cap-1&&s&&s[i];i++)d[i]=s[i];d[i]=0;}

enum BridgeState { BR_OFF=0, BR_ATTACHING=1, BR_READY=2, BR_UNSUPPORTED=3 };
enum WeaponKind { WPN_NONE=0, WPN_SWORD=1, WPN_MACE=2 };
enum TargetKind { TGT_NONE=0, TGT_PLAYER=1, TGT_MOB=2, TGT_OTHER=3 };
struct TargetKindCacheEntry { U64 klass; TargetKind kind; };
static TargetKindCacheEntry g_targetKindCache[12]={}; static int g_targetKindCacheNext=0;

struct RemoteField { int offset; U16 access; char sig[96]; BOOL found; };
struct BridgeVM {
    HANDLE process; DWORD pid; U64 createTime; DWORD hostPid; U64 hostCreateTime; U64 jvmBase; U64 jvmSpan; BridgeState state;
    int structsParsed; int heuristicStructs; int classesScanned; char status[128];
    U64 cldHeadAddr; int cldNext,cldKlasses;
    int klassName,klassNext,klassSuper,klassJavaMirror,klassLayout;
    int oopKlass,oopCompressedKlass;
    int ikFields,ikFieldInfoStream,ikConstants; int symLength,symBody; int cpSize;
    U64 coopsBaseAddr,coopsShiftAddr,cklassBaseAddr,cklassShiftAddr;
    U64 coopsBase,cklassBase; int coopsShift,cklassShift;
    U64 mcKlass,itemsKlass,itemStackKlass,playerKlass,livingKlass,entityKlass;
    RemoteField mcInstance,mcPlayer,mcTarget,mcScreen,mcWorld,mcGameRenderer;
    RemoteField playerHeld,playerProfile;
    RemoteField stackItem;
    RemoteField swordItemFields[7],maceItemField;
    RemoteField entOnGround,entFall,entVehicle,entWater,entEyeWater,entData,entPos;
    RemoteField livingAttackTicker;
    RemoteField levelPlayers;
    RemoteField dataItems,dataItemValue,boxedByteValue;
    RemoteField listElementData,listSize;
    RemoteField gameProfileName,javaStringValue;
    RemoteField rendererCamera;
    RemoteField cameraPos,cameraYaw,cameraPitch;
    RemoteField vecX,vecY,vecZ;
};
static BridgeVM g_bridge={};
static DWORD g_diagHostPid=0; static wchar_t g_diagHostName[260]={0}; static int g_diagHostsTried=0; static int g_diagFailRank=0;
static BYTE g_hotspotScanBuf[262144];

struct InvisibleInfo { U64 oop; double x,y,z; char name[64]; };
struct GameSnapshot {
