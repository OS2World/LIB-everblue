#include "X11.h"

void _XSetClipRectangles(register Display *dpy, GC gc, int clip_x_origin,
		int clip_y_origin, XRectangle *rectangles, int n, int ordering) {
	DBUG_ENTER("_XSetClipRectangles")
    unsigned long dirty;
    register _XExtension *ext;

	EB_Rectangles *ebr = Xmalloc(sizeof(EB_Rectangles));
	ebr->size = n;
	ebr->rectangles = Xmalloc(sizeof(XRectangle) * n);
	memcpy(ebr->rectangles, rectangles, sizeof(XRectangle) * n);
	EB_Resource *ebres = createResource(EBRECTANGLES, ebr);
	addResource(&process->ebprocess->res, ebres);

	gc->values.clip_x_origin = clip_x_origin;
	gc->values.clip_y_origin = clip_y_origin;
	gc->values.clip_mask = (Pixmap)ebres;
	gc->rects = 1;

	dirty = gc->dirty & ~(GCClipMask | GCClipXOrigin | GCClipYOrigin);
	gc->dirty = GCClipMask | GCClipXOrigin | GCClipYOrigin;
	/* call out to any extensions interested */
	for(ext = dpy->ext_procs; ext; ext = ext->next)
		if(ext->flush_GC)
			(*ext->flush_GC)(dpy, gc, &ext->codes);
	gc->dirty = dirty;

	DBUG_VOID_RETURN;
}

int XSetClipRectangles(register Display *dpy, GC gc, int clip_x_origin,
		int clip_y_origin, XRectangle *rectangles, int n, int ordering) {
	DBUG_ENTER("XSetClipRectangles")
	DBUG_RETURN(1);
	LockDisplay(dpy);
	_XSetClipRectangles (dpy, gc, clip_x_origin, clip_y_origin, rectangles, n,
			ordering);
	UnlockDisplay(dpy);
	SyncHandle();
	DBUG_RETURN(1);
}
