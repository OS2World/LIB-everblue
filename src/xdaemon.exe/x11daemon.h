#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define INCL_BASE
#define INCL_GPI
#define INCL_WIN
#include <os2emx.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "../Xlibint.h"
#include "../EBstruc.h"
#include "../EBmessage.h"
#include "../EBhook.h"

#define WM_POINTERENTER 0x041e
#define WM_POINTERLEAVE 0x041f

#define FID_BORDER				0x8009
#define FID_OBJECT				0x800A
#define PMXLIB_DEFAULT_ICON     0x2000

typedef enum {
        PMATM_NULL, PMATM_FONT, PMATM_BITMAP
}       PMAtmType;

typedef struct {
        PMAtmType type;
        union {
                struct Xlib_Font_st *font;
                struct Xlib_Pixmap_st *pixmap;
        } data;
}       PMAtoms;


typedef struct {
        USHORT  cb;
        GC currentGC;
        HDC hdc;
        HPS hps;
        PMAtoms *pmatoms;
        void *reserved;
        XWindowAttributes winattrib;
        XEvent  *lastexpose, *lastconfigure;
        Pixmap background_pixmap;       /* background or None or ParentRelative */
        unsigned long background_pixel; /* background pixel */
        Cursor cursor;
        Bool delete_window_notify;
        char *wm_name, *wm_iconname, *wm_class, *wm_locale;
        XSizeHints *sizehints;
        XWMHints *hints;
        Window wm_client_leader;
}       WinAttribData;

typedef struct Xlib_Pixmap_st {         /* First 5 elements same as WinAttribData */
        USHORT  cb;
        GC currentGC;
        HDC hdc;
        HPS hps;
        PMAtoms *pmatoms;
        struct Xlib_Pixmap_st **pixatoms;
        HBITMAP hbm;
        int width, height;
        Colormap colormap;
} Xlib_Pixmap;

typedef struct {
        XExtData *ext_data;     /* hook for extension to hang data */
        HPS gid;                /* Presentation space handle */
		int rgnheight;
        XGCValues values;       /* shadow structure of values */
// please don't change above that line! Otherwise incompatible to xlib.h
		int rgndeltax;
		Pixmap pixmask;
        WinAttribData *winattrib;
        Xlib_Pixmap *pixmap;
        int path;
        POINTL cp;
} Xlib_GC;


EXPENTRY XEvent *Daemon_createEvent(register EB_Resource **newq, Window w, int type);
XEvent *Daemon_copyEvent(register EB_Resource **newq, EB_Resource *origq);
void Daemon_doEvent(EB_Resource *newq, EB_Resource *event_masks, int mask);
void Daemon_propagateEvent(EB_Resource *newq, int mask, BOOL weakMouseEvent);
void Daemon_recurseEvent(EB_Resource *origq, XEvent *orig, int mask, Window stop);
void Daemon_FreeAllAtoms(Xlib_Pixmap *client);
void Daemon_set_data(HWND window, Atom dataname, UserData *data);
UserData *Xlib_get_data(HWND window, Atom dataname);
void Xlib_set_data(HWND window, Atom dataname, UserData *data);
int XDeleteProperty(Display* dpy, Window window, Atom atom);
EXPENTRY void *smalloc(int size);
EXPENTRY void *srealloc(void *mem, int size);
EXPENTRY void *scalloc(int elements, int size);
EXPENTRY void sfree(void *mem);
EXPENTRY XWindowAttributes *Daemon_getwinattrib(Drawable w, HPS *hps);
char *Daemon_getatomname(Atom atom);
void Xlib_FreeAllAtoms(Xlib_Pixmap *client);
void Xlib_UnmonitorResource(XID *resource);
Atom Daemon_xinternatom(_Xconst char* atom_name, Bool only_if_exists);
void Daemon_purgegc(GC gc);
int autorepeat;
int Xlib_State(int hint);
Bool _XQueryPointer(Window w, Window* root_return, Window* child_return, 
		int* root_x_return, int* root_y_return, int* win_x_return, int* win_y_return,
		unsigned int* mask_return, BOOL msgpointer);
EXPENTRY BOOL Daemon_register();
EXPENTRY void Daemon_xinitialized(HAB hab, HWND mainhwnd);
EXPENTRY void Daemon_shutdown(HAB hab);
EXPENTRY void monitorResource(EB_Resource *res);
EXPENTRY void *getResource(EB_ResID type, XID res);
EXPENTRY EB_Resource *createResource(EB_ResID type, void *structure);
EXPENTRY void addResource(EB_Resource **anchor, EB_Resource *new);
EXPENTRY void unMonitorResource(EB_Resource *res);
EXPENTRY void freeResource(EB_Resource *res);
EXPENTRY void Daemon_doTasks(void freeMethod(EB_Resource *));
EXPENTRY void unChainResource(EB_Resource *res);
EXPENTRY void Daemon_addEventMask(Window w, EB_Resource *procres, long mask);
EXPENTRY void Daemon_freeEventMask(EB_Resource **chain);
EXPENTRY void Daemon_freeEventQueue(EB_Resource **chain);
EXPENTRY Window removeWindow(HWND hwnd);
EXPENTRY Window getWindow(HWND hwnd, BOOL create, EB_Window *new);
EXPENTRY void getPosition(XRectangle *result, HWND child, HWND relativeto);
EXPENTRY HWND getParent(HWND child);

MRESULT EXPENTRY xpmwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY brdrwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY pmhwndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY framewndproc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID APIENTRY Xlib_ExitHandler(ULONG ulTerm);
int Xlib_PMWM_Handler0(HWND* hWnd, ULONG* msg, MPARAM* mp1, MPARAM* mp2);
int Xlib_PMWM_Handler1(HWND* hWnd, ULONG* msg, MPARAM* mp1, MPARAM* mp2);
void initializePM();
void shutDownPM();
void closePipe(HPIPE handle);
void xevent_thread(void *args);
Window um_createWindow(UM_CreateWindow *args);
Bool um_setWindowPos(UM_SetWindowPos *args);
void um_freeResources(EB_Resource *procres);
Bool um_destroyWindow(EB_Resource *windowres);
void um_openPipe(EB_Resource *procres);

HAB pmctls_hab;
HMQ hmq;
HWND mainhwnd; /* Object instance window handle */
int min_keycode, max_keycode;
FILE *logfile;
HPIPE nextpipe;
PFNWP pfnFrame;
