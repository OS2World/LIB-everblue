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
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS) -s

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib -DDEBUG $(CDEFS) $(XDEFS)
DLLFLAGS=-Zdll -Zcrtdll $(CFLAGS)

##################################################################

XLIB_HEADERS=X11.h ../Xlibint.h ../EBdebug.h ../EBstruc.h ../EBmessage.h

XLIB_DLL=X11.dll
XLIB_DEF=X11.def
XLIB2_DEF=X11_exp.def
XLIB_OUT=X11.lib
DLLLIBS=-lxdaemon -lebhook -lemxhandl

XLIB_SRC=\
	DrawText.c Font.c \
	NamedColor.c Free.c CmapRec.c Pointer.c Color.c \
	CopyArea.c Window.c Visual.c XlibInt.c Tree.c \
	Property.c Cursor.c Clear.c Draw.c ResizeWindow.c \
	MapWindow.c FontAlias.c FontNames.c CreateWindow.c \
	QueryBest.c Focus.c Simple.c Xrm.c Pixmap.c \
	Stubs.c NextEvent.c Flush.c Extension.c \
	OpenDisplay.c uconv.c os2.c Atom.c \
	Debug.c GC.c EventToMask.c Context.c \
	NewDisplay.c

XLIB_OBJ=\
	DrawText$(EXT) Font$(EXT) \
	NamedColor$(EXT) Free$(EXT) CmapRec$(EXT) Pointer$(EXT) Color$(EXT) \
	CopyArea$(EXT) Window$(EXT) Visual$(EXT) XlibInt$(EXT) Tree$(EXT) \
	Property$(EXT) Cursor$(EXT) Clear$(EXT) Draw$(EXT) ResizeWindow$(EXT) \
	MapWindow$(EXT) FontAlias$(EXT) FontNames$(EXT) CreateWindow$(EXT) \
	QueryBest$(EXT) Focus$(EXT) Simple$(EXT) Xrm$(EXT) Pixmap$(EXT) \
	Stubs$(EXT) NextEvent$(EXT) Flush$(EXT) Extension$(EXT) \
	OpenDisplay$(EXT) uconv$(EXT) os2$(EXT) Atom$(EXT) \
	Debug$(EXT) GC$(EXT) EventToMask$(EXT) Context$(EXT) \
	NewDisplay$(EXT)

XCMS_OBJ=\
	Xcms/AddDIC$(EXT) Xcms/AddSF$(EXT) Xcms/CCC$(EXT) Xcms/cmsAllCol$(EXT) Xcms/cmsColNm$(EXT) Xcms/cmsGlobls$(EXT) Xcms/cmsInt$(EXT) Xcms/cmsMath$(EXT) Xcms/cmsProp$(EXT) Xcms/cmsTrig$(EXT) Xcms/CvCols$(EXT) Xcms/CvColW$(EXT) Xcms/HVC$(EXT) Xcms/HVCGcC$(EXT) Xcms/HVCGcV$(EXT) Xcms/HVCGcVC$(EXT) Xcms/HVCMnV$(EXT) Xcms/HVCMxC$(EXT) Xcms/HVCMxV$(EXT) Xcms/HVCMxVC$(EXT) Xcms/HVCMxVs$(EXT) Xcms/HVCWpAj$(EXT) Xcms/IdOfPr$(EXT) Xcms/Lab$(EXT) Xcms/LabGcC$(EXT) Xcms/LabGcL$(EXT) Xcms/LabGcLC$(EXT) Xcms/LabMnL$(EXT) Xcms/LabMxC$(EXT) Xcms/LabMxL$(EXT) Xcms/LabMxLC$(EXT) Xcms/LabWpAj$(EXT) Xcms/LRGB$(EXT) Xcms/Luv$(EXT) Xcms/LuvGcC$(EXT) Xcms/LuvGcL$(EXT) Xcms/LuvGcLC$(EXT) Xcms/LuvMnL$(EXT) Xcms/LuvMxC$(EXT) Xcms/LuvMxL$(EXT) Xcms/LuvMxLC$(EXT) Xcms/LuvWpAj$(EXT) Xcms/OfCCC$(EXT) Xcms/PrOfId$(EXT) Xcms/QBlack$(EXT) Xcms/QBlue$(EXT) Xcms/QGreen$(EXT) Xcms/QRed$(EXT) Xcms/QuCol$(EXT) Xcms/QuCols$(EXT) Xcms/QWhite$(EXT) Xcms/SetCCC$(EXT) Xcms/SetGetCols$(EXT) Xcms/StCol$(EXT) Xcms/StCols$(EXT) Xcms/UNDEFINED$(EXT) Xcms/uvY$(EXT) Xcms/XRGB$(EXT) Xcms/xyY$(EXT) Xcms/XYZ$(EXT)

