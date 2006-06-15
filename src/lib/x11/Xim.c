#include "X11.h"
#include "Xmisc/Xlcint.h"

static XIM _OpenIM(XLCd lcd, Display *display, XrmDatabase rdb,
		char *res_name, char *res_class) {
	DBUG_ENTER("_OpenIM")
	DBUG_RETURN(NULL);
}

Bool _XInitIM(XLCd lcd) {
	DBUG_ENTER("_XInitIM")
    lcd->methods->open_im = _OpenIM;
    lcd->methods->register_callback = NULL;
    lcd->methods->unregister_callback = NULL;
	DBUG_RETURN(True);
}
