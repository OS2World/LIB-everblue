#include "daemondll.h"

XGCValues stdgcvalues = { GXcopy, -1, 0, 1, 0, LineSolid, CapButt, JoinMiter,
		FillSolid, EvenOddRule, ArcPieSlice, 0, 0, 0, 0, 0, ClipByChildren,
		True, 0, 0, None, 0, 4 };

USHORT GXtoMixMode[16]	= {
	FM_ZERO,	/* GXclear */
	FM_AND,		/* GXand */
	FM_MASKSRCNOT,	/* GXandReverse */
	FM_OVERPAINT,	/* GXcopy */
	FM_SUBTRACT,	/* GXandInverted */
	FM_LEAVEALONE,	/* GXnoop */
	FM_XOR,		/* GXxor */
	FM_OR,		/* GXor */
	FM_NOTMERGESRC,	/* GXnor */
	FM_NOTXORSRC,	/* GXequiv */
	FM_INVERT,	/* GXinvert */
	FM_MERGESRCNOT,	/* GXorReverse */
	FM_NOTCOPYSRC,	/* GXcopyInverted */
	FM_MERGENOTSRC,	/* GXorInverted */
	FM_NOTMASKSRC,	/* GXnand */
	FM_ONE,		/* GXset */
};

USHORT GXtoROPMode[16]	= {
	ROP_ZERO,	/* GXclear */
	ROP_SRCAND,	/* GXand */
	ROP_SRCERASE,	/* GXandReverse */
	ROP_SRCCOPY,	/* GXcopy */
	0x0022,		/* GXandInverted */
	0x00aa,		/* GXnoop */
	0x0066,		/* GXxor */
	ROP_SRCPAINT,	/* GXor */
	ROP_NOTSRCERASE,/* GXnor */
	0x0099,		/* GXequiv */
	ROP_DSTINVERT,	/* GXinvert */
	0x00dd,		/* GXorReverse */
	ROP_NOTSRCCOPY,	/* GXcopyInverted */
	ROP_MERGEPAINT,	/* GXorInverted */
	0x0077,		/* GXnand */
	ROP_ONE,	/* GXset */
};

USHORT LineStyletoLineType[3] = {
	LINETYPE_SOLID,		/* LineSolid */
	LINETYPE_SHORTDASH,	/* LineOnOffDash */
	LINETYPE_LONGDASH,	/* LineDoubleDash */
};

USHORT CapStyletoLineEnd[4] = {
	LINEEND_FLAT,		/* CapNotLast */
	LINEEND_FLAT,		/* CapButt */
	LINEEND_ROUND,		/* CapRound */
	LINEEND_SQUARE,		/* CapProjecting */
};

USHORT JoinStyletoLineJoin[3] = {
	LINEJOIN_MITRE,		/* JoinMiter */
	LINEJOIN_ROUND,		/* JoinRound */
	LINEJOIN_BEVEL,		/* JoinBevel */
};

void setGCValues(EB_HPS *ebhps, XGCValues *newvalues, Drawable d, Bool force) {
// TODO optimize?
	XGCValues *oldvalues = &ebhps->current;
	
	if(force || oldvalues->function != newvalues->function) {
		GpiSetMix(ebhps->hps, GXtoMixMode[newvalues->function]);
		GpiSetBackMix(ebhps->hps, GXtoMixMode[newvalues->function]);
	}
	if(force || oldvalues->foreground != newvalues->foreground) 
		GpiSetColor(ebhps->hps, newvalues->foreground);
	if(force || oldvalues->background != newvalues->background) 
		GpiSetBackColor(ebhps->hps, newvalues->background);
	if(force || oldvalues->line_style != newvalues->line_style) 
		GpiSetLineType(ebhps->hps, LineStyletoLineType[newvalues->line_style]);
	if(force || oldvalues->cap_style != newvalues->cap_style) 
		GpiSetLineEnd(ebhps->hps, CapStyletoLineEnd[newvalues->cap_style]);
	if(force || oldvalues->join_style != newvalues->join_style) 
		GpiSetLineJoin(ebhps->hps, JoinStyletoLineJoin[newvalues->join_style]);
	if(force || oldvalues->line_width != newvalues->line_width) 
		GpiSetLineWidthGeom(ebhps->hps, newvalues->line_width);
	if(force || oldvalues->font != newvalues->font)
		if(newvalues->font) {
			GpiSetCharSet(ebhps->hps, LCID_DEFAULT);
			EB_Font *font = getResource(EBFONT, newvalues->font);
			GpiCreateLogFont(ebhps->hps, NULL, 1, &font->fattrs);
			GpiSetCharSet(ebhps->hps, 1);
			if(font->psmode)
				GpiSetCharBox(ebhps->hps, &font->sizef);
		} else {
			GpiSetCharSet(ebhps->hps, LCID_DEFAULT);
		}
	if(force || oldvalues->clip_mask != newvalues->clip_mask)
		if(newvalues->clip_mask) {
			if(oldvalues->clip_mask)
				if(((EB_Resource *)oldvalues->clip_mask)->restype == EBPIXMAP) {
				} else {
				}
			if(((EB_Resource *)newvalues->clip_mask)->restype == EBRECTANGLES) {
				EB_Rectangles *ebr = getResource(EBRECTANGLES, newvalues->clip_mask);

				RECTL *rectl, *cp;
				cp = rectl = alloca(sizeof(RECTL) * ebr->size);
				XRectangle *rectangles = ebr->rectangles;
				int i;
				for(i = 0; i < ebr->size; i++, cp++, rectangles++) {
					cp->xRight = (cp->xLeft = rectangles->x + newvalues->clip_x_origin) + rectangles->width;
					cp->yTop = (cp->yBottom = rectangles->y + newvalues->clip_y_origin) + rectangles->height;
				}

				HRGN hrgn = GpiCreateRegion(ebhps->hps, ebr->size, rectl);
				HRGN oldreg;
				GpiSetClipRegion(ebhps->hps, hrgn, &oldreg);
				if(oldreg) {
					GpiDestroyRegion(ebhps->hps, oldreg);
				}
			} else {
				fprintf(stderr, "ClipMask not implemented yet\n");
			}
		}
	*oldvalues = *newvalues;
}
