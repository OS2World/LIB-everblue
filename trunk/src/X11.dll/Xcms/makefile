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
	AddDIC.c AddSF.c CCC.c cmsAllCol.c cmsColNm.c cmsGlobls.c cmsInt.c cmsMath.c cmsProp.c cmsTrig.c CvCols.c CvColW.c HVC.c HVCGcC.c HVCGcV.c HVCGcVC.c HVCMnV.c HVCMxC.c HVCMxV.c HVCMxVC.c HVCMxVs.c HVCWpAj.c IdOfPr.c Lab.c LabGcC.c LabGcL.c LabGcLC.c LabMnL.c LabMxC.c LabMxL.c LabMxLC.c LabWpAj.c LRGB.c Luv.c LuvGcC.c LuvGcL.c LuvGcLC.c LuvMnL.c LuvMxC.c LuvMxL.c LuvMxLC.c LuvWpAj.c OfCCC.c PrOfId.c QBlack.c QBlue.c QGreen.c QRed.c QuCol.c QuCols.c QWhite.c SetCCC.c SetGetCols.c StCol.c StCols.c UNDEFINED.c uvY.c XRGB.c xyY.c XYZ.c
XLIB_OBJ=\
	AddDIC$(EXT) AddSF$(EXT) CCC$(EXT) cmsAllCol$(EXT) cmsColNm$(EXT) cmsGlobls$(EXT) cmsInt$(EXT) cmsMath$(EXT) cmsProp$(EXT) cmsTrig$(EXT) CvCols$(EXT) CvColW$(EXT) HVC$(EXT) HVCGcC$(EXT) HVCGcV$(EXT) HVCGcVC$(EXT) HVCMnV$(EXT) HVCMxC$(EXT) HVCMxV$(EXT) HVCMxVC$(EXT) HVCMxVs$(EXT) HVCWpAj$(EXT) IdOfPr$(EXT) Lab$(EXT) LabGcC$(EXT) LabGcL$(EXT) LabGcLC$(EXT) LabMnL$(EXT) LabMxC$(EXT) LabMxL$(EXT) LabMxLC$(EXT) LabWpAj$(EXT) LRGB$(EXT) Luv$(EXT) LuvGcC$(EXT) LuvGcL$(EXT) LuvGcLC$(EXT) LuvMnL$(EXT) LuvMxC$(EXT) LuvMxL$(EXT) LuvMxLC$(EXT) LuvWpAj$(EXT) OfCCC$(EXT) PrOfId$(EXT) QBlack$(EXT) QBlue$(EXT) QGreen$(EXT) QRed$(EXT) QuCol$(EXT) QuCols$(EXT) QWhite$(EXT) SetCCC$(EXT) SetGetCols$(EXT) StCol$(EXT) StCols$(EXT) UNDEFINED$(EXT) uvY$(EXT) XRGB$(EXT) xyY$(EXT) XYZ$(EXT)

##################################################################

all: $(XLIB_OBJ)
	I:\unixos2\emx\bin\emxomfar.exe r Xcms *.o

clean:
	$(RM) $(XLIB_OBJ) Xcms.lib Xcms.bak

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
