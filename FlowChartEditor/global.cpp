#include "global.h"

#include <cmath>

qreal degToRad(qreal deg) { return deg / 180 * PI; }

qreal radToDeg(qreal rad) { return rad / PI * 180; }

qreal degMod(qreal deg)
{
	deg = fmod(deg + 180.0, 360.0) - 180.0;
	if (deg < -180.0) deg += 360.0;
	return deg;
}
