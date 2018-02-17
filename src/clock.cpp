#include "clock.h"
#include "matrix.h"
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <TimeLib.h>

#define NTPSERVER "pool.ntp.org"
//--------------------------------------------------------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTPSERVER, 0, 600 * 1000);

bool blink = false;

#define SECS_SF (188.0 / 60.0)
//--------------------------------------------------------------------
boolean isDST(time_t t)
{
    tmElements_t te;
    te.Year = year(t) - 1970;
    te.Month = 3;
    te.Day = 1;
    te.Hour = 0;
    te.Minute = 0;
    te.Second = 0;
    time_t dstStart, dstEnd, current;

    dstStart = makeTime(te);
    dstStart = nextSunday(dstStart);
    dstStart = nextSunday(dstStart);
    dstStart = nextSunday(dstStart); //third sunday in march
    dstStart += 2 * SECS_PER_HOUR;   //2AM
    te.Month = 10;
    dstEnd = makeTime(te);
    dstStart = nextSunday(dstStart);
    dstStart = nextSunday(dstStart);
    dstEnd = nextSunday(dstEnd); //third sunday in october
    dstEnd += SECS_PER_HOUR;     //1AM

    return (t >= dstStart && t < dstEnd);
}
//--------------------------------------------------------------------
void drawSeconds(int secs, int16_t background)
{
    int x = 32;
    int y = 0;
    int dx = 1;
    int dy = 0;

    for (int i = 0; i < secs * SECS_SF; ++i)
    {
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
    // adjust time for BST
    // if (isDST(timeClient.getEpochTime()))
    //     timeClient.setTimeOffset(SECS_PER_HOUR);
    // else
    //     timeClient.setTimeOffset(0);

    matrix.startWrite();

    matrix.setTextSize(0);
    matrix.setFont(&FreeSans12pt7b);
    matrix.setTextColor(foreground);
    matrix.black();

    drawSeconds(timeClient.getSeconds() + 1, background);

    blink = !blink;
    if (blink)
    {
        // draw the colon between digit pairs
        matrix.drawRect(31, 11, 2, 2, background);
        matrix.drawRect(31, 18, 2, 2, background);
    }

    String mins(timeClient.getMinutes());
    String hours(timeClient.getHours());

    if (mins.length() == 1)
        mins = "0" + mins;
    if (hours.length() == 1)
        hours = "0" + hours;

    matrix.setCursor(4, 23);
    matrix.print(hours);

    matrix.setCursor(34, 23);
    matrix.print(mins);

    matrix.endWrite();
    vTaskDelay(1000);
}

//--------------------------------------------------------------------
bool updateClock()
{
    return timeClient.update();
}

//--------------------------------------------------------------------
void initClock()
{
    timeClient.begin();
    timeClient.update();
}
