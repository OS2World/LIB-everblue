/*****************************************************************************/
/******************                Resources                ******************/
/*****************************************************************************/

// Task Local Memory
typedef struct {
        HAB hab;
} TLM;

typedef struct {
	PID pid;
	TLM **tlm;
	int threadcount;
	Display *display;
	HPIPE pipeserver;
	struct EB_Resource *hpstodelete; // EBHPS2, other processes can't del. hps
	struct EB_Resource *hpsres; // EBHPS2 resource chain (for destruction when closing displ.)
	struct EB_Resource *res; // arbitrary resource chain (for destruction when closing displ.)
	struct EB_Resource *event_masks; // EBEVENTMASK2 chain
	struct {
		HMTX postmtx;
		HEV postsem;
		MRESULT postret;
	};
} EB_Process;

// we cache a HPS for each Drawable/Process combination
// it is stored in EB_Window and EB_Pixmap
typedef struct {
	XGCValues current;
	HPS hps;
	HDC hdc;		// only needed for Pixmaps
	HBITMAP hbm;	// only needed for Pixmaps
	int currentheight; // reuse old clipping region, when resizing window
} EB_HPS;

typedef struct UserData {
	struct UserData *next;
	Atom            property, type;
	int             format;
	unsigned char*  data;
	int             size;
} UserData;

typedef struct {
	HWND hwnd;
	Bool xpm;
	struct EB_Resource *hpscache; // EBHPS1
    int border_width;
    int class;
	Bool mapped;
    int bit_gravity;
    int win_gravity;
    int backing_store;
    unsigned long backing_planes;
    unsigned long backing_pixel;
    Bool save_under;
    long do_not_propagate_mask;
    Bool override_redirect;
	struct EB_Resource *background_pixmap;
	unsigned long background_pixel;
    struct EB_Resource *border_pixmap;
    unsigned long border_pixel;
	Cursor cursor;
	Bool delete_window_notify;
	char *wm_name, *wm_iconname, *wm_class, *wm_locale;
	XSizeHints *sizehints;
	XWMHints *hints;
	Window wm_client_leader;
	long event_mask;
	struct EB_Resource *event_masks; // EBEVENTMASK1 chain
	struct EB_Resource *next; // Window hash
	UserData *userdata;
} EB_Window;

typedef struct {
	struct EB_Resource *hpscache; // EBHPS1
	PBITMAPINFO pbmih;
	PBYTE data;
	EB_HPS *lastone;
} EB_Pixmap;

typedef struct {
	FATTRS fattrs;
	SIZEF  sizef;
	int    psmode;
	int    descender;
} EB_Font;

typedef struct {
	struct _XGC xgc;
	Pixmap pixmask;
} EB_GContext;

typedef struct {
	// TODO cursor definition, which can get other colors
	// Pixmaps source, mask; Colors foreground, background; int hotspots
	HPOINTER pointer;
} EB_Cursor;

typedef struct {
	int size;
	XRectangle *rectangles;
} EB_Rectangles;

// resource types
typedef enum {
	EBANCHOR, EBWINDOW, EBPIXMAP, EBFONT, EBGCONTEXT, EBCURSOR, EBHPS1, EBHPS2,
			EBPROCESS, EBEVENTMASK1, EBEVENTMASK2, EBRECTANGLES
} EB_ResID;

// resource wrapper
typedef struct EB_Resource {
	EB_ResID restype;
	union {
		int refcount;					// used by Pixmap, Font, Cursor
		long event_mask;        		// used by EBEVENTMASK1
		struct EB_Resource *procres;	// used by EBEVENTMASK2, EBHPS1
		EB_HPS *ebhps;					// used by EBHPS2
	};
	union {
		EB_Window *ebwindow;
		EB_Pixmap *ebpixmap;
		EB_Font *ebfont;
		EB_GContext *ebgcontext;
		EB_Cursor *ebcursor;
		EB_Process *ebprocess;
		EB_Rectangles *rectangles;
		struct EB_Resource *ebresource; // 2nd descriptor for EBHPS#, EBEVENTMASK#
		XEvent *xevent;
		void *structure;
	};
	struct EB_Resource *next;
	struct EB_Resource *prev;
} EB_Resource;

/*****************************************************************************/
/******************                  Fonts                  ******************/
/*****************************************************************************/

typedef struct {
        int pixels, points;
        int avgwidth;
        int hres, vres;
} EB_FontHints;

typedef struct {
	char *alias, *fontname;
} EB_FontAlias;
