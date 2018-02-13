#include "website.h"
#include <FS.h>
#include <SPIFFS.h>

// define a webserver on port 80
ESP32WebServer webserver(80);
//holds the current upload
File fsUploadFile;

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
String getContentType(String filename)
{
	if (webserver.hasArg("download"))
		return "application/octet-stream";
	else if (filename.endsWith(".htm"))
		return "text/html";
	else if (filename.endsWith(".html"))
		return "text/html";
	else if (filename.endsWith(".css"))
		return "text/css";
	else if (filename.endsWith(".js"))
		return "application/javascript";
	else if (filename.endsWith(".png"))
		return "image/png";
	else if (filename.endsWith(".gif"))
		return "image/gif";
	else if (filename.endsWith(".jpg"))
		return "image/jpeg";
	else if (filename.endsWith(".ico"))
		return "image/x-icon";
	else if (filename.endsWith(".xml"))
		return "text/xml";
	else if (filename.endsWith(".pdf"))
		return "application/x-pdf";
	else if (filename.endsWith(".zip"))
		return "application/x-zip";
	else if (filename.endsWith(".gz"))
		return "application/x-gzip";
	return "text/plain";
}
//--------------------------------------------------------------------
bool handleFileRead(String path)
{
	Serial.println("handleFileRead: " + path);
	if (path.endsWith("/"))
		path += "index.htm";

	String contentType = getContentType(path);
	String pathWithGz = path + ".gz";

	Serial.println("check exists: " + pathWithGz + " or " + path);

	if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path))
	{
		if (SPIFFS.exists(pathWithGz))
			path += ".gz";

		Serial.println("Sending file: " + path);

		File file = SPIFFS.open(path, "r");
		size_t sent = webserver.streamFile(file, contentType);
		file.close();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------
void handleFileDelete()
{
	if (webserver.args() == 0)
		return webserver.send(500, "text/plain", "BAD ARGS");
	String path = webserver.arg(0);
	Serial.println("handleFileDelete: " + path);
	if (path == "/")
		return webserver.send(500, "text/plain", "BAD PATH");
	if (!SPIFFS.exists(path))
		return webserver.send(404, "text/plain", "FileNotFound");
	SPIFFS.remove(path);
	webserver.send(200, "text/plain", "");
	path = String();
}
//--------------------------------------------------------------------
void handleFileCreate()
{
	if (webserver.args() == 0)
		return webserver.send(500, "text/plain", "BAD ARGS");
	String path = webserver.arg(0);
	Serial.println("handleFileCreate: " + path);
	if (path == "/")
		return webserver.send(500, "text/plain", "BAD PATH");
	if (SPIFFS.exists(path))
		return webserver.send(500, "text/plain", "FILE EXISTS");
	File file = SPIFFS.open(path, "w");
	if (file)
		file.close();
	else
		return webserver.send(500, "text/plain", "CREATE FAILED");
	webserver.send(200, "text/plain", "");
	path = String();
}
//--------------------------------------------------------------------
void handleFileUpload()
{
	if (webserver.uri() != "/edit")
		return;
	HTTPUpload &upload = webserver.upload();
	if (upload.status == UPLOAD_FILE_START)
	{
		String filename = upload.filename;
		if (!filename.startsWith("/"))
			filename = "/" + filename;
		Serial.print("handleFileUpload Name: ");
		Serial.println(filename);
		fsUploadFile = SPIFFS.open(filename, "w");
		filename = String();
	}
	else if (upload.status == UPLOAD_FILE_WRITE)
	{
		//Serial.print("handleFileUpload Data: "); Serial.println(upload.currentSize);
		if (fsUploadFile)
			fsUploadFile.write(upload.buf, upload.currentSize);
	}
	else if (upload.status == UPLOAD_FILE_END)
	{
		if (fsUploadFile)
			fsUploadFile.close();
		Serial.print("handleFileUpload Size: ");
		Serial.println(upload.totalSize);
	}
}
//--------------------------------------------------------------------
void returnFail(String msg)
{
	webserver.send(500, "text/plain", msg + "\r\n");
}
//--------------------------------------------------------------------
void handleFileList()
{
	if (!webserver.hasArg("dir"))
	{
		returnFail("BAD ARGS");
		return;
	}
	String path = webserver.arg("dir");
	
	Serial.println("handleFileList: " + path);

	if (path != "/" && !SPIFFS.exists((char *)path.c_str()))
	{
		returnFail("BAD PATH");
		return;
	}
	File dir = SPIFFS.open((char *)path.c_str());
	path = String();
	if (!dir.isDirectory())
	{
		dir.close();
		returnFail("NOT DIR");
		return;
	}
	dir.rewindDirectory();

	String output = "[";
	for (int cnt = 0; true; ++cnt)
	{
		File entry = dir.openNextFile();
		if (!entry)
			break;

		if (cnt > 0)
			output += ',';

		output += "{\"type\":\"";
		output += (entry.isDirectory()) ? "dir" : "file";
		output += "\",\"name\":\"";
		// Ignore '/' prefix
		output += entry.name() + 1;
		output += "\"";
		output += "}";
		entry.close();
	}
	output += "]";
	webserver.send(200, "text/json", output);
	dir.close();
}

//--------------------------------------------------------------------
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
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

	SPIFFS.begin();
	listDir(SPIFFS, "/", 0);

	// attach handlers
	webserver.on("/", handleRoot);
	webserver.on("/start", handleStart);
	webserver.on("/list", HTTP_GET, handleFileList);
	webserver.on("/edit", HTTP_PUT, handleFileCreate);	//create file
	webserver.on("/edit", HTTP_DELETE, handleFileDelete); //delete file
	webserver.on("/edit", HTTP_GET, []() {
		if (handleFileRead("/edit.html") == false)
			webserver.send(404, "text/plain", "FileNotFound");
	});
	webserver.on("/edit", HTTP_POST, []() {
		webserver.send(200, "text/plain", "");
	}, handleFileUpload);

	webserver.onNotFound([]() {
		if (handleFileRead(webserver.uri()) == false)
			webserver.send(404, "text/plain", "FileNotFound");
	});

	// start the server
	webserver.begin();
}

//--------------------------------------------------------------------
void loopWebsite()
{
	webserver.handleClient();
}