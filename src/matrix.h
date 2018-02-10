#ifndef MATRIX_H
#define MATRIX_H
#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "ESP32RGBmatrixPanel.h"

extern ESP32RGBmatrixPanel matrix;
extern void displayPWR(const char *text, int16_t colour, int secs, int cycle, int limit);
extern void marquee(const char *msg, int16_t colour);

extern int16_t RGB(uint8 r, uint8 g, uint8 b);
void HSVtoRGB(int hue, int sat, int val, int colors[3]);


#endif  //MATRIX_H