#include "X11.h"

USHORT LineStyletoLineType[3] = {
	LINETYPE_SOLID,		/* LineSolid */
	LINETYPE_SHORTDASH,	/* LineOnOffDash */
	LINETYPE_LONGDASH,	/* LineDoubleDash */
};

int XDrawLine(Display* display, Drawable d, GC gc, int x1, int y1, int x2, int y2) {
	DBUG_ENTER("XDrawLine")
	POINTL ptl;
	int viewheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	GpiBeginPath(ebhps->hps, 1);
	ptl.x = x1; ptl.y = viewheight - y1 - 1;
	GpiMove(ebhps->hps, &ptl);
	ptl.x = x2; ptl.y = viewheight - y2 - 1;
	GpiLine(ebhps->hps, &ptl);
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	DBUG_RETURN(True);
}

int XFillPolygon(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int shape, int mode) {
	DBUG_ENTER("XFillPolygon")
	POLYGON poly;
	int i;
	int rely;
	int relx = 0;
	int viewheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);

	if (!viewheight || !points || ((poly.ulPoints = npoints)<2) || 
			!(poly.aPointl = alloca(sizeof(POINTL) * npoints)))
		DBUG_RETURN(False);

	rely = viewheight - 1;
	for (i=0; i<npoints; i++) {
		poly.aPointl[i].x = points[i].x + relx;
		poly.aPointl[i].y = - points[i].y + rely;
		if(mode == CoordModePrevious)
		{
			relx = poly.aPointl[i].x;
			rely = poly.aPointl[i].y;
		}
	}
	GpiMove(ebhps->hps, &poly.aPointl[npoints-1]);
	GpiBeginArea(ebhps->hps, BA_BOUNDARY | (ebgc->xgc.values.fill_rule == WindingRule ?
			BA_WINDING : BA_ALTERNATE ));
	GpiPolyLine(ebhps->hps, poly.ulPoints, poly.aPointl);
	GpiEndArea(ebhps->hps);
	DBUG_RETURN(True);
}

int XDrawLines(Display* display, Drawable d, GC gc, XPoint* points, int npoints, int mode) {
	DBUG_ENTER("XDrawLines")
	POINTL *pptl;
	int rely;
	int i = 1;
	int relx = 0;
	int viewheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	if (!npoints || !points || !(pptl = alloca(sizeof(POINTL) * npoints)))
		DBUG_RETURN(False);
	rely = viewheight - 1;
	for (i = 0; i < npoints; i++) {
		pptl[i].x = points[i].x + relx;
		pptl[i].y = - points[i].y + rely;
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
	DBUG_RETURN(True);
}

int XDrawSegments(Display* display, Drawable d, GC gc, XSegment* segments, int nsegments) {
	DBUG_ENTER("XDrawSegments")
	POINTL ptl;
	int i = 0;
	int viewheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, gc);

	if(!viewheight || !nsegments || !segments)
		DBUG_RETURN(False);
	GpiBeginPath(ebhps->hps, 1);
	while(i < nsegments) {
		ptl.x = segments->x1; ptl.y = viewheight - 1 - segments->y1;
		GpiMove(ebhps->hps, &ptl);
		ptl.x = segments->x2; ptl.y = viewheight - 1 - segments->y2;
		GpiLine(ebhps->hps, &ptl);
		i++; segments++;
	}
	GpiEndPath(ebhps->hps);
	GpiStrokePath(ebhps->hps, 1, 0);
	DBUG_RETURN(True);
}

int XFillArc(Display* display, Drawable d, GC gc, int x, int y,
	     unsigned int width, unsigned int height, int angle1, int angle2) {
	DBUG_ENTER("XFillArc")
	POINTL lPtCenter, lStartPt, lEndPt;
	ARCPARAMS arcparms;
	FIXED fxStartAngle, fxSweepAngle;
	int reverse = 0;
	int viewheight = getDrawableHeight(d);
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	EB_GContext *ebgc = getResource(EBGCONTEXT, gc->gid);

	if(!viewheight)
		DBUG_RETURN(False);
	arcparms.lR = arcparms.lS = 0L;
	lPtCenter.x = x + (arcparms.lP = width / 2L);
	lPtCenter.y = viewheight - y - (arcparms.lQ = height / 2L) - 1;
	if (angle2 < 0) {
		angle1 -= (angle2 = -angle2);
		reverse = 1;
	};	
	while (angle1 < 0)
		angle1 += 23040;
	/*fxStartAngle = MAKEFIXED(angle1 / 64,(angle1 % 64)<<10);
	 *fxSweepAngle = MAKEFIXED(angle2 / 64,(angle2 % 64)<<10);*/
	fxStartAngle = MAKEFIXED(angle1,0) / 64;
	fxSweepAngle = MAKEFIXED(angle2,0) / 64;

	GpiSetLineType(ebhps->hps, LINETYPE_INVISIBLE);
	GpiMove(ebhps->hps, &lPtCenter);
	GpiSetArcParams(ebhps->hps, (PARCPARAMS)&arcparms);
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, MAKEFIXED(0,0));
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lStartPt);
	GpiSetLineType(ebhps->hps, LineStyletoLineType[ebgc->xgc.values.line_style]);

	GpiBeginArea(ebhps->hps, BA_BOUNDARY | (ebgc->xgc.values.fill_rule == WindingRule ?
			BA_WINDING : BA_ALTERNATE ));
	GpiPartialArc(ebhps->hps, &lPtCenter, MAKEFIXED(1,0), fxStartAngle, fxSweepAngle);
	GpiQueryCurrentPosition(ebhps->hps, (PPOINTL)&lEndPt);
	if(ebgc->xgc.values.arc_mode == ArcPieSlice)
		GpiLine(ebhps->hps, &lPtCenter);
	GpiLine(ebhps->hps, &lStartPt);
	GpiEndArea(ebhps->hps);

	if(reverse)
		GpiMove(ebhps->hps, (PPOINTL)&lStartPt);
	GpiMove(ebhps->hps, (PPOINTL)&lEndPt);
	DBUG_RETURN(False);
}

int XFillRectangle(Display* display, Drawable d, GC gc, int x, int y,
		unsigned int width, unsigned int height) {
	DBUG_ENTER("XFillRectangle")
	POINTL ptl;
	int rc;
	EB_HPS *ebhps = getCachedHPS(process, d, gc);
	int viewheight = getDrawableHeight(d);

	ptl.x = x;
	ptl.y = viewheight - 1 - y;
	rc = GpiMove(ebhps->hps, &ptl);
	ptl.x += width - 1;
	ptl.y -= height - 1;
	rc |= GpiBox(ebhps->hps, DRO_FILL, &ptl, 0, 0);
	if(rc == GPI_OK)
		DBUG_RETURN(True);
	DBUG_RETURN(False);
}