XLC_OBJ=\
	Xlc/FSWrap$(EXT) Xlc/ICWrap$(EXT) Xlc/IMWrap$(EXT) Xlc/lcCharSet$(EXT) Xlc/lcConv$(EXT) Xlc/lcCT$(EXT) Xlc/lcDB$(EXT) Xlc/lcDefConv$(EXT) Xlc/lcDynamic$(EXT) Xlc/lcEuc$(EXT) Xlc/lcFile$(EXT) Xlc/lcGenConv$(EXT) Xlc/lcGeneric$(EXT) Xlc/lcInit$(EXT) Xlc/lcJis$(EXT) Xlc/lcPrTxt$(EXT) Xlc/lcPublic$(EXT) Xlc/lcPubWrap$(EXT) Xlc/lcRM$(EXT) Xlc/lcSjis$(EXT) Xlc/lcStd$(EXT) Xlc/lcTxtPr$(EXT) Xlc/lcUTF8$(EXT) Xlc/lcUTF8Load$(EXT) Xlc/lcUtil$(EXT) Xlc/lcWrap$(EXT) Xlc/mbWrap$(EXT) Xlc/OCWrap$(EXT) Xlc/OMWrap$(EXT) Xlc/RegstFlt$(EXT) Xlc/SetLocale$(EXT) Xlc/utf8Wrap$(EXT) Xlc/wcWrap$(EXT) Xlc/XDefaultOMIF$(EXT) Xlc/XlcDL$(EXT) Xlc/XlcSL$(EXT)

XMISC_OBJ=\
	Xmisc/AutoRep$(EXT) Xmisc/CrBFData$(EXT) Xmisc/CrPFBData$(EXT) Xmisc/Depths$(EXT) Xmisc/ErrDes$(EXT) Xmisc/ErrHndlr$(EXT) Xmisc/FreeEData$(EXT) Xmisc/GCMisc$(EXT) Xmisc/Geom$(EXT) Xmisc/GetDflt$(EXT) Xmisc/GetFProp$(EXT) Xmisc/GetGCVals$(EXT) Xmisc/globals$(EXT) Xmisc/ImUtil$(EXT) Xmisc/InitExt$(EXT) Xmisc/KeyBind$(EXT) Xmisc/Misc$(EXT) Xmisc/ParseCmd$(EXT) Xmisc/ParseGeom$(EXT) Xmisc/PixFormats$(EXT) Xmisc/PolyReg$(EXT) Xmisc/PropAlloc$(EXT) Xmisc/Quarks$(EXT) Xmisc/RdBitF$(EXT) Xmisc/Region$(EXT) Xmisc/TextExt$(EXT) Xmisc/TextExt16$(EXT) Xmisc/udcInf$(EXT) Xmisc/VisUtil$(EXT) Xmisc/WMGeom$(EXT) Xmisc/WrBitF$(EXT) Xmisc/XKBBind$(EXT)

XOM_OBJ=\
	Xom/omDefault$(EXT) Xom/omGeneric$(EXT) Xom/omImText$(EXT) Xom/omText$(EXT) Xom/omTextEsc$(EXT) Xom/omTextExt$(EXT) Xom/omTextPer$(EXT) Xom/omXChar$(EXT)

XPROP_OBJ=\
	Xprop/FetchName$(EXT) Xprop/GetHints$(EXT) Xprop/GetNrmHint$(EXT) Xprop/GetRGBCMap$(EXT) Xprop/GetStCmap$(EXT) Xprop/GetTxtProp$(EXT) Xprop/GetWMCMapW$(EXT) Xprop/GetWMProto$(EXT) Xprop/Iconify$(EXT) Xprop/mbWMProps$(EXT) Xprop/ScrResStr$(EXT) Xprop/SetHints$(EXT) Xprop/SetNrmHint$(EXT) Xprop/SetRGBCMap$(EXT) Xprop/SetStCmap$(EXT) Xprop/SetTxtProp$(EXT) Xprop/SetWMCMapW$(EXT) Xprop/SetWMProto$(EXT) Xprop/StBytes$(EXT) Xprop/StName$(EXT) Xprop/StrToText$(EXT) Xprop/TextToStr$(EXT) Xprop/utf8WMProps$(EXT) Xprop/Withdraw$(EXT) Xprop/WMProps$(EXT)

LIB_OBJ=$(XLIB_OBJ) $(XCMS_OBJ) $(XLC_OBJ) $(XMISC_OBJ) $(XOM_OBJ) $(XPROP_OBJ)

##################################################################

all: dll

clean:
	$(RM) X11.dll $(XLIB_OBJ)

# library

dll: $(LIB_OBJ)
	$(RM) $(XLIB_DLL)
	$(CC) -Zomf $(DLLFLAGS) -o $(XLIB_DLL) $(LIB_OBJ) $(XLIB_DEF) $(DLLLIBS)
	emximp -o $(XLIB_OUT) $(XLIB2_DEF)

# DEPENDANCY INFOMATION

$(XLIB_OBJ): $(XLIB_HEADERS)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
