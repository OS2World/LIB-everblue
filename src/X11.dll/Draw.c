#include "X11.h"

USHORT LineStyletoLineType[3] = {
	LINETYPE_SOLID,		/* LineSolid */
	LINETYPE_SHORTDASH,	/* LineOnOffDash */
	LINETYPE_LONGDASH,	/* LineDoubleDash */
};

int XDrawLine(Display* display, Drawable d, GC gc, int x1, int y1, int x2, int y2) {
	DBUG_ENTER("XDrawLine")
	POINTL ptl;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	GpiBeginPath(ebhps->hps, 1);
	ptl.x = x1; ptl.y = y1;
	GpiMove(ebhps->hps, &ptl);
	ptl.x = x2; ptl.y = y2;
	GpiLine(ebhps->hps, &ptl);
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XFillPolygon(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int shape, int mode) {
	DBUG_ENTER("XFillPolygon")
	POLYGON poly;
	int i;
	int rely = 0;
	int relx = 0;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	if(!points || ((poly.ulPoints = npoints) < 2) || 
			!(poly.aPointl = alloca(sizeof(POINTL) * npoints)))
		DBUG_RETURN(False);

	for(i = 0; i < npoints; i++) {
		poly.aPointl[i].x = points[i].x + relx;
		poly.aPointl[i].y = points[i].y + rely;
		if(mode == CoordModePrevious) {
			relx = poly.aPointl[i].x;
			rely = poly.aPointl[i].y;
		}
	}
	GpiMove(ebhps->hps, &poly.aPointl[npoints-1]);
	GpiBeginArea(ebhps->hps, BA_NOBOUNDARY | (gc->values.fill_rule == WindingRule ?
			BA_WINDING : BA_ALTERNATE ) | BA_EXCL);
	GpiPolyLine(ebhps->hps, poly.ulPoints, poly.aPointl);
	GpiEndArea(ebhps->hps);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawLines(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int mode) {
	DBUG_ENTER("XDrawLines")
	POINTL *pptl;
	int rely = 0;
	int i = 1;
	int relx = 0;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	if (!npoints || !points || !(pptl = alloca(sizeof(POINTL) * npoints)))
		DBUG_RETURN(False);
	for (i = 0; i < npoints; i++) {
		pptl[i].x = points[i].x + relx;
		pptl[i].y = points[i].y + rely;
		if(mode == CoordModePrevious) {
			relx = pptl[i].x;
			rely = pptl[i].y;
		}
	}
	GpiBeginPath(ebhps->hps, 1);
	GpiMove(ebhps->hps, pptl);
	GpiPolyLine(ebhps->hps, npoints, pptl);
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawSegments(Display* display, Drawable d, GC gc, XSegment* segments, int nsegments) {
	DBUG_ENTER("XDrawSegments")
	POINTL ptl;
	int i = 0;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	if(!nsegments || !segments)
		DBUG_RETURN(False);
	GpiBeginPath(ebhps->hps, 1);
	while(i < nsegments) {
		ptl.x = segments->x1; ptl.y = segments->y1;
		GpiMove(ebhps->hps, &ptl);
		ptl.x = segments->x2; ptl.y = segments->y2;
		GpiLine(ebhps->hps, &ptl);
		i++; segments++;
	}
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XFillArc(Display* display, Drawable d, GC gc, int x, int y,
	     unsigned int width, unsigned int height, int angle1, int angle2) {
	DBUG_ENTER("XFillArc")
	POINTL lPtCenter, lStartPt, lEndPt;
	ARCPARAMS arcparms;
	FIXED fxStartAngle, fxSweepAngle;
	int reverse = 0;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	arcparms.lR = arcparms.lS = 0L;
	lPtCenter.x = x + (arcparms.lP = width / 2L);
	lPtCenter.y = y + (arcparms.lQ = height / 2L);
	// fix for GPIEnableYInversion
	angle1 = -angle1;
	angle2 = -angle2;
	if(angle2 > 23040)
		angle2 = 23040;
	if(angle2 < -23040)
		angle2 = -23040;
	if(angle2 < 0) {
		angle1 -= (angle2 = -angle2);
		reverse = 1;
	};	
	while(angle1 < 0)
		angle1 += 23040;
	fxStartAngle = MAKEFIXED(angle1, 0) / 64;
	fxSweepAngle = MAKEFIXED(angle2, 0) / 64;

	GpiSetLineType(ebhps->hps, LINETYPE_INVISIBLE);
	GpiMove(ebhps->hps, &lPtCenter);
	GpiSetArcParams(ebhps->hps, (PARCPARAMS)&arcparms);
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, MAKEFIXED(0,0));
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lStartPt);
	GpiSetLineType(ebhps->hps, LineStyletoLineType[gc->values.line_style]);

	GpiBeginArea(ebhps->hps, BA_NOBOUNDARY | (gc->values.fill_rule == WindingRule ?
			BA_WINDING : BA_ALTERNATE ) | BA_EXCL);
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, fxSweepAngle);
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lEndPt);
	if(gc->values.arc_mode == ArcPieSlice)
		GpiLine(ebhps->hps, &lPtCenter);
	GpiLine(ebhps->hps, &lStartPt);
	GpiEndArea(ebhps->hps);

	if(reverse)
		GpiMove(ebhps->hps, (PPOINTL)&lStartPt);
	GpiMove(ebhps->hps, (PPOINTL)&lEndPt);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(False);
}

