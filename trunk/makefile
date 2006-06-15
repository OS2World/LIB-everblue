
# top level GNU makefile for Everblue

# include configuration
BASEDIR=.
PRJINC=$(BASEDIR)/config/project.in
include $(BASEDIR)/config/rules.in


# Reihenfolge:  ebhandle (gcc2), makekeys, ebhook   X11.dll, xdaemon

# --- module lists for separate versions of gcc
#     module names are the subdirectory below src directory

ifeq ("$(GCCTYPE)","gcc2")
LIB_MODULES=lib\emxhandl
endif

ifeq ("$(GCCTYPE)","gcc3")
UTL_MODULES=util\makekeys
LIB_MODULES=lib\common lib\ebhook
XDM_MODULES=xdaemon\lib xdaemon\app
X11_MODULES=lib\x11\xcms lib\x11\xlibi18n lib\x11\Xmisc lib\x11
SAM_MODULES=samples\test
endif

# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

# --- list all phony targets

.PHONY: MODULE rel deb all util lib xdaemon x11 sam clean

# --- default targets

# - generic default target for building a module

ifdef MODULE
MODULE:
	@$(MAKE) -C src\$(MODULE) $(ARG)
	@cmd /C echo.
endif

# --- compile the world
all: util lib xdaemon x11 sam

# --- compile release version
rel:
	@$(MAKE) all NDEBUG=1

# --- compile debug version
deb:
	@$(MAKE) all DEBUG=1


# --- compile debug version and run debugger
debug: deb
	@cmd /C vadeb


# --- compile targets

util:
ifneq ("$(UTL_MODULES)","")
	@for %a in ($(UTL_MODULES)) do @($(MAKE) $(ARG) MODULE=%a ARG=all)
endif

lib: util
ifneq ("$(LIB_MODULES)","")
	@for %a in ($(LIB_MODULES)) do @($(MAKE) $(ARG) MODULE=%a ARG=all)
endif

xdaemon: util lib
ifneq ("$(XDM_MODULES)","")
	@for %a in ($(XDM_MODULES)) do @($(MAKE) $(ARG) MODULE=%a ARG=all)
endif

x11: util lib xdaemon
ifneq ("$(X11_MODULES)","")
	@for %a in ($(X11_MODULES)) do @($(MAKE) $(ARG) MODULE=%a ARG=all)
endif

sam: util lib xdaemon x11
ifneq ("$(SAM_MODULES)","")
	@for %a in ($(SAM_MODULES)) do @($(MAKE) $(ARG) MODULE=%a ARG=all)
endif

# --- this target to cleanup

clean:
	@echo About to cleanup the target directories ...
	-@for %a in ($(DIRSTOCLEAN)) do @(echo - %a & del %a\* /N $(TO_NUL) & rd %a $(TO_NUL))
	@echo Done.

