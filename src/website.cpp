#include <WiFi.h>
#include <WiFiClient.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "actionRenderer.h"
#include "website/index.htm.gz.h"
#include "website/admin.html.gz.h"
#include "website/edit.html.gz.h"
#include "website/control.html.gz.h"
#include "website/picnic.css.gz.h"
#include "website/style.css.gz.h"
#include "website/microajax.js.gz.h"
#include "website/favicon.ico.gz.h"

#include "website/vue.min.js.gz.h"
#include "website/picnic.min.css.gz.h"

#include "config.h"

//--------------------------------------------------------------------
const char *buildTime = __DATE__ " " __TIME__ " GMT";

AsyncWebServer server(80);
const char *http_username = "matrix";
const char *http_password = "redpill";

const char Page_Restart[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="refresh" content="15; URL=/">
Please Wait....Configuring and Restarting.)=====";

//--------------------------------------------------------------------
void sendGzipResponse(AsyncWebServerRequest *request, const String &mimeType, const uint8_t *content, size_t len)
{
	AsyncWebServerResponse *response = request->beginResponse_P(200, mimeType, content, len);
	response->addHeader("Content-Encoding", "gzip");
	response->addHeader("Last-Modified", buildTime);
	request->send(response);
}

//--------------------------------------------------------------------
void handleConfigPost(AsyncWebServerRequest *request)
{
	SetCfgString("SSID", request->getParam("ssid", true)->value());
	SetCfgString("PASSWORD", request->getParam("password", true)->value());

	if (request->hasParam("bright", true))
	{
		int bright = request->getParam("bright", true)->value().toInt();
		if (bright > 0 && bright <= 10)
		{
			SetCfgInt("Brightness", bright);
			matrix.setBrightness(bright);
		}
	}

	sendGzipResponse(request, "text/html", admin_html_gz, admin_html_gz_len);
	//request->send(204);	// No content
}
//--------------------------------------------------------------------

