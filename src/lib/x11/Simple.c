#include "X11.h"

int XScreenNumberOfScreen(register Screen *scr) {
    DBUG_ENTER("XScreenNumberOfScreen")
    register Display *dpy = scr->display;
    register Screen *dpyscr = dpy->screens;
    register int i;

    for (i = 0; i < dpy->nscreens; i++, dpyscr++)
		if (scr == dpyscr)
			DBUG_RETURN(i);
    DBUG_RETURN(-1);
}

Display *XDisplayOfScreen(Screen* screen) {
	DBUG_ENTER("XDisplayOfScreen")
	Display *result = DisplayOfScreen(screen);
	DBUG_RETURN(result);
}

unsigned long XAllPlanes(void) {
	DBUG_ENTER("XAllPlanes")
	unsigned long result = AllPlanes;
	DBUG_RETURN(result);
}

unsigned long XBlackPixel(Display* display, int screen_number) {
	DBUG_ENTER("XBlackPixel")
	unsigned long black = BlackPixel(display, screen_number);
	DBUG_RETURN(black);
}

unsigned long XWhitePixel(Display* display, int screen_number) {
	DBUG_ENTER("XWhitePixel")
	unsigned long white = WhitePixel(display, screen_number);
	DBUG_RETURN(white);
}

int XConnectionNumber(Display* display) {
	DBUG_ENTER("XConnectionNumber")
	int result = ConnectionNumber(display);
	DBUG_RETURN(result);
}

Colormap XDefaultColormap(Display* display, int screen_number) {
	DBUG_ENTER("XDefaultColormap")
	Colormap result = DefaultColormap(display, screen_number);
	DBUG_RETURN(result);
}

int XDefaultDepth(Display* display, int screen_number) {
	DBUG_ENTER("XDefaultDepth")
	int result = DefaultDepth(display, screen_number);
	DBUG_RETURN(result);
}

GC XDefaultGC(Display* display, int screen_number) {
	DBUG_ENTER("XDefaultGC")
	GC gc = DefaultGC(display, screen_number);
	DBUG_RETURN(gc);
}

Window XRootWindow(Display* display, int screen_number) {
	DBUG_ENTER("XRootWindow")
	Window root = RootWindow(display, screen_number);
	DBUG_RETURN(root);
}

Screen *XScreenOfDisplay(Display* display, int screen_number) {
	DBUG_ENTER("XScreenOfDisplay")
	Screen *screen = ScreenOfDisplay(display, screen_number);
	DBUG_RETURN(screen);
}

int XDefaultScreen(Display* display) {
	DBUG_ENTER("XDefaultScreen")
	int result = DefaultScreen(display);
	DBUG_RETURN(result);
}

Colormap XDefaultColormapOfScreen(Screen* screen) {
	DBUG_ENTER("XDefaultColormapOfScreen")
	Colormap result = DefaultColormapOfScreen(screen);
	DBUG_RETURN(result);
}

int XDefaultDepthOfScreen(Screen* screen) {
	DBUG_ENTER("XDefaultDepthOfScreen")
	int result = DefaultDepthOfScreen(screen);
	DBUG_RETURN(result);
}

GC XDefaultGCOfScreen(Screen* screen) {
	DBUG_ENTER("XDefaultGCOfScreen")
	GC result = DefaultGCOfScreen(screen);
	DBUG_RETURN(result);
}

Window XDefaultRootWindow(Display* display) {
	DBUG_ENTER("XDefaultRootWindow")
	Window root = DefaultRootWindow(display);
	DBUG_RETURN(root);
}

Screen *XDefaultScreenOfDisplay(Display* display) {
	DBUG_ENTER("XDefaultScreenOfData")
	Screen *result = DefaultScreenOfDisplay(display);
	DBUG_RETURN(result);
}

Visual *XDefaultVisual(Display* display, int screen_number) {
	DBUG_ENTER("XDefaultVisual")
	Visual *visual = DefaultVisual(display, screen_number);
	DBUG_RETURN(visual);
}

Visual *XDefaultVisualOfScreen(Screen* screen) {
	DBUG_ENTER("XDefaultVisualOfScreen")
	Visual *result = DefaultVisualOfScreen(screen);
	DBUG_RETURN(result);
}

unsigned long XBlackPixelOfScreen(Screen* screen) {
	DBUG_ENTER("XBlackPixelOfScreen")
	unsigned long result = BlackPixelOfScreen(screen);
	DBUG_RETURN(result);
}

int XCellsOfScreen(Screen* screen) {
	DBUG_ENTER("XCellsOfScreen")
	int result = CellsOfScreen(screen);
	DBUG_RETURN(result);
}

long XEventMaskOfScreen(Screen* screen) {
	DBUG_ENTER("XEventMaskOfScreen")
	long result = EventMaskOfScreen(screen);
	DBUG_RETURN(result);
}

