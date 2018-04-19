#include "actionJob.h"

extern void Beep(int ms);

//--------------------------------------------------------------------
const char *getValue(const char *code, int &val)
{
	val = 0;
	while (*code >= '0' && *code <= '9')
		val = val * 10 + (*code++ - '0');

	return code;
}

//--------------------------------------------------------------------
void TActionJob::Set(const char *code, int16_t colour)
{
	loop = 0;
	limit = 0;
	seconds = 0;
	beepSeconds = 0;
	_lastTick = 0;
	_loopStart = NULL;
	_repeatStart = NULL;

	_colour = colour;
	_code = (code == NULL) ? "" : code;
	_ptr = _code.c_str();
	_state = (*_ptr == '\0') ? SLEEPING : READY;

	if (isActive())
		NextAction(NULL); // init the properties
}
//--------------------------------------------------------------------
void TActionJob::SetState(TJobState state)
{
	_state = state;
	_lastTick = 0;
}

//--------------------------------------------------------------------
void TActionJob::ToggleRun()
{
	if (State() == SLEEPING || State() == DONE)
		return;

	SetState((State() == RUN) ? PAUSE : RUN);
}
//--------------------------------------------------------------------
void TActionJob::Run()
{
	if (State() == SLEEPING || State() == DONE)
		return;

	SetState(RUN);
}
//--------------------------------------------------------------------
void TActionJob::Cancel()
{
	Set("", _colour);
}
//--------------------------------------------------------------------
void TActionJob::Pause()
{
	if (State() != RUN)
		return;

	SetState(PAUSE);
}
//--------------------------------------------------------------------
void TActionJob::Reset()
{
	Set(_code.c_str(), _colour);
}
//--------------------------------------------------------------------
bool TActionJob::NextAction(TActionJob jobs[])
{
	if (_ptr == NULL)
		return false;

	int idx = 0;
	while (true)
	{
		action = *_ptr++; // set the current action to perform

		switch (tolower(action))
		{
		case 'x':
			_ptr = getValue(_ptr, idx);
			if( *_ptr != '\0')	// if we are not at the end of the command string
				Pause();
			if ((jobs != NULL) && (idx > 0) && (idx <= MAX_JOBS))
			{
				if( &jobs[idx - 1] != this)
					jobs[idx - 1].Run();
			}
			return true;
		case 'b': // beep seconds
			_ptr = getValue(_ptr, beepSeconds);
			break;
		case 'r': // rest - special case
			_ptr = getValue(_ptr, seconds);
			if (limit > 0 && loop == limit) // ignore the last rest if in a loop
			{
				seconds = 0;		 // reset seconds as we are ignoreing the last rest
				while (*_ptr == ' ') // skip spaces
					++_ptr;

				if (*_ptr == ')' || *_ptr == ']')
					break;
			}
			return true;
		case 'p': // prep
		case 'w': // work
		case 'c': // countdown
			_ptr = getValue(_ptr, seconds);
			return true;
		case '[': // repeat
			_ptr = getValue(_ptr, repeat);
			_repeatStart = _ptr;
			break;
		case ']': // end repeat
			if ((_repeatStart != NULL) && (--repeat > 0))
			{
				_ptr = _repeatStart; // reset to start of repeat
				break;
			}
			_repeatStart = NULL;
			break;
		case '(': // repeat
			_ptr = getValue(_ptr, limit);
			loop = 1;
			_loopStart = _ptr;
			break;
		case ')': // end repeat
			if ((_loopStart != NULL) && (++loop <= limit))
			{
				_ptr = _loopStart; // reset to start of loop
				break;
			}
			loop = 0;
			limit = 0;
			_loopStart = NULL;
			break;
		case '\0': // end of command
			_ptr = NULL;
			SetState(DONE);
			return true;
		default: // ignore anything we dont recognise
			break;
		}
	}
}
//--------------------------------------------------------------------
bool TActionJob::Update(TActionJob jobs[])
{
	if (State() == SLEEPING) // nothing to do if sleeping
		return false;

	// this is called frequently but we only do something every second
	if ((millis() - _lastTick) < 1000)
		return false;
	_lastTick = millis();

	if (State() == READY || State() == PAUSE)
		return true;

	--seconds;

	if (seconds >= 0 && seconds < beepSeconds)
		Beep((seconds == 0) ? 200 : 50);

	// still working on the current action
	if (seconds >= 0)
		return true;

	// get the next action from the command string
	return NextAction(jobs);
}
