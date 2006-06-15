#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#define INCL_BASE
#define INCL_GPI
#define INCL_WIN
#include <os2emx.h>
#include <X11/X.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "myXlibint.h"
#include "EBstruc.h"
#include "EBmessage.h"
#include "EBhook.h"

#define WM_POINTERENTER 0x041e
#define WM_POINTERLEAVE 0x041f

#define FID_BORDER                              0x8009
#define FID_OBJECT                              0x800A
#define PMXLIB_DEFAULT_ICON     0x2000

EXPENTRY XEvent *Daemon_createEvent(register _XQEvent **newq, Window w, int type);
XEvent *Daemon_copyEvent(register _XQEvent **newq, _XQEvent *origq);
void Daemon_doEvent(_XQEvent *newq, EB_Resource *event_masks, int mask);
EXPENTRY void Daemon_addEvent(_XQEvent *newq, EB_Resource *procres, Bool copy);
void Daemon_propagateEvent(_XQEvent *newq, int mask, BOOL weakMouseEvent);
void Daemon_recurseEvent(_XQEvent *origq, XEvent *orig, int mask, Window stop);
// void Daemon_FreeAllAtoms(Xlib_Pixmap *client);
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
// void Xlib_FreeAllAtoms(Xlib_Pixmap *client);
void Xlib_UnmonitorResource(XID *resource);
Atom Daemon_xinternatom(_Xconst char* atom_name, Bool only_if_exists);
void Daemon_purgegc(GC gc);
extern int auto_repeat;
int Xlib_State(int hint);
Bool _XQueryPointer(Window w, Window* root_return, Window* child_return,
                int* root_x_return, int* root_y_return, int* win_x_return, int* win_y_return,
                unsigned int* mask_return, PPOINTL pptl);
EXPENTRY Window Daemon_RootWindow();
EXPENTRY BOOL Daemon_register();
EXPENTRY void Daemon_xinitialized(HAB hab, HWND mainhwnd);
EXPENTRY void Daemon_shutdown(HAB hab);
EXPENTRY TID getTid();
EXPENTRY TID getPid();
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
EXPENTRY void Daemon_freeEventQueue(Display *display);
EXPENTRY Window removeWindow(HWND hwnd);
EXPENTRY Window getWindow(HWND hwnd, BOOL create, EB_Window *new);
EXPENTRY HWND getValidWindow(HWND child);
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
void closeHandles(HPIPE pipe, HEV event);
void hook_msg(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, ULONG time, POINTL ptl);
Window um_createWindow(UM_CreateWindow *args);
Bool um_setWindowPos(UM_SetWindowPos *args);
void um_freeResources(EB_Resource *procres);
Bool um_destroyWindow(EB_Resource *windowres);
void um_openHandles(EB_Resource *procres);
int um_mapWindow(Window w);
int um_mapSubwindows(Window w);
int um_unmapWindow(Window w);
int um_lowerWindow(Window w);
int um_raiseWindow(Window w);
int um_mapRaised(Window w);
HPOINTER um_createPointer(UM_CreatePointer *args);

HAB pmctls_hab;
HMQ hmq;
HWND mainhwnd; /* Object instance window handle */
int min_keycode, max_keycode;
FILE *logfile;
HPIPE nextpipe;
PFNWP pfnFrame;
