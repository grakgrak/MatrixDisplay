#include "clock.h"
#include "matrix.h"
#include <TimeLib.h>
#include <NTPClientLib.h>

//--------------------------------------------------------------------
time_t _now;	// the current time

#define PIXELS (COLUMNS * 2 + ROWS * 2 - 4)
#define SECS_SF (PIXELS / 60.0)	// number of pixels per second

#define HEIGHT	31

#if COLUMNS == 64
#define FULL_WIDTH	63
#define HALF_WIDTH	31
#else
#define FULL_WIDTH	127
#define HALF_WIDTH	63
#endif

//--------------------------------------------------------------------
void drawSeconds(int secs, int16_t background)
{
	int x = HALF_WIDTH + 1;
	int y = 0;
	int dx = 1;
	int dy = 0;

	for (int i = 0; i < secs * SECS_SF; ++i)
	{
		if (i >= (secs - 1) * SECS_SF)
			matrix.drawPixel(x, y, background);

		switch (i)
		{
		case HALF_WIDTH:
			dx = 0;
			dy = 1;
			break;
		case HALF_WIDTH + HEIGHT:
			dx = -1;
			dy = 0;
			break;
		case FULL_WIDTH + HALF_WIDTH + HEIGHT:
			dx = 0;
			dy = -1;
			break;
		case FULL_WIDTH + HALF_WIDTH + HEIGHT + HEIGHT:
			dx = 1;
			dy = 0;
			break;
		}

		x += dx;
		y += dy;
	}
}

//--------------------------------------------------------------------
void clockHHMMSS(int hour, int minute, int sec, int16_t foreground, int16_t background)
{
	matrixStartWrite();

	matrix.setTextSize(0);
	matrix.setFont(&FreeSans18pt7b);
	matrix.setTextColor(foreground);
	matrix.black();

	int mid = COLUMNS / 2;
	int y = 27;
	//if(sec >=0 && sec < 60)
	//	drawSeconds(sec + 1, background);

	// blink the colon between digit pairs
	if (sec % 2 != 0)
	{
		matrix.drawRect(mid - 23, 11, 2, 2, background);
		matrix.drawRect(mid - 23, 18, 2, 2, background);

		matrix.drawRect(mid + 19, 11, 2, 2, background);
		matrix.drawRect(mid + 19, 18, 2, 2, background);
	}
	
	String mins(minute);
	String hours(hour);
	String secs(sec);

	if (mins.length() == 1)
		mins = "0" + mins;
	if (hours.length() == 1)
		hours = "0" + hours;
	if (secs.length() == 1)
		secs = "0" + secs;

	matrix.setCursor(mid - 62, y);	//40
	matrix.print(hours);

	matrix.setCursor(mid - 20, y);
	matrix.print(mins);

	matrix.setTextColor(background);

	matrix.setCursor(mid + 22, y);
	matrix.print(secs);

	matrixEndWrite();
}
//--------------------------------------------------------------------
// void clockMMSS(int minute, int sec, int16_t foreground, int16_t background)
// {
// 	matrix.startWrite();

// 	matrix.setTextSize(0);
// 	matrix.setFont(&FreeSans18pt7b);
// 	matrix.setTextColor(foreground);
// 	matrix.black();

// 	int mid = COLUMNS / 2;
// 	int y = 27;

// 	// blink the colon between digit pairs
// 	if (sec % 2 != 0)
// 	{
// 		matrix.drawRect(mid - 1, 11, 2, 2, background);
// 		matrix.drawRect(mid - 1, 18, 2, 2, background);
// 	}
	
// 	String mins(minute);
// 	String secs(sec);

// 	if (mins.length() == 1)
// 		mins = "0" + mins;
// 	if (secs.length() == 1)
// 		secs = "0" + secs;

// 	matrix.setCursor(mid - 40, y);
// 	matrix.print(mins);

// 	matrix.setCursor(mid + 2, y);
// 	matrix.print(secs);

// 	matrix.endWrite();
// }

//--------------------------------------------------------------------
void MMSS( const String &mins, const String &secs, int mid, int y, bool blink, int pairWidth)
{
	// blink the colon between digit pairs
	if (blink)
	{
		matrix.drawRect(mid - 1, 11, 2, 2, Colors::GREEN);
		matrix.drawRect(mid - 1, 18, 2, 2, Colors::GREEN);
	}

	matrix.setCursor(mid - (pairWidth + 2), y);
	matrix.print(mins);

	matrix.setCursor(mid + 2, y);
	matrix.print(secs);
}
//--------------------------------------------------------------------
void clockMMSS(int minute, int sec, int count, int idx, int16_t colour)
{
	matrix.startWrite();

	String mins(minute);
	String secs(sec);

	matrix.setTextColor(Colors::WHITE);

	if (mins.length() == 1)
		mins = "0" + mins;
	if (secs.length() == 1)
		secs = "0" + secs;

	int w = COLUMNS / count;
	if( count == 1)
	{
		matrix.setTextSize(0);
		matrix.setFont(&FreeSans18pt7b);
		matrix.black();
		MMSS(mins, secs, w / 2, 27, (sec % 2 != 0), 38);	// width of number pair
	}
	if(count == 2)
	{
		matrix.setFont(NULL);
		matrix.setTextSize(2);

		int x = w * idx;
		matrix.fillRect(x, 0, w, ROWS, 0); // Blank the background
		matrix.drawRect(x, 0, w, ROWS, colour);

		int mid = x + (w / 2);
		MMSS(mins, secs, mid, 8, (sec % 2 != 0), 24);
	}
	if(count == 3 || count == 4)
	{
		matrix.setFont(NULL);
		matrix.setTextSize(1);
		int x = w * idx;
		matrix.fillRect(x, 0, w, ROWS, 0); // Blank the background
		matrix.drawRect(x, 0, w, ROWS, colour);

		int mid = x + (w / 2);
		MMSS(mins, secs, mid, 12, (sec % 2 != 0), 12);
	}

	matrix.endWrite();
}



//--------------------------------------------------------------------
void showTime(int16_t foreground, int16_t background)
{
	clockHHMMSS( hour(_now), minute(_now), second(_now), foreground, background);
}

//--------------------------------------------------------------------
bool updateClock()
{
	_now = now();	// capture the current time - calling now() can cause a sync with the NTP server
	return true;
}

//--------------------------------------------------------------------
void initClock(bool softAP)
{
	// timezone 0, daylight saving adjust, 0 minutes offset for tz
	if(softAP == false)
	{
		NTP.begin("pool.ntp.org", 0, true, 0);
		Serial.println("Started NTP Client.");
	}
}
