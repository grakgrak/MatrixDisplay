#include "actionRenderer.h"
#include "matrix.h"
#include "clock.h"
#include "config.h"
#include <ArduinoJson.h>

TActionRenderer ActionRenderer;

//--------------------------------------------------------------------
void fullPWR(const TActionJob &job, int idx)
{
	if (job.action == '\0')
		return;

	matrix.startWrite();

	int16_t colour;
	const char *text;
	switch (tolower(job.action))
	{
	case 'p':
		text = "Prep";
		colour = Colors::CYAN;
		break;
	case 'w':
		text = "Work";
		colour = Colors::RED;
		break;
	case 'r':
		text = "Rest";
		colour = Colors::GREEN;
		break;
	case 'c':
		text = "";
		colour = Colors::WHITE;
		break;
	}

	int x = idx * 64; // calc the x offset

	matrix.fillRect(x, 0, 64, ROWS, 0); // Blank the background

	// display action
	matrix.setFont(NULL);
	matrix.setTextWrap(false);
	matrix.setTextColor(colour);
	matrix.drawRect(x, 0, 64, ROWS, job.Colour());
	matrix.setTextSize(1);
	matrix.setCursor(x + 4, 4);
	matrix.print(text);

	if (job.loop > 0) // display cycle of limit
	{
		matrix.setCursor(x + 4, 20);
		matrix.print(job.loop);
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print("of");
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print(job.limit);
	}

	// display seconds
	matrix.setTextSize(2);
	if (job.seconds < 10)
		matrix.setCursor(x + 44, 8);
	else
		matrix.setCursor(x + 38, 8);
	matrix.print(job.seconds);

	matrix.endWrite();
}

//--------------------------------------------------------------------
void drawJob(const TActionJob &job, int count, int idx)
{
	if (job.action == '\0') // code is finished so nothing to show
		return;

	if (job.action == 'c') // if this is the countdown timer
	{
		clock(job.seconds / 60, job.seconds % 60, -1, Colors::WHITE, Colors::GREEN);
		return;
	}

	// if we can display 1 or more full PWR
	if (count * 64 <= COLUMNS)
	{
		fullPWR(job, idx);
		return;
	}

	matrix.startWrite();

#if COLUMNS == 64
	int cols[] = {0, 64, 32, 21, 16};
#else
	int cols[] = {0, 64, 64, 42, 32};
#endif

	int x = idx * cols[count] + (count == 3); // offset by 1 if showing 3 windows
	int w = cols[count];
	int mid = x + w / 2;

	// get the colour
	int16_t colour;
	const char *text;
	switch (tolower(job.action))
	{
	case 'p':
		text = "P";
		colour = Colors::CYAN;
		break;
	case 'w':
		text = "W";
		colour = Colors::RED;
		break;
	case 'r':
		text = "R";
		colour = Colors::GREEN;
		break;
	}

	matrix.setFont(NULL);
	matrix.setTextWrap(false);
	matrix.setTextColor(colour);

	// display action
	matrix.fillRect(x, 0, w, ROWS, 0);			  // Blank the background
	matrix.drawRect(x, 0, w, ROWS, job.Colour()); // draw outline
	matrix.setTextSize(1);
	matrix.setCursor(x + 3, 3);
	matrix.print(text);

	// display seconds
	if (count > 3)
	{
		matrix.setTextSize(1);
		matrix.setCursor((job.seconds < 10) ? mid - 2 : mid - 5, 15);
	}
	else
	{
		if (job.loop > 0) // display loop count of limit
		{
			matrix.setCursor(x + 3, 23);
			matrix.print(job.loop);
			matrix.setCursor(matrix.getCursorX() + 1, 23);
			matrix.print("of");
			matrix.setCursor(matrix.getCursorX() + 1, 23);
			matrix.print(job.limit);
		}

		matrix.setTextSize(2);
		matrix.setCursor((job.seconds < 10) ? mid - 5 : mid - 11, job.loop > 0 ? 8 : 11);
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
void TActionRenderer::Edit(const String &key)
{
	SetJson(ListReadKey(key.c_str()));
	Intro = NULL; // stop the intro from playing
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

	if (j.success())
	{
		_intro = j.get<String>("intro");
		Intro = _intro.c_str();

		jobs[0].Set(j.get<const char *>("act1"), Colors::RED);
		jobs[1].Set(j.get<const char *>("act2"), Colors::GREEN);
		jobs[2].Set(j.get<const char *>("act3"), Colors::BLUE);
		jobs[3].Set(j.get<const char *>("act4"), Colors::WHITE);

		// write the data back to the list
		ListIoU(_intro.c_str(), json);
	}
	else
		Serial.println("failed to parse json: " + json);

	// clear the display 
	matrix.startWrite();
	matrix.black();
	matrix.endWrite();
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
	jobs[job - 1].ToggleRun();
}
//--------------------------------------------------------------------
void TActionRenderer::Reset(int job)
{
	jobs[job - 1].Reset();
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
void showJobState(TActionJob &job, int count, int idx)
{
	if (job.isPaused() == false && job.isDone() == false)
		return;

#if COLUMNS == 64
	int cols[] = {0, 64, 32, 21, 16};
#else
	int cols[] = {0, 64, 64, 42, 32};
#endif

	int mid = idx * cols[count] + cols[count] / 2;

	if( job.action == 'c')	// if this is the countdown timer 
		mid = COLUMNS / 2;

	matrix.startWrite();

	if( job.isPaused())
	{
		matrix.fillRect(mid - 8, 6, 17, 20, Colors::BLACK);
		matrix.fillRect(mid - 6, 8, 5, 16, Colors::WHITE);
		matrix.fillRect(mid + 2, 8, 5, 16, Colors::WHITE);
	}
	else
	{
		// blank the box
		matrix.fillRect(idx * cols[count] + 1, 1, cols[count] - 2, 32 - 2, Colors::BLACK);
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
		// update all the jobs
		int idx = 0;
		for (int i = 0; i < 4; ++i)
		{
			TActionJob &j = jobs[i];

			if (j.Update(jobs))
			{
				// Render the job
				drawJob(j, count, idx);
				showJobState(j, count, idx); // show a job state overlay if needed
			}
			if (j.isActive()) // if the job is not sleeping
				++idx;
		}

		if (activeJobCount() == doneJobCount()) // if all jobs finished
		{
			//ResetAll();
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
