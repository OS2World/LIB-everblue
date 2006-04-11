/* $Xorg: Xlibint.h,v 1.5 2001/02/09 02:03:38 xorgcvs Exp $ */

/*

Copyright 1984, 1985, 1987, 1989, 1998  The Open Group

Permission to use, copy, modify, distribute, and sell this software and its
documentation for any purpose is hereby granted without fee, provided that
the above copyright notice appear in all copies and that both that
copyright notice and this permission notice appear in supporting
documentation.

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of The Open Group shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from The Open Group.

*/
/* $XFree86: xc/lib/X11/Xlibint.h,v 3.27 2003/05/27 22:26:26 tsi Exp $ */

#ifndef _XLIBINT_H_
#define _XLIBINT_H_ 1

/*
 *	Xlibint.h - Header definition and support file for the internal
 *	support routines used by the C subroutine interface
 *	library (Xlib) to the X Window System.
 *
 *	Warning, there be dragons here....
 */

#include <X11/Xlib.h>

typedef HMTX _LockInfoRec;
typedef HMTX *LockInfoPtr;

struct _XGC
{
    XExtData *ext_data;	/* hook for extension to hang data */
    GContext gid;	/* protocol ID for graphics context */
    Bool rects;		/* boolean: TRUE if clipmask is list of rectangles */
    Bool dashes;	/* boolean: TRUE if dash-list is really a list */
    unsigned long dirty;/* cache dirty bits */
    XGCValues values;	/* shadow structure of values */
};

