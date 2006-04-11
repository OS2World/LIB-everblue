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

HEADERS=

makekeys_DEF=makekeys.def
EXELIBS=-L.

makekeys_SRC=\
	makekeys.c

makekeys_OBJ=\
	makekeys$(EXT)

##################################################################

all: makekeys

clean:
	$(RM) makekeys makekeys.exe $(makekeys_OBJ)

# program

makekeys: $(makekeys_DEF) $(makekeys_OBJ)
	$(CC) $(EXEFLAGS) -o makekeys $(makekeys_OBJ) $(EXELIBS) $(makekeys_DEF)
	$(RM) makekeys

# DEPENDANCY INFOMATION

$(makekeys_OBJ): $(HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
