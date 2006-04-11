##################################################################
#
# Xlib for Presentation Manager
#
##################################################################

CC=gcc
AS=as
MKLIB=ar r
MAKE=make
CP=cp
RM=rm -f
RC=rc
# If you are using gcc 3.0 change this entry to
EXT=.o 
# EXT=.obj

.SUFFIXES: .c $(EXT)

XDEFS=-DXTHREADS -DX_LOCALE # -DXKB
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=50
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib -DDEBUG $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS)

##################################################################

XLIB_HEADERS=X11.h ../myXlibint.h ../EBdebug.h ../EBstruc.h ../EBmessage.h

XLIB_DLL=X11.dll
XLIB_DEF=X11.def
XLIB2_DEF=X11_exp.def
XLIB_OUT=X11.lib
DLLLIBS=-lxdaemon -lebhook -lemxhandl

XLIB_SRC=\
	SetClipRectangles.c myKeyBind.c myImUtil.c \
	CopyPlane.c Keyboard.c GetHints.c locking.c GetImage.c \
	Xim.c PutImage.c Misc.c DrawText.c Font.c os2.c \
	NamedColor.c Free.c CmapRec.c Pointer.c Color.c \
	CopyArea.c Window.c Visual.c XlibInt.c Tree.c \
	Property.c Cursor.c Clear.c Draw.c ResizeWindow.c \
	MapWindow.c FontAlias.c FontNames.c CreateWindow.c \
	QueryBest.c Focus.c Simple.c myXrm.c Pixmap.c \
	Stubs.c NextEvent.c Flush.c Extension.c \
	OpenDisplay.c uconv.c Atom.c \
	Debug.c GC.c Context.c \
	NewDisplay.c

XLIB_OBJ=\
	SetClipRectangles$(EXT) myKeyBind$(EXT) myImUtil$(EXT) \
	CopyPlane$(EXT) Keyboard$(EXT) GetHints$(EXT) locking$(EXT) GetImage$(EXT) \
	Xim$(EXT) PutImage$(EXT) Misc$(EXT) DrawText$(EXT) Font$(EXT) os2.c \
	NamedColor$(EXT) Free$(EXT) CmapRec$(EXT) Pointer$(EXT) Color$(EXT) \
	CopyArea$(EXT) Window$(EXT) Visual$(EXT) XlibInt$(EXT) Tree$(EXT) \
	Property$(EXT) Cursor$(EXT) Clear$(EXT) Draw$(EXT) ResizeWindow$(EXT) \
	MapWindow$(EXT) FontAlias$(EXT) FontNames$(EXT) CreateWindow$(EXT) \
	QueryBest$(EXT) Focus$(EXT) Simple$(EXT) myXrm$(EXT) Pixmap$(EXT) \
	Stubs$(EXT) NextEvent$(EXT) Flush$(EXT) Extension$(EXT) \
	OpenDisplay$(EXT) uconv$(EXT) Atom$(EXT) \
	Debug$(EXT) GC$(EXT) Context$(EXT) \
	NewDisplay$(EXT)

LIB_OBJ=$(XLIB_OBJ) xcms/Xcms.lib xlibi18n/Xlc.lib Xmisc/Xmisc.lib

##################################################################

all: dll

clean:
	$(RM) X11.dll $(XLIB_OBJ)

# library

dll: $(LIB_OBJ)
	$(RM) $(XLIB_DLL)
	$(CC) -Zomf $(DLLFLAGS) -o $(XLIB_DLL) $(LIB_OBJ) $(XLIB_DEF) $(DLLLIBS)
	emximp -o $(XLIB_OUT) $(XLIB2_DEF)

# DEPENDANCY INFOMATION

$(XLIB_OBJ): $(XLIB_HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
