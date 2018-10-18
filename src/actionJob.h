#pragma once

#include "matrix.h"

#define	MAX_JOBS	4

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

	bool Update(TActionJob jobs[]);
	
	void ToggleRun();
	void Run(TActionJob *caller);
	void Pause();
	void Reset();
	void Cancel(); 

	inline TJobState State() const	{ return _state;}

	inline bool isReady() const { return State() == READY; }
	inline bool isRunning() const { return State() == RUN; }
	inline bool isPaused() const { return State() == PAUSE; }
	inline bool isDone() const { return State() == DONE; }
	inline bool isActive() const { return State() != SLEEPING; }

	inline int16_t Colour() const	{return _colour;}
	inline String Code()	{ return _code;}
  private:
	TActionJob *_caller;
	TJobState _state = SLEEPING; // Pause, run, done
	String _code = "";
	const char *_ptr;
	const char *_loopStart;
	const char *_repeatStart;
	unsigned long _lastTick;
	int16_t _colour;
	bool NextAction(TActionJob jobs[]);
};
