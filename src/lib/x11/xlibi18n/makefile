# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=xlibi18n
TARGET=$(STEM).lib
DEF=$(STEM).def
OBJS=$(STEM).obj \
ICWrap.obj imKStoUCS.obj IMWrap.obj lcCharSet.obj lcConv.obj lcCT.obj lcDB.obj \
lcDynamic.obj lcFile.obj lcGeneric.obj lcInit.obj lcPrTxt.obj lcPublic.obj lcPubWrap.obj
OBJS1= \
lcRM.obj lcStd.obj lcTxtPr.obj lcUTF8.obj lcUtil.obj lcWrap.obj mbWMProps.obj mbWrap.obj \
utf8WMProps.obj utf8Wrap.obj wcWrap.obj XDefaultIMIF.obj XDefaultOMIF.obj XlcDL.obj
OBJS2= \
XlcSL.obj lcDefConv.obj lcEuc.obj lcGenConv.obj lcJis.obj lcSjis.obj lcUTF8Load.obj \
omDefault.obj omGeneric.obj omImText.obj omText.obj omTextEsc.obj omTextExt.obj \
omTextPer.obj omXChar.obj

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
CLIB_FLAGS_USER=-p128

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
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

$(TARGET): $(OBJS) $(OBJS1) $(OBJS2)