struct _XDisplay
{
	XExtData *ext_data;	/* hook for extension to hang data */
	struct _XFreeFuncs *free_funcs; /* internal free functions */
	int fd;			/* Network socket. */
	int conn_checker;         /* ugly thing used by _XEventsQueued */
	int proto_major_version;/* maj. version of server's X protocol */
	int proto_minor_version;/* minor version of server's X protocol */
	char *vendor;		/* vendor of the server hardware */
    int notsupported11;	/* resource ID base */
	int notsupported12;	/* resource ID mask bits */
	int notsupported13;	/* allocator current ID */
	int notsupported14;	/* allocator shift to correct bits */
	void (*notsupported15)();
	int byte_order;		/* screen byte order, LSBFirst, MSBFirst */
	int bitmap_unit;	/* padding and data requirements */
	int bitmap_pad;		/* padding requirements on bitmaps */
	int bitmap_bit_order;	/* LeastSignificant or MostSignificant */
	int nformats;		/* number of pixmap formats in list */
	ScreenFormat *pixmap_format;	/* pixmap format list */
	int vnumber;		/* Xlib's X protocol version number. */
	int release;		/* release of the server */
	struct _XSQEvent *head, *tail;	/* Input event queue. */
	int qlen;		/* Length of input event queue */
	unsigned long last_request_read; /* seq number of last event read */
	unsigned long request;	/* sequence number of last request. */
	void *notsupported19;		/* beginning of last request, or dummy */
	void *notsupported8;		/* Output buffer starting address. */
	void *notsupported9;		/* Output buffer index pointer. */
	void *notsupported7;		/* Output buffer maximum+1 address. */
	int notsupported10; /* maximum number 32 bit words in request*/
	struct _XrmHashBucketRec *db;
	int (*synchandler)(	/* Synchronization handler */
		struct _XDisplay*
		);
	char *display_name;	/* "host:display" string used on this connect*/
	int default_screen;	/* default screen for operations */
	int nscreens;		/* number of screens on this server*/
	Screen *screens;	/* pointer to list of screens */
	unsigned long motion_buffer;	/* size of motion buffer */
	unsigned long flags;	   /* internal connection flags */
	int min_keycode;	/* minimum defined keycode */
	int max_keycode;	/* maximum defined keycode */
	KeySym *keysyms;	/* This server's keysyms */
	XModifierKeymap *modifiermap;	/* This server's modifier keymap */
	int keysyms_per_keycode;/* number of rows */
	char *xdefaults;	/* contents of defaults from server */
	char *scratch_buffer;	/* place to hang scratch buffer */
	unsigned long scratch_length;	/* length of scratch buffer */
	int ext_number;		/* extension number on this display */
	struct _XExten *ext_procs; /* extensions initialized on this display */
	/*
	 * the following can be fixed size, as the protocol defines how
	 * much address space is available. 
	 * While this could be done using the extension vector, there
	 * may be MANY events processed, so a search through the extension
	 * list to find the right procedure for each event might be
	 * expensive if many extensions are being used.
	 */
	void (*notsupported1[128])();
	void (*notsupported2[128])();
	KeySym lock_meaning;	   /* for XLookupString */
	_LockInfoRec lock;   /* multi-thread state, display lock */
	void *notsupported16; /* for internal async */
	unsigned long notsupported18; /* max size of big requests */
	struct _XLockPtrs *lock_fns; /* pointers to threads functions */
	void (*notsupported17)();
	/* things above this line should not move, for binary compatibility */
	struct _XKeytrans *key_bindings; /* for XLookupString */
	Font cursor_font;	   /* for XCreateFontCursor */
	struct _XDisplayAtoms *atoms; /* for XInternAtom */
	unsigned int mode_switch;  /* keyboard group modifiers */
	unsigned int num_lock;  /* keyboard numlock modifiers */
	struct _XContextDB *context_db; /* context database */
	void (*notsupported3)();
	/*
	 * Xcms information
	 */
	struct {
	   XPointer defaultCCCs;  /* pointer to an array of default XcmsCCC */
	   XPointer clientCmaps;  /* pointer to linked list of XcmsCmapRec */
	   XPointer perVisualIntensityMaps;
				  /* linked list of XcmsIntensityMap */
	} cms;
	struct _XIMFilter *im_filters;
	struct _XSQEvent *qfree; /* unallocated event queue elements */
	unsigned long next_event_serial_num; /* inserted into next queue elt */
	struct _XExten *flushes; /* Flush hooks */
	struct _XConnectionInfo *im_fd_info; /* _XRegisterInternalConnection */
	int im_fd_length;	/* number of im_fd_info */
	struct _XConnWatchInfo *conn_watchers; /* XAddConnectionWatch */
	int watcher_count;	/* number of conn_watchers */
	XPointer filedes;	/* struct pollfd cache for _XWaitForReadable */
	void (*notsupported6)();
	XID notsupported20;	/* allocator max ID */
	int notsupported21;	/* major opcode for XC-MISC */
	struct _XkbInfoRec *notsupported22; /* XKB info */
	void *notsupported5; /* transport connection object */
};

#define XAllocIDs(dpy,ids,n) (*(dpy)->idlist_alloc)(dpy,ids,n)

#ifndef _XEVENT_
/*
 * _QEvent datatype for use in input queueing.
 */
typedef struct _XSQEvent
{
    struct _XSQEvent *next;
    XEvent event;
    unsigned long qserial_num;	/* so multi-threaded code can find new ones */
} _XQEvent;
#endif

#include <errno.h>
#define _XBCOPYFUNC _Xbcopy
#include <X11/Xfuncs.h>
#include <X11/Xosdefs.h>

#include <stdlib.h>
#include <string.h>

#include <X11/Xfuncproto.h>

_XFUNCPROTOBEGIN

/*
 * The following definitions can be used for locking requests in multi-threaded
 * address spaces.
 */
/* Author: Stephen Gildea, MIT X Consortium
 *
 * declarations for C Threads locking
 */

/* interfaces for locking.c */
struct _XLockPtrs {
    /* used by all, including extensions; do not move */
    void (*lock_display)(
		Display *dpy
#if defined(XTHREADS_WARN) || defined(XTHREADS_FILE_LINE)
		, char *file
		, int line
#endif
	);
    void (*unlock_display)(
		Display *dpy
#if defined(XTHREADS_WARN) || defined(XTHREADS_FILE_LINE)
		, char *file
		, int line
#endif
	);
};

