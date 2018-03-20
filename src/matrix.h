#ifndef MATRIX_H
#define MATRIX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "NimbusSansL32.h"
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "ESP32RGBmatrixPanel.h"

extern ESP32RGBmatrixPanel matrix;

int16_t RGB(uint8 r, uint8 g, uint8 b);
void HSVtoRGB(int hue, int sat, int val, int colors[3]);

void Marquee(const char *msg, int16_t colour);
void initMatrix();

#endif  //MATRIX_H