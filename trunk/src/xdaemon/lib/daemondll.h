#include <stddef.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#define INCL_BASE
#define INCL_WIN
#define INCL_GPI
#include <os2emx.h>
#include <os2thunk.h>
#include <X11/X.h>
#include "myXlibint.h"
#include <X11/Xutil.h>
#include <X11/Xmu/Misc.h>
#include "EBhook.h"
#include "EBstruc.h"
#include "EBmessage.h"

typedef enum _xgrabtypelist {
    __GrabFree__, 
    GrabPointer,              
    GrabKeyboard,             
    GrabButton,               
    GrabKey,                  
    GrabServer,               
    GrabAny                   
} xgrabtypelist;              

typedef struct _xgrab {
    int type, special, pid;
    Window window;
    Bool owner_events;
    unsigned int event_mask, button, modifiers;
    int pointer_mode, keyboard_mode, keycode;
    Window confine_to;
    Cursor cursor;
    Time time;
    struct _xgrab *next;
} Xlib_Grab;

USHORT EXPENTRY _16_Dos16GetInfoSeg(PSEL pselGlobal, PSEL pselLocal);

BOOL InitializeSharedHeap();
void DestroySharedHeap();
BOOL startDaemon();
EXPENTRY TID getPid();
EXPENTRY void *getResource(EB_ResID type, XID res);
EXPENTRY void Daemon_getPMHandle(EB_Resource *procres, HAB *hab);
EXPENTRY int getDrawableHeight(Drawable d);
void setGCValues(EB_HPS *ebhps, XGCValues *newvalues, Drawable d, Bool force);

EXPENTRY void *smalloc(int size);
EXPENTRY void *scalloc(int elements, int size);
EXPENTRY void *srealloc(void *mem, int size);
EXPENTRY void sfree(void *mem);
EXPENTRY BOOL schck();
EXPENTRY int XGetWindowProperty(Display* display, Window w, Atom property,
	long long_offset, long long_length, Bool delete, Atom req_type, Atom*
	actual_type_return,	int* actual_format_return, unsigned long* nitems_return,
	unsigned long* bytes_after_return, unsigned char** prop_return);
EXPENTRY EB_Resource *createResource(EB_ResID type, void *structure);
EXPENTRY void addResource(EB_Resource **anchor, EB_Resource *new);
EXPENTRY void Daemon_destroyWindow(EB_Resource *winres);
EXPENTRY void freeResource(EB_Resource *res);
EXPENTRY void unChainResource(EB_Resource *res);
EXPENTRY void addResourceLast(EB_Resource **anchor, EB_Resource *new);
EXPENTRY Window getWindow(HWND hwnd, BOOL create, EB_Window *new);
EXPENTRY HWND getParent(HWND child);
EXPENTRY void getPosition(XRectangle *result, HWND child, HWND relativeto);

int daemonthread;
HMTX event_lock, hook_lock;
EB_Resource *processlist;
HWND objwindow;
Bool checkProcesses;
HWND realdesktop;
XGCValues stdgcvalues;
