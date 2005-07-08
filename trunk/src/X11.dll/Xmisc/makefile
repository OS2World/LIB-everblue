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

XDEFS=-DXTHREADS -DX_LOCALE -DXKB -D__FreeBSD__
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib -DDEBUG $(CDEFS) $(XDEFS)

##################################################################

XLIB_SRC=\
	AutoRep.c CrBFData.c CrPFBData.c Depths.c ErrDes.c ErrHndlr.c FreeEData.c GCMisc.c Geom.c GetDflt.c GetFProp.c GetGCVals.c globals.c ImUtil.c InitExt.c KeyBind.c Misc.c ParseCmd.c ParseGeom.c PixFormats.c PolyReg.c PropAlloc.c Quarks.c RdBitF.c Region.c TextExt.c TextExt16.c udcInf.c VisUtil.c WMGeom.c WrBitF.c XKBBind.c
XLIB_OBJ=\
	AutoRep$(EXT) CrBFData$(EXT) CrPFBData$(EXT) Depths$(EXT) ErrDes$(EXT) ErrHndlr$(EXT) FreeEData$(EXT) GCMisc$(EXT) Geom$(EXT) GetDflt$(EXT) GetFProp$(EXT) GetGCVals$(EXT) globals$(EXT) ImUtil$(EXT) InitExt$(EXT) KeyBind$(EXT) Misc$(EXT) ParseCmd$(EXT) ParseGeom$(EXT) PixFormats$(EXT) PolyReg$(EXT) PropAlloc$(EXT) Quarks$(EXT) RdBitF$(EXT) Region$(EXT) TextExt$(EXT) TextExt16$(EXT) udcInf$(EXT) VisUtil$(EXT) WMGeom$(EXT) WrBitF$(EXT) XKBBind$(EXT)

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