int XFillRectangle(Display* display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height) {
	DBUG_ENTER("XFillRectangle")
	POINTL ptl;
	int rc;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	// hack for rdesktop
	// has to be moved to getCachedHPS
	if(gc->values.fill_style == FillStippled || gc->values.fill_style == FillOpaqueStippled) {
		XCopyArea(display, gc->values.stipple, d, gc, 0, 0, width, height, x, y);
		DBUG_RETURN(True);
	}

	ptl.x = x;
	ptl.y = y;
	rc = GpiMove(ebhps->hps, &ptl);
	ptl.x += width - 1;
	ptl.y += height - 1;
	rc |= GpiBox(ebhps->hps, DRO_FILL, &ptl, 0, 0);
	finishedDrawing(d, ebhps);

	if(rc == GPI_OK)
		DBUG_RETURN(True);
	DBUG_RETURN(False);
}

int XDrawPoint(Display* display, Drawable d, GC gc, int x, int y) {
	DBUG_ENTER("XDrawPoint")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL ptl;
	ptl.x = x;
	ptl.y = y;
	GpiSetPel(ebhps->hps, &ptl);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawPoints(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int mode) {
	DBUG_ENTER("XDrawPoints")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL ptl;
	int i = 0;
	int relx = 0;
	int rely = 0;

	if(!npoints || !points)
		DBUG_RETURN(False);

	while (i < npoints) {
		ptl.x = points->x + relx; 
		ptl.y = points->y + rely;
		GpiSetPel(ebhps->hps, &ptl);
		if(mode == CoordModePrevious) {
			relx = ptl.x;
			rely = ptl.y;
		}
		i++; points++;
	}
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawRectangle(Display* display, Drawable d, GC gc, int x, int y, 
		unsigned int width, unsigned int height) {
	DBUG_ENTER("XDrawRectangle")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL ptl;
	ptl.x = x; 
	ptl.y = y;
	GpiBeginPath(ebhps->hps, 1);
	GpiMove(ebhps->hps, &ptl);
	ptl.x += width - 1;
	ptl.y += height - 1;
	GpiBox(ebhps->hps, DRO_OUTLINE, &ptl, 0, 0);
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawRectangles(Display* display, Drawable d, GC gc, XRectangle* rectangles, int nrectangles) {
	DBUG_ENTER("XDrawRectangles")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL ptl;
	int i = 0;
	if(!nrectangles || !rectangles)
		DBUG_RETURN(False);
	GpiBeginPath(ebhps->hps, 1);
	while (i < nrectangles) {
		ptl.x = rectangles->x;
		ptl.y = rectangles->y;
		GpiMove(ebhps->hps, &ptl);
		ptl.x += rectangles->width - 1;
		ptl.y += rectangles->height - 1;
		GpiBox(ebhps->hps, DRO_OUTLINE, &ptl, 0, 0);
		i++;
		rectangles++;
	}
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}

int XDrawArc(Display* display, Drawable d, GC gc, int x, int y,
	     unsigned int width, unsigned int height, int angle1, int angle2) {
	DBUG_ENTER("XDrawArc")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL lPtCenter, lStartPt, lEndPt;
	ARCPARAMS arcparms;
	FIXED fxStartAngle, fxSweepAngle;
	int reverse = 0;
	arcparms.lR = arcparms.lS = 0L;
	lPtCenter.x = x + (arcparms.lP = width / 2L);
	lPtCenter.y = y + (arcparms.lQ = height / 2L);
	// fix for GPIEnableYInversion
	angle1 = -angle1;
	angle2 = -angle2;
	if(angle2 > 23040)
		angle2 = 23040;
	if(angle2 < -23040)
		angle2 = -23040;
	if(angle2 < 0) {
		angle1 -= (angle2 = -angle2);
		reverse = 1;
	};
	while(angle1 < 0)
		angle1 += 23040;
	fxStartAngle = MAKEFIXED(angle1, 0) / 64;
	fxSweepAngle = MAKEFIXED(angle2, 0) / 64;
	GpiSetArcParams(ebhps->hps, (PARCPARAMS)&arcparms);

	GpiSetLineType(ebhps->hps, LINETYPE_INVISIBLE);
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1, 0), fxStartAngle, MAKEFIXED(0, 0));
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lStartPt);
	GpiSetLineType(ebhps->hps, LineStyletoLineType[gc->values.line_style]);
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, fxSweepAngle);
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lEndPt);
	if(reverse)
		GpiMove(ebhps->hps, (PPOINTL)&lStartPt);
	GpiMove(ebhps->hps, (PPOINTL)&lEndPt);
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);
}


