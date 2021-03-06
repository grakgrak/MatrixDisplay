#pragma once

#include "actionJob.h"

class TActionRenderer
{
  public:
	bool Render(); // paints the action jobs onto the display

	String GetJson();
	void SetJson(const String &json);
	void Select(const String &key); // load and run Intro
	void Edit(const String &key);   // just load

	void StartAll();
	void PauseAll();
	void ResetAll();
	void StartPause(int job);
	void Reset(int job);

	const char *Intro = NULL; // intro message string
	const char *Outro = NULL; // outro message string

  private:
	String _intro = ""; // intro command string
	String _outro = ""; // outro command string
	TActionJob jobs[MAX_JOBS];

	int activeJobCount();
	int doneJobCount();
};

extern TActionRenderer ActionRenderer;
