#include "actionRenderer.h"
#include "matrix.h"
#include "config.h"
#include <ArduinoJson.h>

TActionRenderer ActionRenderer;

//--------------------------------------------------------------------
void fullPWR(const TActionJob &job)
{
	if (job.action == '\0')
		return;
	matrix.startWrite();

	int16_t colour;
	const char *text;
	switch (job.action)
	{
	case 'P':
	case 'p':
		text = "Prep";
		colour = Colors::CYAN;
		break;
	case 'W':
	case 'w':
		text = "Work";
		colour = Colors::RED;
		break;
	case 'R':
	case 'r':
		text = "Rest";
		colour = Colors::GREEN;
		break;
	}

	matrix.setFont(NULL);
	matrix.setTextWrap(false);
	matrix.setTextColor(colour);

	// display action
	matrix.black();
	matrix.drawRect(0, 0, 64, 32, colour);
	matrix.setTextSize(1);
	matrix.setCursor(4, 4);
	matrix.print(text);

	if (job.cycle > 0) // display cycle of limit
	{
		matrix.setCursor(4, 20);
		matrix.print(job.cycle);
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print("of");
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print(job.limit);
	}

	// display seconds
	matrix.setTextSize(2);
	if (job.seconds < 10)
		matrix.setCursor(44, 8);
	else
		matrix.setCursor(38, 8);
	matrix.print(job.seconds);

	matrix.endWrite();
}

//--------------------------------------------------------------------
void drawPWR(int count, int pos, const TActionJob &job)
{
	if (job.action == '\0') // code is finished so nothing to show
		return;

	matrix.startWrite();

	//int background[] = {Colors::RED, Colors::GREEN, Colors::BLUE, Colors::WHITE};

	int mul[] = {0, 1, 32, 21, 16};

	int x = pos * mul[count] + (count == 3); // offset by 1 if showing 3 windows
	int w = mul[count];
	int mid = x + w / 2;

	// get the colour
	int16_t colour;
	const char *text;
	switch (job.action)
	{
	case 'P':
	case 'p':
		text = "P";
		colour = Colors::CYAN;
		break;
	case 'W':
	case 'w':
		text = "W";
		colour = Colors::RED;
		break;
	case 'R':
	case 'r':
		text = "R";
		colour = Colors::GREEN;
		break;
	}

	matrix.setFont(NULL);
	matrix.setTextWrap(false);
	matrix.setTextColor(colour);

	// display action
	matrix.fillRect(x, 0, w, 32, 0);			   // Blank the background
	matrix.drawRect(x, 0, w, 32, job.Colour()); // draw outline
	matrix.setTextSize(1);
	matrix.setCursor(x + 3, 3);
	matrix.print(text);

	// display seconds
	if (count > 2)
	{
		matrix.setTextSize(1);
		matrix.setCursor((job.seconds < 10) ? mid - 2 : mid - 5, 15);
	}
	else
	{
		if (job.cycle > 0) // display cycle of limit
		{
			matrix.setCursor(x + 3, 23);
			matrix.print(job.cycle);
			matrix.setCursor(matrix.getCursorX() + 1, 23);
			matrix.print("of");
			matrix.setCursor(matrix.getCursorX() + 1, 23);
			matrix.print(job.limit);
		}

		matrix.setTextSize(2);
		matrix.setCursor((job.seconds < 10) ? mid - 5 : mid - 11, job.cycle > 0 ? 8 : 11);
	}

	matrix.print(job.seconds);

	matrix.endWrite();
}

