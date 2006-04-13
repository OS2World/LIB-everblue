// for the global hook
#define UM_HOOKMSG (WM_USER+0)

// for ProcWindow
#define UM_CHANGEPOINTER (WM_USER+1)

#define UM_FROM_X11_DLL (WM_USER+2)

// for ProcMessage
#define UM_CREATEWINDOW (WM_USER+2)
#define UM_SETWINDOWPOS (WM_USER+3)
#define UM_DESTROYWINDOW (WM_USER+4)
#define UM_CREATEGC (WM_USER+5)
#define UM_CHANGEPROPERTY (WM_USER+6)
#define UM_GETWINPROPERTY (WM_USER+7)
#define UM_REPARENTWINDOW (WM_USER+8)
#define UM_PROPAGATE (WM_USER+9)
#define UM_LOADPOINTER (WM_USER+10)
#define UM_FREERESOURCES (WM_USER+11)
#define UM_OPENHANDLES (WM_USER+12)
#define UM_REPARENT (WM_USER+13)
#define UM_DELTWINDOWPOS (WM_USER+14)
#define UM_CREATEEVENT (WM_USER+15)
#define UM_MAPWINDOW (WM_USER+16)
#define UM_MAPSUBWINDOWS (WM_USER+17)
#define UM_UNMAPWINDOW (WM_USER+18)
#define UM_LOWERWINDOW (WM_USER+19)
#define UM_RAISEWINDOW (WM_USER+20)
#define UM_MAPRAISED (WM_USER+21)
#define UM_CREATEPOINTER (WM_USER+22)

typedef struct {
	Window parent;
	long event_mask;
	EB_Resource *process;
	int x;
	int y;
	int width;
	int height;
	EB_Window *newwind;
} UM_CreateWindow;

typedef struct {
	EB_Resource *procres;
	_XQEvent *event;
} UM_CreateEvent;

typedef struct {
	Window window;
	int x;
	int y;
	unsigned long width;
	unsigned long height;
} UM_SetWindowPos;

typedef struct {
        Display*        display;
        Window          w;
        Atom            property, type;
        int             format, mode;
        unsigned char*  data;
        int             nelements;
} UM_ChangeProperty;

typedef struct {
        Display*        display;
        Window          w;
        Atom            property;
        long            long_offset, long_length;
        Bool            delete;
        Atom            req_type, *actual_type_return;
        int*            actual_format_return;
        unsigned long   *nitems_return, *bytes_after_return;
        unsigned char** prop_return;
} UM_GetWinProperty;

typedef struct {
        Display*        display;
        Window          w, p;
        int             x, y;
} UM_ReparentWindow;

typedef struct {
	int x, y;
	Pixmap pointer;
	Pixmap color;
} UM_CreatePointer;
