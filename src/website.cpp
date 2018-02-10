#include "website.h"

// define a webserver on port 80
ESP32WebServer webserver(80);

String homepage()
{
	String html = R"(	
	<!DOCTYPE HTML><html>
	<head>
	<meta name='viewport' content='width=device-width, initial-scale=1'>
	</head>
	<h1>Matrix 64x32 Control</h1>
	<form action='/interp' method='POST'>
	Command String:<br>
	<input type='text' name='code' value=''/>
	<br><br>
	<input type='submit' value='Submit'/>
	</form>
	</html>
)";

    return html;
}


void handleRoot()
{
    String html = homepage();
	webserver.setContentLength(html.length());
	webserver.send(200, "text/html", html);
}

void handleInterp()
{
	if(webserver.hasArg("code"))
	{
		String code = webserver.arg("code");
		
		Serial.println(code);

		actions.AddAction(code);
	}

    String html = homepage();
	webserver.setContentLength(html.length());
	webserver.send(200, "text/html", html);
}

void initWebsite()
{
	Serial.println("init Website");

   	// attach handlers
	webserver.on("/", handleRoot);
	webserver.on("/interp", handleInterp);

	// start the server
	webserver.begin();
}

void loopWebsite()
{
	webserver.handleClient();
}