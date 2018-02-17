#pragma once

#include "actionJob.h"

class TActionRenderer
{
  public:
	bool Render(); // paints the action jobs onto the display

	String Get()	{return _settings;}
	void Set(const String &intro, const String &act1, const String &act2, const String &act3, const String &act4);
	
	void StartAll();
	void PauseAll();
	void ResetAll();
	void StartPause(int job);
	void Reset(int job);

	String Intro; // intro command string
	String Outro; // outro command string

  private:
	String _settings;
	TActionJob jobs[4];

	int activeJobCount();
	int doneJobCount();
};

extern TActionRenderer ActionRenderer;

