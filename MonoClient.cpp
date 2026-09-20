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

    BOOL ready; U64 player,target,world; WeaponKind weapon; TargetKind targetKind;
    BOOL onGround,touchWater,eyeWater,hasVehicle,sprinting,attackTickerKnown,criticalStateKnown; double fallDistance; int attackTicker;
    int invisiblePlayers; char firstInvisible[64]; InvisibleInfo invis[16]; int invisCount;
    double camX,camY,camZ; float camYaw,camPitch; BOOL cameraReady;
};
static GameSnapshot g_game={}; static GameSnapshot g_engineNext={};
static ULONGLONG g_lastAttackTick=0; static int g_attackCount=0;
static void PublishGameSnapshot(const GameSnapshot& src){memcpy(&g_game,&src,sizeof(src));}

static BOOL RMem(U64 addr,void* out,SIZE_T n){ if(!g_bridge.process||!addr)return FALSE; SIZE_T got=0; return ReadProcessMemory(g_bridge.process,(LPCVOID)(ULONG_PTR)addr,out,n,&got)&&got==n; }
static U8 R8(U64 a){U8 v=0;RMem(a,&v,1);return v;} static U16 R16(U64 a){U16 v=0;RMem(a,&v,2);return v;}
static U32 R32(U64 a){U32 v=0;RMem(a,&v,4);return v;} static U64 R64(U64 a){U64 v=0;RMem(a,&v,8);return v;}
static BOOL RStr(U64 a,char* out,int cap){if(!out||cap<2){return FALSE;}out[0]=0;if(!a)return FALSE;for(int i=0;i<cap-1;i++){char c=0;if(!RMem(a+i,&c,1))return FALSE;out[i]=c;if(!c)return TRUE;}out[cap-1]=0;return TRUE;}

