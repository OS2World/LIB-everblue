##################################################################
#
# Xlib for Presentation Manager
#
##################################################################

CC=gcc
AS=as
MKLIB=ar r
MAKE=make
CP=cp
RM=rm -f
RC=rc
# If you are using gcc 3.0 change this entry to
EXT=.o 
# EXT=.obj

.SUFFIXES: .c $(EXT)

XDEFS=-DXTHREADS -DX_LOCALE # -DXKB
CDEFS=-D__UNIXOS2__ -DOS2I18N -DGDK_UC_FIX -DFONTDPI=85
CFLAGS=-Zmtd -Zomf -O3 -mpentium -Wall $(CDEFS) $(XDEFS)

# Uncomment the following lines to build a debug version.
# Will require ICSDEBUG or SD386 to debug in OMF mode

CFLAGS=-Zmt -Zomf -O -g -Wall -L/usr/lib -DDEBUG $(CDEFS) $(XDEFS)

##################################################################

XLIB_SRC=\
	FetchName.c GetHints.c GetNrmHint.c GetRGBCMap.c GetStCmap.c GetTxtProp.c GetWMCMapW.c GetWMProto.c Iconify.c mbWMProps.c ScrResStr.c SetHints.c SetNrmHint.c SetRGBCMap.c SetStCmap.c SetTxtProp.c SetWMCMapW.c SetWMProto.c StBytes.c StName.c StrToText.c TextToStr.c utf8WMProps.c Withdraw.c WMProps.c
XLIB_OBJ=\
	FetchName$(EXT) GetHints$(EXT) GetNrmHint$(EXT) GetRGBCMap$(EXT) GetStCmap$(EXT) GetTxtProp$(EXT) GetWMCMapW$(EXT) GetWMProto$(EXT) Iconify$(EXT) mbWMProps$(EXT) ScrResStr$(EXT) SetHints$(EXT) SetNrmHint$(EXT) SetRGBCMap$(EXT) SetStCmap$(EXT) SetTxtProp$(EXT) SetWMCMapW$(EXT) SetWMProto$(EXT) StBytes$(EXT) StName$(EXT) StrToText$(EXT) TextToStr$(EXT) utf8WMProps$(EXT) Withdraw$(EXT) WMProps$(EXT)

##################################################################

all: $(XLIB_OBJ)

clean:
	$(RM) $(XLIB_OBJ)

##################################################################

.c$(EXT):
	$(CC) -c $(CFLAGS) $<

.s$(EXT):
	$(CC) -c $(CFLAGS) $<

# END OF MAKEFILE
