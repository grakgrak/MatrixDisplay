#ifndef WEBSITE_H
#define WEBSITE_H
#include <ESP32WebServer.h>
#include "actions.h"


extern void initWebsite();
extern void handleRoot();
extern void handleInterp();

extern void loopWebsite();

//extern ESP32WebServer webserver;

#endif      //WEBSITE_H
