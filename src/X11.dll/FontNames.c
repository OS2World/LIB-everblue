#include "X11.h"
#include <X11/keysym.h>

int FontFileCountDashes (char *name, int namelen) {
    DBUG_ENTER("FontFileCountDashes")
    int	ndashes = 0;

    while (namelen--)
	if (*name++ == '-')
	    ++ndashes;
    DBUG_RETURN(ndashes);
}

int RenderFontName(FONTMETRICS *pfm, char *name, EB_FontHints *hint, EB_Font *xfont)
{
	DBUG_ENTER("RenderFontName")
	char tempstr[256], *family = NULL, *ch;
	char *charset = "iso8859", *weight = "medium", *foundry = "adobe";
	char *width = "normal", slant = 'r', spacing = 'p';
	int pixels = 0, points = 0, hdpi = 0, vdpi = 0, avgwidth = 0;
	int codepage = 1;

	if(pfm->fsType & FM_TYPE_FIXED)
		spacing = 'm';

	if(pfm->fsType & FM_TYPE_ATOMS)
		family = XGetAtomName(NULL, (Atom)pfm->FamilyNameAtom);
	if(!family) {
		family = Xmalloc(strlen(pfm->szFamilyname) + 16);
		if(!family)
			return 0;
		strcpy(family, pfm->szFamilyname);
	}

	for(ch = family; *ch; ch++) 
		if(*ch == '-')
			*ch = ' ';
		else
			*ch = tolower(*ch);

	if (!strcmp(family, "warpsans") || !strncmp(family, "lotus", 5) ||
	    strstr(family, "klinzhai") || strstr(family, "symbol") || 
	    strstr(family, "bar code") || !strncmp(family, "wing", 4))
	  foundry = "misc"; else
	if (strstr(family, " mt") || strstr(family, " ms") ||
	    !strncmp(family, "monotype", 8)) foundry = "monotype";
	
	if (!strcmp(family, "tms rmn") || !strcmp(family, "times new roman")) strcpy(family, "times"); else
	if (!strcmp(family, "helv")) strcpy(family, "helvetica"); else
	if (!strncmp(family, "system ", 7)) {strcpy(family, family+7); foundry="misc";}
	if (!strcmp(family, "vio")) {strcpy(family, "fixed"); foundry="misc"; spacing = 'c';}

	if (pfm->fsDefn & FM_DEFN_OUTLINE && hint) { /* outline font */
		Daemon_getPMHandle(process, NULL);
		HPS hps = WinGetScreenPS(HWND_DESKTOP);
		FATTRS fat = { sizeof(FATTRS), 0, pfm->lMatch, "",
			pfm->idRegistry, 1004, 0, 0, 0, 0 };
		strncpy(fat.szFacename,pfm->szFacename,FACESIZE);

		if (GpiCreateLogFont(hps, NULL, 254L, &fat) != GPI_ERROR) {
			SIZEF sizf = {0,0};
			long my_pel = 0, my_vres = 0;

			my_vres = hint->vres;
			if(!my_vres)
				my_vres = hint->hres;
			if(!my_vres)
				my_vres = 100;
			if (hint->points)
				my_pel = MAKEFIXED(hint->points * my_vres,0) / 720L;
			if (!my_pel && hint->pixels)
				my_pel = MAKEFIXED(hint->pixels,0);
			if (!my_pel && !hint->avgwidth)
				my_pel = MAKEFIXED(pfm->lEmHeight,0);

			if (hint->avgwidth || my_pel) {
				// em sqare wider than avgwidth, so * 2
				if (hint->avgwidth)
					sizf.cx = MAKEFIXED(hint->avgwidth * 2,0);
				if (my_pel)
					sizf.cy = my_pel;
				else {
					sizf.cy = sizf.cx;
					if (hint->hres)
						sizf.cy *= my_vres / hint->hres;
					my_pel = sizf.cy;
				}
				if (!sizf.cx) {
					sizf.cx = sizf.cy;
					if (hint->hres)
						sizf.cx *= hint->hres / my_vres;
				}
				GpiSetCharSet(hps, 254L);
				// accurate font selection
				sizf.cx*=10;
				sizf.cy*=10;
				if (!GpiSetCharBox(hps, &sizf)) printf("GpiSetCharBox Error\n");
				GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), pfm);
				if(hint->avgwidth && pfm->lAveCharWidth) {
					sizf.cx = ((unsigned long long)sizf.cx * hint->avgwidth)/(pfm->lAveCharWidth);
					if(!my_pel)
						sizf.cy = ((unsigned long long)sizf.cy * hint->avgwidth)/(pfm->lAveCharWidth);
				}
				if(my_pel && pfm->lEmHeight) {
					sizf.cy = ((unsigned long long)sizf.cy * my_pel)/(pfm->lEmHeight)/0x1000;
					if(!hint->avgwidth)
						sizf.cx = ((unsigned long long)sizf.cx * my_pel)/(pfm->lEmHeight)/0x1000;
				}
				sizf.cx /= 10;
				sizf.cy /= 10;
				if (!GpiSetCharBox(hps, &sizf)) printf("GpiSetCharBox Error\n");
				points = hint->points;
				if(!hint->points)
					points = (720 * my_pel) / my_vres / 0x10000;
				if(!hint->points && hint->pixels)
					pixels = hint->pixels;
				else
					pixels = FIXEDINT(my_pel);
				avgwidth = hint->avgwidth;
				if(!avgwidth)
					avgwidth = FIXEDINT(sizf.cx / 2);
				hdpi = hint->hres;
				hdpi = hint->vres;
				if(!hdpi) hdpi = vdpi;
				if(!vdpi) vdpi = hdpi;
			}
			else
				fprintf(stderr, "cannot determine font size!");
			if (xfont) {
				memcpy(&xfont->fattrs, &fat, sizeof(FATTRS));
				memcpy(&xfont->sizef, &sizf, sizeof(SIZEF));
				xfont->psmode = 1;
				xfont->descender = pfm->lMaxDescender;
			}

			GpiSetCharSet(hps, 0L);
			GpiDeleteSetId(hps, 254L);
		} else printf("Failed!");
		WinReleasePS(hps);
	} else 
		if (xfont) {
			FATTRS fat = { sizeof(FATTRS), 0, pfm->lMatch, "",
				pfm->idRegistry, 1004, pfm->lMaxBaselineExt, pfm->lAveCharWidth, 0, 0 };
			strncpy(fat.szFacename,pfm->szFacename,FACESIZE);
			memcpy(&xfont->fattrs, &fat, sizeof(FATTRS));
			xfont->psmode = 0;
		}

	if(!(pfm->fsDefn & FM_DEFN_OUTLINE)) { /* bitmapped font */
		pixels = pfm->lEmHeight;
		hdpi = (25*pfm->sXDeviceRes - 600) / 24;
		vdpi = (25*pfm->sYDeviceRes - 600) / 24;
		points = pixels * vdpi / 10;
		avgwidth = pfm->lAveCharWidth * 10;
	}

	if (pfm->usCodePage == 437)  { charset = "ibm"; codepage = 437; } else
	if (pfm->usCodePage == 850)  { charset = "ibm"; codepage = 850; } else
	if (pfm->usCodePage == 1208) { charset = "utf"; codepage = 8; } else
	if (pfm->usCodePage == 1200) { charset = "ucs"; codepage = 2; }

	switch (pfm->usWeightClass) {
	case 1:	weight = "ultralight"; break;
	case 2: weight = "extralight"; break;
	case 3: weight = "light"; break;
	case 4: weight = "semilight"; break;
	case 6: weight = "semibold"; break;
	case 7: weight = "bold"; break;
	case 8: weight = "extrabold"; break;
	case 9: weight = "ultrabold";
	}

	switch (pfm->usWidthClass) {
	case 1: width = "ultracondensed"; break;
	case 2: width = "extracondensed"; break;
	case 3: width = "condensed"; break;
	case 4: width = "semicondensed"; break;
	case 6: width = "semiexpanded"; break;
	case 7: width = "expanded"; break;
	case 8: width = "extraexpanded"; break;
	case 9: width = "ultraexpanded";
	}	

	if (pfm->fsSelection & FM_SEL_ITALIC) slant = 'i'; else
	if (pfm->sCharSlope) slant = 'o';

	sprintf(tempstr,"-%s-%s-%s-%c-%s--%d-%d-%d-%d-%c-%d-%s-%d",
		foundry, family, weight, slant, width, pixels, 
		points, hdpi, vdpi, spacing, avgwidth, charset, codepage);

	Xfree(family);
	if (name) strcpy(name, tempstr);
	int len = strlen(tempstr) + 1;
	DBUG_RETURN(len);
}

