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
CFLAGS=-Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)
DLLFLAGS=-nodefaultlibs -Zdll $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CDEFS=-DDEBUG -DOS2I18N -DX_LOCALE -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zomf -O -g -Wall -L/usr/lib $(CDEFS) $(XDEFS)
DLLFLAGS=-nodefaultlibs -Zdll $(CFLAGS)

##################################################################

HEADERS=ebhookdll.h ../EBhook.h

EBH_DLL=ebhook.dll
EBH_DEF=ebhook.def
EBH_OUT=ebhook.lib
DLLLIBS=-L. -lgcc -llibend -llibos2

EBH_SRC=\
	mutex.c ebhook.c InitTerm.c crt3.s

EBH_OBJ=\
	mutex$(EXT) ebhook$(EXT) InitTerm$(EXT) crt3$(EXT)

##################################################################

all: ebhook

clean:
	$(RM) ebhook.dll $(XDMN_OBJ)

# library

ebhook: $(EBH_OBJ)
	$(RM) $(EBH_DLL)
	$(CC) -Zomf $(DLLFLAGS) -o $(EBH_DLL) $(EBH_OBJ) $(DLLLIBS) $(EBH_DEF)
	emximp -o $(EBH_OUT) $(EBH_DEF)

# DEPENDANCY INFOMATION

$(EBH_OBJ): $(HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
