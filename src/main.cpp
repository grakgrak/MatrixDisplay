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

#define BEEPER_PIN 33
#define WIFI_CONNECT_TIMEOUT 15000

#define HOSTNAME "Matrix64"
#define SOFT_AP_PASSWORD "taketheredpill"

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
bool WiFiConnect(const String &ssid, const String &passwd, int timeout)
{
	if(ssid != "")	// try to connect to the router
	{
		WiFi.begin(ssid.c_str(), passwd.c_str());
	
		String hostname = GetCfgString("HostName", HOSTNAME);
		WiFi.setHostname(hostname.c_str());

		unsigned long start = millis();
		while (WiFi.status() != WL_CONNECTED)
		{
			if ((millis() - start) > timeout) // allow time to connect to WiFi - if not then restart
				break;
			delay(750);
			Serial.print(".");
		}

		if(WiFi.status() == WL_CONNECTED)	// if connected then all good
		{
			String ip = WiFi.localIP().toString();
			Serial.println("\nWiFi Connected - " + ip);

			Marquee("WiFi", Colors::GREEN);
			Marquee(ip.c_str(), Colors::GREEN);
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------------
void initWifi()
{
	Serial.print("InitWiFi: ");

	String ssid = GetCfgString("SSID", "");
	String passwd = GetCfgString("PASSWORD", "");

	Serial.println(ssid);
	Serial.println(passwd);

	if( WiFiConnect("BT-R2Z5", "RNfmYVLu6HUX", 4000))
		return;
	if( WiFiConnect(ssid, passwd, WIFI_CONNECT_TIMEOUT))
		return;

	// ssid is not set or connection attempt timed out
	softAP = true;

	WiFi.mode(WIFI_AP);
	WiFi.softAP(HOSTNAME, SOFT_AP_PASSWORD);

	String ip = WiFi.softAPIP().toString();
	Serial.println("\nStarting SoftAP - " + ip);

	Marquee("SoftAP", Colors::BLUE);
	Marquee(ip.c_str(), Colors::BLUE);
}

//--------------------------------------------------------------------
void display_task(void *pvParameter)
{
	ActionRenderer.Intro = "Training Display...";
	while (true)
	{
		if (otaUpdate == false)
			if (ActionRenderer.Render() == false)	// if action renderer is idle then
			{
				if(softAP)
					showTime(Colors::CYAN, Colors::BLUE); //  display the clock
				else
					showTime(Colors::WHITE, Colors::GREEN); //  display the clock
				vTaskDelay(100);
			}

		vTaskDelay(10);
	}
}

//--------------------------------------------------------------------
void initTasks(bool softAP)
{
	Serial.println("Start main task");
	xTaskCreate(&display_task, "display_task", 10000, NULL, uxTaskPriorityGet(NULL), NULL);
}

//--------------------------------------------------------------------
void init_OTA(bool softAP)
{
	if(softAP)
		return;

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
	ArduinoOTA.setPassword(HOSTNAME);
	//ArduinoOTA.setPasswordHash(eecbf409f68cc72c961d8251147ef222);	// MD5 hash of Matrix64

	ArduinoOTA.begin();
}

//--------------------------------------------------------------------
void Beep(int ms)
{
	digitalWrite(BEEPER_PIN, LOW);
	delay(ms);
	digitalWrite(BEEPER_PIN, HIGH);
}
//--------------------------------------------------------------------
void setup()
{
	Serial.begin(115200);
	delay(10);
	Serial.setDebugOutput(true);
	Serial.println("Setup");

	pinMode(BEEPER_PIN, OUTPUT);	// beeper pin
	digitalWrite(BEEPER_PIN, HIGH);

	init_config();
	initMatrix();

	Marquee("Connecting...", Colors::BLUE);

	initWifi();
	initWebsite(softAP);
	initClock(softAP);
	initTasks(softAP);
	init_OTA(softAP);

	Serial.println("Setup Finished.");
}

//--------------------------------------------------------------------
void loop()
{
	if(softAP)
	{
		handleWebsite(softAP);
		updateClock();
			
		vTaskDelay(100);
		// reboot after 5 mins in softap mode
		// if( millis() > 60 * 1000 * 5)
		// 	esp_restart();
	}
	else
	{
		if (isWiFiConnected(750))
		{
			updateClock();
			ArduinoOTA.handle();
		}
		vTaskDelay(100);
	}
}
