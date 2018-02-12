#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "matrix.h"
#include "actions.h"
#include "website.h"
#include "SPI.h"
#include "WiFi.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ArduinoOTA/ArduinoOTA.h

#define BEEPER_PIN 25
#define WIFI_CONNECT_TIMEOUT 12000

//#define SSID "MatrixAP"
//#define PASSWORD "taketheredpill"
#define SSID "BTHub6-ZPR3"
#define PASSWORD "Hxv7Kx4raPV7"
#define HOSTNAME "Matrix64"

//--------------------------------------------------------------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 600 * 1000);

bool otaUpdate = false;
bool blink = false;

//--------------------------------------------------------------------
void drawSeconds(int secs, int16_t background)
{
	int dx[] = {1, 0, -1, -1, 0, 1, 0};	// seven slots for safety
	int dy[] = {0, 1, 0, 0, -1, 0, 0};

	int x = 32;
	int y = 0;

	for (int i = 0; i <= secs * 3.186; ++i)
	{
		matrix.drawPixel(x, y, background);

		x += dx[i / 31];
		y += dy[i / 31];

		if (x == 32)	// fudge to get the pixels aligned properly
			matrix.drawPixel(x--, y, background);
	}
}

//--------------------------------------------------------------------
void clock(int16_t foreground, int16_t background)
{
	matrix.startWrite();

	matrix.setTextSize(0);
	matrix.setFont(&FreeSans12pt7b);
	matrix.setTextColor(foreground);
	matrix.black();

	blink = !blink;
	if (blink)
	{
		// draw the colon between digit pairs
		matrix.drawRect(31, 11, 2, 2, background);
		matrix.drawRect(31, 18, 2, 2, background);
	}

	drawSeconds(timeClient.getSeconds(), background);

	String mins(timeClient.getMinutes());
	String hours(timeClient.getHours());

	if (mins.length() == 1)
		mins = "0" + mins;
	if (hours.length() == 1)
		hours = "0" + hours;

	matrix.setCursor(4, 23);
	matrix.print(hours);

	matrix.setCursor(34, 23);
	matrix.print(mins);

	matrix.endWrite();
	vTaskDelay(1000);
}


//--------------------------------------------------------------------
void main_task(void *pvParameter)
{
	matrix.setBrightness(2);

	//actions.push( "'Training Display Demo...' b1 c5 b5 c5 b10 c5 b3 'Squad Workout' p10 w3 r3 [3 w5 r5] '2Finished' D2");

	ActionRenderer.Intro = "[2'Training Display...']";

	while (true)
	{
		if( otaUpdate == false)
			if (ActionRenderer.Render() == false)	// if action renderer is idle then 
				clock(Colors::WHITE, Colors::GREEN);	//  display the clock

		vTaskDelay(10);
	}
}

//--------------------------------------------------------------------
void webserver_task(void *pvParameter)
{
	// Inspect our own high water mark on entering the task. 
	//int stackusage = uxTaskGetStackHighWaterMark(NULL);
	while (true)
	{
		if( otaUpdate == false)
			loopWebsite();

		vTaskDelay(1);
	}
}

//--------------------------------------------------------------------
/* create a hardware timer */
hw_timer_t *displayUpdateTimer = NULL;
void IRAM_ATTR onDisplayUpdate()
{
	if( otaUpdate == false)
		matrix.update();
}

//--------------------------------------------------------------------
bool isWiFiConnected(int timeout)
{
    static unsigned long lastConnectedTime = 0;

    if (WiFi.status() != WL_CONNECTED)
    {
        if ((millis() - lastConnectedTime) > WIFI_CONNECT_TIMEOUT) // allow time to connect to WiFi - if not then restart
        {
            //marquee("Wifi Connect Timeout Restart...", RGB(127,127,127));
			Serial.println("Wifi Connect Timeout Restart...");
            esp_restart();
        }
        delay(timeout);
    }
    else
        lastConnectedTime = millis();

    return WiFi.status() == WL_CONNECTED;
}

//--------------------------------------------------------------------
void initWifi()
{
	Serial.println("InitWiFi");

	WiFi.begin(SSID, PASSWORD);
    WiFi.setHostname(HOSTNAME);

    //marquee("Connecting to WiFi...", RGB(127,127,127));

    while (isWiFiConnected(750) == false)
		Serial.print(".");

	String msg = "WiFi Connected - " + WiFi.localIP().toString();
    //marquee(msg.c_str(), RGB(127,127,127));

	timeClient.begin();
	timeClient.update();
	//Serial.println(WiFi.softAPIP());
	//WiFi.softAP(SSID, PASSWORD);
	//Serial.println(WiFi.softAPIP());
}

//--------------------------------------------------------------------
void initTasks()
{
	Serial.println("Start main task");
	xTaskCreate(&main_task, "main_task", 8000, NULL, uxTaskPriorityGet(NULL), NULL);

	Serial.println("Start webserver task");
	xTaskCreate(&webserver_task, "webserver_task", 8000, NULL, uxTaskPriorityGet(NULL), NULL);
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
}

//--------------------------------------------------------------------
void init_OTA()
{
	Serial.println("init OTA");

    // ArduinoOTA callback functions
    ArduinoOTA.onStart([]() {
		matrix.black();
		otaUpdate = true;
        Serial.println("OTA starting...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("OTA done.Reboot...");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        static unsigned int prevPcnt = 100;
        unsigned int pcnt = (progress / (total / 100));
        unsigned int roundPcnt = 5 * (int)(pcnt / 5);
        if (roundPcnt != prevPcnt)
        {
            prevPcnt = roundPcnt;
            Serial.println("OTA upload " + String(roundPcnt) + "%");
        }
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.print("OTA Error " + String(error) + ":");
        const char *line2 = "";
        switch (error)
        {
        case OTA_AUTH_ERROR:
            line2 = "Auth Failed";
            break;
        case OTA_BEGIN_ERROR:
            line2 = "Begin Failed";
            break;
        case OTA_CONNECT_ERROR:
            line2 = "Connect Failed";
            break;
        case OTA_RECEIVE_ERROR:
            line2 = "Receive Failed";
            break;
        case OTA_END_ERROR:
            line2 = "End Failed";
            break;
        }
        Serial.println(line2);
    });

    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname(HOSTNAME);
    //ArduinoOTA.setPassword(HOSTNAME);
	//ArduinoOTA.setPasswordHash(eecbf409f68cc72c961d8251147ef222);	// MD5 has of Matrix64

    ArduinoOTA.begin();
}
//--------------------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	Serial.println("Setup");

	// pinMode(BEEPER_PIN, OUTPUT);	// beeper pin
	// digitalWrite(BEEPER_PIN, LOW);

	initWifi();
	initWebsite();
	initTasks();
	initMatrix();
    init_OTA();
	Serial.println("Setup Finished");
}

//--------------------------------------------------------------------
void loop()
{
	if(isWiFiConnected(750))
	{
		if( timeClient.update() == false)
			vTaskDelay(3000);
        ArduinoOTA.handle();
	}
	vTaskDelay(100);
}
