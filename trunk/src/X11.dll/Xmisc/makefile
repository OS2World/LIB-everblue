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
	AutoRep.c ChGC.c ChkIfEv.c ChkMaskEv.c ChkTypEv.c ChkTypWEv.c ChkWinEv.c CrBFData.c CrPFBData.c Cursor.c Depths.c DisName.c ErrDes.c ErrHndlr.c evtomask.c FetchName.c FilterEv.c Flush.c FreeEData.c FSWrap.c GCMisc.c Geom.c GetFProp.c GetGCVals.c \
	GetStCmap.c GetTxtProp.c GetWMCMapW.c GetWMProto.c globals.c Iconify.c IfEvent.c MaskEvent.c NextEvent.c OCWrap.c OMWrap.c ParseCmd.c ParseGeom.c PeekEvent.c PeekIfEv.c Pending.c PixFormats.c PolyReg.c PropAlloc.c PutBEvent.c Quarks.c RdBitF.c Region.c \
	RegstFlt.c ScrResStr.c SetBack.c SetClMask.c SetClOrig.c SetFont.c SetFore.c SetFunc.c SetHints.c SetLocale.c SetLStyle.c SetNrmHint.c SetPMask.c SetRGBCMap.c SetState.c SetStCmap.c SetStip.c SetTile.c SetTSOrig.c SetTxtProp.c SetWMCMapW.c SetWMProto.c StBytes.c StName.c StrToText.c TextExt.c TextExt16.c TextToStr.c udcInf.c VisUtil.c WinEvent.c Withdraw.c WMGeom.c WMProps.c WrBitF.c \
	KeysymStr.c
#	StrKeysym.c GetDflt.c Xrm.c \

XLIB_OBJ=\
	AutoRep$(EXT) ChGC$(EXT) ChkIfEv$(EXT) ChkMaskEv$(EXT) ChkTypEv$(EXT) ChkTypWEv$(EXT) ChkWinEv$(EXT) CrBFData$(EXT) CrPFBData$(EXT) Cursor$(EXT) Depths$(EXT) DisName$(EXT) ErrDes$(EXT) ErrHndlr$(EXT) evtomask$(EXT) FetchName$(EXT) FilterEv$(EXT) Flush$(EXT) FreeEData$(EXT) FSWrap$(EXT) GCMisc$(EXT) Geom$(EXT) GetFProp$(EXT) GetGCVals$(EXT) \
	GetStCmap$(EXT) GetTxtProp$(EXT) GetWMCMapW$(EXT) GetWMProto$(EXT) globals$(EXT) Iconify$(EXT) IfEvent$(EXT) MaskEvent$(EXT) NextEvent$(EXT) OCWrap$(EXT) OMWrap$(EXT) ParseCmd$(EXT) ParseGeom$(EXT) PeekEvent$(EXT) PeekIfEv$(EXT) Pending$(EXT) PixFormats$(EXT) PolyReg$(EXT) PropAlloc$(EXT) PutBEvent$(EXT) Quarks$(EXT) RdBitF$(EXT) Region$(EXT) \
	RegstFlt$(EXT) ScrResStr$(EXT) SetBack$(EXT) SetClMask$(EXT) SetClOrig$(EXT) SetFont$(EXT) SetFore$(EXT) SetFunc$(EXT) SetHints$(EXT) SetLocale$(EXT) SetLStyle$(EXT) SetNrmHint$(EXT) SetPMask$(EXT) SetRGBCMap$(EXT) SetState$(EXT) SetStCmap$(EXT) SetStip$(EXT) SetTile$(EXT) SetTSOrig$(EXT) SetTxtProp$(EXT) SetWMCMapW$(EXT) SetWMProto$(EXT) StBytes$(EXT) StName$(EXT) StrToText$(EXT) TextExt$(EXT) TextExt16$(EXT) TextToStr$(EXT) udcInf$(EXT) VisUtil$(EXT) WinEvent$(EXT) Withdraw$(EXT) WMGeom$(EXT) WMProps$(EXT) WrBitF$(EXT) \
	KeysymStr$(EXT)
#	StrKeysym$(EXT) GetDflt$(EXT) Xrm$(EXT) \

##################################################################

all: $(XLIB_OBJ)
	I:\unixos2\emx\bin\emxomfar.exe r Xmisc *.o

clean:
	$(RM) $(XLIB_OBJ) Xmisc.bak Xmisc.lib

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
