#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "matrix.h"
#include "actionRenderer.h"
#include "website.h"
#include "config.h"
#include "SPI.h"
#include "WiFi.h"
#include "clock.h"
#include <ArduinoOTA.h> // https://github.com/esp8266/Arduino/blob/master/libraries/ArduinoOTA/ArduinoOTA.h

#define BEEPER_PIN 25
#define WIFI_CONNECT_TIMEOUT 20000

//#define SSID "MatrixAP"
//#define PASSWORD "taketheredpill"
#define SSID "BTHub6-ZPR3"
#define PASSWORD "Hxv7Kx4raPV7"
#define HOSTNAME "Matrix64"

bool otaUpdate = false;
bool softAP = false;

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

//--------------------------------------------------------------------
void initWifi()
{
	Serial.println("InitWiFi");

	//String ssid = SSID;
	//String passwd = PASSWORD;
	String ssid = GetCfgString("SSID", "");
	String passwd = GetCfgString("PASSWORD", "");
	String hostname = GetCfgString("HostName", HOSTNAME);

	if(ssid != "")	// try to connect to the router
	{
		WiFi.begin(ssid.c_str(), passwd.c_str());
		WiFi.setHostname(hostname.c_str());

		unsigned long start = millis();
		while (WiFi.status() != WL_CONNECTED)
		{
			if ((millis() - start) > WIFI_CONNECT_TIMEOUT) // allow time to connect to WiFi - if not then restart
				break;
			delay(750);
			Serial.print(".");
		}

		if(WiFi.status() == WL_CONNECTED)	// if connected then all good
		{
			Serial.println("WiFi Connected - " + WiFi.localIP().toString());
			return;
		}
	}

	// ssid is not set or connection attempt timed out
	softAP = true;
	Marquee("SoftAP", Colors::BLUE);
	Serial.println("Starting SoftAP");

	WiFi.softAP("Matrix64", "taketheredpill");

	Serial.println(WiFi.softAPIP());
}

//--------------------------------------------------------------------
void main_task(void *pvParameter)
{
	ActionRenderer.Intro = "Training Display...      Training Display...";
	while (true)
	{
		if (otaUpdate == false)
			if (ActionRenderer.Render() == false)	// if action renderer is idle then
				clock(Colors::WHITE, Colors::GREEN); //  display the clock

		vTaskDelay(10);
	}
}

//--------------------------------------------------------------------
void initTasks()
{
	Serial.println("Start main task");
	xTaskCreate(&main_task, "main_task", 8000, NULL, uxTaskPriorityGet(NULL), NULL);
}

//--------------------------------------------------------------------
void init_OTA()
{
	Serial.println("init OTA");

	// ArduinoOTA callback functions
	ArduinoOTA.onStart([]() {
		otaUpdate = true;
		matrix.black();
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
	//ArduinoOTA.setPasswordHash(eecbf409f68cc72c961d8251147ef222);	// MD5 hash of Matrix64

	ArduinoOTA.begin();
}
//--------------------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	delay(10);
	Serial.setDebugOutput(true);
	Serial.println("Setup");

	// pinMode(BEEPER_PIN, OUTPUT);	// beeper pin
	// digitalWrite(BEEPER_PIN, LOW);

	init_config();
	initMatrix();

	Marquee("Booting...", Colors::BLUE);

	initWifi();
	initWebsite(softAP);
	if(softAP == false)
	{
		initClock();
		initTasks();
		init_OTA();
	}
	Serial.println("Setup Finished");
}

//--------------------------------------------------------------------
void loop()
{
	if(softAP)
		Marquee("SoftAP", Colors::BLUE);
	else
		if (isWiFiConnected(750))
		{
			if (updateClock() == false)
				vTaskDelay(3000);

			ArduinoOTA.handle();
		}
	vTaskDelay(10);
}
