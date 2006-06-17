# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h


# definitions for this module
STEM=emxhandl
TARGET=emxhandl.lib
DEF=$(STEM).def
DEF2=$(STEM)_exp.def
EXE=$(STEM).dll
OBJS=$(STEM).obj
OBJDIR=emxhandl
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(BINDIR_BS)\,$(EXE)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
DLL=1
MT=1
LXLITE=1

# for EMX compiler, -Zso requires -Zsys (not compatible with Innotek gcc)
L_FAGS_USER=-Zcrtdll

# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

# make sure this is compiled with emx version of gcc
DESCRIPTION=$(STEM) makefile
$(shell cmd /c @echo $(STARTMSG))

ifneq ("$(GCCTYPE)","gcc2")
$(shell cmd /c echo error in makefile: $(DESCRIPTION) makefile must be compiled with emx gcc $(TO_ERR))
$(shell cmd /c echo $(ENDMSG) $(TO_ERR))
	@echo $(ENDMSG)
	error
endif

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
	@emximp -o $(BINDIR)/$(STEM).lib $(DEF2)

$(EXE): $(OBJS) $(DEF)

