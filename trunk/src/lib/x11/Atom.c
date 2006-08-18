#include "X11.h"

// this module provides functions for translating a String to an Atom (= int)
// XGetAtomName, XGetAtomNames, XInternAtom, XInternAtoms

// see include/XAtom.h
// see XServer/dix/initatoms.c
// see XServer/dix/BuiltInAtoms
// see xlib.pdf 4.3

// TODO: move to xdaemon (and with it add locking)
//       (atoms should be defined system-wide)
// Optionally: preinitialize AtomTbl at compile time


static char *AtomTbl[1024] = {"", NULL};
static int LastAtom = 1;
static char AtomBuf[65536], *AtomTail = AtomBuf;

char* atmNames[XA_LAST_PREDEFINED + 1] = { NULL,
	"PRIMARY",		"SECONDARY",		"ARC",			"ATOM",
	"BITMAP",		"CARDINAL",		"COLORMAP",		"CURSOR",
	"CUT_BUFFER0",		"CUT_BUFFER1",		"CUT_BUFFER2",		"CUT_BUFFER3",
	"CUT_BUFFER4",		"CUT_BUFFER5",		"CUT_BUFFER6",		"CUT_BUFFER7",
	"DRAWABLE",		"FONT",			"INTEGER",		"PIXMAP",
	"POINT",		"RECTANGLE",		"RESOURCE_MANAGER",	"RGB_COLOR_MAP",
	"RGB_BEST_MAP",		"RGB_BLUE_MAP",		"RGB_DEFAULT_MAP",	"RGB_GRAY_MAP",
	"RGB_GREEN_MAP",	"RGB_RED_MAP",		"STRING",		"VISUALID",
	"WINDOW",		"WM_COMMAND",		"WM_HINTS",		"WM_CLIENT_MACHINE",
	"ICON_NAME",		"ICON_SIZE",		"WM_NAME",		"WM_NORMAL_HINTS",
	"WM_SIZE_HINTS",	"WM_ZOOM_HINTS",	"MIN_SPACE",		"NORM_SPACE",
	"MAX_SPACE",		"END_SPACE",		"SUPERSCRIPT_X",	"SUPERSCRIPT_Y",
	"SUBSCRIPT_X",		"SUBSCRIPT_Y",		"UNDERLINE_POSITION",	"UNDERLINE_THICKNESS",
	"STRIKEOUT_ASCENT",	"STRIKEOUT_DESCENT",	"ITALIC_ANGLE",		"X_HEIGHT",
	"QUAD_WIDTH",		"WEIGHT",		"POINT_SIZE",		"RESOLUTION",
	"COPYRIGHT",		"NOTICE",		"FONT_NAME",		"FAMILY_NAME",
	"FULL_NAME",		"CAP_HEIGHT",		"WM_CLASS",		"WM_TRANSIENT_FOR"
};

char** atomNames = (char **)&atmNames;

char *XGetAtomName(Display *dpy, Atom atom) {
	DBUG_ENTER("XGetAtomName")
	char *name = NULL;

	if(atom > 0 && atom <= LastAtom)
		name = Xstrdup(AtomTbl[atom]);
	DBUG_RETURN(name);
}

Status XGetAtomNames(Display* dpy, Atom* atoms, int count, char** names_return) {
	DBUG_ENTER("XGetAtomNames")
	while(atoms && count && names_return) {
		*names_return = XGetAtomName(dpy, *atoms);
		count--; atoms++; names_return++;
	}
	DBUG_RETURN(True);
}

Atom XInternAtom(Display* dpy, _Xconst char* atom_name, Bool only_if_exists) {
	char **tbl = AtomTbl;
	int i = 0;

	if(atom_name) {
		while(i < LastAtom) {
			if(!strcmp(*tbl, atom_name)) {
				return (Atom)i;
			}
			i++; tbl++;
		}
		if(!only_if_exists) {
			strcpy(AtomTbl[i = LastAtom++] = AtomTail, atom_name);
			AtomTail += strlen(atom_name) + 1;
			return (Atom)i;
		}
	}
	return (Atom)None;
}

Status XInternAtoms(Display* dpy, char** names, int count, Bool onlyIfExists, Atom* atoms_return) {
	DBUG_ENTER("XInternAtoms")
	while(names && count) {
		Atom atom;
		atom = XInternAtom(dpy, *names, onlyIfExists);
		if(atoms_return)
			*(atoms_return++) = atom;
		count--;
		names++;
	}
	DBUG_RETURN(True);
}

void Xlib_InitAtoms(Display* dpy) {
	DBUG_ENTER("Xlib_InitAtoms")
	Atom i;

	for(i = 1; i <= XA_LAST_PREDEFINED; i++) {
		if(XInternAtom(dpy, atmNames[i], FALSE) == i)
			continue;
		fprintf(stderr,"Error initialising atoms!");
		abort();
	}
	DBUG_VOID_RETURN;
}
