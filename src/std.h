#ifndef _NOTEPAD_STD_H
#define _NOTEPAD_STD_H

/* attributes */
#ifdef _MSC_VER
    #define NORETURN __declspec(noreturn)
#else
    #define NORETURN __attribute__((noreturn))
#endif

/* stdbool.h */
typedef unsigned char bool;
#define true ((bool)1)
#define false ((bool)0)

/* stdint.h */
#ifdef _MSC_VER
    typedef __int8 int8_t;
    typedef __int16 int16_t;
    typedef __int32 int32_t;
    typedef __int64 int64_t, ptrdiff_t, intmax_t;
    typedef unsigned __int8 uint8_t;
    typedef unsigned __int16 uint16_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t, size_t, uintptr_t;
#else
    typedef char  int8_t;
    typedef short int16_t;
    typedef int   int32_t;
    typedef unsigned char  uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int   uint32_t;
    #if defined(__GNUC__)
        __extension__ typedef long long int64_t;
        __extension__ typedef unsigned long long uint64_t;
    #elif defined(__TINYC__)
        typedef long long int64_t;
        typedef unsigned long long uint64_t;
    #endif
    typedef int64_t ptrdiff_t, intptr_t, intmax_t;
    typedef uint64_t size_t, uintptr_t;
#endif

/* stdarg.h */
#if defined(_MSC_VER)
    typedef void *va_list;
    #define va_start(_list, _arg) __va_start(&(_list), (_arg))
#else
    typedef char *va_list;
    #define va_start(_list, _arg) __builtin_va_start((_list), (_arg))
#endif

/* windows.h */
#ifdef _MSC_VER
    #define WINAPI __stdcall
#else
    #define WINAPI __attribute__((stdcall))
#endif

#define CALLBACK WINAPI

typedef int32_t BOOL, LONG;
typedef int64_t LONG_PTR, LPARAM, LRESULT, LONGLONG;
typedef uint8_t BYTE, *LPBYTE;
typedef uint16_t WORD, WCHAR, USHORT, ATOM;
typedef uint32_t DWORD, *LPDWORD, UINT, ULONG;
typedef uint64_t UINT_PTR, ULONG_PTR, DWORD_PTR, WPARAM;
typedef size_t SIZE_T;
typedef void *PVOID, *LPVOID, *HANDLE, *HWND, *HINSTANCE, *HICON, *HCURSOR, *HBRUSH, *HMODULE, *HLOCAL,
*HMENU, *HDC, *HGDIOBJ, *HGLRC, *PROC, *FARPROC, *HRSRC, *HGLOBAL;
typedef const void *LPCVOID;
typedef const char *LPCSTR;
typedef WCHAR *PWSTR, *LPWSTR;
typedef const WCHAR *PCWSTR, *LPCWSTR;

typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSEXW {
    UINT      cbSize;
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCWSTR   lpszMenuName;
    LPCWSTR   lpszClassName;
    HICON     hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;

typedef struct _LIST_ENTRY _LIST_ENTRY;

typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY, *PLIST_ENTRY, *PRLIST_ENTRY;

typedef struct _RTL_CRITICAL_SECTION _RTL_CRITICAL_SECTION;

typedef struct _RTL_CRITICAL_SECTION_DEBUG {
    WORD Type;
    WORD CreatorBackTraceIndex;
    struct _RTL_CRITICAL_SECTION *CriticalSection;
    LIST_ENTRY ProcessLocksList;
    DWORD EntryCount;
    DWORD ContentionCount;
    DWORD Flags;
    WORD  CreatorBackTraceIndexHigh;
    WORD  SpareWORD;
} RTL_CRITICAL_SECTION_DEBUG, *PRTL_CRITICAL_SECTION_DEBUG;

#pragma pack(push, 8)
typedef struct _RTL_CRITICAL_SECTION {
    PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
    LONG LockCount;
    LONG RecursionCount;
    HANDLE OwningThread;
    HANDLE LockSemaphore;
    ULONG_PTR SpinCount;
} RTL_CRITICAL_SECTION, *PRTL_CRITICAL_SECTION;
typedef RTL_CRITICAL_SECTION CRITICAL_SECTION, *LPCRITICAL_SECTION;
#pragma pack(pop)

typedef union _LARGE_INTEGER {
    struct {
        ULONG LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        ULONG LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef struct tagPOINT {
    LONG  x;
    LONG  y;
} POINT, *PPOINT, *LPPOINT;

typedef struct tagMSG {
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG, *PMSG, *LPMSG;

typedef struct tagRECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT, *LPRECT;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _STARTUPINFOW {
    DWORD   cb;
    LPWSTR  lpReserved;
    LPWSTR  lpDesktop;
    LPWSTR  lpTitle;
    DWORD   dwX;
    DWORD   dwY;
    DWORD   dwXSize;
    DWORD   dwYSize;
    DWORD   dwXCountChars;
    DWORD   dwYCountChars;
    DWORD   dwFillAttribute;
    DWORD   dwFlags;
    WORD    wShowWindow;
    WORD    cbReserved2;
    LPBYTE  lpReserved2;
    HANDLE  hStdInput;
    HANDLE  hStdOutput;
    HANDLE  hStdError;
} STARTUPINFOW, *LPSTARTUPINFOW;

typedef struct _PROCESS_INFORMATION {
    HANDLE hProcess;
    HANDLE hThread;
    DWORD dwProcessId;
    DWORD dwThreadId;
} PROCESS_INFORMATION, *LPPROCESS_INFORMATION;

typedef struct tagPIXELFORMATDESCRIPTOR
{
    WORD  nSize;
    WORD  nVersion;
    DWORD dwFlags;
    BYTE  iPixelType;
    BYTE  cColorBits;
    BYTE  cRedBits;
    BYTE  cRedShift;
    BYTE  cGreenBits;
    BYTE  cGreenShift;
    BYTE  cBlueBits;
    BYTE  cBlueShift;
    BYTE  cAlphaBits;
    BYTE  cAlphaShift;
    BYTE  cAccumBits;
    BYTE  cAccumRedBits;
    BYTE  cAccumGreenBits;
    BYTE  cAccumBlueBits;
    BYTE  cAccumAlphaBits;
    BYTE  cDepthBits;
    BYTE  cStencilBits;
    BYTE  cAuxBuffers;
    BYTE  iLayerType;
    BYTE  bReserved;
    DWORD dwLayerMask;
    DWORD dwVisibleMask;
    DWORD dwDamageMask;
} PIXELFORMATDESCRIPTOR, *LPPIXELFORMATDESCRIPTOR;

typedef UINT_PTR (CALLBACK *LPOFNHOOKPROC) (HWND, UINT, WPARAM, LPARAM);

typedef struct tagOFNW {
   DWORD         lStructSize;
   HWND          hwndOwner;
   HINSTANCE     hInstance;
   LPCWSTR       lpstrFilter;
   LPWSTR        lpstrCustomFilter;
   DWORD         nMaxCustFilter;
   DWORD         nFilterIndex;
   LPWSTR        lpstrFile;
   DWORD         nMaxFile;
   LPWSTR        lpstrFileTitle;
   DWORD         nMaxFileTitle;
   LPCWSTR       lpstrInitialDir;
   LPCWSTR       lpstrTitle;
   DWORD         Flags;
   WORD          nFileOffset;
   WORD          nFileExtension;
   LPCWSTR       lpstrDefExt;
   LPARAM        lCustData;
   LPOFNHOOKPROC lpfnHook;
   LPCWSTR       lpTemplateName;
   void         *pvReserved;
   DWORD         dwReserved;
   DWORD         FlagsEx;
} OPENFILENAMEW, *LPOPENFILENAMEW;

typedef void (*TIMERPROC)(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        PVOID Pointer;
    } DUMMYUNIONNAME;
    HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID lpParameter);

typedef struct tagSCROLLINFO {
  UINT cbSize;
  UINT fMask;
  int  nMin;
  int  nMax;
  UINT nPage;
  int  nPos;
  int  nTrackPos;
} SCROLLINFO, *LPSCROLLINFO;

typedef const LPSCROLLINFO LPCSCROLLINFO;

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;
    struct {
      WORD wProcessorArchitecture;
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;
  LPVOID    lpMinimumApplicationAddress;
  LPVOID    lpMaximumApplicationAddress;
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *LPSYSTEM_INFO;

#define NULL                               ((void*)0)
#define TRUE                               ((BOOL)1)
#define FALSE                              ((BOOL)0)
#define MB_OK                              0x00000000L
#define MB_ICONERROR                       0x00000010L
#define INVALID_HANDLE_VALUE               ((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_SIZE                  ((DWORD)0xFFFFFFFF)
#define ATTACH_PARENT_PROCESS              ((DWORD)-1)
#define STD_OUTPUT_HANDLE                  ((DWORD)-11)
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define MAKEINTRESOURCE(res)               ((ULONG_PTR)(USHORT)res)
#define IDC_ARROW                          MAKEINTRESOURCE(32512)
#define IDC_IBEAM                          MAKEINTRESOURCE(32513)
#define WS_OVERLAPPEDWINDOW                0x00CF0000L
#define WS_CHILD                           0x40000000L
#define WS_VISIBLE                         0x10000000L
#define WS_HSCROLL                         0x00100000L
#define WS_VSCROLL                         0x00200000L
#define CW_USEDEFAULT                      ((int)0x80000000)
#define SW_HIDE                            0
#define SW_SHOW                            5
#define WM_SIZE                            0x0005
#define WM_PAINT                           0x000F
#define WM_CLOSE                           0x0010
#define WM_USER                            0x0400
#define WM_COMMAND                         0x0111
#define WM_CHAR                            0x0102
#define WM_TIMER                           0x0113
#define WM_QUIT                            0x0012
#define WM_MOUSEWHEEL                      0x020A
#define WM_VSCROLL                         0x0115
#define MF_BYCOMMAND                       0x00000000L
#define MFT_STRING                         0x00000000L
#define MFT_BITMAP                         0x00000004L
#define MFT_MENUBARBREAK                   0x00000020L
#define MFT_MENUBREAK                      0x00000040L
#define MFT_OWNERDRAW                      0x00000100L
#define MFT_RADIOCHECK                     0x00000200L
#define MFT_SEPARATOR                      0x00000800L
#define MFT_RIGHTORDER                     0x00002000L
#define MFT_RIGHTJUSTIFY                   0x00004000L
#define MFS_GRAYED                         0x00000003L
#define MFS_DISABLED                       0x00000003L
#define MFS_CHECKED                        0x00000008L
#define MFS_HILITE                         0x00000080L
#define MFS_ENABLED                        0x00000000L
#define MFS_UNCHECKED                      0x00000000L
#define MFS_UNHILITE                       0x00000000L
#define MFS_DEFAULT                        0x00001000L
#define STATUSCLASSNAMEW                   L"msctls_statusbar32"
#define SBARS_SIZEGRIP                     0x0100
#define SBARS_TOOLTIPS                     0x0800
#define SBT_NOBORDERS                      0x0100
#define SB_SETPARTS                        (WM_USER+4)
#define SB_SETTEXTW                        (WM_USER+11)
#define CS_VREDRAW                         0x0001
#define CS_HREDRAW                         0x0002
#define CS_DBLCLKS                         0x0008
#define CS_OWNDC                           0x0020
#define WHITE_BRUSH                        0
#define BLACK_BRUSH                        4
#define LOWORD(l)                          ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)                          ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define PFD_TYPE_RGBA                      0
#define PFD_MAIN_PLANE                     0
#define PFD_DOUBLEBUFFER                   0x00000001
#define PFD_DRAW_TO_WINDOW                 0x00000004
#define PFD_SUPPORT_OPENGL                 0x00000020
#define GENERIC_READ                       0x80000000L
#define GENERIC_WRITE                      0x40000000L
#define FILE_SHARE_READ                    0x00000001
#define FILE_SHARE_WRITE                   0x00000002
#define FILE_SHARE_DELETE                  0x00000004
#define CREATE_ALWAYS                      2
#define OPEN_EXISTING                      3
#define OPEN_ALWAYS                        4
#define FILE_ATTRIBUTE_NORMAL              0x80
#define FILE_FLAG_NO_BUFFERING             0x20000000
#define FILE_FLAG_OVERLAPPED               0x40000000
#define FILE_FLAG_WRITE_THROUGH            0x80000000
#define PAGE_READONLY                      0x02
#define PAGE_READWRITE                     0x04
#define FILE_MAP_WRITE                     0x0002
#define FILE_MAP_READ                      0x0004
#define FILE_MAP_ALL_ACCESS                0x000F001FL
#define OFN_EXPLORER                       0x00080000
#define OFN_PATHMUSTEXIST                  0x00000800
#define OFN_HIDEREADONLY                   0x00000004
#define RT_RCDATA                          MAKEINTRESOURCE(10)
#define MEM_COMMIT                         0x00001000
#define MEM_RESERVE                        0x00002000
#define MEM_DECOMMIT                       0x00004000
#define MEM_RELEASE                        0x00008000
#define INFINITE                           0xFFFFFFFF
#define WAIT_FAILED                        0xFFFFFFFF
#define WAIT_TIMEOUT                       258L
#define WAIT_OBJECT_0                      0
#define WAIT_ABANDONED                     0x00000080L
#define WAIT_ABANDONED_0                   0x00000080L
#define PM_NOREMOVE                        0x0000
#define PM_REMOVE                          0x0001
#define QS_ALLINPUT                        0xFFFF
#define ERROR_HANDLE_EOF                   38
#define WHEEL_DELTA                        120
#define GET_WHEEL_DELTA_WPARAM(wParam)     ((short)HIWORD(wParam))
#define WC_SCROLLBARW                      L"ScrollBar"
#define SBS_VERT                           0x0001L
#define SBS_RIGHTALIGN                     0x0004L
#define SM_CXVSCROLL                       2
#define SB_LINEUP                          0
#define SB_LINELEFT                        0
#define SB_LINEDOWN                        1
#define SB_LINERIGHT                       1
#define SB_PAGEUP                          2
#define SB_PAGELEFT                        2
#define SB_PAGEDOWN                        3
#define SB_PAGERIGHT                       3
#define SB_THUMBPOSITION                   4
#define SB_THUMBTRACK                      5
#define SB_TOP                             6
#define SB_LEFT                            6
#define SB_BOTTOM                          7
#define SB_RIGHT                           7
#define SB_ENDSCROLL                       8
#define SB_VERT                            1
#define SIF_RANGE                          0x0001
#define SIF_PAGE                           0x0002
#define SIF_POS                            0x0004
#define SIF_DISABLENOSCROLL                0x0008
#define SIF_TRACKPOS                       0x0010
#define SIF_ALL                            (SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS)
#define HEAP_NO_SERIALIZE                  0x00000001
#define HEAP_GENERATE_EXCEPTIONS           0x00000004
#define HEAP_ZERO_MEMORY                   0x00000008
#define HEAP_REALLOC_IN_PLACE_ONLY         0x00000010

/* kernel32.dll */
NORETURN void ExitProcess(int code);
DWORD GetCurrentProcessId(void);
DWORD GetCurrentThreadId(void);
DWORD GetLastError(void);
HMODULE GetModuleHandleW(LPCWSTR lpModuleName);
LPWSTR GetCommandLineW(void);
void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
BOOL WINAPI AttachConsole(DWORD dwProcessId);
HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
BOOL WINAPI GetConsoleMode(HANDLE hConsoleHandle, LPDWORD lpMode);
BOOL WINAPI SetConsoleMode(HANDLE hConsoleHandle, DWORD dwMode);
BOOL WINAPI WriteConsoleW(
    HANDLE hConsoleOutput,
    LPCWSTR lpText,
    DWORD nNumberOfCharsToWrite,
    LPDWORD lpNumberOfCharsWritten,
    LPVOID lpReserved
);
BOOL IsDebuggerPresent(void);
void OutputDebugStringW(LPCWSTR lpOutputString);
HLOCAL LocalFree(HLOCAL hMem);
BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency);
BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
BOOL CreateProcessW(
    LPCWSTR               lpApplicationName,
    LPWSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCWSTR               lpCurrentDirectory,
    LPSTARTUPINFOW        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
);
HMODULE LoadLibraryW(LPCWSTR lpLibFileName);
BOOL FreeLibrary(HMODULE hLibModule);
FARPROC GetProcAddress(HMODULE hModule, LPCSTR lpProcName);
HANDLE CreateFileW(
    LPCWSTR               lpFileName,
    DWORD                 dwDesiredAccess,
    DWORD                 dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD                 dwCreationDisposition,
    DWORD                 dwFlagsAndAttributes,
    HANDLE                hTemplateFile
);
BOOL CloseHandle(HANDLE hObject);
HANDLE CreateFileMappingW(
    HANDLE                hFile,
    LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    DWORD                 flProtect,
    DWORD                 dwMaximumSizeHigh,
    DWORD                 dwMaximumSizeLow,
    LPCWSTR               lpName
);
LPVOID MapViewOfFile(
    HANDLE hFileMappingObject,
    DWORD  dwDesiredAccess,
    DWORD  dwFileOffsetHigh,
    DWORD  dwFileOffsetLow,
    SIZE_T dwNumberOfBytesToMap
);
BOOL UnmapViewOfFile(LPCVOID lpBaseAddress);
BOOL GetFileSizeEx(HANDLE hFile, PLARGE_INTEGER lpFileSize);
HRSRC FindResourceW(HMODULE hModule, LPCWSTR lpName, LPCWSTR lpType);
HGLOBAL LoadResource(HMODULE hModule, HRSRC hResInfo);
LPVOID LockResource(HGLOBAL hResData);
DWORD SizeofResource(HMODULE hModule, HRSRC hResInfo);
LPVOID VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
BOOL VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);
BOOL ReadFile(
    HANDLE       hFile,
    LPVOID       lpBuffer,
    DWORD        nNumberOfBytesToRead,
    LPDWORD      lpNumberOfBytesRead,
    LPOVERLAPPED lpOverlapped
);
BOOL GetOverlappedResult(
    HANDLE       hFile,
    LPOVERLAPPED lpOverlapped,
    LPDWORD      lpNumberOfBytesTransferred,
    BOOL         bWait
);
BOOL WINAPI CancelIoEx(HANDLE hFile, LPOVERLAPPED lpOverlapped);
HANDLE CreateEventW(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL                  bManualReset,
    BOOL                  bInitialState,
    LPCWSTR               lpName
);
BOOL SetEvent(HANDLE hEvent);
BOOL ResetEvent(HANDLE hEvent);
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds);
HANDLE CreateThread(
    LPSECURITY_ATTRIBUTES  lpThreadAttributes,
    SIZE_T                 dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID                 lpParameter,
    DWORD                  dwCreationFlags,
    LPDWORD                lpThreadId
);
void GetSystemInfo(LPSYSTEM_INFO lpSystemInfo);
LPVOID HeapAlloc(HANDLE hHeap, DWORD dwFlags, SIZE_T dwBytes);
LPVOID HeapReAlloc(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes);
BOOL HeapFree(HANDLE hHeap, DWORD dwFlags, LPVOID lpMem);
HANDLE GetProcessHeap(void);

/* user32.dll */
int MessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);
HICON LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName);
HCURSOR LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName);
ATOM RegisterClassExW(const WNDCLASSEXW *lpWndClass);
BOOL UnregisterClassW(LPCWSTR lpClassName, HINSTANCE hInstance);
HWND CreateWindowExW(
    DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
);
BOOL DestroyWindow(HWND hWnd);
BOOL ShowWindow(HWND hWnd, int nCmdShow);
LRESULT DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
void WINAPI PostQuitMessage(int nExitCode);
BOOL GetMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
BOOL PeekMessageW(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
BOOL TranslateMessage(const MSG *lpMsg);
LRESULT DispatchMessageW(const MSG *lpMsg);
LRESULT WINAPI SendMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL WINAPI PostMessageW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL IsWindowVisible(HWND hWnd);
HMENU LoadMenuW(HINSTANCE hInstance, LPCWSTR lpMenuName);
BOOL ModifyMenuW(HMENU hMnu, UINT uPosition, UINT uFlags, UINT_PTR uIDNewItem, LPCWSTR lpNewItem);
HDC GetDC(HWND hWnd);
BOOL MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, BOOL bRepaint);
BOOL GetClientRect(HWND hWnd, LPRECT lpRect);
BOOL UpdateWindow(HWND hWnd);
BOOL InvalidateRect(HWND hWnd, const RECT *lpRect, BOOL bErase);
UINT_PTR SetTimer(HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc);
BOOL KillTimer(HWND hWnd, UINT_PTR uIDEvent);
DWORD MsgWaitForMultipleObjects(
    DWORD nCount,
    const HANDLE *pHandles,
    BOOL fWaitAll,
    DWORD dwMilliseconds,
    DWORD dwWakeMask
);
int GetSystemMetrics(int nIndex);
int SetScrollInfo(HWND hwnd, int nBar, LPCSCROLLINFO lpsi, BOOL redraw);
BOOL GetScrollInfo(HWND hwnd, int nBar, LPSCROLLINFO lpsi);

