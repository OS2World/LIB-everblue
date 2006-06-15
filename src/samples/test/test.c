#define I_NEED_OS2_H
#define INCL_DOS
#include <os2emx.h>

#include <stdio.h>
#include <string.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <X11/Xpoll.h>
#include <errno.h>

#include "icone.inc"

#define COMPILE_SHOWBITMAP 1


#define MAX_MAPPED_STRING_LENGTH 10

int main(void)
{
        Display *display;
        int screen;
        Window window;
        XSetWindowAttributes attributes;
        XEvent xevent;
        KeySym keysym;
        XComposeStatus compose;
        int exitprog = 0;
        GC gc;
        Visual *visual;
        int depth;
        Cursor cursor;
        Font font;
        Pixmap icone;

        printf("Locale support: %x\n", XSupportsLocale());
        display = XOpenDisplay("localhost");
#if 1
        if(!display) {
                printf("Error opening display!\n");
                return 1;
        }

        printf("display address: %x\n", (int)display);

        screen = DefaultScreen(display);
        visual = DefaultVisual(display, screen);
        depth = DefaultDepth(display, screen);

        attributes.background_pixel = WhitePixel(display, screen);
        attributes.bit_gravity = ForgetGravity;

        if (!(font = XLoadFont(display, "fixed")))
                printf("Unable to load font!\n");
        printf("font address: %x\n", (int)font);

        XSelectInput(display, DefaultRootWindow(display), -1);
        cursor = XCreateFontCursor(display, XC_X_cursor);
        XDefineCursor(display, DefaultRootWindow(display), cursor);

        window = XCreateWindow(display, DefaultRootWindow(display),
                300, 300, 300, 300, 5, depth, InputOutput,
                visual, CWBackPixel | CWBitGravity, &attributes);
        printf("window address: %x\n", (int)window);

        XSelectInput(display, window, -1);

        XDefineCursor(display, window, cursor);

        XMapWindow(display, window);

#if COMPILE_SHOWBITMAP
        icone = XCreateBitmapFromData(display, window, icone_bits, icone_width, icone_height);
#endif

        gc = XCreateGC(display, window, 0, NULL);
        printf("gc address: %x\n", (int)gc);

        XSetForeground(display, gc, BlackPixel(display, screen));
        XSetBackground(display, gc, WhitePixel(display, screen));
        XSetLineAttributes(display, gc, 15, LineSolid, CapNotLast, JoinBevel);
        if(font)
;//             XSetFont(display, gc, font);

        while(exitprog == 0) {
                XPeekEvent(display, &xevent);
                switch(xevent.type) {
                case CreateNotify:
                        printf("Peeked CreateNotify event.\n");
                        break;
                case DestroyNotify:
                        printf("Peeked DestroyNotify event.\n");
                        break;
                case FocusIn:
                        printf("Peeked FocusIn event.\n");
                        break;
                case FocusOut:
                        printf("Peeked FocusOut event.\n");
                        break;
                case EnterNotify:
                        printf("Peeked EnterNotify event.\n");
                        break;
                case LeaveNotify:
                        printf("Peeked LeaveNotify event.\n");
                        break;
                }
                XNextEvent(display, &xevent);
                switch(xevent.type) {
                case Expose: {
                        char st[80];
                        XClearWindow(display, xevent.xany.window);
                        XDrawLine(display, xevent.xexpose.window, gc,
                                xevent.xexpose.x, xevent.xexpose.y, xevent.xexpose.x +
                                xevent.xexpose.width - 1, xevent.xexpose.y + xevent.xexpose.height - 1);
                        XDrawLine(display, xevent.xexpose.window, gc,
                                xevent.xexpose.x + xevent.xexpose.width - 1, xevent.xexpose.y,
                                xevent.xexpose.x, xevent.xexpose.y + xevent.xexpose.height - 1);
                        sprintf(st, "Expose (x=%d,y=%d,width=%d,height=%d)",
                                xevent.xexpose.x, xevent.xexpose.y,
                                xevent.xexpose.width, xevent.xexpose.height);
                        XDrawString(display, xevent.xexpose.window, gc, 0, 20, st, strlen(st));
#if COMPILE_SHOWBITMAP
                        XCopyArea(display, icone, window, gc, 0, 0, icone_width, icone_height, 0, 0);
#endif
                        break;
                }
                case DestroyNotify:
                        exitprog++;
                        break;
/*              case KeyPress: {
                        char buffer[MAX_MAPPED_STRING_LENGTH];
                        int bufsize = MAX_MAPPED_STRING_LENGTH;
                        int count = XLookupString(&xevent.xkey, buffer, bufsize, &keysym, &compose);
                        buffer[count] = 0;
                        if (count)
                                printf("Keypressed: '%s'  (keycode = 0x%x)\n",
                                        buffer, xevent.xkey.keycode);
                        else {
                                char *tmp = XKeysymToString(keysym);
                                if(tmp)
                                        printf("Graykey: [%s] (keycode = 0x%x)\n", tmp, xevent.xkey.keycode);
                                else
                                        printf("Unknown keycode! 0x%x\n", xevent.xkey.keycode);
                        }
                        break;
                }*/
                }
        }
        XDestroyWindow(display, window);
#endif
        DosSleep(5000);
        XCloseDisplay(display);

        return 0;
}
