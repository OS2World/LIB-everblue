#include "X11.h"

#define UCSCPSIZE 16

UniChar ucsCodepage[UCSCPSIZE];

void Xlib_InitOS2I18N()
{
	ULONG cp[3];
	/*DosQueryCp(3, cp, &rc);*/
	cp[0] = 1004;
	UniMapCpToUcsCp(cp[0], ucsCodepage, UCSCPSIZE);
}