int XHeightMMOfScreen(Screen* screen) {
	DBUG_ENTER("XHeightMMOfScreen")
	int result = HeightMMOfScreen(screen);
	DBUG_RETURN(result);
}

int XHeightOfScreen(Screen* screen) {
	DBUG_ENTER("XHeightOfScreen")
	int result = HeightOfScreen(screen);
	DBUG_RETURN(result);
}

int XMaxCmapsOfScreen(Screen* screen) {
	DBUG_ENTER("XMaxCmapsOfScreen")
	int result = MaxCmapsOfScreen(screen);
	DBUG_RETURN(result);
}

int XMinCmapsOfScreen(Screen* screen) {
	DBUG_ENTER("XMinCmapsOfScreen")
	int result = MinCmapsOfScreen(screen);
	DBUG_RETURN(result);
}

int XPlanesOfScreen(Screen* screen) {
	DBUG_ENTER("XPlanesOfScreen")
	int result = PlanesOfScreen(screen);
	DBUG_RETURN(result);
}

Window XRootWindowOfScreen(Screen* screen) {
	DBUG_ENTER("XRootWindowOfScreen")
	Window result = RootWindowOfScreen(screen);
	DBUG_RETURN(result);
}

unsigned long XWhitePixelOfScreen(Screen* screen) {
	DBUG_ENTER("XWhitePixelOfScreen")
	unsigned long result = WhitePixelOfScreen(screen);
	DBUG_RETURN(result);
}

int XWidthMMOfScreen(Screen* screen) {
	DBUG_ENTER("XWidthMMOfScreen")
	int result = WidthMMOfScreen(screen);
	DBUG_RETURN(result);
}

int XWidthOfScreen(Screen* screen) {
	DBUG_ENTER("XWidthOfScreen")
	int result = WidthOfScreen(screen);
	DBUG_RETURN(result);
}

int XDisplayCells(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayCells")
	int result = DisplayCells(display, screen_number);
	DBUG_RETURN(result);
}

int XDisplayHeight(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayHeight")
	int height = DisplayHeight(display, screen_number);
	DBUG_RETURN(height);
}

int XDisplayHeightMM(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayHeightMM")
	int height = DisplayHeightMM(display, screen_number);
	DBUG_RETURN(height);
}

int XDisplayPlanes(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayPlanes")
	int planes = DisplayPlanes(display, screen_number);
	DBUG_RETURN(planes);
}

char *XDisplayString(Display* display) {
	DBUG_ENTER("XDisplayString")
	char *result = DisplayString(display);
	DBUG_RETURN(result);
}

int XDisplayWidth(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayWidth")
	int width = DisplayWidth(display, screen_number);
	DBUG_RETURN(width);
}

int XDisplayWidthMM(Display* display, int screen_number) {
	DBUG_ENTER("XDisplayWidthMM")
	int width = DisplayWidthMM(display, screen_number);
	DBUG_RETURN(width);
}

int XProtocolRevision(Display *display) {
   DBUG_ENTER("XProtocolRevision")
   int result = display->proto_major_version;
   DBUG_RETURN(result);
}

int XProtocolVersion(Display *display){
   DBUG_ENTER("XProtocolVersion")
   int result = display->proto_minor_version;
   DBUG_RETURN(result);
}

char *XServerVendor(Display *display) {
	DBUG_ENTER("XServerVendor")
	char *result = display?display->vendor:NULL;
	DBUG_RETURN(result);
}

int XVendorRelease(Display *display) {
	DBUG_ENTER("XVendorRelease")
	int result = display?display->release:-1;
	DBUG_RETURN(result);
}

int XQLength(Display* display) {
	DBUG_ENTER("XQLength")
	int result = QLength(display);
	DBUG_RETURN(result);
}

int XScreenCount(Display* display) {
	DBUG_ENTER("XScreenCount")
	int result = ScreenCount(display);
	DBUG_RETURN(result);
}

int XImageByteOrder(Display* display) {
	DBUG_ENTER("XImageByteOrder")
	int result = ImageByteOrder(display);
	DBUG_RETURN(result);
}

int XBitmapUnit(Display* display) {
	DBUG_ENTER("XBitmapUnit")
	int result = BitmapUnit(display);
	DBUG_RETURN(result);
}

int XBitmapBitOrder(Display* display) {
	DBUG_ENTER("XBitmapBitOrder")
	int result = BitmapBitOrder(display);
	DBUG_RETURN(result);
}

int XBitmapPad(Display* display) {
	DBUG_ENTER("XBitmapPad")
	int result = BitmapPad(display);
	DBUG_RETURN(result);
}

Bool XDoesSaveUnders(Screen* screen) {
	DBUG_ENTER("XDoesSaveUnders")
	Bool result = DoesSaveUnders(screen);
	DBUG_RETURN(result);
}

int XDoesBackingStore(Screen* screen) {
	DBUG_ENTER("XDoesBackingStore")
	int result = DoesBackingStore(screen);
	DBUG_RETURN(result);
}
