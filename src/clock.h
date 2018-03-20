#pragma once
#include <Arduino.h>

bool updateClock();
void clock(int hour, int minute, int sec, int16_t foreground, int16_t background);
void showTime(int16_t foreground, int16_t background);

void initClock(bool softAP);