/* in XlibInt.c */
extern void (*_XCreateMutex_fn)(
    LockInfoPtr /* lock */
);
extern void (*_XFreeMutex_fn)(
    LockInfoPtr /* lock */
);
extern void (*_XLockMutex_fn)(
    LockInfoPtr	/* lock */
#if defined(XTHREADS_WARN) || defined(XTHREADS_FILE_LINE)
    , char * /* file */
    , int /* line */
#endif
);
extern void (*_XUnlockMutex_fn)(
    LockInfoPtr	/* lock */
#if defined(XTHREADS_WARN) || defined(XTHREADS_FILE_LINE)
    , char * /* file */
    , int /* line */
#endif
);

extern LockInfoPtr _Xglobal_lock;

#if defined(XTHREADS_WARN) || defined(XTHREADS_FILE_LINE)
#define LockDisplay(d)	     if ((d)->lock_fns) (*(d)->lock_fns->lock_display)((d),__FILE__,__LINE__)
#define UnlockDisplay(d)     if ((d)->lock_fns) (*(d)->lock_fns->unlock_display)((d),__FILE__,__LINE__)
#define _XLockMutex(lock)		if (_XLockMutex_fn) (*_XLockMutex_fn)(lock,__FILE__,__LINE__)
#define _XUnlockMutex(lock)	if (_XUnlockMutex_fn) (*_XUnlockMutex_fn)(lock,__FILE__,__LINE__)
#else
/* used everywhere, so must be fast if not using threads */
#define LockDisplay(d)	     if ((d)->lock_fns) (*(d)->lock_fns->lock_display)(d)
#define UnlockDisplay(d)     if ((d)->lock_fns) (*(d)->lock_fns->unlock_display)(d)
#define _XLockMutex(lock)		if (_XLockMutex_fn) (*_XLockMutex_fn)(lock)
#define _XUnlockMutex(lock)	if (_XUnlockMutex_fn) (*_XUnlockMutex_fn)(lock)
#endif
#define _XCreateMutex(lock)	if (_XCreateMutex_fn) (*_XCreateMutex_fn)(lock);
#define _XFreeMutex(lock)	if (_XFreeMutex_fn) (*_XFreeMutex_fn)(lock);

_System void *smalloc(int size);
_System void *srealloc(void *mem, int size);
_System void *scalloc(int elements, int size);
_System void sfree(void *mem);

#define Xfree(ptr) sfree((ptr))

#define Xmalloc(size) smalloc(size)
#define Xrealloc(ptr, size) srealloc(ptr, size)
#define Xcalloc(nelem, elsize) scalloc(nelem, elsize)

#include <stddef.h>

#define LOCKED 1
#define UNLOCKED 0

#ifndef BUFSIZE
#define BUFSIZE 2048			/* X output buffer size. */
#endif
#ifndef PTSPERBATCH
#define PTSPERBATCH 1024		/* point batching */
#endif
#ifndef WLNSPERBATCH
#define WLNSPERBATCH 50			/* wide line batching */
#endif
#ifndef ZLNSPERBATCH
#define ZLNSPERBATCH 1024		/* thin line batching */
#endif
#ifndef WRCTSPERBATCH
#define WRCTSPERBATCH 10		/* wide line rectangle batching */
#endif
#ifndef ZRCTSPERBATCH
#define ZRCTSPERBATCH 256		/* thin line rectangle batching */
#endif
#ifndef FRCTSPERBATCH
#define FRCTSPERBATCH 256		/* filled rectangle batching */
#endif
#ifndef FARCSPERBATCH
#define FARCSPERBATCH 256		/* filled arc batching */
#endif
#ifndef CURSORFONT
#define CURSORFONT "cursor"		/* standard cursor fonts */
#endif

/*
 * Display flags
 */
