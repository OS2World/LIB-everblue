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
# EXT=.o 
EXT=.obj

.SUFFIXES: .c $(EXT)

XDEFS=-DXTHREADS -DX_LOCALE # -DXKB
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -D__ST_MT_ERRNO -DDEBUG -Zomf -O -g -Wall -L/usr/lib $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS)

##################################################################

XLIB_HEADERS=

XLIB_DLL=emxhandl.dll
XLIB_DEF=emxhandl.def
XLIB2_DEF=emxhandl_exp.def
XLIB_OUT=emxhandl.lib
DLLLIBS=

XLIB_SRC=\
	emxhandl.c

LIB_OBJ=\
	emxhandl$(EXT)

##################################################################

all: dll

clean:
	$(RM) emxhandl.dll $(LIB_OBJ)

# library

dll: $(LIB_OBJ)
	$(RM) $(XLIB_DLL)
	gcc -Zomf $(DLLFLAGS) -o $(XLIB_DLL) $(LIB_OBJ) $(XLIB_DEF) $(DLLLIBS)
	emximp -o $(XLIB_OUT) $(XLIB2_DEF)

# DEPENDANCY INFOMATION

$(XLIB_OBJ): $(XLIB_HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
