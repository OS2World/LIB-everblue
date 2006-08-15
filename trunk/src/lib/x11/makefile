# makefile for creating the utility makekeys.exe
# to generate ks_tables.h from keysymdef.h

# definitions for this module
STEM=X11
TARGET=$(STEM).lib
EXE=$(STEM).dll
DEF=$(STEM).def
OBJS=$(STEM).obj \
SetClipRectangles.obj myKeyBind.obj myImUtil.obj CopyPlane.obj Keyboard.obj GetHints.obj \
locking.obj GetImage.obj Xim.obj PutImage.obj Misc.obj DrawText.obj Font.obj os2.obj \
NamedColor.obj Free.obj CmapRec.obj Pointer.obj Color.obj CopyArea.obj Window.obj 
OBJS1= \
Visual.obj XlibInt.obj Tree.obj Property.obj Cursor.obj Clear.obj Draw.obj ResizeWindow.obj \
MapWindow.obj FontAlias.obj FontNames.obj CreateWindow.obj QueryBest.obj Focus.obj \
Simple.obj myXrm.obj Pixmap.obj Stubs.obj NextEvent.obj Flush.obj Extension.obj \
OpenDisplay.obj uconv.obj Atom.obj Debug.obj GC.obj Context.obj NewDisplay.obj

OBJDIR=$(STEM)

LIBS=ebhook emxhandl xdaemon common 
CLIBS=xcms.lib xlibi18n.lib xmisc.lib


# don't specify all obj files in cleanup list,
# as addprefix will fail on such a long list (make_3_79_1)
CLEANUP= $(addprefix $(BINDIR_BS)\,$(TARGET)) \
         $(addprefix $(OBJDIR_BS)\,$(OBJS))
CLEANUP1=$(addprefix $(OBJDIR_BS)\,$(OBJS1))
        
DLL=1
IMPLIB=1
MT=1
LXLITE=1

# --- extra symbols for X11 source
# myXrm.c       __UNIXOS2__           (x.org)
# DrawText.c    OS2I18N               -> contitional compile aufl”sen
# Font.c        GDK_UC_FIX            -> font.h
# FontNames.c   FONTDPI               -> font.h
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85

XDEFS=-DXTHREADS -DX_LOCALE # -DXKB
C_FLAGS_USER=$(CDEFS) $(XDEFS)
CLIB_FLAGS_USER=-p128

# include configuration
BASEDIR=../../..
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in

DESCRIPTION=$(STEM) makefile
$(shell cmd /c @echo $(STARTMSG))

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# extend include path to headers
C_INCLUDE_PATH:=$(C_INCLUDE_PATH);$(BASEDIR)\src\xdaemon\app;$(BASEDIR)\src\lib\ebhook;

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- pseudo targets

all: $(TARGET) $(EXE)
	@echo $(ENDMSG)

clean:
	@echo About to cleanup the target files ...
	-@for %a in ($(CLEANUP))  do @(echo - %a & del %a /N $(TO_NUL))
	-@for %a in ($(CLEANUP1)) do @(echo - %a & del %a /N $(TO_NUL))
	@echo Done.
	@echo $(ENDMSG)

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

# --- dependencies and explicit rules

# have to define a explicit command here due to different name of the 2nd def file
$(TARGET): X11_exp.def
	$(ILIB) $(BINDIR)/$*.lib X11_exp.def

$(EXE): $(OBJS) $(OBJS1)

