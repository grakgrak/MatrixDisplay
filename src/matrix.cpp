#include "matrix.h"

//G1	R1 |
//GND	B1 |
//G2	R2 |
//GND	B2  |
//B		A   |
//D		C  |
//LAT	CLK|
//GND	OE |

//--------------------------------------------------------------------
//Display panel connection gpio pins
ESP32RGBmatrixPanel matrix(
    23, 22, 21,    // OE CLK LAT
    17, 16, 4,     // RGB 1
    0, 2, 15,      // RGB2
    26, 27, 14, 12 // A B C D
);                 //Flexible connection

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
    matrix.setFont(&FreeSans12pt7b);
    matrix.setTextColor(colour);

    int16_t x1, y1;
    uint16_t w, h;

    matrix.getTextBounds((char *)msg, 0, 0, &x1, &y1, &w, &h);

    w += 1;
    int pos = 64;
    while (pos >= -w)
    {
        matrix.startWrite();
        matrix.black();
        matrix.setCursor(pos, 22);
        matrix.print(msg);
        pos -= 1;
        matrix.endWrite();
        vTaskDelay(15);
    }
}

void FullPWR(int action, int secs, int cycle, int limit)
{
    if (action == '\0')
        return;
    matrix.startWrite();

    int16_t colour;
    const char *text;
    switch (action)
    {
    case 'p':
        text = "Prep";
        colour = Colors::CYAN;
        break;
    case 'w':
        text = "Work";
        colour = Colors::RED;
        break;
    case 'r':
        text = "Rest";
        colour = Colors::GREEN;
        break;
    }

    matrix.setFont(NULL);
    matrix.setTextWrap(false);
    matrix.setTextColor(colour);

    // display action
    matrix.black();
    matrix.drawRect(0, 0, 64, 32, colour);
    matrix.setTextSize(1);
    matrix.setCursor(4, 4);
    matrix.print(text);

    if (cycle > 0) // display cycle of limit
    {
        matrix.setCursor(4, 20);
        matrix.print(cycle);
        matrix.setCursor(matrix.getCursorX() + 1, 20);
        matrix.print("of");
        matrix.setCursor(matrix.getCursorX() + 1, 20);
        matrix.print(limit);
    }

    // display seconds
    matrix.setTextSize(2);
    if (secs < 10)
        matrix.setCursor(44, 8);
    else
        matrix.setCursor(38, 8);
    matrix.print(secs);

    matrix.endWrite();
}

//--------------------------------------------------------------------
void drawPWR(int count, int pos, int action, int secs, int cycle, int limit)
{
    if (action == '\0') // code is finished so nothing to show
        return;

    matrix.startWrite();

    int background[] = {Colors::RED, Colors::GREEN, Colors::BLUE, Colors::WHITE};

    int mul[] = {0, 1, 32, 21, 16};

    int x = pos * mul[count] + (count == 3); // offset by 1 if showing 3 windows
    int w = mul[count];
    int mid = x + w / 2;

    // get the colour
    int16_t colour;
    const char *text;
    switch (action)
    {
    case 'p':
        text = "P";
        colour = Colors::CYAN;
        break;
    case 'w':
        text = "W";
        colour = Colors::RED;
        break;
    case 'r':
        text = "R";
        colour = Colors::GREEN;
        break;
    }

    matrix.setFont(NULL);
    matrix.setTextWrap(false);
    matrix.setTextColor(colour);

    // display action
    matrix.fillRect(x, 0, w, 32, 0);               // Blank the background
    matrix.drawRect(x, 0, w, 32, background[pos]); // draw outline
    matrix.setTextSize(1);
    matrix.setCursor(x + 3, 3);
    matrix.print(text);

    if (cycle > 0) // display cycle of limit
    {
        matrix.setCursor(x + 4, 20);
        matrix.print(cycle);
        matrix.setCursor(matrix.getCursorX() + 1, 20);
        matrix.print("of");
        matrix.setCursor(matrix.getCursorX() + 1, 20);
        matrix.print(limit);
    }

    // display seconds
    if (count > 2)
        matrix.setTextSize(1);
    else
        matrix.setTextSize(2);

    if (secs < 10)
        matrix.setCursor(mid - 4, 11);
    else
        matrix.setCursor(mid - 8, 11);
    matrix.print(secs);

    matrix.endWrite();
}

//--------------------------------------------------------------------
void HalfPWR(int pos, int action, int secs, int cycle, int limit)
{
    drawPWR(2, pos, action, secs, cycle, limit);
}
//--------------------------------------------------------------------
void ThirdPWR(int pos, int action, int secs, int cycle, int limit)
{
    drawPWR(3, pos, action, secs, cycle, limit);
}
//--------------------------------------------------------------------
void QuarterPWR(int pos, int action, int secs, int cycle, int limit)
{
    drawPWR(4, pos, action, secs, cycle, limit);
}
