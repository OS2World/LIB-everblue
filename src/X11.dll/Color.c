#include "X11.h"

Status XQueryColor(register Display *dpy, Colormap cmap, XColor *def) {
	DBUG_ENTER("XQueryColor")
	def->red = (def->pixel >> 8) & 0xff00;
	def->blue = (def->pixel << 8) & 0xff00;
	def->green = def->pixel & 0xff00;;
	def->flags = DoRed | DoGreen | DoBlue;
	DBUG_RETURN(1);
}

Status XQueryColors(register Display *dpy, Colormap cmap, XColor *defs, int ncolors) {
	DBUG_ENTER("XQueryColors")
	register int i;

	for (i = 0; i < ncolors; i++)
		XQueryColor(dpy, cmap, &defs[i]);
	DBUG_RETURN(1);
}

// no palette support by Everblue
Status XStoreColor(Display* display, Colormap colormap, XColor* screen_in) {
	DBUG_ENTER("XStoreColor")
	DBUG_RETURN(False);
}

int XStoreColors(Display* display, Colormap colormap, XColor* color, int ncolors) {
	DBUG_ENTER("XStoreColors")
	DBUG_RETURN(False);
}

// should return new colormap handles!!! TODO!!!
Colormap XCreateColormap(Display* dpy, Window w, Visual* visual, int alloc) {
	DBUG_ENTER("XCreateColormap")
	DBUG_RETURN(COLORMAP);
}

int XFreeColormap(Display* dpy, Colormap colormap) {
	DBUG_ENTER("XFreeColormap")
	DBUG_RETURN(1);
}

Colormap XCopyColormapAndFree(Display* dpy, Colormap colormap) {
	DBUG_ENTER("XCopyColormapAndFree")
	DBUG_RETURN(COLORMAP);
}

int XInstallColormap(Display* display, Colormap colormap) {
	DBUG_ENTER("XInstallColormap")
	DBUG_RETURN(True);
}

int XUninstallColormap(Display* display, Colormap colormap) {
	DBUG_ENTER("XUninstallColormap")
	DBUG_RETURN(True);
}

Colormap *XListInstalledColormaps(Display* display, Window w, int* num_return) {
	DBUG_ENTER("XListInstalledColormaps")
	Colormap *cm = Xmalloc(sizeof(Colormap *));
	*cm = COLORMAP;
	*num_return = 1;
	DBUG_RETURN(cm);
}

int XSetWindowColormap(Display* dpy, Window w, Colormap colormap) {
	DBUG_ENTER("XSetWindowColormap")
	DBUG_RETURN(1);
}

Status XAllocColor(Display *display, Colormap colormap, XColor *screen_in_out) {
	DBUG_ENTER("XAllocColor")
	screen_in_out->blue = (screen_in_out->blue & 0xFF00);
	screen_in_out->green = (screen_in_out->green & 0xFF00);
	screen_in_out->red =  (screen_in_out->red & 0xFF00);
	screen_in_out->pixel = screen_in_out->red * 256 + screen_in_out->green + (screen_in_out->blue >> 8);
	DBUG_RETURN(True);
}
