#include "X11.h"

FONTMETRICS *Xlib_LoadFont(char* name, EB_Font *ebfont) {
	DBUG_ENTER("Xlib_LoadFont")
	HPS hps;
	int i, plen, tmp;
	LONG cFonts, lTemp = 0;
	char pattern[256], tempname[256];
	EB_FontHints *hints = NULL;
	FONTMETRICS *pfm, *cpfm, *npfm;

//	printf("Loading %s\n", name);
	if (name)
		strcpy(pattern, (char*)name);
	else
		memset(pattern, 0, sizeof(pattern));

	strlwr(pattern);
	plen = strlen(pattern);
	/*fprintf(stderr,"pattern = '%s',\n",pattern);*/
	
	/* First, we go through the aliases */
	if(plen)
		for(i=0; i < sizeof(FontAlias) / sizeof(EB_FontAlias); i++) 
			if(!plen || PatternMatch(pattern, plen, FontAlias[i].alias,
					strlen(FontAlias[i].alias))) {
				strcpy(pattern, (char*) FontAlias[i].fontname);
				/*fprintf(stderr, "Alias for '%s',\n",pattern);*/
				break;
			}

	if(!(plen = strlen(pattern)))
		DBUG_RETURN(NULL);

	/* Now, we go through the available fonts */

	hps = WinGetScreenPS(HWND_DESKTOP);
	hints = Xlib_ParseFontHints(hps, pattern, plen);

	cFonts = GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL, 
			&lTemp, sizeof(FONTMETRICS), NULL);
	cpfm = pfm = Xmalloc( cFonts * sizeof(FONTMETRICS) );
	GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL,
			&cFonts, sizeof(FONTMETRICS), pfm);
	WinReleasePS(hps);

	/* Now try to find first matching font */
	for (lTemp = 0; lTemp < cFonts; lTemp++, cpfm++) {
		tmp = RenderFontName(cpfm, tempname, hints, ebfont);
		if(PatternMatch(pattern, plen, tempname, tmp-1)) {
//printf("Found: %s\n", tempname);
			npfm = Xmalloc(sizeof(FONTMETRICS));
			memcpy(npfm, cpfm, sizeof(FONTMETRICS));
			Xfree(pfm);
			if(hints)
				Xfree(hints);
			/*fprintf(stderr,"Pattern matches font '%s'\n",tempname);*/
			DBUG_RETURN(npfm);
		}
	}
	/*fprintf(stderr,"No font from %d matched pattern!\n",cFonts);*/
	Xfree(pfm);
	if(hints)
		Xfree(hints);
	DBUG_RETURN(NULL);
}

XFontProp *Xlib_NewFontProp(HPS hps, FONTMETRICS *pfm, int *n_properties) {
	DBUG_ENTER("Xlib_NewFontProp")
	XFontProp *prop, *p;
	int propcount = 10;
	
	prop = p = Xmalloc(propcount * sizeof(XFontProp));

	p->name = XA_SUPERSCRIPT_X; p->card32 = pfm->lSuperscriptXOffset; p++;
	p->name = XA_SUPERSCRIPT_Y; p->card32 = pfm->lSuperscriptYOffset; p++;
	p->name = XA_SUBSCRIPT_X; p->card32 = pfm->lSubscriptXOffset; p++;
	p->name = XA_SUBSCRIPT_Y; p->card32 = pfm->lSubscriptYOffset; p++;
	p->name = XA_UNDERLINE_POSITION; p->card32 = pfm->lUnderscorePosition; p++;
	p->name = XA_UNDERLINE_THICKNESS; p->card32 = pfm->lUnderscoreSize; p++;
	p->name = XA_STRIKEOUT_ASCENT; p->card32 = pfm->lStrikeoutPosition + pfm->lStrikeoutSize - 1; p++;
	p->name = XA_STRIKEOUT_DESCENT; p->card32 = pfm->lStrikeoutPosition - pfm->lStrikeoutSize +1; p++;
	p->name = XA_X_HEIGHT; p->card32 = pfm->lMaxAscender - pfm->lInternalLeading; p++;
	p->name = XA_POINT_SIZE; p->card32 = pfm->lEmHeight * 10;
	
	*n_properties = propcount;
	DBUG_RETURN(prop);
}

