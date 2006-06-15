#include "X11.h"

void EMXFree(void *data);

int _XFree(void *data) {
	DBUG_ENTER("XFree")
	if(data) {
		if(data < (void *)0x30000000) // TODO - bad hack
			EMXFree(data);    // workaround for support libaries,
		else                  // which use old EMX malloc with XFree
			sfree(data);
		DBUG_RETURN(True);
	} else
		DBUG_RETURN(False);
}

int XFree(void *data) {
	DBUG_ENTER("XFree")
	int ret = _XFree(data);
	DBUG_RETURN(ret);
}
