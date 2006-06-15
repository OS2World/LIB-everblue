#include "X11.h"

/*
 * Given a visual id, find the visual structure for this id on this display.
 */
Visual *_XVIDtoVisual (Display *dpy, VisualID id) {
	DBUG_ENTER("_XVIDtoVisual")
	register int i, j, k;
	register Screen *sp;
	register Depth *dp;
	register Visual *vp;
	for (i = 0; i < dpy->nscreens; i++) {
		sp = &dpy->screens[i];
		for (j = 0; j < sp->ndepths; j++) {
			dp = &sp->depths[j];
			/* if nvisuals == 0 then visuals will be NULL */
			for (k = 0; k < dp->nvisuals; k++) {
				vp = &dp->visuals[k];
				if (vp->visualid == id) DBUG_RETURN(vp);
			}
		}
	}
	DBUG_RETURN(NULL);
}