void initWebsite(bool softAP)
{
	Serial.println("Init Website");

	if (softAP == false)
	{
		MDNS.begin(WiFi.getHostname());
		MDNS.addService("http", "tcp", 80);
	}


	// Json Data Load / Save
	server.on("/json/list", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/json", ListReadAll());
	});

	server.on("/json/load", HTTP_POST, [](AsyncWebServerRequest *request) {
		ActionRenderer.SetJson(request->getParam("schedule", true)->value());
		request->send(200, "text/json", "{status:\"ok\"}");
	});

	server.on("/json/load", HTTP_GET, [](AsyncWebServerRequest *request) {
		if(request->hasParam("name"))
			ActionRenderer.Select(request->getParam("name")->value());
		request->send(200, "text/json", ActionRenderer.GetJson());
	});
	
	server.on("/json/delete", HTTP_GET, [](AsyncWebServerRequest *request) {
		if(request->hasParam("name"))
			ListDelete(request->getParam("name")->value().c_str());
		request->send(204);
	});


	// Data Load / Save
	server.on("/config", HTTP_POST, handleConfigPost);
	server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request) {
		String values =
			"ssid|" + GetCfgString("SSID", "MySSID") + "|input\n" +
			"password|" + GetCfgString("PASSWORD", "MyPassword") + "|input\n" +
			"bright|" + String(GetCfgInt("Brightness", 2)) + "|input\n";

		request->send(200, "text/plain", values);
	});


	// server.on("/vue.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
	// 	sendGzipResponse(request, "text/html", vue_min_js_gz, vue_min_js_gz_len);
	// });
	



	// web pages
	if(softAP)
	{
		server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
			sendGzipResponse(request, "text/html", admin_html_gz, admin_html_gz_len);
		});
	}
	else
	{
		server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
			sendGzipResponse(request, "text/html", index_htm_gz, index_htm_gz_len);
		});
	}
	server.on("/index.htm", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/html", index_htm_gz, index_htm_gz_len);
	});
	server.on("/admin.html", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/html", admin_html_gz, admin_html_gz_len);
	});
	server.on("/edit.html", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/html", edit_html_gz, edit_html_gz_len);
	});
	server.on("/control.html", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/html", control_html_gz, control_html_gz_len);
	});
	server.on("/picnic.css", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/css", picnic_css_gz, picnic_css_gz_len);
	});
	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/css", style_css_gz, style_css_gz_len);
	});
	server.on("/microajax.js", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "text/javascript", microajax_js_gz, microajax_js_gz_len);
	});
	server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
		sendGzipResponse(request, "image/x-icon", favicon_ico_gz, favicon_ico_gz_len);
	});

	// Actions

	server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send_P(200, "text/html", Page_Restart);
		delay(1000);
		esp_restart();
	});

	server.on("/clock", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.SetJson("{}");
		request->send(204);
	});

	server.on("/start-all", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.StartAll();
		request->send(204); // No content
	});
	server.on("/pause-all", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.PauseAll();
		request->send(204); // No content
	});
	server.on("/reset-all", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.ResetAll();
		request->send(204); // No content
	});

	server.on("/startpause-act1", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.StartPause(1);
		request->send(204); // No content
	});
	server.on("/startpause-act2", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.StartPause(2);
		request->send(204); // No content
	});
	server.on("/startpause-act3", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.StartPause(3);
		request->send(204); // No content
	});
	server.on("/startpause-act4", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.StartPause(4);
		request->send(204); // No content
	});

	server.on("/reset-act1", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.Reset(1);
		request->send(204); // No content
	});
	server.on("/reset-act2", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.Reset(2);
		request->send(204); // No content
	});
	server.on("/reset-act3", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.Reset(3);
		request->send(204); // No content
	});
	server.on("/reset-act4", HTTP_GET, [](AsyncWebServerRequest *request) {
		ActionRenderer.Reset(4);
		request->send(204); // No content
	});

	server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request) {
		String text = "High Stack: " + String(uxTaskGetStackHighWaterMark(NULL))
			+ "\nHeap: " + String(ESP.getFreeHeap());
		request->send(200, "text/plain", text);
	});

	server.onNotFound([](AsyncWebServerRequest *request) {
		Serial.print("\nNOT_FOUND: ");
		switch (request->method())
		{
		case HTTP_GET:
			Serial.print("GET");
			break;
		case HTTP_POST:
			Serial.print("POST");
			break;
		case HTTP_DELETE:
			Serial.print("DELETE");
			break;
		case HTTP_PUT:
			Serial.print("PUT");
			break;
		case HTTP_PATCH:
			Serial.print("PATCH");
			break;
		case HTTP_HEAD:
			Serial.print("HEAD");
			break;
		case HTTP_OPTIONS:
			Serial.print("OPTIONS");
			break;
		default:
			Serial.print("UNKNOWN");
			break;
		}
		Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

		if (request->contentLength())
		{
			Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
			Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
		}

		int headers = request->headers();
		int i;
		for (i = 0; i < headers; i++)
		{
			AsyncWebHeader *h = request->getHeader(i);
			Serial.printf("_HEADER[%s]: %s\n", h->name().c_str(), h->value().c_str());
		}

		int params = request->params();
		for (i = 0; i < params; i++)
		{
			AsyncWebParameter *p = request->getParam(i);
			if (p->isFile())
				Serial.printf("_FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
			else
			{
				Serial.print(p->isPost() ? "_POST" : "_GET");
				Serial.printf("[%s]: %s\n", p->name().c_str(), p->value().c_str());
			}
		}

		request->send(404);
	});

	// server.onFileUpload([](AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) {
	// 	if (!index)
	// 		Serial.printf("UploadStart: %s\n", filename.c_str());
	// 	Serial.printf("%s", (const char *)data);
	// 	if (final)
	// 		Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
	// });

	// server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
	// 	if (!index)
	// 		Serial.printf("BodyStart: %u\n", total);
	// 	Serial.printf("%s", (const char *)data);
	// 	if (index + len == total)
	// 		Serial.printf("BodyEnd: %u\n", total);
	// });

	DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
	server.begin();
}
