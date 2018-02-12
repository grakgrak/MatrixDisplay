#include "website.h"

// define a webserver on port 80
ESP32WebServer webserver(80);

//--------------------------------------------------------------------
String homepage()
{
	String html = R"(	
	<!DOCTYPE HTML><html>
	<head>
	<meta name='viewport' content='width=device-width, initial-scale=1'>
	</head>
	<h1>Matrix 64x32 Control</h1>
	<form action='/start' method='POST'>
	Intro String:<br>
	<input type='text' name='intro' value=''/>
	<br>Activity 1:<br>
	<input type='text' name='act1' value=''/>
	<br>Activity 2:<br>
	<input type='text' name='act2' value=''/>
	<br>Activity 3:<br>
	<input type='text' name='act3' value=''/>
	<br>Activity 4:<br>
	<input type='text' name='act4' value=''/>
	<br><br>
	<input type='submit' value='Submit'/>
	</form>
	</html>
	)";

	return html;
}

//--------------------------------------------------------------------
void handleRoot()
{
	String html = homepage();
	webserver.setContentLength(html.length());
	webserver.send(200, "text/html", html);
}

//--------------------------------------------------------------------
void handleStart()
{
	if (webserver.hasArg("intro"))
	{
		ActionRenderer.Set( 
			webserver.arg("intro"),
			webserver.arg("act1"),
			webserver.arg("act2"),
			webserver.arg("act3"),
			webserver.arg("act4"));
	}

	String html = homepage();
	webserver.setContentLength(html.length());
	webserver.send(200, "text/html", html);
}

//--------------------------------------------------------------------
void initWebsite()
{
	Serial.println("init Website");

	// attach handlers
	webserver.on("/", handleRoot);
	webserver.on("/start", handleStart);

	// start the server
	webserver.begin();
}

//--------------------------------------------------------------------
void loopWebsite()
{
	webserver.handleClient();
}