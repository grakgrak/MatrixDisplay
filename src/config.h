#pragma once
#include <Arduino.h>

// #define DBG_PORT Serial

// #ifdef DEBUG_MATRIX64
// #define DEBUG(...) DBG_PORT.printf(__VA_ARGS__)
// #define DEBUGLN(...) DBG_PORT.println(__VA_ARGS__)
// #else
// #define DEBUG(...)
// #endif


void init_config();

String GetCfgString(const char *key, const char *defaultVal);
void SetCfgString(const char *key, const String &val);

int GetCfgInt(const char *key, int defaultVal);
void SetCfgInt(const char *key, int val);

void SetTraining(const char *key, const String &val);
String GetTraining(const char *key);
void DeleteTraining(const char *key);

String ListReadAll();
String ListReadKey( const char *key);
void ListIoU( const char *key, const String &data);
void ListDelete( const char *key);

void SetAdminJson(const String &json);
String GetAdminJson();

