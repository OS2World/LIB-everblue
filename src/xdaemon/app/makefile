# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h


# definitions for this module
STEM=xdaemon
DEF=$(STEM).def
EXE=$(STEM).exe
OBJS=$(STEM).obj InitializePM.obj PMWM.obj \
ProcBorder.obj ProcFrame.obj ProcHookmsg.obj ProcMessage.obj ProcWindow.obj \
ShutDownPM.obj CalcDelta.obj ProcInput.obj CursorMovement.obj \
UMCreatePointer.obj UMCreateWindow.obj UMDestroyWindow.obj UMFreeResources.obj \
UMMapWindow.obj UMOpenPipe.obj UMSetWindowPos.obj

OBJDIR=xdaemon_exe
CLEANUP= $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))

MT=1
LXLITE=1
PMPRINTF=1
LIBS=xdaemon ebhook common


# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) app makefile
$(shell cmd /c @echo $(STARTMSG))

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# extend include path to headers
C_INCLUDE_PATH:=$(C_INCLUDE_PATH);..\..\lib\ebhook;..\..\lib\x11;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(EXE)
	@echo $(ENDMSG)

clean:
	@echo About to cleanup the target files ...
	-@for %a in ($(CLEANUP)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

$(STEM).exe: $(OBJS) $(DEF)