EB_FontHints *Xlib_ParseFontHints(HPS hps, char *pattern, int plen)
{
	DBUG_ENTER("Xlib_ParseFontHints")
	if (FontFileCountDashes(pattern,plen) == 14) {
		EB_FontHints *hints;
		/* prepare hints.... */
		char *st = (char *)pattern;
		char spixels[8], spoints[8], swidth[8];
		char shres[8], svres[8];
#ifndef FONTDPI
		LONG res[2];
#endif
		int i = 0;
		while (i<7) if (*(st++) == '-') i++;
		strncpy(spixels,st,7);
		while (i<8) if (*(st++) == '-') i++;
		strncpy(spoints,st,7);
		while (i<9) if (*(st++) == '-') i++;
		strncpy(shres,st,7);
		while (i<10) if (*(st++) == '-') i++;
		strncpy(svres,st,7);
		while (i<12) if (*(st++) == '-') i++;
		strncpy(swidth,st,7);
		hints = Xmalloc(sizeof(EB_FontHints));
		hints->pixels = atoi(spixels);
		hints->points = atoi(spoints);
		hints->avgwidth = atoi(swidth);
		hints->hres = atoi(shres);
		hints->vres = atoi(svres); 
		if (!hints->vres && hints->hres) hints->vres = hints->hres;
		if (!hints->hres && hints->vres) hints->hres = hints->vres;
		if (!hints->hres && !hints->vres) {
#ifdef FONTDPI
			hints->hres = hints->vres = FONTDPI;
#else
			HDC hdc = GpiQueryDevice(hps);
			DevQueryCaps(hdc, CAPS_HORIZONTAL_FONT_RES, 2, res);
			hints->hres = (25*res[0] - 600) / 24;
			hints->vres = (25*res[1] - 600) / 24;
#endif
		}
#if 0
		if (hints->points && !hints->pixels) {
			hints->pixels = (hints->points * hints->vres + 360) / 720;
			printf("pixel height = %d\n",hints->pixels);
		}
#endif
		DBUG_RETURN(hints);
	}
	DBUG_RETURN(NULL);
}

