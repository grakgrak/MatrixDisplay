#pragma once
#include <Arduino.h>

bool updateClock();
void clockHHMMSS(int hour, int minute, int sec, int16_t foreground, int16_t background);
void clockMMSS(int minute, int sec, int16_t foreground, int16_t background);
void clockMMSS(int minute, int sec, int count, int idx, int16_t foreground);
void showTime(int16_t foreground, int16_t background);

void initClock(bool softAP);
