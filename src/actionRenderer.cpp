#include "actionRenderer.h"
#include "matrix.h"

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

	if (job.cycle > 0) // display cycle of limit
	{
		matrix.setCursor(x + 4, 20);
		matrix.print(job.cycle);
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print("of");
		matrix.setCursor(matrix.getCursorX() + 1, 20);
		matrix.print(job.limit);
	}

	// display seconds
	if (count > 2)
	{
		matrix.setTextSize(1);
		matrix.setCursor((job.seconds < 10) ? mid - 2 : mid - 5, 15);
	}
	else
	{
		matrix.setTextSize(2);
		matrix.setCursor((job.seconds < 10) ? mid - 5 : mid - 11, 11);
	}

	matrix.print(job.seconds);

	matrix.endWrite();
}

//--------------------------------------------------------------------
void TActionRenderer::Set(const String &intro, const String &act1, const String &act2, const String &act3, const String &act4)
{
	_settings = "intro|"+intro+"|input\n" + "act1|"+act1+"|input\n" + "act2|"+act2+"|input\n" + "act3|"+act3+"|input\n" + "act4|"+act4+"|input\n";
	Serial.println("Setting Jobs: " + _settings);

	Intro = intro;
	jobs[0].Set(act1, Colors::RED);
	jobs[1].Set(act2, Colors::GREEN);
	jobs[2].Set(act3, Colors::BLUE);
	jobs[3].Set(act4, Colors::WHITE);
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
		matrix.fillRect(pos - 5, 10, 4, 12, Colors::WHITE);
		matrix.fillRect(pos + 2, 10, 4, 12, Colors::WHITE);
	}

	if (job.isDone())
	{
		matrix.fillRect(pos - 5, 10, 4, 12, Colors::GREEN);
		matrix.fillRect(pos + 2, 10, 4, 12, Colors::GREEN);
	}

	matrix.endWrite();
}
//--------------------------------------------------------------------
bool TActionRenderer::Render() // paints the action jobs onto the display
{
	// run the intro commands
	if (Intro.length() > 0)
	{
		Marquee(Intro.c_str(), Colors::WHITE);
		Intro = "";
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

			if (j.Update())
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
			// clear the jobs back to sleeping
			for (int i = 0; i < 4; ++i)
				jobs[i].Cancel();
		}

		return true;
	}

	// run the out commands
	if (Outro.length() > 0)
	{
		Marquee(Outro.c_str(), Colors::WHITE);
		Outro = "";
		return true;
	}

	return false;
}