/* shell32.dll */
LPWSTR *CommandLineToArgvW(LPCWSTR lpCmdLine, int *pNumArgs);

/* shlwapi.dll */
int wnsprintfW(PWSTR pszDest, int cchDest, PCWSTR pszFmt, ...);
int wvnsprintfW(PWSTR pszDest, int cchDest, PCWSTR pszFmt, va_list arglist);
LPCSTR PathFindFileNameA(LPCSTR pszPath);
LPCWSTR PathFindFileNameW(LPCWSTR pszPath);

/* comctl32.dll */
void InitCommonControls(void);

/* gdi32.dll */
HGDIOBJ GetStockObject(int i);
int WINAPI ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR *ppfd);
int WINAPI DescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
BOOL WINAPI SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR * ppfd);

/* opengl32.dll */
HGLRC WINAPI wglCreateContext(HDC hdc);
BOOL WINAPI wglDeleteContext(HGLRC hglrc);
PROC WINAPI wglGetProcAddress(LPCSTR lpName);
BOOL WINAPI wglMakeCurrent(HDC hdc, HGLRC hglrc);
BOOL WINAPI SwapBuffers(HDC hdc);

/* comdlg32.dll */
BOOL GetSaveFileNameW(LPOPENFILENAMEW ofn);

/* intrinsics */
#ifdef _MSC_VER
    #pragma function(memset, memcpy)
#endif
void *memset(void *dst, int c, size_t count);
void *memcpy(void *dst, const void *src, size_t count);

uint16_t byteswap_u16(uint16_t v);
uint32_t byteswap_u32(uint32_t v);

#endif /* _NOTEPAD_STD_H */
