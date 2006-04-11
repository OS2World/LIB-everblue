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

XDEFS=-DXKB -DDESTROY_XID
CDEFS=-DOS2I18N -DX_LOCALE -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CDEFS=-DX_LOCALE -DX_WCHAR -D__UNIXOS2__ -DDEBUG -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -O2 -D__ST_MT_ERRNO $(CDEFS) $(XDEFS)
DLLFLAGS=-Zomf -Zdll -Zmap -Zbin-files $(CFLAGS)

##################################################################

HEADERS=daemondll.h ../myXlibint.h ../EBstruc.h ../EBmessage.h ../EBhook.h

LIB_RES=xdaemon.res
XDLL_DLL=xdaemon.dll
XDLL_DEF=xdaemon.def
XDLL_OUT=xdaemon.lib
DLLLIBS=-lsocket -lebhook -lX11

XDLL_SRC=\
	GC.c Drawable.c \
	WindowTools.c WindowHash.c EventMask.c Thread.c \
	stubs.c register.c HeapShared.c InitTerm.c \
	Window.c EventQueue.c Grab.c Resource.c \
	message.c

XDLL_OBJ=\
	GC$(EXT) Drawable$(EXT) \
	WindowTools$(EXT) WindowHash$(EXT) EventMask$(EXT) Thread$(EXT) \
	stubs$(EXT) register$(EXT) HeapShared$(EXT) InitTerm$(EXT) \
	Window$(EXT) EventQueue$(EXT) Grab$(EXT) Resource$(EXT) \
	message$(EXT)

##################################################################

all: dmndll

clean:
	$(RM) xdaemon.dll $(XDMN_OBJ)

# library

$(LIB_RES): xdaemon.rc
	rc -r -p -x xdaemon.rc $(LIB_RES)

dmndll: $(XDLL_OBJ) $(LIB_RES)
	$(RM) $(XDLL_DLL)
	$(CC) -Zomf $(DLLFLAGS) -o $(XDLL_DLL) $(XDLL_OBJ) $(DLLLIBS) $(XDLL_DEF)
	emximp -o $(XDLL_OUT) $(XDLL_DEF)
	$(RC) -x2 $(LIB_RES) $(XDLL_DLL) > nul

# DEPENDANCY INFOMATION

$(XDLL_OBJ): $(HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
