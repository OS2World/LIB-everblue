#include "X11.h"
#include "Xcms/Xcmsint.h"

/*
 *      FORWARD DECLARATIONS
 */
static void _XcmsFreeClientCmaps(Display *dpy);


/************************************************************************
 *									*
 *			PRIVATE INTERFACES				*
 *									*
 ************************************************************************/

/*
 *	NAME
 *		CmapRecForColormap
 *
 *	SYNOPSIS
 */
static XcmsCmapRec *
CmapRecForColormap(
    Display *dpy,
    Colormap cmap)
/*
 *	DESCRIPTION
 *		Find the corresponding XcmsCmapRec for cmap.  In not found
 *		this routines attempts to create one.
 *
 *	RETURNS
 *		Returns NULL if failed; otherwise the address to
 *		the corresponding XcmsCmapRec.
 *
 */
{
    XcmsCmapRec *pRec;

    for (pRec = (XcmsCmapRec *)dpy->cms.clientCmaps; pRec != NULL;
	    pRec = pRec->pNext) {
	if (pRec->cmapID == cmap) {
	    return(pRec);
	}
    }

    /*
     * Can't find an XcmsCmapRec associated with cmap in our records.
     */
    if((pRec = _XcmsAddCmapRec(dpy, cmap, RootWindow(dpy, 0),
		    DefaultVisual(dpy, 0))) == NULL)
		return((XcmsCmapRec *)NULL);
    pRec->ccc = XcmsCreateCCC(
	    dpy,
	    0,			/* screenNumber */
	    DefaultVisual(dpy, 0),
	    (XcmsColor *)NULL,	/* clientWhitePt */
	    (XcmsCompressionProc)NULL,  /* gamutCompProc */
	    (XPointer)NULL,	/* gamutCompClientData */
	    (XcmsWhiteAdjustProc)NULL,  /* whitePtAdjProc */
	    (XPointer)NULL	/* whitePtAdjClientData */
	    );
    return(pRec);
}



/************************************************************************
 *									*
 *			API PRIVATE INTERFACES				*
 *									*
 ************************************************************************/

/*
 *	NAME
 *		_XcmsAddCmapRec
 *
 *	SYNOPSIS
 */
XcmsCmapRec *
_XcmsAddCmapRec(dpy, cmap, windowID, visual)
    Display *dpy;
    Colormap cmap;
    Window windowID;
    Visual *visual;
/*
 *	DESCRIPTION
 *		Create an XcmsCmapRec for the specified cmap, windowID,
 *		and visual, then adds it to its list of CmapRec's.
 *
 *	RETURNS
 *		Returns NULL if failed; otherwise the address to
 *		the added XcmsCmapRec.
 *
 */
{
    XcmsCmapRec *pNew;

    if ((pNew = (XcmsCmapRec *) Xcalloc(1, (unsigned) sizeof(XcmsCmapRec)))
	    == NULL) {
	return((XcmsCmapRec *)NULL);
    }

    pNew->cmapID = cmap;
    pNew->dpy = dpy;
    pNew->windowID = windowID;
    pNew->visual = visual;
    pNew->pNext = (XcmsCmapRec *)dpy->cms.clientCmaps;
    dpy->cms.clientCmaps = (XPointer)pNew;
    dpy->free_funcs->clientCmaps = _XcmsFreeClientCmaps;

    /*
     * Note, we don't create the XcmsCCC for pNew->ccc here because
     * it may require the use of XGetWindowAttributes (a round trip request)
     * to determine the screen.
     */
    return(pNew);
}


/*
 *	NAME
 *		_XcmsCopyCmapRecAndFree
 *
 *	SYNOPSIS
 */
XcmsCmapRec *
_XcmsCopyCmapRecAndFree(
    Display *dpy,
    Colormap src_cmap,
    Colormap copy_cmap)
/*
 *	DESCRIPTION
 *		Augments Xlib's XCopyColormapAndFree() to copy
 *		XcmsCmapRecs.
 *
 *	RETURNS
 *		Returns NULL if failed; otherwise the address to
 *		the copy XcmsCmapRec.
 *
 */
{
    XcmsCmapRec *pRec_src;
    XcmsCmapRec *pRec_copy;

    if ((pRec_src = CmapRecForColormap(dpy, src_cmap)) != NULL) {
	pRec_copy =_XcmsAddCmapRec(dpy, copy_cmap, pRec_src->windowID,
		pRec_src->visual);
	if (pRec_copy != NULL && pRec_src->ccc) {
	    pRec_copy->ccc = (XcmsCCC)Xcalloc(1, (unsigned) sizeof(XcmsCCCRec));
	    memcpy((char *)pRec_copy->ccc, (char *)pRec_src->ccc,
		   sizeof(XcmsCCCRec));
	}
	return(pRec_copy);
    }
    return((XcmsCmapRec *)NULL);
}


/*
 *	NAME
 *		_XcmsDeleteCmapRec
 *
 *	SYNOPSIS
 */
void
_XcmsDeleteCmapRec(
    Display *dpy,
    Colormap cmap)
