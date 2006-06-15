# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=common
TARGET=$(STEM).lib
DEF=$(STEM).def
OBJS=$(STEM).obj \
sem.obj

OBJDIR=$(STEM)

# don't specify all obj files in cleanup list,
# as addprefix will fail on such a long list (make_3_79_1)
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
CLEANUP1=$(addprefix $(OBJDIR_BS)\,$(OBJS1))
CLEANUP2=$(addprefix $(OBJDIR_BS)\,$(OBJS2))
        
MT=1
LXLITE=1

# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) makefile
$(shell cmd /c @echo $(STARTMSG))

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(TARGET) printf.obj
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

# -- exclitit rule to place obj in main binary dir
printf.obj: printf.c
	$(CC) $(C_FLAGS) -o $(BINDIR)/$*.obj $*.c $(GCC_FILTER)