int PatternMatch(char *pat, int patdashes, char *string, int stringdashes) {
    DBUG_ENTER("PatternMatch")
    char        c,
                t;

    if (stringdashes < patdashes)
	DBUG_RETURN(0);
    for (;;) {
	switch (c = *pat++) {
	case '*':
	    if (!(c = *pat++))
		DBUG_RETURN(1);
	    if (c == XK_minus) {
		patdashes--;
		for (;;) {
		    while ((t = *string++) != XK_minus)
			if (!t)
			    DBUG_RETURN(0);
		    stringdashes--;
		    if (PatternMatch(pat, patdashes, string, stringdashes))
			DBUG_RETURN(1);
		    if (stringdashes == patdashes)
			DBUG_RETURN(0);
		}
	    } else {
		for (;;) {
		    while ((t = *string++) != c) {
			if (!t)
			    DBUG_RETURN(0);
			if (t == XK_minus) {
			    if (stringdashes-- < patdashes)
				DBUG_RETURN(0);
			}
		    }
		    if (PatternMatch(pat, patdashes, string, stringdashes))
			DBUG_RETURN(1);
		}
	    }
	case '?':
	    if (*string++ == XK_minus)
		stringdashes--;
	    break;
	case '\0':
	    return (*string == '\0');
	case XK_minus:
	    if (*string++ == XK_minus) {
		patdashes--;
		stringdashes--;
		break;
	    }
	    DBUG_RETURN(0);
	default:
	    if (c == *string++)
		break;
	    DBUG_RETURN(0);
	}
    }
}