/*
 *	DESCRIPTION
 *		Removes and frees the specified XcmsCmapRec structure
 *		from the linked list of structures.
 *
 *	RETURNS
 *		void
 *
 */
{
    XcmsCmapRec **pPrevPtr;
    XcmsCmapRec *pRec;
    int scr;

    /* If it is the default cmap for a screen, do not delete it,
     * because the server will not actually free it */
    for (scr = ScreenCount(dpy); --scr >= 0; ) {
	if (cmap == DefaultColormap(dpy, scr))
	    return;
    }

    /* search for it in the list */
    pPrevPtr = (XcmsCmapRec **)&dpy->cms.clientCmaps;
    while ((pRec = *pPrevPtr) && (pRec->cmapID != cmap)) {
	pPrevPtr = &pRec->pNext;
    }

    if (pRec) {
	if (pRec->ccc) {
	    XcmsFreeCCC(pRec->ccc);
	}
	*pPrevPtr = pRec->pNext;
	Xfree((char *)pRec);
    }
}


/*
 *	NAME
 *		_XcmsFreeClientCmaps
 *
 *	SYNOPSIS
 */
static void
_XcmsFreeClientCmaps(
    Display *dpy)
/*
 *	DESCRIPTION
 *		Frees all XcmsCmapRec structures in the linked list
 *		and sets dpy->cms.clientCmaps to NULL.
 *
 *	RETURNS
 *		void
 *
 */
{
    XcmsCmapRec *pRecNext, *pRecFree;

    pRecNext = (XcmsCmapRec *)dpy->cms.clientCmaps;
    while (pRecNext != NULL) {
	pRecFree = pRecNext;
	pRecNext = pRecNext->pNext;
	if (pRecFree->ccc) {
	    /* Free the XcmsCCC structure */
	    XcmsFreeCCC(pRecFree->ccc);
	}
	/* Now free the XcmsCmapRec structure */
	Xfree((char *)pRecFree);
    }
    dpy->cms.clientCmaps = (XPointer)NULL;
}



/************************************************************************
 *									*
 *			PUBLIC INTERFACES				*
 *									*
 ************************************************************************/

/*
 *	NAME
 *		XcmsCCCOfColormap
 *
 *	SYNOPSIS
 */
XcmsCCC 
XcmsCCCOfColormap(dpy, cmap)
    Display *dpy;
    Colormap cmap;
/*
 *	DESCRIPTION
 *		Finds the XcmsCCC associated with the specified colormap.
 *
 *	RETURNS
 *		Returns NULL if failed; otherwise the address to
 *		the associated XcmsCCC structure.
 *
 */
{
    XWindowAttributes windowAttr;
    XcmsCmapRec *pRec;
    int nScrn = ScreenCount(dpy);
    int i;

    if ((pRec = CmapRecForColormap(dpy, cmap)) != NULL) {
	if (pRec->ccc) {
	    /* XcmsCmapRec already has a XcmsCCC */
	    return(pRec->ccc);
	}

	/*
	 * The XcmsCmapRec does not have a XcmsCCC yet, so let's create
	 * one.  But first, we need to know the screen associated with
	 * cmap, so use XGetWindowAttributes() to extract that
	 * information.  Unless, of course there is only one screen!!
	 */
	if (nScrn == 1) {
	    /* Assume screenNumber == 0 */
	    return(pRec->ccc = XcmsCreateCCC(
		    dpy,
		    0,			/* screenNumber */
		    pRec->visual,
		    (XcmsColor *)NULL,	/* clientWhitePt */
		    (XcmsCompressionProc)NULL,  /* gamutCompProc */
		    (XPointer)NULL,	/* gamutCompClientData */
		    (XcmsWhiteAdjustProc)NULL,  /* whitePtAdjProc */
		    (XPointer)NULL	/* whitePtAdjClientData */
		    ));
	} else {
	    if (XGetWindowAttributes(dpy, pRec->windowID, &windowAttr)) {
		for (i = 0; i < nScrn; i++) {
		    if (ScreenOfDisplay(dpy, i) == windowAttr.screen) {
			return(pRec->ccc = XcmsCreateCCC(
				dpy,
				i,		   /* screenNumber */
				pRec->visual,
				(XcmsColor *)NULL, /* clientWhitePt */
				(XcmsCompressionProc)NULL, /* gamutCompProc */
				(XPointer)NULL,	   /* gamutCompClientData */
				(XcmsWhiteAdjustProc)NULL, /* whitePtAdjProc */
				(XPointer)NULL	   /* whitePtAdjClientData */
				));
		    }
		}
	    }
	}
    }

    /*
     * No such cmap
     */
    return(NULL);
}

XcmsCCC XcmsSetCCCOfColormap(dpy, cmap, ccc)
    Display *dpy;
    Colormap cmap;
    XcmsCCC ccc;
{
    XcmsCCC prev_ccc = NULL;
    XcmsCmapRec *pRec;

    pRec = CmapRecForColormap(dpy, cmap);
    if (pRec) {
	prev_ccc = pRec->ccc;
	pRec->ccc = ccc;
    }
    return prev_ccc;
}