static U64 RemoteExportAt(U64 b,const char* wanted){
    if(!b)return 0;U16 mz=R16(b);if(mz!=0x5A4D)return 0;U32 peoff=R32(b+0x3c);if(peoff<64||peoff>0x4000||R32(b+peoff)!=0x00004550)return 0;
    U64 opt=b+peoff+24;U16 magic=R16(opt);if(magic!=0x20b&&magic!=0x10b)return 0;U32 expRva=R32(opt+(magic==0x20b?112:96));if(!expRva)return 0;U64 ed=b+expRva;
    U32 funcs=R32(ed+28),names=R32(ed+32),ords=R32(ed+36),n=R32(ed+24);if(!funcs||!names||!ords||n==0||n>20000)return 0;
    char nm[128];for(U32 i=0;i<n;i++){U32 nr=R32(b+names+i*4ULL);if(!nr)continue;if(!RStr(b+nr,nm,128))continue;if(AEq(nm,wanted)){U16 ord=R16(b+ords+i*2ULL);U32 fr=R32(b+funcs+ord*4ULL);return fr?b+fr:0;}}
    return 0;
}
static BOOL LooksLikeHotSpotImage(U64 base){
    if(!base)return FALSE;
    return RemoteExportAt(base,"gHotSpotVMStructs") &&
           RemoteExportAt(base,"gHotSpotVMStructEntryArrayStride") &&
           RemoteExportAt(base,"gHotSpotVMTypes");
}
static U64 RemoteImageSize(U64 b){
    if(!b||R16(b)!=0x5A4D)return 0;U32 peoff=R32(b+0x3c);if(peoff<64||peoff>0x4000||R32(b+peoff)!=0x00004550)return 0;
    U64 opt=b+peoff+24;U16 magic=R16(opt);if(magic!=0x20b&&magic!=0x10b)return 0;U32 sz=R32(opt+56);if(sz<0x10000||sz>0x40000000)return 0;return (U64)sz;
}
static BOOL MemReadable(const MEMORY_BASIC_INFORMATION_X64& m){return m.State==MEM_COMMIT && !(m.Protect&PAGE_GUARD) && !(m.Protect&PAGE_NOACCESS);}
static int LocalFindBytes(const BYTE* b,int n,const char* needle,int nl,int* out,int cap){
    if(!b||!needle||nl<=0||n<nl||!out||cap<=0)return 0;int c=0;for(int i=0;i<=n-nl&&c<cap;i++){int j=0;for(;j<nl;j++)if(b[i+j]!=(BYTE)needle[j])break;if(j==nl)out[c++]=i;}return c;
}
static int ImageFindAscii(U64 base,U64 size,const char* needle,U64* out,int cap){
    if(!base||!size||!needle||!out||cap<=0)return 0;int nl=0;while(needle[nl]&&nl<120)nl++;if(nl<=0)return 0;int found=0;U64 end=base+size,addr=base;MEMORY_BASIC_INFORMATION_X64 mbi;
    while(addr<end&&found<cap){SIZE_T q=VirtualQueryEx(g_bridge.process,(LPCVOID)(ULONG_PTR)addr,&mbi,sizeof(mbi));if(q!=sizeof(mbi))break;U64 rb=(U64)(ULONG_PTR)mbi.BaseAddress,re=rb+(U64)mbi.RegionSize;if(re>base&&rb<end&&MemReadable(mbi)){
        U64 st=rb<base?base:rb,lim=re>end?end:re;for(U64 p=st;p<lim&&found<cap;){SIZE_T want=(SIZE_T)((lim-p)>sizeof(g_hotspotScanBuf)?sizeof(g_hotspotScanBuf):(lim-p));SIZE_T got=0;if(ReadProcessMemory(g_bridge.process,(LPCVOID)(ULONG_PTR)p,g_hotspotScanBuf,want,&got)&&got>=(SIZE_T)nl){int offs[16];int nn=LocalFindBytes(g_hotspotScanBuf,(int)got,needle,nl,offs,16);for(int i=0;i<nn&&found<cap;i++)out[found++]=p+(U64)offs[i];}if(want<=128)break;U64 step=(U64)want-(U64)(nl>1?nl-1:0);if(!step)break;p+=step;}}
        if(re<=addr)break;addr=re;
    }return found;
}
static int ImageFindU64(U64 base,U64 size,U64 value,U64* out,int cap){
    if(!base||!size||!value||!out||cap<=0)return 0;int found=0;U64 end=base+size,addr=base;MEMORY_BASIC_INFORMATION_X64 mbi;
    while(addr<end&&found<cap){SIZE_T q=VirtualQueryEx(g_bridge.process,(LPCVOID)(ULONG_PTR)addr,&mbi,sizeof(mbi));if(q!=sizeof(mbi))break;U64 rb=(U64)(ULONG_PTR)mbi.BaseAddress,re=rb+(U64)mbi.RegionSize;if(re>base&&rb<end&&MemReadable(mbi)){
        U64 st=rb<base?base:rb,lim=re>end?end:re;for(U64 p=st;p<lim&&found<cap;){SIZE_T want=(SIZE_T)((lim-p)>sizeof(g_hotspotScanBuf)?sizeof(g_hotspotScanBuf):(lim-p));SIZE_T got=0;if(ReadProcessMemory(g_bridge.process,(LPCVOID)(ULONG_PTR)p,g_hotspotScanBuf,want,&got)&&got>=8){for(SIZE_T i=0;i+8<=got;i+=8){U64 v=0;memcpy(&v,g_hotspotScanBuf+i,8);if(v==value&&found<cap)out[found++]=p+(U64)i;}}if(want<=8)break;U64 step=(U64)want-8;if(!step)break;p+=step;}}
        if(re<=addr)break;addr=re;
    }return found;
}
static BOOL FastRemoteString(U64 a,char* out,int cap){
    if(!a||!out||cap<2)return FALSE;out[0]=0;SIZE_T got=0;int want=cap-1;if(want>127)want=127;BYTE tmp[128];if(!ReadProcessMemory(g_bridge.process,(LPCVOID)(ULONG_PTR)a,tmp,(SIZE_T)want,&got)||got==0)return FALSE;int n=0;for(;n<(int)got&&n<cap-1;n++){char c=(char)tmp[n];out[n]=c;if(!c)return n>0;}out[n]=0;return n>0;
}
static BOOL PlausibleVMStructEntry(U64 e,U64 base,U64 size){
    BYTE b[48];if(!RMem(e,b,sizeof(b)))return FALSE;U64 tp=0,fp=0;U32 st=0;memcpy(&tp,b,8);memcpy(&fp,b+8,8);memcpy(&st,b+24,4);if(!tp||!fp||st>1)return FALSE;if(tp<base||tp>=base+size||fp<base||fp>=base+size)return FALSE;char t[80],f[80];if(!FastRemoteString(tp,t,80)||!FastRemoteString(fp,f,80))return FALSE;return t[0]&&f[0];
}
static U64 FindVMStructAnchor(U64 base,U64 size){
    U64 ta[8],fa[16];int tn=ImageFindAscii(base,size,"ClassLoaderDataGraph",ta,8),fn=ImageFindAscii(base,size,"_head",fa,16);if(!tn||!fn)return 0;
    for(int ti=0;ti<tn;ti++){U64 refs[64];int rn=ImageFindU64(base,size,ta[ti],refs,64);for(int r=0;r<rn;r++){BYTE b[48];if(!RMem(refs[r],b,sizeof(b)))continue;U64 fp=0;U32 st=0;U64 addr=0;memcpy(&fp,b+8,8);memcpy(&st,b+24,4);memcpy(&addr,b+40,8);BOOL fm=FALSE;for(int fi=0;fi<fn;fi++)if(fp==fa[fi]){fm=TRUE;break;}if(fm&&st==1&&addr&&PlausibleVMStructEntry(refs[r],base,size))return refs[r];}}
    return 0;
}
static BOOL HasHotSpotMarkersRange(U64 base,U64 size){if(!base||size<0x10000||size>0x40000000)return FALSE;U64 a[2],b[2],c[2];return ImageFindAscii(base,size,"ClassLoaderDataGraph",a,2)>0&&ImageFindAscii(base,size,"_fieldinfo_stream",b,2)>0&&ImageFindAscii(base,size,"CompressedOops",c,2)>0;}
static BOOL HasHotSpotMarkers(U64 base){U64 size=RemoteImageSize(base);return size?HasHotSpotMarkersRange(base,size):FALSE;}
static U64 AllocationSpan(U64 base,BOOL*hasExec){
    if(hasExec)*hasExec=FALSE;if(!base)return 0;U64 p=base,end=base;MEMORY_BASIC_INFORMATION_X64 m;for(int g=0;g<8192;g++){SIZE_T q=VirtualQueryEx(g_bridge.process,(LPCVOID)(ULONG_PTR)p,&m,sizeof(m));if(q!=sizeof(m)||(U64)(ULONG_PTR)m.AllocationBase!=base)break;U64 re=(U64)(ULONG_PTR)m.BaseAddress+(U64)m.RegionSize;if(MemReadable(m)&&(m.Protect&(PAGE_EXECUTE|PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))&&hasExec)*hasExec=TRUE;if(re>end)end=re;if(re<=p)break;p=re;}return end>base?end-base:0;
}
static int HeuristicConstantPoolSize(U64 base,U64 size){
    U64 ss[8];int sn=ImageFindAscii(base,size,"ConstantPool",ss,8);for(int si=0;si<sn;si++){U64 refs[64];int rn=ImageFindU64(base,size,ss[si],refs,64);for(int i=0;i<rn;i++){BYTE b[40];if(!RMem(refs[i],b,sizeof(b)))continue;U64 tp=0,sp=0,sz=0;U32 a=0,c=0,d=0;memcpy(&tp,b,8);memcpy(&sp,b+8,8);memcpy(&a,b+16,4);memcpy(&c,b+20,4);memcpy(&d,b+24,4);memcpy(&sz,b+32,8);if(tp==ss[si]&&(sp==0||(sp>=base&&sp<base+size))&&a<=1&&c<=1&&d<=1&&sz>=32&&sz<=512&&(sz%8)==0)return (int)sz;}}return 64;
}
static void ResetVMStructOffsets(){
    g_bridge.cldHeadAddr=0;g_bridge.cldNext=g_bridge.cldKlasses=g_bridge.klassName=g_bridge.klassNext=g_bridge.klassSuper=g_bridge.klassJavaMirror=g_bridge.klassLayout=-1;g_bridge.oopKlass=g_bridge.oopCompressedKlass=-1;
    g_bridge.ikFields=g_bridge.ikFieldInfoStream=g_bridge.ikConstants=g_bridge.symLength=g_bridge.symBody=-1;g_bridge.cpSize=0;g_bridge.coopsBaseAddr=g_bridge.coopsShiftAddr=g_bridge.cklassBaseAddr=g_bridge.cklassShiftAddr=0;g_bridge.coopsBase=g_bridge.cklassBase=0;g_bridge.coopsShift=g_bridge.cklassShift=3;
}
static BOOL ParseVMStructsHeuristic(U64 base){
    U64 size=g_bridge.jvmSpan?g_bridge.jvmSpan:RemoteImageSize(base);if(!size)return FALSE;U64 anchor=FindVMStructAnchor(base,size);if(!anchor)return FALSE;ResetVMStructOffsets();
    struct HNeed{const char*t;const char*f;int*o;U64*a;};HNeed n[]={
      {"ClassLoaderDataGraph","_head",0,&g_bridge.cldHeadAddr},{"ClassLoaderData","_next",&g_bridge.cldNext,0},{"ClassLoaderData","_klasses",&g_bridge.cldKlasses,0},
      {"Klass","_name",&g_bridge.klassName,0},{"Klass","_next_link",&g_bridge.klassNext,0},{"Klass","_super",&g_bridge.klassSuper,0},{"Klass","_java_mirror",&g_bridge.klassJavaMirror,0},{"Klass","_layout_helper",&g_bridge.klassLayout,0},
      {"oopDesc","_metadata._klass",&g_bridge.oopKlass,0},{"oopDesc","_metadata._compressed_klass",&g_bridge.oopCompressedKlass,0},
      {"InstanceKlass","_fields",&g_bridge.ikFields,0},{"InstanceKlass","_fieldinfo_stream",&g_bridge.ikFieldInfoStream,0},{"InstanceKlass","_constants",&g_bridge.ikConstants,0},{"Symbol","_length",&g_bridge.symLength,0},{"Symbol","_body",&g_bridge.symBody,0},
      {"CompressedOops","_narrow_oop._base",0,&g_bridge.coopsBaseAddr},{"CompressedOops","_narrow_oop._shift",0,&g_bridge.coopsShiftAddr},{"CompressedOops","_base",0,&g_bridge.coopsBaseAddr},{"CompressedOops","_shift",0,&g_bridge.coopsShiftAddr},
      {"CompressedKlassPointers","_narrow_klass._base",0,&g_bridge.cklassBaseAddr},{"CompressedKlassPointers","_narrow_klass._shift",0,&g_bridge.cklassShiftAddr},{"CompressedKlassPointers","_base",0,&g_bridge.cklassBaseAddr},{"CompressedKlassPointers","_shift",0,&g_bridge.cklassShiftAddr}
    };int nc=(int)(sizeof(n)/sizeof(n[0]));U64 start=anchor;for(int i=0;i<4096;i++){if(start<base+48)break;U64 p=start-48;if(!PlausibleVMStructEntry(p,base,size))break;start=p;}
    char t[96],f[96];int matched=0;for(U64 e=start,g=0;g<12000&&e+48<=base+size;g++,e+=48){BYTE b[48];if(!RMem(e,b,sizeof(b)))break;U64 tp=0,fp=0,off=0,addr=0;U32 st=0;memcpy(&tp,b,8);memcpy(&fp,b+8,8);memcpy(&st,b+24,4);memcpy(&off,b+32,8);memcpy(&addr,b+40,8);if(!tp||!fp)break;if(tp<base||tp>=base+size||fp<base||fp>=base+size)break;if(!FastRemoteString(tp,t,96)||!FastRemoteString(fp,f,96))break;for(int i=0;i<nc;i++)if(AEq(t,n[i].t)&&AEq(f,n[i].f)){if(st&&n[i].a&&!*n[i].a){*n[i].a=addr;matched++;}else if(!st&&n[i].o&&*n[i].o<0){*n[i].o=(int)off;matched++;}}}
    g_bridge.cpSize=HeuristicConstantPoolSize(base,size);if(g_bridge.coopsBaseAddr)g_bridge.coopsBase=R64(g_bridge.coopsBaseAddr);if(g_bridge.coopsShiftAddr)g_bridge.coopsShift=(int)R32(g_bridge.coopsShiftAddr);if(g_bridge.cklassBaseAddr)g_bridge.cklassBase=R64(g_bridge.cklassBaseAddr);if(g_bridge.cklassShiftAddr)g_bridge.cklassShift=(int)R32(g_bridge.cklassShiftAddr);
    g_bridge.heuristicStructs=1;return matched>=10&&g_bridge.cldHeadAddr&&g_bridge.cldNext>=0&&g_bridge.cldKlasses>=0&&g_bridge.klassName>=0&&g_bridge.klassNext>=0&&g_bridge.klassSuper>=0&&g_bridge.klassJavaMirror>=0&&(g_bridge.ikFields>=0||g_bridge.ikFieldInfoStream>=0)&&g_bridge.ikConstants>=0&&g_bridge.symLength>=0&&g_bridge.symBody>=0;
}
static U64 FindJvmBase(DWORD pid){
    HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);
    U64 moduleBases[1024];U8 moduleHints[1024];int moduleCount=0;BOOL sawJvmNamed=FALSE;
    if(snap!=INVALID_HANDLE_VALUE){
        MODULEENTRY32W me;memset(&me,0,sizeof(me));me.dwSize=sizeof(me);
        if(Module32FirstW(snap,&me))do{
            U64 mb=(U64)(ULONG_PTR)me.modBaseAddr;
            BOOL jn=WEqualI(me.szModule,L"jvm.dll")||AEndsI(me.szExePath,L"\jvm.dll")||WContainsI(me.szModule,L"hotspot")||WContainsI(me.szModule,L"jvm");
            BOOL hint=jn||WContainsI(me.szModule,L"pulse")||WContainsI(me.szModule,L"java");
            if(moduleCount<1024){moduleBases[moduleCount]=mb;moduleHints[moduleCount]=(U8)(hint?1:0);moduleCount++;}
            if(jn)sawJvmNamed=TRUE;
            if(jn){

                if(LooksLikeHotSpotImage(mb)){CloseHandle(snap);g_bridge.heuristicStructs=0;g_bridge.jvmSpan=RemoteImageSize(mb);return mb;}
                if(HasHotSpotMarkers(mb)){CloseHandle(snap);g_bridge.heuristicStructs=1;g_bridge.jvmSpan=RemoteImageSize(mb);return mb;}
            }
        }while(Module32NextW(snap,&me));
        CloseHandle(snap);
    }
    for(int i=0;i<moduleCount;i++){if(LooksLikeHotSpotImage(moduleBases[i])){g_bridge.heuristicStructs=0;g_bridge.jvmSpan=RemoteImageSize(moduleBases[i]);return moduleBases[i];}if((i&15)==15)Sleep(0);}
    wchar_t pp[520];pp[0]=0;GetProcessPathStrict(pid,pp,520);const wchar_t* pb=BaseNamePtr(pp);
    BOOL direct=(pid==g_bridge.pid)||WContainsI(pb,L"pulse")||WContainsI(pb,L"lunar")||WContainsI(pb,L"badlion")||WContainsI(pb,L"feather")||WContainsI(pb,L"aiaber")||WContainsI(pb,L"altber");
    BOOL deepAllowed=direct||sawJvmNamed;if(!deepAllowed)return 0;
    for(int i=0;i<moduleCount;i++){if(i!=0&&!moduleHints[i])continue;U64 sz=RemoteImageSize(moduleBases[i]);if(sz>=0x200000&&sz<=0x08000000&&HasHotSpotMarkers(moduleBases[i])){g_bridge.heuristicStructs=1;g_bridge.jvmSpan=sz;return moduleBases[i];}if((i&3)==3)Sleep(0);}
    if(!g_bridge.process)return 0;U64 addr=0x10000ULL,lastAlloc=0;int allocs=0;MEMORY_BASIC_INFORMATION_X64 mbi;
    while(addr<0x0000800000000000ULL){SIZE_T q=VirtualQueryEx(g_bridge.process,(LPCVOID)(ULONG_PTR)addr,&mbi,sizeof(mbi));if(q!=sizeof(mbi))break;U64 ab=(U64)(ULONG_PTR)mbi.AllocationBase;BOOL candidateType=(mbi.Type==MEM_IMAGE||mbi.Type==MEM_MAPPED||mbi.Type==MEM_PRIVATE);
        if(mbi.State==MEM_COMMIT&&candidateType&&ab&&ab!=lastAlloc&&!(mbi.Protect&PAGE_GUARD)&&!(mbi.Protect&PAGE_NOACCESS)){lastAlloc=ab;U16 mz=R16(ab);if(mz==0x5A4D){if(LooksLikeHotSpotImage(ab)){g_bridge.heuristicStructs=0;g_bridge.jvmSpan=RemoteImageSize(ab);return ab;}U64 sz=RemoteImageSize(ab);if(sz>=0x200000&&sz<=0x08000000&&HasHotSpotMarkers(ab)){g_bridge.heuristicStructs=1;g_bridge.jvmSpan=sz;return ab;}}else{BOOL ex=FALSE;U64 span=AllocationSpan(ab,&ex);if(ex&&span>=0x200000&&span<=0x08000000&&HasHotSpotMarkersRange(ab,span)){g_bridge.heuristicStructs=1;g_bridge.jvmSpan=span;return ab;}}allocs++;if((allocs&15)==15)Sleep(0);}
        U64 next=(U64)(ULONG_PTR)mbi.BaseAddress+(U64)mbi.RegionSize;if(next<=addr)break;addr=next;}
    return 0;
}
static U64 RemoteExport(const char* wanted){return RemoteExportAt(g_bridge.jvmBase,wanted);}
struct VMNeed { const char* type; const char* field; int* offOut; U64* addrOut; };
static BOOL ParseVMStructsExported(){
    U64 pTable=RemoteExport("gHotSpotVMStructs"),pStride=RemoteExport("gHotSpotVMStructEntryArrayStride"),pType=RemoteExport("gHotSpotVMStructEntryTypeNameOffset"),pField=RemoteExport("gHotSpotVMStructEntryFieldNameOffset"),pStatic=RemoteExport("gHotSpotVMStructEntryIsStaticOffset"),pOff=RemoteExport("gHotSpotVMStructEntryOffsetOffset"),pAddr=RemoteExport("gHotSpotVMStructEntryAddressOffset");
    if(!pTable||!pStride||!pType||!pField||!pStatic||!pOff||!pAddr)return FALSE;
    U64 table=R64(pTable),stride=R64(pStride),to=R64(pType),fo=R64(pField),so=R64(pStatic),oo=R64(pOff),ao=R64(pAddr);if(!table||stride<24||stride>256)return FALSE;
    ResetVMStructOffsets();
    VMNeed needs[]={
      {"ClassLoaderDataGraph","_head",0,&g_bridge.cldHeadAddr},{"ClassLoaderData","_next",&g_bridge.cldNext,0},{"ClassLoaderData","_klasses",&g_bridge.cldKlasses,0},
      {"Klass","_name",&g_bridge.klassName,0},{"Klass","_next_link",&g_bridge.klassNext,0},{"Klass","_super",&g_bridge.klassSuper,0},{"Klass","_java_mirror",&g_bridge.klassJavaMirror,0},{"Klass","_layout_helper",&g_bridge.klassLayout,0},
      {"oopDesc","_metadata._klass",&g_bridge.oopKlass,0},{"oopDesc","_metadata._compressed_klass",&g_bridge.oopCompressedKlass,0},
      {"InstanceKlass","_fields",&g_bridge.ikFields,0},{"InstanceKlass","_fieldinfo_stream",&g_bridge.ikFieldInfoStream,0},{"InstanceKlass","_constants",&g_bridge.ikConstants,0},{"Symbol","_length",&g_bridge.symLength,0},{"Symbol","_body",&g_bridge.symBody,0},
      {"CompressedOops","_narrow_oop._base",0,&g_bridge.coopsBaseAddr},{"CompressedOops","_narrow_oop._shift",0,&g_bridge.coopsShiftAddr},
      {"CompressedKlassPointers","_narrow_klass._base",0,&g_bridge.cklassBaseAddr},{"CompressedKlassPointers","_narrow_klass._shift",0,&g_bridge.cklassShiftAddr},
      {"CompressedOops","_base",0,&g_bridge.coopsBaseAddr},{"CompressedOops","_shift",0,&g_bridge.coopsShiftAddr},{"CompressedKlassPointers","_base",0,&g_bridge.cklassBaseAddr},{"CompressedKlassPointers","_shift",0,&g_bridge.cklassShiftAddr}
    };
    int count=(int)(sizeof(needs)/sizeof(needs[0]));char tn[96],fn[96];
    for(U64 e=table,guard=0;guard<12000;guard++,e+=stride){U64 tp=R64(e+to);if(!tp)break;U64 fp=R64(e+fo);if(!fp)continue;if(!RStr(tp,tn,96)||!RStr(fp,fn,96))continue;int isStatic=(int)R32(e+so);
      for(int i=0;i<count;i++)if(AEq(tn,needs[i].type)&&AEq(fn,needs[i].field)){if(isStatic&&needs[i].addrOut)*needs[i].addrOut=R64(e+ao);else if(!isStatic&&needs[i].offOut)*needs[i].offOut=(int)R64(e+oo);}
    }
    U64 pTypes=RemoteExport("gHotSpotVMTypes"),pTS=RemoteExport("gHotSpotVMTypeEntryArrayStride"),pTN=RemoteExport("gHotSpotVMTypeEntryTypeNameOffset"),pSZ=RemoteExport("gHotSpotVMTypeEntrySizeOffset");g_bridge.cpSize=0;
    if(pTypes&&pTS&&pTN&&pSZ){U64 t=R64(pTypes),st=R64(pTS),no=R64(pTN),sz=R64(pSZ);char n[96];for(U64 e=t,g=0;e&&g<6000;g++,e+=st){U64 np=R64(e+no);if(!np)break;if(RStr(np,n,96)&&AEq(n,"ConstantPool")){g_bridge.cpSize=(int)R64(e+sz);break;}}}
    if(g_bridge.coopsBaseAddr)g_bridge.coopsBase=R64(g_bridge.coopsBaseAddr);if(g_bridge.coopsShiftAddr)g_bridge.coopsShift=(int)R32(g_bridge.coopsShiftAddr);else g_bridge.coopsShift=3;
    if(g_bridge.cklassBaseAddr)g_bridge.cklassBase=R64(g_bridge.cklassBaseAddr);if(g_bridge.cklassShiftAddr)g_bridge.cklassShift=(int)R32(g_bridge.cklassShiftAddr);else g_bridge.cklassShift=3;
    if(g_bridge.cpSize<=0)g_bridge.cpSize=64;
    return g_bridge.cldHeadAddr&&g_bridge.cldNext>=0&&g_bridge.cldKlasses>=0&&g_bridge.klassName>=0&&g_bridge.klassNext>=0&&g_bridge.klassSuper>=0&&g_bridge.klassJavaMirror>=0&&(g_bridge.ikFields>=0||g_bridge.ikFieldInfoStream>=0)&&g_bridge.ikConstants>=0&&g_bridge.symLength>=0&&g_bridge.symBody>=0;
}
static BOOL ParseVMStructs(){g_bridge.heuristicStructs=0;if(ParseVMStructsExported())return TRUE;return ParseVMStructsHeuristic(g_bridge.jvmBase);}
static BOOL ReadSymbol(U64 sym,char* out,int cap){if(!sym||cap<2)return FALSE;U16 len=R16(sym+g_bridge.symLength);if(!len||len>4095)return FALSE;int n=(len<cap-1)?len:cap-1;if(!RMem(sym+g_bridge.symBody,out,n))return FALSE;out[n]=0;return TRUE;}
static BOOL KlassName(U64 k,char*out,int cap){if(!k)return FALSE;U64 sp=R64(k+g_bridge.klassName);return ReadSymbol(sp,out,cap);}
static U64 ResolveKlass(U64 oop){if(!oop)return 0;char n[96];if(g_bridge.oopCompressedKlass>=0){U32 nk=R32(oop+(U64)g_bridge.oopCompressedKlass);if(nk){U64 k=g_bridge.cklassBase+((U64)nk<<g_bridge.cklassShift);if(k&&KlassName(k,n,96))return k;}}if(g_bridge.oopKlass>=0){U64 k=R64(oop+(U64)g_bridge.oopKlass);if(k&&KlassName(k,n,96))return k;}U32 nk=R32(oop+8);if(nk){U64 k=g_bridge.cklassBase+((U64)nk<<g_bridge.cklassShift);if(k&&KlassName(k,n,96))return k;}U64 k=R64(oop+8);if(k&&KlassName(k,n,96))return k;return 0;}
static U64 DecodeOop32(U32 n){return n?g_bridge.coopsBase+((U64)n<<g_bridge.coopsShift):0;}
static U64 ReadOopAt(U64 a){U32 n=R32(a);if(n){U64 o=DecodeOop32(n);if(ResolveKlass(o))return o;}U64 o=R64(a);if(o&&ResolveKlass(o))return o;return 0;}
static BOOL ReadOopAtChecked(U64 a,U64*out){if(!out||!a)return FALSE;*out=0;U32 n=0;if(!RMem(a,&n,sizeof(n)))return FALSE;if(n==0)return TRUE;U64 o=DecodeOop32(n);if(o&&ResolveKlass(o)){*out=o;return TRUE;}U64 full=0;if(!RMem(a,&full,sizeof(full)))return FALSE;if(full==0)return TRUE;if(ResolveKlass(full)){*out=full;return TRUE;}return FALSE;}
static BOOL MatchAny(const char*s,const char*const* a,int n){for(int i=0;i<n;i++)if(AEq(s,a[i]))return TRUE;return FALSE;}
struct WantedClass { const char*const* aliases; int aliasCount; U64* out; };
static BOOL FindClassesBatch(WantedClass* wants,int wantCount){
    if(!wants||wantCount<=0)return FALSE;for(int i=0;i<wantCount;i++)if(wants[i].out)*wants[i].out=0;
    U64 cld=R64(g_bridge.cldHeadAddr);char n[180];int unresolved=wantCount,cc=0;
    for(int cg=0;cld&&cg<10000&&unresolved>0;cg++,cld=R64(cld+g_bridge.cldNext)){U64 k=R64(cld+g_bridge.cldKlasses);for(int kg=0;k&&kg<120000&&unresolved>0;kg++,k=R64(k+g_bridge.klassNext)){

            cc++;if(!KlassName(k,n,180))continue;
            for(int i=0;i<wantCount;i++)if(wants[i].out&&!*wants[i].out&&MatchAny(n,wants[i].aliases,wants[i].aliasCount)){*wants[i].out=k;unresolved--;}
        }
    }
    g_bridge.classesScanned=cc;return unresolved==0;
}
static U64 FindSuperClassAliases(U64 start,const char*const* aliases,int aliasCount){char n[180];for(U64 k=start,g=0;k&&g<64;g++,k=R64(k+g_bridge.klassSuper)){if(KlassName(k,n,180)&&MatchAny(n,aliases,aliasCount))return k;}return 0;}
static BOOL ReadCPSymbol(U64 cp,U16 idx,char*out,int cap){if(!cp||!idx)return FALSE;U64 sp=R64(cp+(U64)g_bridge.cpSize+(U64)idx*8ULL);return ReadSymbol(sp,out,cap);}
static BOOL U5Next(U64 data,int limit,int*pos,U32*out){
    if(!data||!pos||!out||*pos<0||*pos>=limit)return FALSE;int p=*pos;U32 b=R8(data+(U64)p);if(b<1)return FALSE;U32 sum=b-1;p++;if(sum<191){*pos=p;*out=sum;return TRUE;}int shift=6;
    for(int i=1;i<5;i++){if(p>=limit)return FALSE;b=R8(data+(U64)p);if(b<1)return FALSE;sum+=(b-1)<<shift;p++;if(b<192||i==4){*pos=p;*out=sum;return TRUE;}shift+=6;}return FALSE;
}
static BOOL FindFieldDeclaredLegacy(U64 k,const char*const* aliases,int aliasCount,RemoteField*out){
    if(g_bridge.ikFields<0)return FALSE;U64 fa=R64(k+g_bridge.ikFields),cp=R64(k+g_bridge.ikConstants);if(!fa||!cp)return FALSE;int len=(int)R32(fa);if(len<=0||len>12000)return FALSE;char nm[96],sig[96];int fc=len/6;U64 d=fa+4;
    for(int i=0;i<fc;i++){U64 b=d+(U64)i*12;U16 access=R16(b),ni=R16(b+2),si=R16(b+4),lo=R16(b+8),hi=R16(b+10);if(!ReadCPSymbol(cp,ni,nm,96)||!MatchAny(nm,aliases,aliasCount))continue;U32 packed=(U32)lo|((U32)hi<<16);U32 off=(packed&3u)==1u?(packed>>2):packed;out->offset=(int)off;out->access=access;out->found=TRUE;out->sig[0]=0;ReadCPSymbol(cp,si,sig,96);ACopy(out->sig,sig,96);return TRUE;}return FALSE;
}
static BOOL FindFieldDeclaredU5(U64 k,const char*const* aliases,int aliasCount,RemoteField*out){
    if(g_bridge.ikFieldInfoStream<0)return FALSE;U64 fis=R64(k+g_bridge.ikFieldInfoStream),cp=R64(k+g_bridge.ikConstants);if(!fis||!cp)return FALSE;int bytes=(int)R32(fis);if(bytes<=0||bytes>262144)return FALSE;U64 data=fis+4;int pos=0;U32 javaCount=0,injectedCount=0;if(!U5Next(data,bytes,&pos,&javaCount)||!U5Next(data,bytes,&pos,&injectedCount))return FALSE;if(javaCount>8192||injectedCount>1024)return FALSE;char nm[96],sig[96];
    for(U32 i=0;i<javaCount;i++){U32 ni=0,si=0,off=0,access=0,flags=0;if(!U5Next(data,bytes,&pos,&ni)||!U5Next(data,bytes,&pos,&si)||!U5Next(data,bytes,&pos,&off)||!U5Next(data,bytes,&pos,&access)||!U5Next(data,bytes,&pos,&flags))return FALSE;if(flags&1u){U32 z;if(!U5Next(data,bytes,&pos,&z))return FALSE;}if(flags&4u){U32 z;if(!U5Next(data,bytes,&pos,&z))return FALSE;}if(flags&16u){U32 z;if(!U5Next(data,bytes,&pos,&z))return FALSE;}if(ni>65535||si>65535)continue;if(!ReadCPSymbol(cp,(U16)ni,nm,96)||!MatchAny(nm,aliases,aliasCount))continue;out->offset=(int)off;out->access=(U16)access;out->found=TRUE;out->sig[0]=0;ReadCPSymbol(cp,(U16)si,sig,96);ACopy(out->sig,sig,96);return TRUE;}return FALSE;
}
static BOOL FindFieldOnKlass(U64 start,const char*const* aliases,int aliasCount,RemoteField*out){if(!start||!out)return FALSE;for(U64 k=start,sg=0;k&&sg<64;sg++,k=R64(k+g_bridge.klassSuper)){if(g_bridge.ikFieldInfoStream>=0){if(FindFieldDeclaredU5(k,aliases,aliasCount,out))return TRUE;}else if(FindFieldDeclaredLegacy(k,aliases,aliasCount,out))return TRUE;}return FALSE;}
static BOOL FindField1(U64 k,const char*a,RemoteField*out){const char* x[]={a};return FindFieldOnKlass(k,x,1,out);} 
static BOOL FindField3(U64 k,const char*a,const char*b,const char*c,RemoteField*out){const char*x[]={a,b,c};return FindFieldOnKlass(k,x,3,out);} 
static BOOL FindField4(U64 k,const char*a,const char*b,const char*c,const char*d,RemoteField*out){const char*x[]={a,b,c,d};return FindFieldOnKlass(k,x,4,out);} 
static U64 KlassMirror(U64 k){U64 handle=R64(k+g_bridge.klassJavaMirror);return handle?R64(handle):0;}
static U64 ReadStaticOop(U64 k,const RemoteField& f){U64 m=KlassMirror(k);return (m&&f.found)?ReadOopAt(m+f.offset):0;}
static U64 ReadObjField(U64 o,const RemoteField&f){return (o&&f.found)?ReadOopAt(o+f.offset):0;}