XCharStruct *Xlib_NewCharBounds(HPS hps, FONTMETRICS *pfm, XCharStruct *min, XCharStruct *max, int offset) {
	DBUG_ENTER("Xlib_NewCharBounds")
	XCharStruct *cstr, *p;
	LONG i;
	char buf[2] = {(char)0, (char)0};
	POINTL aptl[TXTBOX_COUNT];

	cstr = Xcalloc(pfm->sLastChar + 1 + offset, sizeof(XCharStruct));

	i = pfm->sDefaultChar;
	p = &cstr[i - pfm->sFirstChar + offset];
	buf[0] = (char)i;
	if(GpiQueryTextBox(hps, i<0xff?1:2, (PSZ)buf, TXTBOX_COUNT, aptl)) {
		GpiConvert(hps, CVTC_WORLD, CVTC_DEVICE, TXTBOX_COUNT, aptl);
		p->lbearing = aptl[TXTBOX_BOTTOMLEFT].x;
		p->rbearing = aptl[TXTBOX_TOPRIGHT].x;
		p->width = aptl[TXTBOX_CONCAT].x;
		p->ascent = aptl[TXTBOX_TOPLEFT].y - 1;
		p->descent = 1 - aptl[TXTBOX_BOTTOMRIGHT].y;
	}

	min->lbearing = max->lbearing = p->lbearing;
	min->rbearing = max->rbearing = p->rbearing;
	min->width = max->width = p->width;
	min->ascent = max->ascent = p->ascent;
	min->descent = max->descent = p->descent;
	min->attributes = max->attributes = 0;

	p = cstr;
	for(i=pfm->sFirstChar - offset; i<=pfm->sFirstChar + pfm->sLastChar; i++, p++) {
		buf[0] = (char)i;
		if(!GpiQueryTextBox(hps, i<0xff?1:2, (PSZ)buf, TXTBOX_COUNT, aptl)) {
			memcpy(p, &cstr[pfm->sDefaultChar + offset], sizeof(XCharStruct));
			continue;
		}

		GpiConvert(hps, CVTC_WORLD, CVTC_DEVICE, TXTBOX_COUNT, aptl);

		p->lbearing = aptl[TXTBOX_BOTTOMLEFT].x;
		p->rbearing = aptl[TXTBOX_TOPRIGHT].x;
		p->width = aptl[TXTBOX_CONCAT].x;

		p->ascent = aptl[TXTBOX_TOPLEFT].y - 1;
		p->descent = 1 - aptl[TXTBOX_BOTTOMRIGHT].y;

		if(p->rbearing < min->lbearing) min->lbearing = p->lbearing;
		if(p->lbearing > max->lbearing) max->lbearing = p->lbearing;
		if(p->rbearing < min->rbearing) min->rbearing = p->rbearing;
		if(p->rbearing > max->rbearing) max->rbearing = p->rbearing;
		if(p->width < min->width) min->width = p->width;
		if(p->width > max->width) max->width = p->width;
		if(p->ascent < min->ascent) min->ascent = p->ascent;
		if(p->ascent > max->ascent) max->ascent = p->ascent;
		if(p->descent > max->descent) max->descent = p->descent;
		if(p->descent < min->descent) min->descent = p->descent;
	}
	DBUG_RETURN(cstr);
}

static XFontStruct *_XQueryFont(register Display *dpy, XID fid, FONTMETRICS *pfm) {
	DBUG_ENTER("_XQueryFont")
	register XFontStruct *fs = Xmalloc(sizeof(XFontStruct));
	register FONTMETRICS *fm = pfm?pfm:Xmalloc(sizeof(FONTMETRICS));
	EB_Font *font = getResource(EBFONT, fid);
	HPS hps = NULLHANDLE;
	
	if(!fid || !fm) {
		Xfree(fs);
		if(!pfm && fm)
			Xfree(fm);
		DBUG_RETURN(0);
	}

	hps = WinGetScreenPS(HWND_DESKTOP);
	GpiCreateLogFont(hps, NULL, 1, &font->fattrs);
	GpiSetCharSet(hps, 1);
	if(font->psmode)
		GpiSetCharBox(hps, &font->sizef);
	if(!pfm)
		GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), fm);

#ifdef GDK_UC_FIX
	fm->sFirstChar = 0;
	fm->sLastChar = 255;
	fm->sDefaultChar >>= 8;
#endif

	fs->ext_data = NULL;
	fs->fid = fid;
	fs->direction = FontLeftToRight;
	fs->min_byte1 = fm->sFirstChar >> 8;
	fs->max_byte1 = (fm->sFirstChar + fm->sLastChar) >> 8;
	if(fs->min_byte1 == fs->max_byte1) {
		fs->min_char_or_byte2 = fm->sFirstChar & 0xff;
		fs->max_char_or_byte2 = (fm->sFirstChar + fm->sLastChar) & 0xff;
	} else {
		fs->min_char_or_byte2 = 0;
		fs->max_char_or_byte2 = 0xff;
	}
	fs->default_char = fm->sFirstChar + fm->sDefaultChar;
	fs->all_chars_exist = (fm->fsType & FM_TYPE_FIXED) ? True : False;
	fs->properties = Xlib_NewFontProp(hps, fm, &fs->n_properties);
	fs->per_char = Xlib_NewCharBounds(hps, fm, &fs->min_bounds, &fs->max_bounds, (fm->sFirstChar & 0xff) - fs->min_char_or_byte2);
	fs->ascent = fs->max_bounds.ascent;
	fs->descent = fs->max_bounds.descent;

	GpiDeleteSetId(hps,1);
	WinReleasePS(hps);
	if(!pfm)
		Xfree(fm);
	DBUG_RETURN(fs);
}

XFontStruct *XLoadQueryFont(Display* dpy, _Xconst char* name) {
	DBUG_ENTER("XLoadQueryFont")
	EB_Font *ebf = scalloc(1, sizeof(EB_Font));
	FONTMETRICS *pfm;

	pfm = Xlib_LoadFont((char*)name, ebf);
	if (pfm) {
		EB_Resource *ebres = createResource(EBFONT, ebf);
		addResource(&process->ebprocess->res, ebres);
		XFontStruct *xfs = _XQueryFont(dpy, (XID)ebres, pfm);
		Xfree(pfm);
		/*fprintf(stderr,"Font %x loaded '%s'\n",ebf, name);*/
		DBUG_RETURN(xfs);
	}
	sfree(ebf);
	DBUG_RETURN(NULL);
}

Font XLoadFont(Display* dpy, _Xconst char* name) {
	DBUG_ENTER("XLoadFont")

	EB_Font *font = Xmalloc(sizeof(EB_Font));
	FONTMETRICS *pfm;

	memset(font, 0, sizeof(EB_Font));

	pfm = Xlib_LoadFont((char*)name, font);
	
	if(pfm) {
		EB_Resource *fontres;

		Xfree(pfm);
		/*fprintf(stderr, "Font %x loaded '%s'\n", (Font)font, name);*/
		fontres = createResource(EBFONT, font);
		addResource(&process->ebprocess->res, fontres);
		DBUG_RETURN((Font)fontres);
	}
	Xfree(font);
	DBUG_RETURN((Font)0);
}
