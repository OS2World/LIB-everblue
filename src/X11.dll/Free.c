#include "X11.h"

int XFree(void *data) {
	DBUG_ENTER("XFree")
	if(data) {
		free(data);
		DBUG_RETURN(True);
	} else
		DBUG_RETURN(False);
}
