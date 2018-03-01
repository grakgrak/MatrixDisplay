#include "clock.h"
#include "matrix.h"
#include <TimeLib.h>
#include <NTPClientLib.h>

//--------------------------------------------------------------------
time_t _now;	// the current time
#define SECS_SF (188.0 / 60.0)	// number of pixels per second

//--------------------------------------------------------------------
void drawSeconds(int secs, int16_t background)
{
	int x = 32;
	int y = 0;
	int dx = 1;
	int dy = 0;

	for (int i = 0; i < secs * SECS_SF; ++i)
	{
		if (i >= (secs - 1) * SECS_SF)
			matrix.drawPixel(x, y, background);

		switch (i)
		{
		case 31:
			dx = 0;
			dy = 1;
			break;
		case 62:
			dx = -1;
			dy = 0;
			break;
		case 125:
			dx = 0;
			dy = -1;
			break;
		case 156:
			dx = 1;
			dy = 0;
			break;
		}

		x += dx;
		y += dy;
	}
}

//--------------------------------------------------------------------
void clock(int16_t foreground, int16_t background)
{
	matrix.startWrite();

	matrix.setTextSize(0);
	matrix.setFont(&FreeSans12pt7b);
	matrix.setTextColor(foreground);
	matrix.black();

	int secs = second(_now);
	drawSeconds(secs + 1, background);

	// blink the colon between digit pairs
	if (secs % 2 == 0)
	{
		matrix.drawRect(31, 11, 2, 2, background);
		matrix.drawRect(31, 18, 2, 2, background);
	}

	String mins(minute(_now));
	String hours(hour(_now));

	if (mins.length() == 1)
		mins = "0" + mins;
	if (hours.length() == 1)
		hours = "0" + hours;

	matrix.setCursor(4, 23);
	matrix.print(hours);

	matrix.setCursor(34, 23);
	matrix.print(mins);

	matrix.endWrite();
}

//--------------------------------------------------------------------
bool updateClock()
{
	_now = now();	// capture the current time - calling now() can cause a sync with the NTP server
	return true;
}

//--------------------------------------------------------------------
void initClock()
{
	// timezone 0, daylight saving adjust, 0 minutes offset for tz
	NTP.begin("pool.ntp.org", 0, true, 0);
}
