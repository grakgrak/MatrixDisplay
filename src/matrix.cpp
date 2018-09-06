#include "matrix.h"
#include "config.h"

// R1	G1
// B1	GND
// R2	G2
// B2	GND
// A	B
// C	D
// CLK	LAT
// OE	GND

ESP32RGBmatrixPanel matrix(
	25, 23, 22,	// OE CLK LAT
	21, 27, 2,	 // RGB 1
	16, 5, 4,	  // RGB2
	18, 17, 26, 19 // A B C D
);

// Original pin layout - also used on PCB
// ESP32RGBmatrixPanel matrix(
// 	14, 23, 22,	// OE CLK LAT
// 	0, 15, 2,	 // RGB 1
// 	16, 5, 4,	  // RGB2
// 	18, 17, 26, 19 // A B C D
// );
//--------------------------------------------------------------------
//Display panel connection gpio pins
// ESP32RGBmatrixPanel matrix(
// 	23, 22, 21,	// OE CLK LAT
// 	17, 16, 4,	 // RGB 1
// 	0, 2, 15,	  // RGB2
// 	26, 27, 14, 12 // A B C D
// );

//--------------------------------------------------------------------
int16_t RGB(uint8 r, uint8 g, uint8 b)
{
	return ESP32RGBmatrixPanel::AdafruitColor(r, g, b);
}

//--------------------------------------------------------------------
void HSVtoRGB(int hue, int sat, int val, int colors[3])
{
	int r;
	int g;
	int b;
	int base;

	if (sat == 0)
	{ // Acromatic color (gray). Hue doesn't mind.
		colors[0] = val;
		colors[1] = val;
		colors[2] = val;
	}
	else
	{

		base = ((255 - sat) * val) >> 8;

		switch (hue / 60)
		{
		case 0:
			r = val;
			g = (((val - base) * hue) / 60) + base;
			b = base;
			break;

		case 1:
			r = (((val - base) * (60 - (hue % 60))) / 60) + base;
			g = val;
			b = base;
			break;

		case 2:
			r = base;
			g = val;
			b = (((val - base) * (hue % 60)) / 60) + base;
			break;

		case 3:
			r = base;
			g = (((val - base) * (60 - (hue % 60))) / 60) + base;
			b = val;
			break;

		case 4:
			r = (((val - base) * (hue % 60)) / 60) + base;
			g = base;
			b = val;
			break;

		case 5:
			r = val;
			g = base;
			b = (((val - base) * (60 - (hue % 60))) / 60) + base;
			break;
		}

		colors[0] = r;
		colors[1] = g;
		colors[2] = b;
	}
}

//--------------------------------------------------------------------
void Marquee(const char *msg, int16_t colour)
{
	matrix.setTextSize(0);
	matrix.setTextWrap(false);
	//matrix.setFont(&FreeSans12pt7b);
	matrix.setFont(&Nimbus_Sans_L_Regular_Condensed_32);
	matrix.setTextColor(colour);

	int16_t x1, y1;
	uint16_t w, h;

	matrix.getTextBounds((char *)msg, 0, 0, &x1, &y1, &w, &h);

	int y = 25;	// 22 for 12 pt font

	w += 3;
	int pos = COLUMNS;
	while (pos >= -w)
	{
		matrix.startWrite();
		matrix.black();
		matrix.setCursor(pos, y);
		matrix.print(msg);
		pos -= 1;
		matrixEndWrite();
		vTaskDelay(10);
	}
}

//--------------------------------------------------------------------
/* create a hardware timer */
hw_timer_t *displayUpdateTimer = NULL;
void IRAM_ATTR onDisplayUpdate()
{
	matrix.update();
}
//--------------------------------------------------------------------
void initMatrix()
{
	/* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
	displayUpdateTimer = timerBegin(0, 80, true);

	/* Attach onTimer function to our timer */
	timerAttachInterrupt(displayUpdateTimer, &onDisplayUpdate, true);
	timerAlarmWrite(displayUpdateTimer, 5, true);
	timerAlarmEnable(displayUpdateTimer);

	matrix.black();
	matrix.setBrightness(GetCfgInt("Brightness", 2));

	Serial.println("Matrix init.");
}
//--------------------------------------------------------------------
int _writeCounter = 0;
void matrixStartWrite()
{
	if( _writeCounter++ == 0)
		matrix.startWrite();
}
//--------------------------------------------------------------------
void matrixEndWrite()
{
	if(--_writeCounter == 0)
		matrix.endWrite();
}

