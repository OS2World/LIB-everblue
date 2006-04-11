#include "X11.h"

Status
XGetClassHint(dpy, w, classhint)
	Display *dpy;
	Window w;
	XClassHint *classhint;	/* RETURN */
{
    int len_name, len_class;

    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;
    unsigned char *data = NULL;
    if (XGetWindowProperty(dpy, w, XA_WM_CLASS, 0L, (long)BUFSIZ, False,
        XA_STRING, 
	&actual_type,
	&actual_format, &nitems, &leftover, &data) != Success)
           return (0);
	
   if ( (actual_type == XA_STRING) && (actual_format == 8) ) {
	len_name = strlen((char *) data);
	if (! (classhint->res_name = Xmalloc((unsigned) (len_name+1)))) {
	    Xfree((char *) data);
	    return (0);
	}
	strcpy(classhint->res_name, (char *) data);
	if (len_name == nitems) len_name--;
	len_class = strlen((char *) (data+len_name+1));
	if (! (classhint->res_class = Xmalloc((unsigned) (len_class+1)))) {
	    Xfree(classhint->res_name);
	    classhint->res_name = (char *) NULL;
	    Xfree((char *) data);
	    return (0);
	}
	strcpy(classhint->res_class, (char *) (data+len_name+1));
	Xfree( (char *) data);
	return(1);
	}
    if (data) Xfree( (char *) data);
    return(0);
}

Status XGetCommand (dpy, w, argvp, argcp)
    Display *dpy;
    Window w;
    char ***argvp;
    int *argcp;
{
    XTextProperty tp;
    int argc;
    char **argv;

    if (!XGetTextProperty (dpy, w, &tp, XA_WM_COMMAND)) return 0;

    if (tp.encoding != XA_STRING || tp.format != 8) {
	if (tp.value) Xfree ((char *) tp.value);
	return 0;
    }


    /*
     * ignore final <NUL> if present since UNIX WM_COMMAND is nul-terminated
     */
    if (tp.nitems && (tp.value[tp.nitems - 1] == '\0')) tp.nitems--;


    /*
     * create a string list and return if successful
     */
    if (!XTextPropertyToStringList (&tp, &argv, &argc)) {
	if (tp.value) Xfree ((char *) tp.value);
	return (0);
    }

    if (tp.value) Xfree ((char *) tp.value);
    *argvp = argv;
    *argcp = argc;
    return 1;
}

Status
XGetNormalHints (dpy, w, hints)
	Display *dpy;
	Window w;
	XSizeHints *hints;
{
	return (XGetSizeHints(dpy, w, hints, XA_WM_NORMAL_HINTS));
}

Status
XGetTransientForHint(dpy, w, propWindow)
	Display *dpy;
	Window w;
	Window *propWindow;
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long leftover;
    Window *data = NULL;
    if (XGetWindowProperty(dpy, w, XA_WM_TRANSIENT_FOR, 0L, 1L, False,
        XA_WINDOW, 
	&actual_type,
	&actual_format, &nitems, &leftover, (unsigned char **) &data)
	!= Success) {
	*propWindow = None;
	return (0);
	}
    if ( (actual_type == XA_WINDOW) && (actual_format == 32) &&
	 (nitems != 0) ) {
	*propWindow = *data;
	Xfree( (char *) data);
	return (1);
	}
    *propWindow = None;
    if (data) Xfree( (char *) data);
    return(0);
}

Status XGetWMNormalHints (dpy, w, hints, supplied)
    Display *dpy;
    Window w;
    XSizeHints *hints;
    long *supplied;
{
    return (XGetWMSizeHints (dpy, w, hints, supplied, XA_WM_NORMAL_HINTS));
}

Status
XGetZoomHints (dpy, w, zhints)
	Display *dpy;
	Window w;
	XSizeHints *zhints;
{
	return (XGetSizeHints(dpy, w, zhints, XA_WM_ZOOM_HINTS));
}