char **XListFonts(register Display *dpy, _Xconst char *pattern, int maxNames,
		int *actualCount) {
	DBUG_ENTER("XListFonts")
	char **flist, **fpos;
	char *ch;

	Daemon_getPMHandle(process, NULL);
	if(maxNames <= 0) {
		if(actualCount)
			*actualCount = 0;
		DBUG_RETURN(NULL);
	}

	if(pattern == NULL) {	/* List all fonts */
		HPS hps;
		LONG cFonts, lTemp = 0L, lSize;
		FONTMETRICS *pfm, *cpfm;
		int i, j, aliascount = 0, physcount = 0;
		char *tmp, *tmpbuf;

		hps = WinGetScreenPS(HWND_DESKTOP);
		cFonts = GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL, 
				&lTemp, sizeof(FONTMETRICS), NULL);
		if(cFonts > maxNames)
			cFonts = maxNames;
		pfm = Xmalloc( cFonts * sizeof(FONTMETRICS) );
		GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL,
				&cFonts, sizeof(FONTMETRICS), pfm);
		WinReleasePS(hps);

		for(i=0,lSize = 0; FontAlias[i].alias; i++) {
			lSize += strlen(FontAlias[i].alias)+1;
			aliascount++;
		}
		if(aliascount > maxNames)
			aliascount = maxNames;
		if(cFonts + aliascount > maxNames)
			cFonts = maxNames - aliascount;
		if(actualCount)
			*actualCount = cFonts + aliascount;

		/* Now we translate the font list into XLFD */
		tmp = tmpbuf = Xmalloc(256 * cFonts);

		for(cpfm = pfm, lTemp = 0; lTemp < cFonts; lTemp++, cpfm++) {
			i = RenderFontName(cpfm, tmp, NULL, NULL);
			lSize += i;
			tmp += i;
			physcount++;
		}
		Xfree(pfm);

		flist = fpos = Xmalloc((cFonts + aliascount + 1) * sizeof(char *));
		ch = Xmalloc(lSize + 1);
		*(ch++) = '*';	

		for (i=0; i<aliascount; i++, fpos++) {
			strcpy((*fpos = ch), FontAlias[i].alias);
			j = strlen(ch) + 1;
			ch += j;
		}
		for (tmp=tmpbuf, i=0; i<physcount; i++, fpos++) {
			strcpy((*fpos = ch), tmp);
			j = strlen(ch) + 1;
			ch += j; tmp += j;
		}
		*fpos = NULL;
		Xfree(tmpbuf);

	} else {

		HPS hps;
		LONG cFonts, lTemp = 0L, lSize;
		FONTMETRICS *pfm, *cpfm;
		int plen, i, j, aliascount = 0, physcount = 0;
		EB_FontHints *hints = NULL;
		char my_pattern[256], *tmp, *tmpbuf;

		strcpy(my_pattern, (char *)pattern);
		strlwr(my_pattern); plen = strlen(my_pattern);

		hps = WinGetScreenPS(HWND_DESKTOP);
		hints = Xlib_ParseFontHints(hps, my_pattern, plen);
		cFonts = GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL, 
				&lTemp, sizeof(FONTMETRICS), NULL);
		pfm = Xmalloc( cFonts * sizeof(FONTMETRICS) );
		GpiQueryFonts(hps, QF_PUBLIC | QF_PRIVATE, NULL,
				&cFonts, sizeof(FONTMETRICS), pfm);
		WinReleasePS(hps);

		for (i=0,lSize = 0; FontAlias[i].alias; i++) {
			j = strlen(FontAlias[i].alias);
			if (!PatternMatch(my_pattern, plen, FontAlias[i].alias, j)) continue;
				lSize += j + 1; aliascount++;
		}
		if (aliascount > maxNames) {
			aliascount = maxNames;
			tmpbuf = NULL;
		} else {
			/* Now we translate the font list into XLFD */
			tmp = tmpbuf = Xmalloc(256 * cFonts);

			for (cpfm = pfm, lTemp = 0; lTemp < cFonts; lTemp++, cpfm++) {
				i = RenderFontName(cpfm, tmp, hints, NULL);
				if(!PatternMatch(my_pattern, plen, tmp, i-1))
					continue;
				lSize += i;
				tmp += i;
				physcount++;
			}
			Xfree(pfm); if (hints) Xfree(hints);

			if(physcount + aliascount > maxNames)
				physcount = maxNames - aliascount;
		}
		if(actualCount)
			*actualCount = physcount + aliascount;

		flist = fpos = Xmalloc((physcount + aliascount + 1) * sizeof(char *));
		ch = Xmalloc (lSize + 1);
		*(ch++) = '*';	
		flist[0] = ch;

		for(i=0; i<aliascount; i++, fpos++) {
			j = strlen(FontAlias[i].alias);
			if(!PatternMatch(my_pattern, plen, FontAlias[i].alias, j))
				continue;
			strcpy((*fpos = ch), FontAlias[i].alias);
			ch += j + 1;
		}
		if(tmpbuf)
			for (tmp=tmpbuf, i=0; i<physcount; i++, fpos++) {
				strcpy((*fpos = ch), tmp);
				j = strlen(ch) + 1;
				ch += j; tmp += j;
			}
		*fpos = NULL;
		if(tmpbuf)
			Xfree(tmpbuf);
	}
	DBUG_RETURN(flist);
}

int XFreeFontNames(list)
char **list;
{       
	DBUG_ENTER("XFreeFontNames")
	DBUG_RETURN(1);
	if (list) {
		DBUG_POINT("*(list[0]-1)");
		if (!*(list[0]-1)) { /* from ListFontsWithInfo */
			register char **names;
			for (names = list+1; *names; names++) {
				DBUG_POINT("Xfree(*names)");
				Xfree (*names);
			}
		}
		DBUG_POINT("Xfree(list[0]-1)");
		Xfree (list[0]-1);
		DBUG_POINT("Xfree(list)");
		Xfree ((char *)list);
	}
	DBUG_RETURN(1);
}
