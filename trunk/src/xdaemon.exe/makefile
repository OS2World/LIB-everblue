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

XDEFS=-DXKB
CDEFS=-DOS2I18N -DX_LOCALE -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)
EXEFLAGS=-Zexe -Zomf $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CDEFS=-DDEBUG -DOS2I18N -DX_LOCALE -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib $(CDEFS) $(XDEFS)
EXEFLAGS=-Zexe -Zomf $(CFLAGS)

##################################################################

HEADERS=x11daemon.h ../EBstruc.h ../EBmessage.h ../EBhook.h

XDMN_DEF=xdaemon.def
EXELIBS=-L. -lxdaemon -lebhook

XDMN_SRC=\
	ProcFrame.c XEventThread.c UMOpenPipe.c \
	UMDestroyWindow.c UMFreeResources.c UMSetWindowPos.c \
	UMCreateWindow.c ProcBorder.c InitializePM.c ProcMessage.c \
	ShutDownPM.c xdaemon.c ProcWindow.c \
	PMWM.c

XDMN_OBJ=\
	ProcFrame$(EXT) XEventThread$(EXT) UMOpenPipe$(EXT) \
	UMDestroyWindow$(EXT) UMFreeResources$(EXT) UMSetWindowPos$(EXT) \
	UMCreateWindow$(EXT) ProcBorder$(EXT) InitializePM$(EXT) ProcMessage$(EXT) \
	ShutDownPM$(EXT) xdaemon$(EXT) ProcWindow$(EXT) \
	PMWM$(EXT)

##################################################################

all: xdaemon

clean:
	$(RM) xdaemon xdaemon.exe $(XDMN_OBJ)

# program

xdaemon: $(XDMN_DEF) $(XDMN_OBJ)
	$(CC) $(EXEFLAGS) -o xdaemon $(XDMN_OBJ) $(EXELIBS) $(XDMN_DEF)
	$(RM) xdaemon

# DEPENDANCY INFOMATION

$(XDMN_OBJ): $(HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
