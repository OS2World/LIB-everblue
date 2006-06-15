#include "X11.h"

#if 0
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
#endif

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

int _XUpdateGCCache (register GC gc, register unsigned long mask, register XGCValues *attr) {
	DBUG_ENTER("_XUpdateGCCache")
	register XGCValues *gv = &gc->values;

	if (mask & GCFunction)
		if (gv->function != attr->function) {
			gv->function = attr->function;
			gc->dirty |= GCFunction;
		}
	
	if (mask & GCPlaneMask)
		if (gv->plane_mask != attr->plane_mask) {
			gv->plane_mask = attr->plane_mask;
			gc->dirty |= GCPlaneMask;
		}

	if (mask & GCForeground)
		if (gv->foreground != attr->foreground) {
			gv->foreground = attr->foreground;
			gc->dirty |= GCForeground;
		}

	if (mask & GCBackground)
		if (gv->background != attr->background) {
			gv->background = attr->background;
			gc->dirty |= GCBackground;
		}

	if (mask & GCLineWidth)
		if (gv->line_width != attr->line_width) {
			gv->line_width = attr->line_width;
			gc->dirty |= GCLineWidth;
		}

	if (mask & GCLineStyle)
		if (gv->line_style != attr->line_style) {
			gv->line_style = attr->line_style;
			gc->dirty |= GCLineStyle;
		}

	if (mask & GCCapStyle)
		if (gv->cap_style != attr->cap_style) {
			gv->cap_style = attr->cap_style;
			gc->dirty |= GCCapStyle;
		}
	
	if (mask & GCJoinStyle)
		if (gv->join_style != attr->join_style) {
			gv->join_style = attr->join_style;
			gc->dirty |= GCJoinStyle;
		}

	if (mask & GCFillStyle)
		if (gv->fill_style != attr->fill_style) {
			gv->fill_style = attr->fill_style;
			gc->dirty |= GCFillStyle;
		}

	if (mask & GCFillRule)
		if (gv->fill_rule != attr->fill_rule) {
			gv->fill_rule = attr->fill_rule;
			gc->dirty |= GCFillRule;
	}

	if (mask & GCArcMode)
		if (gv->arc_mode != attr->arc_mode) {
		gv->arc_mode = attr->arc_mode;
		gc->dirty |= GCArcMode;
	}

	/* always write through tile change, since client may have changed pixmap contents */
	if (mask & GCTile) {
		gv->tile = attr->tile;
		gc->dirty |= GCTile;
	}

	/* always write through stipple change, since client may have changed pixmap contents */
	if (mask & GCStipple) {
		gv->stipple = attr->stipple;
		gc->dirty |= GCStipple;
	}

	if (mask & GCTileStipXOrigin)
		if (gv->ts_x_origin != attr->ts_x_origin) {
			gv->ts_x_origin = attr->ts_x_origin;
			gc->dirty |= GCTileStipXOrigin;
		}

	if (mask & GCTileStipYOrigin)
		if (gv->ts_y_origin != attr->ts_y_origin) {
			gv->ts_y_origin = attr->ts_y_origin;
			gc->dirty |= GCTileStipYOrigin;
		}

	if (mask & GCFont)
		if (gv->font != attr->font) {
			gv->font = attr->font;
			gc->dirty |= GCFont;
		}

	if (mask & GCSubwindowMode)
		if (gv->subwindow_mode != attr->subwindow_mode) {
			gv->subwindow_mode = attr->subwindow_mode;
			gc->dirty |= GCSubwindowMode;
		}

	if (mask & GCGraphicsExposures)
		if (gv->graphics_exposures != attr->graphics_exposures) {
			gv->graphics_exposures = attr->graphics_exposures;
			gc->dirty |= GCGraphicsExposures;
		}

	if (mask & GCClipXOrigin)
		if (gv->clip_x_origin != attr->clip_x_origin) {
			gv->clip_x_origin = attr->clip_x_origin;
			gc->dirty |= GCClipXOrigin;
		}

	if (mask & GCClipYOrigin)
		if (gv->clip_y_origin != attr->clip_y_origin) {
			gv->clip_y_origin = attr->clip_y_origin;
			gc->dirty |= GCClipYOrigin;
		}

	/* always write through mask change, since client may have changed pixmap contents */
	if (mask & GCClipMask) {
		gv->clip_mask = attr->clip_mask;
		gc->dirty |= GCClipMask;
		gc->rects = 0;
	}

	if(mask & GCDashOffset)
		if(gv->dash_offset != attr->dash_offset) {
			gv->dash_offset = attr->dash_offset;
			gc->dirty |= GCDashOffset;
		}

	if(mask & GCDashList)
		if((gv->dashes != attr->dashes) || (gc->dashes == True)) {
			gv->dashes = attr->dashes;
			gc->dirty |= GCDashList;
			gc->dashes = 0;
		}
	DBUG_RETURN(0);
}

int XCopyGC (register Display *dpy, GC srcGC, unsigned long mask, GC destGC) {
	DBUG_ENTER("XCopyGC")
	register _XExtension *ext;
	int result;

	mask &= (1L << (GCLastBit + 1)) - 1;

	if(!mask)
		DBUG_RETURN(0);

	LockDisplay(dpy);

	result = XChangeGC(dpy, destGC, mask, &srcGC->values);

	/* call out to any extensions interested */
	for(ext = dpy->ext_procs; ext; ext = ext->next)
		if(ext->copy_GC)
			(*ext->copy_GC)(dpy, destGC, &ext->codes);
	UnlockDisplay(dpy);
	SyncHandle();
	DBUG_RETURN(result);
}

GContext XGContextFromGC(GC gc)
{ 
   DBUG_ENTER("GContextFromGC");
   DBUG_RETURN(gc->gid); 
}

#if 0
int XSetForeground(Display* display, GC gc, unsigned long foreground) {
	DBUG_ENTER("XSetForeground")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	gc->values.foreground = foreground;
	DBUG_RETURN(True);
}

int XSetBackground(Display* display, GC gc, unsigned long background) {
	DBUG_ENTER("XSetBackground")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	gc->values.background = background;
	DBUG_RETURN(True);
}

int XSetLineAttributes(Display* display, GC gc, unsigned int line_width,
		int line_style, int cap_style, int join_style) {
	DBUG_ENTER("XSetLineAttributes")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);

	if(!ebgc)
		DBUG_RETURN(False);
	gc->values.join_style = join_style;
	gc->values.cap_style = cap_style;
	gc->values.line_style = line_style;
	gc->values.line_width = line_width;
	DBUG_RETURN(True);
}

int XSetFunction(Display* display, GC gc, int function) {
	DBUG_ENTER("XSetFunction")
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);
	if(!ebgc)
		DBUG_RETURN(False);
	gc->values.function = function;
	DBUG_RETURN(True);
}
#endif
