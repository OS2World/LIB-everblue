#include "X11.h"

void AssociateGC(HPS prevhps, HPS hps, HRGN clip_mask)
{
	DBUG_ENTER("AssociateGC")

	GpiSetClipRegion(prevhps, NULLHANDLE, NULL);
	GpiSetClipRegion(hps, clip_mask, NULL);
	DBUG_VOID_RETURN;
}

void EndPath(int path, HPS hps, XGCValues *gcv)
{
	DBUG_ENTER("XFlushGC")
	switch(path) {
	case GC_FILLPATH:
		GpiEndPath(hps);
		GpiFillPath(hps, 1L, gcv->fill_rule == WindingRule ?
			FPATH_WINDING : FPATH_ALTERNATE );
		break;
	case GC_STROKEPATH:
		GpiEndPath(hps);
		GpiStrokePath(hps, 1L, 0);
		break;
	case GC_AREAFILL:
		GpiEndArea(hps);
		break;
	}
	path = GC_NOPATH;
	DBUG_VOID_RETURN;
}

GC XCreateGC(Display* display, Drawable d, unsigned long valuemask, XGCValues* values) {
	DBUG_ENTER("XCreateGC")
	EB_GContext *ebgc = Xcalloc(1, sizeof(EB_GContext));
	EB_Resource *gcres = createResource(EBGCONTEXT, ebgc);
	ebgc->xgc.gid = (GContext)gcres;
	addResource(&process->ebprocess->res, gcres);

	ebgc->xgc.values = stdgcvalues;

	if (values && valuemask) {
		if (valuemask & GCFunction) ebgc->xgc.values.function = values->function;
		if (valuemask & GCForeground) ebgc->xgc.values.foreground = values->foreground;
		if (valuemask & GCBackground) ebgc->xgc.values.background = values->background;
		if (valuemask & GCLineWidth) ebgc->xgc.values.line_width = values->line_width;
		if (valuemask & GCLineStyle) ebgc->xgc.values.line_style = values->line_style;
		if (valuemask & GCCapStyle)  ebgc->xgc.values.cap_style  = values->cap_style;
		if (valuemask & GCJoinStyle) ebgc->xgc.values.join_style = values->join_style;
		if (valuemask & GCFillStyle) ebgc->xgc.values.fill_style = values->fill_style;
		if (valuemask & GCFillRule)  ebgc->xgc.values.fill_rule  = values->fill_rule;
		if (valuemask & GCFont)      ebgc->xgc.values.font       = values->font;
		if (valuemask & GCArcMode)   ebgc->xgc.values.arc_mode   = values->arc_mode;
		if (valuemask & GCStipple)   ebgc->xgc.values.stipple    = values->stipple;
	}

	if(ebgc->xgc.values.font)
		monitorResource((EB_Resource *)ebgc->xgc.values.font);
	if(ebgc->xgc.values.stipple)
		monitorResource((EB_Resource *)ebgc->xgc.values.stipple);

	DBUG_RETURN(&ebgc->xgc);
}

int XSetForeground(Display* display, GC gc, unsigned long foreground) {
	DBUG_ENTER("XSetForeground")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	ebgc->xgc.values.foreground = foreground;
	DBUG_RETURN(True);
}

int XSetBackground(Display* display, GC gc, unsigned long background) {
	DBUG_ENTER("XSetBackground")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	ebgc->xgc.values.background = background;
	DBUG_RETURN(True);
}

int XSetLineAttributes(Display* display, GC gc, unsigned int line_width,
		int line_style, int cap_style, int join_style) {
	DBUG_ENTER("XSetLineAttributes")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);

	if(!ebgc)
		DBUG_RETURN(False);
	ebgc->xgc.values.join_style = join_style;
	ebgc->xgc.values.cap_style = cap_style;
	ebgc->xgc.values.line_style = line_style;
	ebgc->xgc.values.line_width = line_width;
	DBUG_RETURN(True);
}

int XSetFunction(Display* display, GC gc, int function) {
	DBUG_ENTER("XSetFunction")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	ebgc->xgc.values.function = function;
	DBUG_RETURN(True);
}