int XDrawArcs(Display* display, Drawable d, GC gc, XArc* arcs, int narcs) {
	DBUG_ENTER("XDrawArcs")
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	POINTL lPtCenter, lStartPt;
	ARCPARAMS arcparms;
	FIXED fxStartAngle, fxSweepAngle;
	int reverse = 0;
	if(!narcs || !arcs)
		DBUG_RETURN(False);
	while(narcs--) {
		int angle1 = arcs->angle1, angle2 = arcs->angle2;
		arcparms.lR = arcparms.lS = 0L;
		lPtCenter.x = arcs->x + (arcparms.lP = arcs->width / 2L);
		lPtCenter.y = arcs->y + (arcparms.lQ = arcs->height / 2L);
		if (angle2 < 0) {
			angle1 -= (angle2 = -angle2);
			reverse = 1;
		};	
		while (angle1 < 0) angle1 += 23040;
		/*fxStartAngle = MAKEFIXED(angle1 / 64,(angle1 % 64)<<10);
		 *fxSweepAngle = MAKEFIXED(angle2 / 64,(angle2 % 64)<<10);*/
		fxStartAngle = MAKEFIXED(angle1,0)/64;
		fxSweepAngle = MAKEFIXED(angle2,0)/64;
		GpiSetLineType(ebhps->hps, LINETYPE_INVISIBLE);
		GpiSetArcParams(ebhps->hps, (PARCPARAMS)&arcparms);
		GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, MAKEFIXED(0,0));
		GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lStartPt);
		GpiSetLineType(ebhps->hps, LineStyletoLineType[gc->values.line_style]);
		GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, fxSweepAngle);
		arcs++;
	}
	finishedDrawing(d, ebhps);
	DBUG_RETURN(True);		
}