#define XlibDisplayIOError	(1L << 0)
#define XlibDisplayClosing	(1L << 1)
#define XlibDisplayNoXkb	(1L << 2)
#define XlibDisplayPrivSync	(1L << 3)
#define XlibDisplayProcConni	(1L << 4) /* in _XProcessInternalConnection */
#define XlibDisplayReadEvents	(1L << 5) /* in _XReadEvents */
#define XlibDisplayReply	(1L << 5) /* in _XReply */
#define XlibDisplayWriting	(1L << 6) /* in _XFlushInt, _XSend */
#define XlibDisplayDfltRMDB     (1L << 7) /* mark if RM db from XGetDefault */

#define SyncHandle() \
	if (dpy->synchandler) (*dpy->synchandler)(dpy)

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

#define CI_NONEXISTCHAR(cs) (((cs)->width == 0) && \
			     (((cs)->rbearing|(cs)->lbearing| \
			       (cs)->ascent|(cs)->descent) == 0))

/* 
 * CI_GET_CHAR_INFO_1D - return the charinfo struct for the indicated 8bit
 * character.  If the character is in the column and exists, then return the
 * appropriate metrics (note that fonts with common per-character metrics will
 * return min_bounds).  If none of these hold true, try again with the default
 * char.
 */
#define CI_GET_CHAR_INFO_1D(fs,col,def,cs) \
{ \
    cs = def; \
    if (col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[(col - fs->min_char_or_byte2)]; \
	    if (CI_NONEXISTCHAR(cs)) cs = def; \
	} \
    } \
}

#define CI_GET_DEFAULT_INFO_1D(fs,cs) \
  CI_GET_CHAR_INFO_1D (fs, fs->default_char, NULL, cs)



/*
 * CI_GET_CHAR_INFO_2D - return the charinfo struct for the indicated row and 
 * column.  This is used for fonts that have more than row zero.
 */
#define CI_GET_CHAR_INFO_2D(fs,row,col,def,cs) \
{ \
    cs = def; \
    if (row >= fs->min_byte1 && row <= fs->max_byte1 && \
	col >= fs->min_char_or_byte2 && col <= fs->max_char_or_byte2) { \
	if (fs->per_char == NULL) { \
	    cs = &fs->min_bounds; \
	} else { \
	    cs = &fs->per_char[((row - fs->min_byte1) * \
			        (fs->max_char_or_byte2 - \
				 fs->min_char_or_byte2 + 1)) + \
			       (col - fs->min_char_or_byte2)]; \
	    if (CI_NONEXISTCHAR(cs)) cs = def; \
        } \
    } \
}

#define CI_GET_DEFAULT_INFO_2D(fs,cs) \
{ \
    unsigned int r = (fs->default_char >> 8); \
    unsigned int c = (fs->default_char & 0xff); \
    CI_GET_CHAR_INFO_2D (fs, r, c, NULL, cs); \
}


typedef void (*FreeFuncType) (
    Display*	/* display */
);

typedef int (*FreeModmapType) (
    XModifierKeymap*	/* modmap */
);

/*
 * This structure is private to the library.
 */
typedef struct _XFreeFuncs {
    FreeFuncType atoms;		/* _XFreeAtomTable */
    FreeModmapType modifiermap;	/* XFreeModifierMap */
    FreeFuncType key_bindings;	/* _XFreeKeyBindings */
    FreeFuncType context_db;	/* _XFreeContextDB */
    FreeFuncType defaultCCCs;	/* _XcmsFreeDefaultCCCs */
    FreeFuncType clientCmaps;	/* _XcmsFreeClientCmaps */
    FreeFuncType intensityMaps;	/* _XcmsFreeIntensityMaps */
    FreeFuncType im_filters;	/* _XFreeIMFilters */
    FreeFuncType notsupported23;		/* _XkbFreeInfo */
} _XFreeFuncRec;

/* types for InitExt.c */
typedef int (*CreateGCType) (
    Display*	/* display */,
    GC		/* gc */,
    XExtCodes*	/* codes */
);

