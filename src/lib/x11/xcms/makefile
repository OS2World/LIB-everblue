# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=xcms
TARGET=$(STEM).lib
DEF=$(STEM).def
OBJS=$(STEM).obj \
AddDIC.obj AddSF.obj CCC.obj cmsAllCol.obj cmsColNm.obj cmsGlobls.obj cmsInt.obj \
cmsMath.obj cmsProp.obj cmsTrig.obj CvCols.obj CvColW.obj HVC.obj HVCGcC.obj HVCGcV.obj
OBJS1= \
HVCGcVC.obj HVCMnV.obj HVCMxC.obj HVCMxV.obj HVCMxVC.obj HVCMxVs.obj HVCWpAj.obj IdOfPr.obj \
Lab.obj LabGcC.obj LabGcL.obj LabGcLC.obj LabMnL.obj LabMxC.obj LabMxL.obj LabMxLC.obj \
LabWpAj.obj LRGB.obj Luv.obj LuvGcC.obj LuvGcL.obj LuvGcLC.obj LuvMnL.obj LuvMxC.obj
OBJS2= \
LuvMxL.obj LuvMxLC.obj LuvWpAj.obj OfCCC.obj PrOfId.obj QBlack.obj QBlue.obj QGreen.obj \
QRed.obj QuCol.obj QuCols.obj QWhite.obj SetCCC.obj SetGetCols.obj StCol.obj StCols.obj \
UNDEFINED.obj uvY.obj XRGB.obj xyY.obj XYZ.obj

OBJDIR=$(STEM)

# don't specify all obj files in cleanup list,
# as addprefix will fail on such a long list (make_3_79_1)
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
CLEANUP1=$(addprefix $(OBJDIR_BS)\,$(OBJS1))
CLEANUP2=$(addprefix $(OBJDIR_BS)\,$(OBJS2))
        
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

# extend include path to headers
C_INCLUDE_PATH:=$(C_INCLUDE_PATH);$(BASEDIR)\src\include\common;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(TARGET)
	@echo $(ENDMSG)

clean:
	@echo About to cleanup the target files ...
	-@for %a in ($(CLEANUP))  do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP1)) do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP2)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

$(TARGET): $(OBJS) $(OBJS1) $(OBJS2)

