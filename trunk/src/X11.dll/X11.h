#define VENDOR_RELEASE 0
#define COLORMAP 1

#define I_NEED_OS2_H
#define INCL_WIN
#define INCL_DOS
#define INCL_GPI
#include <os2emx.h>
#include <io.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <os2thunk.h>
#include <uconv.h>
#include <fcntl.h>

#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>
#include <X11/Xresource.h>

#include "../myXlibint.h"
#include "../EBdebug.h"
#include "../EBstruc.h"
#include "../EBmessage.h"
#include "../EBhook.h"

#define GC_NOPATH               (0)
#define GC_STROKEPATH           (1)
#define GC_FILLPATH             (2)
#define GC_AREAFILL             (3)
#define GC_FLUSHPATH            (4)

BOOL APIENTRY GpiEnableYInversion(HPS hps, LONG lHeight);

Bool OsInitColors();
EXPENTRY Window Daemon_RootWindow();
EXPENTRY void *smalloc(int size);
EXPENTRY void *srealloc(void *mem, int size);
EXPENTRY void *scalloc(int elements, int size);
EXPENTRY void sfree(void *mem);
EXPENTRY BOOL schck();
EXPENTRY void Daemon_removeEvent(XEvent *ev);
EXPENTRY Status Daemon_SendEvent(Display *display, Window w, Bool propagate, 
                  long event_mask, XEvent *event_send, Bool send_event);
EXPENTRY Bool Daemon_getProcess(EB_Resource **procres);
EXPENTRY EB_Resource *createResource(EB_ResID type, void *structure);
EXPENTRY void *getResource(EB_ResID type, XID res);
EXPENTRY void freeResource(EB_Resource *res);
EXPENTRY void freeResources(EB_Resource *procres);
EXPENTRY void addResource(EB_Resource **anchor, EB_Resource *new);
EXPENTRY void monitorResource(EB_Resource *res);
EXPENTRY MRESULT Daemon_exec(EB_Resource *process, ULONG id, void *params);
EXPENTRY void Daemon_getPMHandle(EB_Resource *procres, HAB *hab);
EXPENTRY void Daemon_Thread_Close(EB_Resource **procres);
EXPENTRY void Daemon_died(EB_Resource *procres);
EXPENTRY int Daemon_addEventMask(Window w, EB_Resource *procres, long mask);
EXPENTRY int getDrawableHeight(Drawable d);
EXPENTRY void getDrawableGeometry(Drawable d, int *x, int *y, unsigned int *width,
		unsigned int *height, unsigned int *border_width, unsigned int *depth);
EXPENTRY EB_HPS *getCachedHPS(EB_Resource *procres, Drawable d, GC gc);
EXPENTRY TID getTid();
EXPENTRY void finishedDrawing(Drawable d, EB_HPS *ebhps);
EXPENTRY void freeOldHPS(EB_Resource **anchor);
EXPENTRY HWND getParent(HWND child);
EXPENTRY Window getWindow(HWND hwnd, BOOL create, EB_Window *newebw);
EXPENTRY EB_Resource *Daemon_findEventMask(EB_Resource *winchain, EB_Resource *procres);
EXPENTRY long Daemon_orEventMasks(EB_Resource *winchain);
EXPENTRY XEvent *Daemon_createEvent(register _XQEvent **newq, Window w, int type);

extern HWND realdesktop;
extern XGCValues stdgcvalues;
HMTX event_lock;

void Xlib_InitOS2I18N();
Display *newdisplay();
char *Xstrdup(const char *string);
void freeDisplay(Display *dpy);
void Xlib_InitAtoms(Display* dpy);
void Xlib_GetDrawableInfo(Display *display, Drawable d, GC gc, HPS *hps, int pathtype, long *width, long *height, long *depth);
int XNoOp(Display* display);
int PatternMatch(char *pat, int patdashes, char *string, int stringdashes);
EB_FontHints *Xlib_ParseFontHints(HPS hps, char *pattern, int plen);
int RenderFontName(FONTMETRICS *pfm, char *name, EB_FontHints *hint, EB_Font *xfont);
EB_Resource *createResource(EB_ResID type, void *structure);

EB_Resource *EventQueue;
int pmout[2];
EB_Resource *process;

EB_FontAlias FontAlias[37];
