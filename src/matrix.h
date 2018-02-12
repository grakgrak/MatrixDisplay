#ifndef MATRIX_H
#define MATRIX_H

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeSans9pt7b.h"
#include "ESP32RGBmatrixPanel.h"

extern ESP32RGBmatrixPanel matrix;

void Marquee(const char *msg, int16_t colour);

void FullPWR(int action, int secs, int cycle, int limit);
void HalfPWR(int pos, int action, int secs, int cycle, int limit);
void ThirdPWR(int pos, int action, int secs, int cycle, int limit);
void QuarterPWR(int pos, int action, int secs, int cycle, int limit);


int16_t RGB(uint8 r, uint8 g, uint8 b);
void HSVtoRGB(int hue, int sat, int val, int colors[3]);


#endif  //MATRIX_H