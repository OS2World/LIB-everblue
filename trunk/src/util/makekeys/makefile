# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h


# definitions for this module
TARGET=ks_tables.h
STEM=makekeys
DEF=$(STEM).def

EXE=$(STEM).exe
OBJS=$(STEM).obj
OBJDIR=$(STEM)
CLEANUP= $(addprefix $(CMPDIR_BS)\,$(TARGET)) \
         $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
LXLITE=1


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

# no inference rule available for this special action
$(TARGET): keysymdef.h  $(STEM).exe
	@echo generating $(CMPDIR)/$*
	@makekeys < keysymdef.h > $(CMPDIR_BS)/$(TARGET)

$(STEM).exe: $(OBJS) $(DEF)

