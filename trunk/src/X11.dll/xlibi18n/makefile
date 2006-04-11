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
	ICWrap.c imKStoUCS.c IMWrap.c lcCharSet.c lcConv.c lcCT.c lcDB.c lcDynamic.c lcFile.c lcGeneric.c lcInit.c lcPrTxt.c lcPublic.c lcPubWrap.c lcRM.c lcStd.c lcTxtPr.c lcUTF8.c lcUtil.c lcWrap.c mbWMProps.c mbWrap.c utf8WMProps.c utf8Wrap.c wcWrap.c XDefaultIMIF.c XDefaultOMIF.c XlcDL.c XlcSL.c lcDefConv.c lcEuc.c lcGenConv.c lcJis.c lcSjis.c lcUTF8Load.c omDefault.c omGeneric.c omImText.c omText.c omTextEsc.c omTextExt.c omTextPer.c omXChar.c
XLIB_OBJ=\
	ICWrap$(EXT) imKStoUCS$(EXT) IMWrap$(EXT) lcCharSet$(EXT) lcConv$(EXT) lcCT$(EXT) lcDB$(EXT) lcDynamic$(EXT) lcFile$(EXT) lcGeneric$(EXT) lcInit$(EXT) lcPrTxt$(EXT) lcPublic$(EXT) lcPubWrap$(EXT) lcRM$(EXT) lcStd$(EXT) lcTxtPr$(EXT) lcUTF8$(EXT) lcUtil$(EXT) lcWrap$(EXT) mbWMProps$(EXT) mbWrap$(EXT) utf8WMProps$(EXT) utf8Wrap$(EXT) wcWrap$(EXT) XDefaultIMIF$(EXT) XDefaultOMIF$(EXT) XlcDL$(EXT) XlcSL$(EXT) lcDefConv$(EXT) lcEuc$(EXT) lcGenConv$(EXT) lcJis$(EXT) lcSjis$(EXT) lcUTF8Load$(EXT) omDefault$(EXT) omGeneric$(EXT) omImText$(EXT) omText$(EXT) omTextEsc$(EXT) omTextExt$(EXT) omTextPer$(EXT) omXChar$(EXT)

##################################################################

all: $(XLIB_OBJ)
	I:\unixos2\emx\bin\emxomfar.exe -p64 r Xlc *.o

clean:
	$(RM) $(XLIB_OBJ) Xlc.lib Xlc.bak

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
