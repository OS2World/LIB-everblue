# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h


# definitions for this module
STEM=xdaemon
TARGET=$(STEM).lib
DEF=$(STEM).def
EXE=$(STEM).dll
RES=$(STEM).res
OBJS=$(STEM).obj Drawable.obj EventMask.obj EventQueue.obj GC.obj Grab.obj \
HeapShared.obj InitTerm.obj message.obj register.obj Resource.obj \
stubs.obj Thread.obj Window.obj WindowHash.obj WindowTools.obj Pointer.obj

OBJDIR=xdaemon_dll
CLEANUP= $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS)) \
         $(addprefix $(CMPDIR_BS)\,$(RES))
DLL=1
MT=1
LXLITE=1
IMPLIB=1
LIBS=ebhook common

# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) lib makefile
$(shell cmd /c @echo $(STARTMSG))

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# extend include path to headers
C_INCLUDE_PATH:=$(C_INCLUDE_PATH);$(BASEDIR)\src\lib\ebhook;$(BASEDIR)\src\lib\x11;..\app;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(TARGET)
	@echo $(ENDMSG)

clean:
	@echo About to cleanup the target files ...
	-@for %a in ($(CLEANUP)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

$(TARGET): $(EXE)

$(EXE): $(OBJS) $(DEF) $(RES)

