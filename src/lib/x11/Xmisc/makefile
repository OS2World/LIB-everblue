# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=xmisc
TARGET=$(STEM).lib
DEF=$(STEM).def
OBJS=$(STEM).obj \
AutoRep.obj ChGC.obj ChkIfEv.obj ChkMaskEv.obj ChkTypEv.obj ChkTypWEv.obj \
ChkWinEv.obj CrBFData.obj CrPFBData.obj Cursor.obj Depths.obj DisName.obj \
ErrDes.obj ErrHndlr.obj evtomask.obj FetchName.obj FilterEv.obj Flush.obj \
FreeEData.obj FSWrap.obj GCMisc.obj Geom.obj GetFProp.obj GetGCVals.obj
OBJS1= \
GetStCmap.obj GetTxtProp.obj GetWMCMapW.obj GetWMProto.obj globals.obj \
Iconify.obj IfEvent.obj MaskEvent.obj NextEvent.obj OCWrap.obj OMWrap.obj \
ParseCmd.obj ParseGeom.obj PeekEvent.obj PeekIfEv.obj Pending.obj PixFormats.obj \
PolyReg.obj PropAlloc.obj PutBEvent.obj Quarks.obj RdBitF.obj Region.obj
OBJS2= \
RegstFlt.obj ScrResStr.obj SetBack.obj SetClMask.obj SetClOrig.obj SetFont.obj \
SetFore.obj SetFunc.obj SetHints.obj SetLocale.obj SetLStyle.obj SetNrmHint.obj \
SetPMask.obj SetRGBCMap.obj SetState.obj SetStCmap.obj SetStip.obj SetTile.obj \
SetTSOrig.obj SetTxtProp.obj SetWMCMapW.obj SetWMProto.obj StBytes.obj StName.obj
OBJS3= \
StrToText.obj TextExt.obj TextExt16.obj TextToStr.obj udcInf.obj VisUtil.obj \
WinEvent.obj Withdraw.obj WMGeom.obj WMProps.obj WrBitF.obj KeysymStr.obj

OBJDIR=$(STEM)

# don't specify all obj files in cleanup list,
# as addprefix will fail on such a long list (make_3_79_1)
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
CLEANUP1=$(addprefix $(OBJDIR_BS)\,$(OBJS1))
CLEANUP2=$(addprefix $(OBJDIR_BS)\,$(OBJS2))
CLEANUP3=$(addprefix $(OBJDIR_BS)\,$(OBJS3))
        
DLL=1
MT=1
LXLITE=1

# --- extra symbols for X11 source
CDEFS=-D__UNIXOS2__
XDEFS=-DXTHREADS -DX_LOCALE # -DXKB
C_FLAGS_USER=$(CDEFS) $(XDEFS)

# include configuration
BASEDIR=../../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) makefile
$(shell cmd /c @echo $(STARTMSG))

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(TARGET)
	@echo $(ENDMSG)

clean:
	@echo About to cleanup the target files ...
	-@for %a in ($(CLEANUP))  do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP1)) do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP2)) do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP3)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

# cannot use inference rule of rules.in here, as the long depedency list will blow up gmake

$(TARGET): $(OBJS) $(OBJS1) $(OBJS2) $(OBJS3) 
	@for %a in ($(OBJS))  do @(echo $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a & $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a)
	@for %a in ($(OBJS1)) do @(echo $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a & $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a)
	@for %a in ($(OBJS2)) do @(echo $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a & $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a)
	@for %a in ($(OBJS3)) do @(echo $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a & $(CLIB) r $(BINDIR_BS)\$@ $(OBJDIR_BS)\%a)
	@if exist  $(BINDIR_BS)\$*.bak del $(BINDIR_BS)\$*.bak

