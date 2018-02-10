#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "matrix.h"
#include "actions.h"
#include "website.h"
#include "SPI.h"
#include "WiFi.h"
#include <NTPClient.h>

#define BEEPER_PIN 25
#define WIFI_CONNECT_TIMEOUT 12000

//#define SSID "MatrixAP"
//#define PASSWORD "taketheredpill"
#define SSID "BTHub6-ZPR3"
#define PASSWORD "Hxv7Kx4raPV7"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 0, 600 * 1000);
bool blink = false;

void drawSeconds(int secs, int16_t background)
{
	int dx[7] = {1, 0, -1, -1, 0, 1, 0};	// seven slots for safety
	int dy[7] = {0, 1, 0, 0, -1, 0, 0};

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

void main_task(void *pvParameter)
{
	Serial.println("main task Started");

	matrix.setBrightness(3);

	//actions.push( "'Training Display Demo...' b1 c5 b5 c5 b10 c5 b3 'Squad Workout' p10 w3 r3 [3 w5 r5] '2Finished' D2");
	actions.AddAction("'Training Display Demo...'c1000");

	while (true)
	{
		String code = actions.GetAction();

		if (code != "")
			actions.Interpret(code.c_str(), 0, 0);
		else
			clock(RGB(127,127,127), RGB(0,127,0));

		vTaskDelay(1);
	}
}

void webserver_task(void *pvParameter)
{
	Serial.println("webserver task Started");

	while (true)
	{
		loopWebsite();

		vTaskDelay(1);
	}
}

/* create a hardware timer */
hw_timer_t *displayUpdateTimer = NULL;
void IRAM_ATTR onDisplayUpdate()
{
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
            Serial.println("Wifi Connect Timeout Restart...");
            esp_restart();
        }
        delay(timeout);
    }
    else
        lastConnectedTime = millis();

    return WiFi.status() == WL_CONNECTED;
}

void initWifi()
{
	Serial.println("InitWiFi");

	WiFi.begin(SSID, PASSWORD);
    WiFi.setHostname("Matrix64");

    while (isWiFiConnected(750) == false)
        Serial.println("Connecting to WiFi...");
    Serial.println("WiFi Connected - " + WiFi.localIP().toString());

	timeClient.begin();

	//Serial.println(WiFi.softAPIP());


	//WiFi.softAP(SSID, PASSWORD);
	//Serial.println(WiFi.softAPIP());

	initWebsite();
}

void initTasks()
{
	Serial.println("Starting Tasks");

	xTaskCreate(&main_task, "main_task", 2048, NULL, 5, NULL);
	xTaskCreate(&webserver_task, "webserver_task", 8000, NULL, 2, NULL);

	/* 1 tick take 1/(80MHZ/80) = 1us so we set divider 80 and count up */
	displayUpdateTimer = timerBegin(0, 80, true);

	/* Attach onTimer function to our timer */
	timerAttachInterrupt(displayUpdateTimer, &onDisplayUpdate, true);
	timerAlarmWrite(displayUpdateTimer, 2, true);
	timerAlarmEnable(displayUpdateTimer);
}

void setup()
{
	Serial.begin(115200);
	Serial.println("Setup");

	// pinMode(BEEPER_PIN, OUTPUT);	// beeper pin
	// digitalWrite(BEEPER_PIN, LOW);

	initWifi();
	initTasks();
	Serial.println("OK");
}

void loop()
{
	timeClient.update();
	vTaskDelay(1000);
}
