#include "X11.h"
#include "Xcms/Xcmsint.h"

typedef struct _dbEntry * dbEntryPtr;
typedef struct _dbEntry {
	dbEntryPtr link;
	unsigned short red;
	unsigned short green;
	unsigned short blue;
	char					 name[1];	/* some compilers complain if [0] */
} dbEntry;

extern void _XcmsCopyISOLatin1Lowered();

#define HASHSIZE 511
static dbEntryPtr hashTab[HASHSIZE];
char *rgbPath = "/usr/X11R6/lib/X11/rgb";

static dbEntryPtr lookup(char *name, int len, Bool create) {
	DBUG_ENTER("lookup")
	unsigned int h = 0, g;
	dbEntryPtr entry, *prev = NULL;
	char *str = name;

	if(!(name = (char*)alloca(len +1)))
		DBUG_RETURN(NULL);
	_XcmsCopyISOLatin1Lowered(name, str, len);
	
	name[len] = '\0';

	for(str = name; *str; str++) {
		h = (h << 4) + *str;
		if ((g = h) & 0xf0000000)
			h ^= (g >> 24);
		h &= g;
	}
	h %= HASHSIZE;

	if (( entry = hashTab[h] )) {
		for( ; entry; prev = (dbEntryPtr*)entry, entry = entry->link )
			if(! strcmp(name, entry->name) )
				break;
	} else
		prev = &(hashTab[h]);

	if(!entry && create && (entry = (dbEntryPtr)Xmalloc(sizeof(dbEntry) +len))) {
		*prev = entry;
		entry->link = NULL;
		strcpy( entry->name, name );
	}

	DBUG_RETURN(entry);
}


Bool OsInitColors() {
	DBUG_ENTER("OsInitColors")
	FILE *rgb;
	char *path;
	char line[BUFSIZ];
	char name[BUFSIZ];
	int red, green, blue, lineno = 0;
	dbEntryPtr entry;

	static Bool was_here = FALSE;

	if(!was_here) {
		char *tmp = (char*)__XOS2RedirRoot(rgbPath);
		path = (char*)alloca(strlen(tmp) +5);
		strcpy(path, tmp);
		strcat(path, ".txt");
		if(!(rgb = fopen(path, "r"))) {
			fprintf(stderr, "Couldn't open RGB_DB '%s'\n", rgbPath );
			fprintf(stderr, "Xlated path '%s'\n", path );
			/*DEALLOCATE_LOCAL(path);*/
			DBUG_RETURN(FALSE);
		}

		while(fgets(line, sizeof(line), rgb)) {
			lineno++;
			if(sscanf(line,"%d %d %d %[^\n\r]\n", &red, &green, &blue, name) == 4) {
				if(red >= 0	 && red <= 0xff && green >= 0 && green <= 0xff &&
						blue >= 0	&& blue <= 0xff) {
					if((entry = lookup(name, strlen(name), TRUE))) {
						entry->red  = (red * 65535) / 255;
						entry->green = (green * 65535) / 255;
						entry->blue	= (blue	* 65535) / 255;
					}
				} else
					fprintf(stderr,"Value out of range: %s:%d\n", path, lineno);
			} else if(*line && *line != '#' && *line != '!')
				fprintf(stderr,"Syntax Error: %s:%d\n", path, lineno);
		}
			
		fclose(rgb);
		/*DEALLOCATE_LOCAL(path);*/

		was_here = TRUE;
	}
	DBUG_RETURN(TRUE);
}

Bool OsLookupColor(int screen, char *name, unsigned len, unsigned short *pred,
		unsigned short *pgreen, unsigned short *pblue) {
	DBUG_ENTER("OsLookupColor")
	dbEntryPtr entry;

	if((entry = lookup(name, len, FALSE))) {
		*pred	 = entry->red;
		*pgreen = entry->green;
		*pblue	= entry->blue;
		DBUG_RETURN(TRUE);
	}

	DBUG_RETURN(FALSE);
}