typedef int (*CopyGCType)(
    Display*	/* display */,
    GC		/* gc */,
    XExtCodes*	/* codes */
);

typedef int (*FlushGCType) (
    Display*	/* display */,
    GC		/* gc */,
    XExtCodes*	/* codes */
);

typedef int (*FreeGCType) (
    Display*	/* display */,
    GC		/* gc */,
    XExtCodes*	/* codes */
);

typedef int (*CreateFontType) (
    Display*	/* display */,
    XFontStruct* /* fs */,
    XExtCodes*	/* codes */
);

typedef int (*FreeFontType) (
    Display*	/* display */,
    XFontStruct* /* fs */,
    XExtCodes*	/* codes */
);

typedef int (*CloseDisplayType) (
    Display*	/* display */,
    XExtCodes*	/* codes */
);

typedef char* (*ErrorStringType) (
    Display*	/* display */,
    int		/* code */,
    XExtCodes*	/* codes */,
    char*	/* buffer */,
    int		/* nbytes */
);

typedef void (*PrintErrorType)(
    Display*	/* display */,
    XErrorEvent* /* ev */,
    void*	/* fp */
);

typedef void (*BeforeFlushType)(
    Display*	/* display */,
    XExtCodes*	/* codes */,
    _Xconst char* /* data */,
    long	/* len */
);

/*
 * This structure is private to the library.
 */
typedef struct _XExten {		/* private to extension mechanism */
	struct _XExten *next;		/* next in list */
	XExtCodes codes;		/* public information, all extension told */
	CreateGCType create_GC;		/* routine to call when GC created */
	CopyGCType copy_GC;		/* routine to call when GC copied */
	FlushGCType flush_GC;		/* routine to call when GC flushed */
	FreeGCType free_GC;		/* routine to call when GC freed */
	CreateFontType create_Font;	/* routine to call when Font created */
	FreeFontType free_Font;		/* routine to call when Font freed */
	CloseDisplayType close_display;	/* routine to call when connection closed */
	void *notsupported;		/* who to call when an error occurs */
	ErrorStringType error_string;	/* routine to supply error string */
	char *name;			/* name of this extension */
	PrintErrorType error_values;	/* routine to supply error values */
	BeforeFlushType before_flush;	/* routine to call when sending data */
	struct _XExten *next_flush;	/* next in list of those with flushes */
} _XExtension;

/* extension hooks */

#ifdef DataRoutineIsProcedure
extern void Data(Display *dpy, char *data, long len);
#endif
extern int _XIOError(
    Display*	/* dpy */
);
extern int (*_XIOErrorFunction)(
    Display*	/* dpy */
);
extern int (*_XErrorFunction)(
    Display*		/* dpy */,
    XErrorEvent*	/* error_event */
);
extern void _XEatData(
    Display*		/* dpy */,
    unsigned long	/* n */
);
extern char *_XAllocScratch(
    Display*		/* dpy */,
    unsigned long	/* nbytes */
);
extern char *_XAllocTemp(
    Display*		/* dpy */,
    unsigned long	/* nbytes */
);
extern void _XFreeTemp(
    Display*		/* dpy */,
    char*		/* buf */,
    unsigned long	/* nbytes */
);
extern Visual *_XVIDtoVisual(
    Display*	/* dpy */,
    VisualID	/* id */
);
extern int _XGetHostname(
    char*	/* buf */,
    int		/* maxlen */
);
extern Screen *_XScreenOfWindow(
    Display*	/* dpy */,
    Window	/* w */
);
extern void _XFlush(
    Display*	/* dpy */
);
extern int _XEventsQueued(
    Display*	/* dpy */,
    int 	/* mode */
);
extern void _XReadEvents(
    Display*	/* dpy */
);
extern void _XDeq(
    Display*	/* dpy */,
    _XQEvent*	/* prev */,
    _XQEvent*	/* qelt */
);

