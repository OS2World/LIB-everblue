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
	FSWrap.c ICWrap.c IMWrap.c lcCharSet.c lcConv.c lcCT.c lcDB.c lcDefConv.c lcDynamic.c lcEuc.c lcFile.c lcGenConv.c lcGeneric.c lcInit.c lcJis.c lcPrTxt.c lcPublic.c lcPubWrap.c lcRM.c lcSjis.c lcStd.c lcTxtPr.c lcUTF8.c lcUTF8Load.c lcUtil.c lcWrap.c mbWrap.c OCWrap.c OMWrap.c RegstFlt.c SetLocale.c utf8Wrap.c wcWrap.c XDefaultOMIF.c XlcDL.c XlcSL.c
XLIB_OBJ=\
	FSWrap$(EXT) ICWrap$(EXT) IMWrap$(EXT) lcCharSet$(EXT) lcConv$(EXT) lcCT$(EXT) lcDB$(EXT) lcDefConv$(EXT) lcDynamic$(EXT) lcEuc$(EXT) lcFile$(EXT) lcGenConv$(EXT) lcGeneric$(EXT) lcInit$(EXT) lcJis$(EXT) lcPrTxt$(EXT) lcPublic$(EXT) lcPubWrap$(EXT) lcRM$(EXT) lcSjis$(EXT) lcStd$(EXT) lcTxtPr$(EXT) lcUTF8$(EXT) lcUTF8Load$(EXT) lcUtil$(EXT) lcWrap$(EXT) mbWrap$(EXT) OCWrap$(EXT) OMWrap$(EXT) RegstFlt$(EXT) SetLocale$(EXT) utf8Wrap$(EXT) wcWrap$(EXT) XDefaultOMIF$(EXT) XlcDL$(EXT) XlcSL$(EXT)

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