//--------------------------------------------------------------------
void TActionRenderer::Select(const String &key)
{
	SetJson(ListReadKey(key.c_str()));
}
//--------------------------------------------------------------------
String TActionRenderer::GetJson()
{
	StaticJsonBuffer<512> jsonBuffer;
	JsonObject &j = jsonBuffer.createObject();

	j["intro"] = _intro;
	j["act1"] = jobs[0].Code();
	j["act2"] = jobs[1].Code();
	j["act3"] = jobs[2].Code();
	j["act4"] = jobs[3].Code();

	String res;
	j.printTo(res);
Serial.println("GetJson: " + res);
	return res;
}
//--------------------------------------------------------------------
void TActionRenderer::SetJson(const String &json)
{
Serial.println("SetJson: " + json);

	StaticJsonBuffer<512> jsonBuffer;
	JsonObject &j = jsonBuffer.parseObject(json);

	if(j.success())
	{
		_intro = j.get<String>("intro");
		Intro = _intro.c_str();

		jobs[0].Set(j.get<const char*>("act1"), Colors::RED);
		jobs[1].Set(j.get<const char*>("act2"), Colors::GREEN);
		jobs[2].Set(j.get<const char*>("act3"), Colors::BLUE);
		jobs[3].Set(j.get<const char*>("act4"), Colors::WHITE);

		// write the data back to the list
		ListIoU(_intro.c_str(), json);
	}
	else
		Serial.println("failed to parse json: " + json);
}
//--------------------------------------------------------------------
void TActionRenderer::StartAll()
{
	for (int i = 0; i < 4; ++i)
		jobs[i].Run();
}
//--------------------------------------------------------------------
void TActionRenderer::PauseAll()
{
	for (int i = 0; i < 4; ++i)
		jobs[i].Pause();
}
//--------------------------------------------------------------------
void TActionRenderer::ResetAll()
{
	for (int i = 0; i < 4; ++i)
		jobs[i].Reset();
}
//--------------------------------------------------------------------
void TActionRenderer::StartPause(int job)
{
	jobs[job-1].ToggleRun();
}
//--------------------------------------------------------------------
void TActionRenderer::Reset(int job)
{
	jobs[job-1].Reset();
}
//--------------------------------------------------------------------
int TActionRenderer::activeJobCount()
{
	int count = 0;
	for (int i = 0; i < 4; ++i)
		if (jobs[i].isActive())
			++count;
	return count;
}
//--------------------------------------------------------------------
int TActionRenderer::doneJobCount()
{
	int count = 0;
	for (int i = 0; i < 4; ++i)
		if (jobs[i].isDone())
			++count;
	return count;
}

//--------------------------------------------------------------------
void showJobState(TActionJob &job, int count, int pos)
{
	if (job.isRunning())
		return;

	int mul[] = {0, 64, 32, 21, 16};

	pos = pos * mul[count] + mul[count] / 2;

	matrix.startWrite();

	if (job.isPaused())
	{
		matrix.fillRect(pos - 6, 8, 5, 16, Colors::WHITE);
		matrix.fillRect(pos + 2, 8, 5, 16, Colors::WHITE);
	}

	if (job.isDone())
	{
		matrix.fillRect(pos - 6, 8, 5, 16, Colors::YELLOW);
		matrix.fillRect(pos + 2, 8, 5, 16, Colors::YELLOW);
	}

	matrix.endWrite();
}
//--------------------------------------------------------------------
bool TActionRenderer::Render() // paints the action jobs onto the display
{
	// run the intro commands
	if (Intro != NULL)
	{
		Marquee(Intro, Colors::WHITE);
		Intro = NULL;
		return true;
	}

	// if some action jobs to run
	int count = activeJobCount();
	if (count > 0)
	{
		int beepSecs = count == 1 ? 4 : 0;

		// update all the jobs
		int idx = 0;
		for (int i = 0; i < 4; ++i)
		{
			TActionJob &j = jobs[i];

			if (j.Update(beepSecs))
			{
				// Render the job
				switch (count)
				{
				case 1:
					fullPWR(j);
					break;
				case 2:
					drawPWR(2, idx, j);
					break;
				case 3:
					drawPWR(3, idx, j);
					break;
				case 4:
					drawPWR(4, idx, j);
					break;
				}
				showJobState(j, count, idx); // show a job state overlay if needed
			}
			if (j.isActive()) // if the job is not sleeping
				++idx;
		}

		if (activeJobCount() == doneJobCount()) // if all jobs finished
		{
			ResetAll();
			// clear the jobs back to sleeping
			//for (int i = 0; i < 4; ++i)
			//	jobs[i].Reset();
		}

		return true;
	}

	// run the out commands
	if (Outro != NULL)
	{
		Marquee(Outro, Colors::WHITE);
		Outro = NULL;
		return true;
	}

	return false;
}
