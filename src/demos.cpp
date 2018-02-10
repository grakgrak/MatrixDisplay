#include "demos.h"
#include <NTPClient.h>

void lavaDemo()
{
	int i = 0;
	int colors[3];
	while (i < 200)
	{
		for (int y = 0; y < ROWS; ++y)
		{
			for (int x = 0; x < COLUMNS; x++)
			{
				auto t = i / 20.0;
				auto val = sin(t + x / 20.0) * sin(t + y / 10.0) * 255.0;
				HSVtoRGB(abs(val), 255, 255, colors);
				matrix.drawPixel(x, y, colors[0], colors[1], colors[2]);
			}
		}
		i += 1;
		vTaskDelay(15);
	}
}

void wheaterDemo()
{
	byte br = 127;
	auto white = RGB(br, br, br);
	auto red = RGB(br, 0, 0);
	auto cyan = RGB(0, br, br);
	auto blue = RGB(0, 0, br);

	matrix.setFont(NULL);
	matrix.black();
	matrix.setTextWrap(false);
	matrix.setTextSize(2);

	matrix.setCursor(2, 1);
	matrix.setTextColor(white);
	matrix.print("18:36");

	matrix.setTextSize(1);
	matrix.setCursor(0, 16);
	matrix.setTextColor(blue);
	matrix.print("-11.3");

	matrix.setCursor(34, 16);
	matrix.setTextColor(red);
	matrix.print("+19.3");

	matrix.setCursor(1, 24);
	matrix.setTextColor(cyan);
	matrix.print("68%");
	matrix.setCursor(22, 24);
	matrix.print("1013hPa");
	vTaskDelay(8000);
}

void primitivesDemo()
{
	matrix.black();
	int speed = 100;
	for (int i = 0; i < 16; i += 2)
	{
		matrix.drawRect(i, i, 64 - 2 * i, 32 - 2 * i, Colors::RED);
		vTaskDelay(speed);
	}

	matrix.black();
	for (int i = 0; i < 90; i += 10)
	{
		matrix.drawLine(0, 0, 64, tan(i * PI / 180.0) * 64, Colors::CYAN);
		vTaskDelay(speed);
	}

	matrix.black();
	matrix.drawCircle(50, 20, 11, Colors::BLUE);
	vTaskDelay(speed);
	matrix.drawCircle(22, 11, 8, Colors::WHITE);
	vTaskDelay(speed);
	matrix.fillCircle(11, 22, 10, Colors::RED);
	vTaskDelay(speed);
	matrix.fillCircle(34, 11, 8, Colors::GREEN);
	vTaskDelay(speed * 10);
}

