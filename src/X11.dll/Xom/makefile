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
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib -DDEBUG $(CDEFS) $(XDEFS)

##################################################################

XLIB_SRC=\
	omDefault.c omGeneric.c omImText.c omText.c omTextEsc.c omTextExt.c omTextPer.c omXChar.c
XLIB_OBJ=\
	omDefault$(EXT) omGeneric$(EXT) omImText$(EXT) omText$(EXT) omTextEsc$(EXT) omTextExt$(EXT) omTextPer$(EXT) omXChar$(EXT)

##################################################################

all: $(XLIB_OBJ)

clean:
	$(RM) $(XLIB_OBJ)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
