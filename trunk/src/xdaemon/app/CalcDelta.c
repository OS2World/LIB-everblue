#include "x11daemon.h"

// OS/2 supports sophisticated sizing of windows:
// windows are repainted while dragging the mouse
// X11 programs have problems for elements in
// the center of windows due to rounding errors
// I use an algorithm that completely eliminates those errors
// -> ddelta

int CalcXDelta(int oldsize, int newsize, int gravity) {
	int delta = newsize - oldsize;
	int ddelta = (delta > 0 ? 1 : -1) * (1 - (newsize % 2));

	switch(gravity) {
	case EastGravity:
	case SouthEastGravity:
	case NorthEastGravity:
		return delta;
	case CenterGravity:
	case SouthGravity:
	case NorthGravity:
		return (delta + ddelta) / 2;
	}
	return 0;
}

int CalcYDelta(int oldsize, int newsize, int gravity) {
	int delta = newsize - oldsize;
	int ddelta = (delta > 0 ? 1 : -1) * (1 - (newsize % 2));

	switch(gravity) {
	case NorthGravity:
	case NorthWestGravity:
	case NorthEastGravity:
	case UnmapGravity:
		return delta;
	case CenterGravity:
	case WestGravity:
	case EastGravity:
		return (delta + ddelta) / 2;
	}
	return 0;
}
