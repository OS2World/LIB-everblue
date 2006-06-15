# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h


# definitions for this module
STEM=test
DEF=$(STEM).def
EXE=$(STEM).exe
OBJS=$(STEM).obj
OBJDIR=$(STEM)
CLEANUP= $(addprefix $(CMPDIR_BS)\,$(TARGET)) \
         $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
LXLITE=1
LIBS=x11


# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) makefile
$(shell cmd /c @echo $(STARTMSG))

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

$(STEM).obj: $(STEM).c icone.inc
