// for ProcWindow
#define UM_CHANGEPOINTER (WM_USER+0)

// for ProcMessage
#define UM_CREATEWINDOW (WM_USER+0)
#define UM_SETWINDOWPOS (WM_USER+1)
#define UM_DESTROYWINDOW (WM_USER+2)
#define UM_CREATEGC (WM_USER+3)
#define UM_CHANGEPROPERTY (WM_USER+4)
#define UM_GETWINPROPERTY (WM_USER+5)
#define UM_REPARENTWINDOW (WM_USER+6)
#define UM_PROPAGATE (WM_USER+7)
#define UM_LOADPOINTER (WM_USER+8)
#define UM_FREERESOURCES (WM_USER+9)
#define UM_OPENPIPE (WM_USER+0xA)
#define UM_REPARENT (WM_USER+0xB)
#define UM_DELTWINDOWPOS (WM_USER+0xC)

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
