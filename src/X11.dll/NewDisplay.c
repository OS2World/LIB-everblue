#include "X11.h"

ScreenFormat screenFormats[] = {
	{NULL, 24, 32, 32},
	{NULL, 16, 16, 16},
	{NULL, 8,  8, 8},
	{NULL, 1,  1, 8}
};

Display *newdisplay() {
	register Display *dpy;		/* New Display object being created. */
	register Visual *vp;
	register Screen *sp;
	register Depth *dp;
	Atom type;
	int format;
	long nitems, bytes;
	unsigned char *prop;
	EB_Resource *root;
    XGCValues values;
	LONG res[2];
	HPS hps;
	HDC hdc;

	hps = WinGetScreenPS(HWND_DESKTOP);
	hdc = GpiQueryDevice(hps);
	DevQueryCaps(hdc, CAPS_HORIZONTAL_RESOLUTION, 2, res);
	WinReleasePS(hps);
    values.foreground = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOWTEXT, 0);
   	values.background = WinQuerySysColor(HWND_DESKTOP, SYSCLR_WINDOW, 0);

	root = Daemon_RootWindow();
	dpy = (Display *)Xcalloc(1, sizeof(Display));
	vp = Xmalloc(sizeof(Visual));
	sp = Xmalloc(sizeof(Screen));
	dp = Xmalloc(sizeof(Depth));

    sp->display	    = dpy;
   	sp->root 	    = (Window)root;
    sp->cmap 	    = 1;
   	sp->white_pixel = values.background;
    sp->black_pixel = values.foreground;
   	sp->root_input_mask = NoEventMask;
    sp->width	    = WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN);
   	sp->height	    = WinQuerySysValue(HWND_DESKTOP, SV_CYSCREEN);
    sp->mwidth	    = (sp->width * 1000) / res[0];
   	sp->mheight	    = (sp->height * 1000) / res[1];
    sp->min_maps    = 1;
   	sp->max_maps    = 1;
    sp->backing_store= NotUseful;
   	sp->save_unders = False;
    sp->root_depth  = 32;
   	sp->ext_data   = NULL;
    sp->ndepths	    = 1;
    sp->depths = dp;
   	sp->root_visual = vp;
    sp->default_gc = XCreateGC(NULL, sp->root, GCForeground | GCBackground, &values);

	dp->depth = 32;
	dp->nvisuals = 1;
	dp->visuals = vp;

	vp->visualid = 1;
	vp->class = TrueColor;
	vp->bits_per_rgb = 8;
	vp->map_entries	= 16;
	vp->red_mask	= 0xff0000;
	vp->green_mask	= 0x00ff00;
	vp->blue_mask	= 0x0000ff;
	vp->ext_data	= NULL;

	dpy->ext_data		= (XExtData *)NULL;
	dpy->free_funcs = Xcalloc(1, sizeof(_XFreeFuncRec));
	dpy->conn_checker = 0; // TODO: needed?
	dpy->proto_major_version = X_PROTOCOL;
	dpy->proto_minor_version = X_PROTOCOL_REVISION;
	dpy->vendor = "Everblue by Netlabs";
	dpy->byte_order		= LSBFirst;
	dpy->bitmap_unit	= 8;
	dpy->bitmap_pad		= 8;
	dpy->bitmap_bit_order   = LSBFirst;
	dpy->nformats		= sizeof(screenFormats) / sizeof(ScreenFormat);
	dpy->pixmap_format = screenFormats;
	dpy->vnumber = X_PROTOCOL;
	dpy->release 		= VENDOR_RELEASE;
	/* Set up the input event queue and input event queue parameters. */
	dpy->head = dpy->tail = NULL;
	dpy->qlen = 0;
	dpy->last_request_read = 0;
	dpy->request = 0;
	dpy->db = (struct _XrmHashBucketRec *)NULL;
	dpy->display_name = XDisplayName(NULL);
	dpy->default_screen = 0;
	dpy->nscreens = 1;
	dpy->screens = sp;
	dpy->motion_buffer	= 0; // TODO? setup->motionBufferSize;
	dpy->flags		= 0;
	dpy->min_keycode	= 0; // TODO -> Xlib_Keyboard.c
	dpy->max_keycode	= 0; // TODO -> Xlib_Keyboard.c
	dpy->keysyms		= (KeySym *)NULL;
	dpy->modifiermap	= NULL;
	dpy->keysyms_per_keycode = 0;
	dpy->xdefaults		= (char *)NULL;
	dpy->scratch_buffer	= NULL;
	dpy->scratch_length	= 0L;
	dpy->ext_number 	= 0;
	dpy->ext_procs		= (_XExtension *)NULL;
	dpy->lock_meaning	= NoSymbol;
	dpy->lock		= NULL;
	dpy->lock_fns		= NULL;
	dpy->key_bindings	= NULL;
	dpy->cursor_font	= None;
	dpy->atoms		= NULL;
	dpy->mode_switch = 0; // TODO
	dpy->num_lock = 0; // TODO
	dpy->context_db		= NULL;
	dpy->cms.defaultCCCs	= NULL;
	dpy->cms.clientCmaps	= NULL;
	dpy->cms.perVisualIntensityMaps = NULL;
	dpy->im_filters		= NULL;
	dpy->qfree		= NULL;
	dpy->next_event_serial_num = 1;
	dpy->flushes		= NULL;
	dpy->im_fd_info		= NULL;
	dpy->im_fd_length	= 0;
	dpy->conn_watchers	= NULL;
	dpy->watcher_count	= 0;
	dpy->filedes		= NULL;
	dpy->xkb_info		= NULL;

	XGetWindowProperty(dpy, (Window)root, XA_RESOURCE_MANAGER, 0, 100000000,
			False, XA_STRING, &type, &format, &nitems, &bytes, &prop);
	if(nitems >= 0)
		dpy->xdefaults = prop;

#ifdef MOTIFBC
	{
	    extern Display *_XHeadOfDisplayList;
	    _XHeadOfDisplayList = dpy;
	}
#endif

 	return(dpy);
}

char *XDisplayName(_Xconst char* display) {
	return("everblue/localhost:0.0");
}

void freeDisplay(Display *dpy) {
	Xfree(dpy->screens->root_visual);
    Xfree(dpy->screens->depths);
	Xfree(dpy->screens);
	Xfree(dpy->free_funcs);
	Xfree(dpy);
}
