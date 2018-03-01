#pragma once

#include "matrix.h"

class TActionJob
{
	enum TJobState
	{
		SLEEPING,
		READY,
		PAUSE,
		RUN,
		DONE
	};

  public:
	void Set(const char *code, int16_t colour);
	void SetState(TJobState state);

	int action; // Prep, Work, Rest
	int seconds;
	int beepSeconds;
	int loop;
	int limit;
	int repeat;

	bool Update();
	
	void ToggleRun();
	void Run();
	void Pause();
	void Reset();
	void Cancel(); 

	inline TJobState State() const	{ return _state;}

	inline bool isReady() { return State() == READY; }
	inline bool isRunning() { return State() == RUN; }
	inline bool isPaused() { return State() == PAUSE; }
	inline bool isDone() { return State() == DONE; }
	inline bool isActive() { return State() != SLEEPING; }

	inline int16_t Colour() const	{return _colour;}
	inline String Code()	{ return _code;}
  private:
	TJobState _state = SLEEPING; // Pause, run, done
	String _code = "";
	const char *_ptr;
	const char *_loopStart;
	const char *_repeatStart;
	unsigned long _lastTick;
	int16_t _colour;
	bool NextAction();
};