static BOOL ReadSharedFlags(U64 p,U8* out){if(!out||!p)return FALSE;RemoteField df=g_bridge.entData;if(!df.found){U64 pk=ResolveKlass(p);const char*ea[]={"cgk","net/minecraft/class_1297","net/minecraft/world/entity/Entity","net/minecraft/entity/Entity"};U64 decl=FindSuperClassAliases(pk,ea,4);if(!decl||!FindField4(decl,"az","field_6011","entityData","dataTracker",&df))return FALSE;}U64 data=ReadObjField(p,df);if(!data)return FALSE;RemoteField ef=g_bridge.dataItems;if(!ef.found){U64 dk=ResolveKlass(data);if(!FindField4(dk,"e","field_13331","itemsById","entries",&ef))return FALSE;}U64 arr=ReadObjField(data,ef);if(!arr)return FALSE;U64 entry=ReadOopAt(arr+16);if(!entry)entry=ReadOopAt(arr+24);if(!entry)return FALSE;RemoteField vf=g_bridge.dataItemValue;if(!vf.found){U64 ek=ResolveKlass(entry);if(!FindField4(ek,"b","field_13338","value","initialValue",&vf))return FALSE;}U64 boxed=ReadObjField(entry,vf);if(!boxed)return FALSE;RemoteField bv=g_bridge.boxedByteValue;if(!bv.found){U64 bk=ResolveKlass(boxed);if(!FindField1(bk,"value",&bv))return FALSE;}*out=R8(boxed+bv.offset);return TRUE;}
static BOOL ReadLocalSharedFlags(U64 p,U8*out){return ReadSharedFlags(p,out);}

