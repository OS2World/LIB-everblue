# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=ebhook
TARGET=$(STEM).lib
DEF=$(STEM).def
EXE=$(STEM).dll
OBJS=$(STEM).obj crt3.obj access.obj
OBJDIR=$(STEM)
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
DLL=1
ST=1
L_FAGS_USER=-Zno-fork -nodefaultlibs
IMPLIB=1
LXLITE=1
LIBS=libos2 common

# include configuration
BASEDIR=../../..
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
	-@for %a in ($(CLEANUP)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

$(TARGET): $(EXE)

$(EXE): $(OBJS) $(DEF)

access.obj: access.h

ebhook.obj: ebhook.c ebhook.h access.h