Status XParseColor(register Display *dpy, Colormap cmap, _Xconst char *spec,
		XColor *def) {
	register int n, i;
	int r, g, b;
	unsigned short sr, sg, sb;
	char c;
	XcmsCCC ccc;
	XcmsColor cmsColor;

	DBUG_ENTER("XParseColor")
	if(!spec)
		return(0);
	n = strlen(spec);
	if(*spec == '#') {
		/*
		 * RGB
		 */
		spec++;
		n--;
		if(n != 3 && n != 6 && n != 9 && n != 12)
			return (0);
		n /= 3;
		g = b = 0;
		do {
			r = g;
			g = b;
			b = 0;
			for (i = n; --i >= 0; ) {
				c = *spec++;
				b <<= 4;
				if (c >= '0' && c <= '9')
					b |= c - '0';
				else if (c >= 'A' && c <= 'F')
					b |= c - ('A' - 10);
				else if (c >= 'a' && c <= 'f')
					b |= c - ('a' - 10);
				else
					return (0);
			}
		} while(*spec != '\0');
		n <<= 2;
		n = 16 - n;
		def->red = r << n;
		def->green = g << n;
		def->blue = b << n;
		def->flags = DoRed | DoGreen | DoBlue;
		return (1);
	}


	/*
	 * Let's Attempt to use Xcms and i18n approach to Parse Color
	 */
	if ((ccc = XcmsCCCOfColormap(dpy, cmap)) != (XcmsCCC)NULL) {
		const char *tmpName = spec;

		switch(_XcmsResolveColorString(ccc, &tmpName, &cmsColor, XcmsRGBFormat)) {
		case XcmsSuccess:
		case XcmsSuccessWithCompression:
			cmsColor.pixel = def->pixel;
			_XcmsRGB_to_XColor(&cmsColor, def, 1);
			return(1);
		case XcmsFailure:
		case _XCMS_NEWNAME:
			/*
			 * if the result was _XCMS_NEWNAME tmpName points to
			 * a string in cmsColNm.c:pairs table, for example,
			 * gray70 would become tekhvc:0.0/70.0/0.0
			 */
			break;
		}
	}

	/*
	 * Xcms and i18n methods failed.
	 */
	n = strlen (spec);
	char *speccopy = strdup(spec);
	if(!OsLookupColor(0, speccopy, n, &sr, &sg, &sb))
	    DBUG_RETURN(0);
	
	def->red = sr;
	def->green = sg;
	def->blue = sb;

	def->pixel = (((unsigned long)sr >> 8) * 65536 | ((unsigned long)sg >> 8)) * 256
			| ((unsigned long)sb >> 8);

	/*printf("Color '%s' -> (%d,%d,%d) = %ld\n",spec,sr,sg,sb,def->pixel);*/

	DBUG_RETURN(1);
}

Status XAllocNamedColor(register Display *dpy, Colormap cmap,
		_Xconst char *colorname, XColor *hard_def, XColor *exact_def) {

    unsigned short red, green, blue;
    XcmsCCC ccc;
    XcmsColor cmsColor_exact;
    Status ret;

    /*
     * Let's Attempt to use Xcms and i18n approach to Parse Color
     */
    if((ccc = XcmsCCCOfColormap(dpy, cmap)) != (XcmsCCC)NULL) {
		const char *tmpName = colorname;

		switch (_XcmsResolveColorString(ccc, &tmpName, &cmsColor_exact,
				XcmsRGBFormat)) {
		case XcmsSuccess:
		case XcmsSuccessWithCompression:
		    _XcmsRGB_to_XColor(&cmsColor_exact, exact_def, 1);
		    memcpy((char *)hard_def, (char *)exact_def, sizeof(XColor));
	    	ret = XAllocColor(dpy, cmap, hard_def);
		    exact_def->pixel = hard_def->pixel;
		    return(ret);
		case XcmsFailure:
		case _XCMS_NEWNAME:
		    /*
		     * if the result was _XCMS_NEWNAME tmpName points to
	    	 * a string in cmsColNm.c:pairs table, for example,
		     * gray70 would become tekhvc:0.0/70.0/0.0
		     */
		    break;
		}
    }

    /*
     * Xcms and i18n approach failed.
     */
	long n = strlen (colorname);
	if(!OsLookupColor(0,(char *)colorname, n, &red, &green, &blue))
	    return 0;
	
	exact_def->red = red;
	exact_def->green = green;
	exact_def->blue = blue;

	hard_def->red = (red &= 0xff00);
	hard_def->green = (green &= 0xff00);
	hard_def->blue = (blue &= 0xff00);

	exact_def->pixel = hard_def->pixel = (hard_def->red << 8) |
			hard_def->green | (hard_def->blue >> 8);

	/*printf("Color '%s' -> (%d,%d,%d) = #%06x\n",colorname,red >> 8,green >> 8,blue >> 8,exact_def->pixel);*/

	return 1;
}
