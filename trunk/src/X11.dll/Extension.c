#include "X11.h"

// server based extensions not supported
// for local ones XAddExtension() is used
// perhaps this function must be changed to simulate extensions

XExtCodes *XInitExtension(Display *dpy, _Xconst char *name) {
	DBUG_ENTER("XInitExtension")
	printf("Program would use extension %s\n", name);
	DBUG_RETURN(NULL);
}

Bool XQueryExtension(register Display *dpy, _Xconst char *name,
	int *major_opcode, int *first_event, int *first_error) {       
	DBUG_ENTER("XQueryExtension")
	fprintf(stderr,"XQueryExtension: '%s'\n",name);
	*major_opcode = 0;
	*first_event = 0;
	*first_error = 0;
	DBUG_RETURN(False);
}

char **XListExtensions(Display *dpy, int *nextensions) {
	*nextensions = 0;
	return(NULL);
}

int XFreeExtensionList (char **list) {
	return 1;
}