extern XID _XAllocID(Display *dpy);
extern void _XAllocIDs(Display *dpy, XID *ids, int count);

extern int _XFreeExtData(
    XExtData*	/* extension */
);

extern int (*XESetCreateGC(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
	      GC			/* gc */,
	      XExtCodes*		/* codes */
	    )		/* proc */
))(
    Display*, GC, XExtCodes*
);

extern int (*XESetCopyGC(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	      
))(
    Display*, GC, XExtCodes*
);

extern int (*XESetFlushGC(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	     
))(
    Display*, GC, XExtCodes*
);

extern int (*XESetFreeGC(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              GC			/* gc */,
              XExtCodes*		/* codes */
            )		/* proc */	     
))(
    Display*, GC, XExtCodes*
);

extern int (*XESetCreateFont(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              XFontStruct*		/* fs */,
              XExtCodes*		/* codes */
            )		/* proc */    
))(
    Display*, XFontStruct*, XExtCodes*
);

extern int (*XESetFreeFont(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              XFontStruct*		/* fs */,
              XExtCodes*		/* codes */
            )		/* proc */    
))(
    Display*, XFontStruct*, XExtCodes*
); 

extern int (*XESetCloseDisplay(
    Display*		/* display */,
    int			/* extension */,
    int (*) (
	      Display*			/* display */,
              XExtCodes*		/* codes */
            )		/* proc */    
))(
    Display*, XExtCodes*
);

extern char* (*XESetErrorString(
    Display*		/* display */,
    int			/* extension */,
    char* (*) (
	        Display*		/* display */,
                int			/* code */,
                XExtCodes*		/* codes */,
                char*			/* buffer */,
                int			/* nbytes */
              )		/* proc */	       
))(
    Display*, int, XExtCodes*, char*, int
);

extern void (*XESetPrintErrorValues (
    Display*		/* display */,
    int			/* extension */,
    void (*)(
	      Display*			/* display */,
	      XErrorEvent*		/* ev */,
	      void*			/* fp */
	     )		/* proc */
))(
    Display*, XErrorEvent*, void*
);

extern void (*XESetBeforeFlush(
    Display*		/* display */,
    int			/* error_number */,
    void (*) (
	       Display*			/* display */,
	       XExtCodes*		/* codes */,
	       _Xconst char*		/* data */,
	       long			/* len */
            )		/* proc */   
))(
    Display*, XExtCodes*, _Xconst char*, long
);

struct _XConnWatchInfo {	/* info from XAddConnectionWatch */
    XConnectionWatchProc fn;
    XPointer client_data;
    struct _XConnWatchInfo *next;
};

extern char* __XOS2RedirRoot(
    char*
);

extern int _XTextHeight(
    XFontStruct*	/* font_struct */,
    _Xconst char*	/* string */,
    int			/* count */
);

extern int _XTextHeight16(
    XFontStruct*	/* font_struct */,
    _Xconst XChar2b*	/* string */,
    int			/* count */
);

#define _XOpenFile(path,flags) open(path,flags)
#define _XFopenFile(path,mode) fopen(path,mode)

extern int _XF86LoadQueryLocaleFont(
    Display*		/* dpy */,
    _Xconst char*	/* name*/,
    XFontStruct**	/* xfp*/,
    Font*		/* fidp */
);

extern int _XDefaultError(
        Display *dpy,
        XErrorEvent *event);

extern int _XDefaultIOError(
        Display *dpy);

extern void _XSetClipRectangles (
    register Display *dpy,
    GC gc,
    int clip_x_origin, int clip_y_origin,
    XRectangle *rectangles,
    int n,
    int ordering);

Status _XGetWindowAttributes(
    register Display *dpy,
    Window w,
    XWindowAttributes *attr);

int _XPutBackEvent (
    register Display *dpy, 
    register XEvent *event);

_XFUNCPROTOEND

#endif /* _XLIBINT_H_ */