static BOOL CacheGameFields(){
    const char* mcAliases[]={"gfj","net/minecraft/class_310","net/minecraft/client/Minecraft","net/minecraft/client/MinecraftClient"};
    const char* itAliases[]={"dlx","net/minecraft/class_1802","net/minecraft/world/item/Items","net/minecraft/item/Items"};
    const char* stackAliases[]={"net/minecraft/class_1799","net/minecraft/world/item/ItemStack","net/minecraft/item/ItemStack"};
    const char* playerAliases[]={"ddm","net/minecraft/class_1657","net/minecraft/world/entity/player/Player","net/minecraft/entity/player/PlayerEntity"};
    const char* livingAliases[]={"chl","net/minecraft/class_1309","net/minecraft/world/entity/LivingEntity","net/minecraft/entity/LivingEntity"};
    const char* entityAliases[]={"cgk","net/minecraft/class_1297","net/minecraft/world/entity/Entity","net/minecraft/entity/Entity"};
    WantedClass wants[]={{mcAliases,4,&g_bridge.mcKlass},{itAliases,4,&g_bridge.itemsKlass},{stackAliases,3,&g_bridge.itemStackKlass},{playerAliases,4,&g_bridge.playerKlass},{livingAliases,4,&g_bridge.livingKlass},{entityAliases,4,&g_bridge.entityKlass}};
    if(!FindClassesBatch(wants,(int)(sizeof(wants)/sizeof(wants[0]))))return FALSE;
    if(!FindField4(g_bridge.mcKlass,"A","field_1700","instance","INSTANCE",&g_bridge.mcInstance))return FALSE;
    FindField3(g_bridge.mcKlass,"s","field_1724","player",&g_bridge.mcPlayer);FindField4(g_bridge.mcKlass,"t","field_1692","crosshairPickEntity","targetedEntity",&g_bridge.mcTarget);FindField3(g_bridge.mcKlass,"x","field_1755","screen",&g_bridge.mcScreen);
    FindField4(g_bridge.playerKlass,"cK","field_7525","lastItemInMainHand","selectedItem",&g_bridge.playerHeld);FindField3(g_bridge.livingKlass,"bz","field_6273","attackStrengthTicker",&g_bridge.livingAttackTicker);
    FindField3(g_bridge.entityKlass,"bc","field_5952","onGround",&g_bridge.entOnGround);FindField3(g_bridge.entityKlass,"an","field_6017","fallDistance",&g_bridge.entFall);FindField3(g_bridge.entityKlass,"aT","field_6034","vehicle",&g_bridge.entVehicle);FindField4(g_bridge.entityKlass,"au","field_5957","wasTouchingWater","touchingWater",&g_bridge.entWater);FindField4(g_bridge.entityKlass,"aw","field_6000","wasEyeInWater","submergedInWater",&g_bridge.entEyeWater);FindField4(g_bridge.entityKlass,"az","field_6011","entityData","dataTracker",&g_bridge.entData);FindField3(g_bridge.itemStackKlass,"t","field_8038","item",&g_bridge.stackItem);
    const char* swordObf[7]={"qn","qs","qx","qC","qH","qM","qR"};const char* swordInt[7]={"field_8091","field_61338","field_8528","field_8845","field_8371","field_8802","field_22022"};const char* swordNamed[7]={"WOODEN_SWORD","COPPER_SWORD","STONE_SWORD","GOLDEN_SWORD","IRON_SWORD","DIAMOND_SWORD","NETHERITE_SWORD"};
    for(int i=0;i<7;i++)FindField3(g_bridge.itemsKlass,swordObf[i],swordInt[i],swordNamed[i],&g_bridge.swordItemFields[i]);FindField3(g_bridge.itemsKlass,"wn","field_49814","MACE",&g_bridge.maceItemField);
    BOOL anyWeaponRoot=g_bridge.maceItemField.found;for(int i=0;i<7;i++)if(g_bridge.swordItemFields[i].found)anyWeaponRoot=TRUE;
    return g_bridge.mcInstance.found&&g_bridge.mcPlayer.found&&g_bridge.mcTarget.found&&g_bridge.mcScreen.found&&g_bridge.playerHeld.found&&g_bridge.stackItem.found&&g_bridge.livingAttackTicker.found&&anyWeaponRoot;
}
struct ProcEdge { DWORD pid; DWORD ppid; };
static ProcEdge g_procEdges[2048];
static int SnapshotProcEdges(ProcEdge* out,int cap){if(!out||cap<=0)return 0;HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(snap==INVALID_HANDLE_VALUE)return 0;PROCESSENTRY32W pe;memset(&pe,0,sizeof(pe));pe.dwSize=sizeof(pe);int n=0;if(Process32FirstW(snap,&pe))do{if(pe.th32ProcessID&&n<cap){out[n].pid=pe.th32ProcessID;out[n].ppid=pe.th32ParentProcessID;n++;}}while(Process32NextW(snap,&pe));CloseHandle(snap);return n;}
static BOOL BridgeHostNameLikely(DWORD pid){wchar_t path[520];path[0]=0;if(!GetProcessPathStrict(pid,path,520))return FALSE;const wchar_t* b=BaseNamePtr(path);return WEqualI(b,L"java.exe")||WEqualI(b,L"javaw.exe")||WContainsI(b,L"pulse")||WContainsI(b,L"lunar")||WContainsI(b,L"badlion")||WContainsI(b,L"feather")||WContainsI(b,L"minecraft")||WContainsI(b,L"aiaber")||WContainsI(b,L"altber");}
static BOOL BridgeHostNameBad(DWORD pid){wchar_t path[520];path[0]=0;if(!GetProcessPathStrict(pid,path,520))return TRUE;const wchar_t* b=BaseNamePtr(path);return WContainsI(b,L"webview")||WContainsI(b,L"crash")||WContainsI(b,L"updater")||WContainsI(b,L"update")||WContainsI(b,L"renderer")||WContainsI(b,L"cef")||WContainsI(b,L"helper");}
static int AddUniquePid(DWORD* out,int n,int cap,DWORD pid){if(!pid)return n;for(int i=0;i<n;i++)if(out[i]==pid)return n;if(n<cap)out[n++]=pid;return n;}
static U64 AbsDiff64(U64 a,U64 b){return a>b?a-b:b-a;}
struct HostRank {DWORD pid;U64 delta;int score;};
static void AddRankedHost(HostRank* a,int* n,int cap,DWORD pid,U64 rootCreate,int bonus){if(!a||!n||!pid||*n>=cap)return;for(int i=0;i<*n;i++)if(a[i].pid==pid)return;U64 ct=GetProcCreateTimeValue(pid);if(!ct)return;U64 d=AbsDiff64(ct,rootCreate);int score=bonus;if(d<=120ULL*10000000ULL)score+=800-(int)(d/(1500000ULL));a[*n].pid=pid;a[*n].delta=d;a[*n].score=score;(*n)++;}
static void SortHosts(HostRank* a,int n){for(int i=1;i<n;i++){HostRank v=a[i];int j=i-1;while(j>=0&&a[j].score<v.score){a[j+1]=a[j];j--;}a[j+1]=v;}}
static __declspec(noinline) int BuildBridgeHostList(DWORD root,DWORD* out,int cap){

    if(!out||cap<=0)return 0;int n=0;U64 rootCreate=GetProcCreateTimeValue(root);n=AddUniquePid(out,n,cap,root);
    int ec=SnapshotProcEdges(g_procEdges,2048);HostRank ranked[96];int rn=0;
    if(ec>0){DWORD cur=root;for(int depth=0;depth<4;depth++){DWORD parent=0;for(int i=0;i<ec;i++)if(g_procEdges[i].pid==cur){parent=g_procEdges[i].ppid;break;}if(!parent||parent==4)break;if(!BridgeHostNameBad(parent))AddRankedHost(ranked,&rn,96,parent,rootCreate,BridgeHostNameLikely(parent)?1200:500);cur=parent;}
        DWORD frontier[64];int fn=1;frontier[0]=root;for(int depth=0;depth<4&&fn>0;depth++){DWORD next[64];int nn=0;for(int i=0;i<ec;i++){BOOL child=FALSE;for(int j=0;j<fn;j++)if(g_procEdges[i].ppid==frontier[j]){child=TRUE;break;}if(!child)continue;if(nn<64)next[nn++]=g_procEdges[i].pid;if(!BridgeHostNameBad(g_procEdges[i].pid))AddRankedHost(ranked,&rn,96,g_procEdges[i].pid,rootCreate,BridgeHostNameLikely(g_procEdges[i].pid)?1500:650);}fn=nn;for(int i=0;i<fn;i++)frontier[i]=next[i];}}
    HANDLE snap=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    if(snap!=INVALID_HANDLE_VALUE){PROCESSENTRY32W pe;memset(&pe,0,sizeof(pe));pe.dwSize=sizeof(pe);if(Process32FirstW(snap,&pe))do{if(!pe.th32ProcessID||pe.th32ProcessID==4||pe.th32ProcessID==GetCurrentProcessId()||pe.th32ProcessID==root)continue;BOOL java=WEqualI(pe.szExeFile,L"java.exe")||WEqualI(pe.szExeFile,L"javaw.exe");BOOL known=WContainsI(pe.szExeFile,L"minecraft")||WContainsI(pe.szExeFile,L"lunar")||WContainsI(pe.szExeFile,L"badlion")||WContainsI(pe.szExeFile,L"feather")||WContainsI(pe.szExeFile,L"pulse");if(java||known)AddRankedHost(ranked,&rn,96,pe.th32ProcessID,rootCreate,java?1100:500);}while(Process32NextW(snap,&pe));CloseHandle(snap);}
    SortHosts(ranked,rn);for(int i=0;i<rn&&n<cap;i++)n=AddUniquePid(out,n,cap,ranked[i].pid);return n;
}
static BOOL IsJvmNamedModule(const wchar_t* n,const wchar_t* p){return WEqualI(n,L"jvm.dll")||WContainsI(n,L"hotspot")||WContainsI(n,L"jvm")||AEndsI(p,L"\jvm.dll")||WContainsI(p,L"\server\jvm");}
static int BuildGlobalJvmModuleHosts(DWORD root,DWORD* out,int cap){
    if(!out||cap<=0)return 0;U64 rc=GetProcCreateTimeValue(root);HostRank ranked[64];int rn=0;HANDLE ps=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);if(ps==INVALID_HANDLE_VALUE)return 0;PROCESSENTRY32W pe;memset(&pe,0,sizeof(pe));pe.dwSize=sizeof(pe);
    if(Process32FirstW(ps,&pe))do{DWORD pid=pe.th32ProcessID;if(!pid||pid==4||pid==GetCurrentProcessId()||pid==root||BridgeHostNameBad(pid))continue;HANDLE ms=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);if(ms==INVALID_HANDLE_VALUE)continue;MODULEENTRY32W me;memset(&me,0,sizeof(me));me.dwSize=sizeof(me);BOOL hit=FALSE;if(Module32FirstW(ms,&me))do{if(IsJvmNamedModule(me.szModule,me.szExePath)){hit=TRUE;break;}}while(Module32NextW(ms,&me));CloseHandle(ms);if(hit)AddRankedHost(ranked,&rn,64,pid,rc,1800);}while(Process32NextW(ps,&pe));CloseHandle(ps);SortHosts(ranked,rn);int n=0;for(int i=0;i<rn&&n<cap;i++)n=AddUniquePid(out,n,cap,ranked[i].pid);return n;
}
static ULONGLONG g_bridgeRetryAt=0; static volatile LONG g_bridgeAttachBusy=0; static DWORD g_attachPid=0; static ULONGLONG g_attachCreate=0;
static DWORD g_bridgeAttemptPid=0; static ULONGLONG g_bridgeAttemptCreate=0; static int g_bridgeAttemptCount=0;
static void ResetBridgeAttempts(){g_bridgeRetryAt=0;g_bridgeAttemptPid=g_bridgePidHint;g_bridgeAttemptCreate=g_bridgeCreateHint;g_bridgeAttemptCount=0;}
static void BridgeClose(){if(g_bridge.process)CloseHandle(g_bridge.process);memset(&g_bridge,0,sizeof(g_bridge));GameSnapshot z={};PublishGameSnapshot(z);memset(g_targetKindCache,0,sizeof(g_targetKindCache));g_targetKindCacheNext=0;}
static int BridgeTryHost(DWORD targetPid,U64 targetCreate,DWORD hostPid,char* failOut,int failCap){
    BridgeClose();g_bridge.state=BR_ATTACHING;g_bridge.pid=targetPid;g_bridge.createTime=targetCreate;g_bridge.hostPid=hostPid;g_bridge.hostCreateTime=GetProcCreateTimeValue(hostPid);g_diagHostsTried++;
    wchar_t hp[520];hp[0]=0;GetProcessPathStrict(hostPid,hp,520);g_diagHostPid=hostPid;WCopy(g_diagHostName,BaseNamePtr(hp),260);
    ACopy(g_bridge.status,"opening candidate JVM host",128);g_bridge.process=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,FALSE,hostPid);if(!g_bridge.process)g_bridge.process=OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION|PROCESS_VM_READ,FALSE,hostPid);
    if(!g_bridge.process){ACopy(g_bridge.status,"cannot open JVM host process for read",128);if(failOut)ACopy(failOut,g_bridge.status,failCap);return 1;}
    ACopy(g_bridge.status,"read-only JVM/HotSpot probe",128);g_bridge.jvmBase=FindJvmBase(hostPid);
    if(!g_bridge.jvmBase){ACopy(g_bridge.status,"JVM/HotSpot image or embedded VMStructs not found",128);if(failOut)ACopy(failOut,g_bridge.status,failCap);return 2;}
    ACopy(g_bridge.status,g_bridge.heuristicStructs?"embedded HotSpot markers found - recovering VMStructs":"HotSpot found - parsing VMStructs",128);if(!ParseVMStructs()){ACopy(g_bridge.status,"HotSpot candidate found, VMStruct recovery failed",128);if(failOut)ACopy(failOut,g_bridge.status,failCap);return 3;}
    g_bridge.structsParsed=1;ACopy(g_bridge.status,g_bridge.heuristicStructs?"exportless VMStructs recovered - resolving 1.21.11":(g_bridge.ikFieldInfoStream>=0?"JDK21 fieldinfo stream ready - resolving 1.21.11":"legacy field table ready - resolving 1.21.11"),128);
    if(!CacheGameFields()){ACopy(g_bridge.status,"JVM ready, 1.21.11 mapping resolve failed",128);if(failOut)ACopy(failOut,g_bridge.status,failCap);return 4;}
    g_bridge.state=BR_READY;ACopy(g_bridge.status,g_bridge.heuristicStructs?"Minecraft bridge ready (embedded HotSpot)":"Minecraft bridge ready",128);return 5;
}
static BOOL TryHostSet(DWORD targetPid,U64 targetCreate,const DWORD* hosts,int hc,char* bestFail,int failCap,int* bestRank,DWORD* bestHost){for(int i=0;i<hc;i++){char f[128];f[0]=0;int rank=BridgeTryHost(targetPid,targetCreate,hosts[i],f,128);if(rank==5)return TRUE;if(rank>*bestRank){*bestRank=rank;*bestHost=hosts[i];if(f[0])ACopy(bestFail,f,failCap);}}return FALSE;}
static BOOL BridgeAttachPid(DWORD pid){
    U64 targetCreate=GetProcCreateTimeValue(pid);if(!targetCreate){BridgeClose();g_bridge.state=BR_UNSUPPORTED;ACopy(g_bridge.status,"selected PID vanished before attach",128);return FALSE;}
    g_diagHostsTried=0;g_diagHostPid=0;g_diagHostName[0]=0;g_diagFailRank=0;char bestFail[128];bestFail[0]=0;int bestRank=0;DWORD bestHost=0;
    DWORD hosts[32];int hc=BuildBridgeHostList(pid,hosts,32);if(TryHostSet(pid,targetCreate,hosts,hc,bestFail,128,&bestRank,&bestHost))return TRUE;
    DWORD deep[32];int dc=BuildGlobalJvmModuleHosts(pid,deep,32);if(TryHostSet(pid,targetCreate,deep,dc,bestFail,128,&bestRank,&bestHost))return TRUE;
    BridgeClose();g_bridge.pid=pid;g_bridge.createTime=targetCreate;g_bridge.state=BR_UNSUPPORTED;g_diagFailRank=bestRank;g_diagHostPid=bestHost;if(bestHost){wchar_t hp[520];hp[0]=0;if(GetProcessPathStrict(bestHost,hp,520))WCopy(g_diagHostName,BaseNamePtr(hp),260);}
    if(bestFail[0])ACopy(g_bridge.status,bestFail,128);else ACopy(g_bridge.status,"no readable Java/JVM host found for selected Minecraft session",128);return FALSE;
}
static DWORD WINAPI BridgeAttachWorker(LPVOID){
    SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_IDLE);
    DWORD pid=g_attachPid;ULONGLONG expected=g_attachCreate;BOOL ok=BridgeAttachPid(pid);
    if(pid!=g_bridgePidHint||expected!=g_bridgeCreateHint){BridgeClose();}
    else if(ok){g_bridgeRetryAt=0;}
    else{ULONGLONG now=GetTickCount64();if(g_bridgeAttemptCount<=1)g_bridgeRetryAt=now+5000;else if(g_bridgeAttemptCount==2)g_bridgeRetryAt=now+12000;else g_bridgeRetryAt=now+60000;}
    g_bridgeAttachBusy=0;return 0;
}
static BOOL BridgeEnsure(){
    if(g_bridge.state==BR_READY&&g_bridge.pid==g_bridgePidHint&&g_bridge.createTime==g_bridgeCreateHint&&g_bridgePidHint)return TRUE;
    if(!g_bridgePidHint)return FALSE;if(g_bridgeAttemptPid!=g_bridgePidHint||g_bridgeAttemptCreate!=g_bridgeCreateHint)ResetBridgeAttempts();if(g_bridgeAttachBusy)return FALSE;
    ULONGLONG now=GetTickCount64();if(now<g_bridgeRetryAt)return FALSE;g_attachPid=g_bridgePidHint;g_attachCreate=g_bridgeCreateHint;g_bridgeAttachBusy=1;g_bridgeAttemptCount++;
    DWORD tid=0;HANDLE th=CreateThread(0,0,BridgeAttachWorker,0,0,&tid);if(!th){g_bridgeAttachBusy=0;g_bridgeRetryAt=now+30000;ACopy(g_bridge.status,"attach worker create failed",128);return FALSE;}CloseHandle(th);return FALSE;
}
static WeaponKind CurrentWeapon(U64 player){U64 st=ReadObjField(player,g_bridge.playerHeld);if(!st)return WPN_NONE;U64 item=ReadObjField(st,g_bridge.stackItem);if(!item)return WPN_NONE;U64 mace=ReadStaticOop(g_bridge.itemsKlass,g_bridge.maceItemField);if(mace&&item==mace)return WPN_MACE;for(int i=0;i<7;i++){U64 sword=ReadStaticOop(g_bridge.itemsKlass,g_bridge.swordItemFields[i]);if(sword&&item==sword)return WPN_SWORD;}
