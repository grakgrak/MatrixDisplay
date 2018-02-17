#include "actionJob.h"

//--------------------------------------------------------------------
const char *getValue(const char *code, int &val)
{
	val = 0;
	while (*code >= '0' && *code <= '9')
		val = val * 10 + (*code++ - '0');

	return code;
}

//--------------------------------------------------------------------
void TActionJob::Set(const String &code, int16_t colour)
{
	cycle = 0;
	limit = 0;
	seconds = 0;
	_lastTick = 0;
	_start = NULL;

	_colour = colour;
	_code = code;
	_ptr = _code.c_str();
	_state = (*_ptr == '\0') ? SLEEPING : READY;

	if( isActive())
		NextAction();	// init the properties
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
	Set(_code, _colour);
}
//--------------------------------------------------------------------
bool TActionJob::NextAction()
{
	while(true)
	{
		action = *_ptr; // get the current action to perform
		switch (action)
		{
		case 'P': // prep
		case 'p': // prep
		case 'W': // work
		case 'w': // work
		case 'R': // rest
		case 'r': // rest
			_ptr = getValue(++_ptr, seconds);
			return true;
		case '[': // repeat
			_ptr = getValue(++_ptr, limit);
			cycle = 1;
			_start = _ptr;
			break;
		case ']': // end repeat
			if ((_start != NULL) && (++cycle < limit))
			{
				_ptr = _start; // reset to start of loop
				break;
			}
			_start = NULL;
			++_ptr;
			break;
		case '\0': // end of command
			SetState(DONE);
			return true;
		default: // ignore anything we dont recognise
			++_ptr;
			break;
		}
	}
}
//--------------------------------------------------------------------
bool TActionJob::Update()
{
	if (State() == SLEEPING) // nothing to do if sleeping
		return false;

	// this is called frequently but we only do something every second
	if ((millis() - _lastTick) < 1000)
		return false;
	_lastTick = millis();

	if (State() == READY || State() == PAUSE)
		return true;

	// still working on the current action
	if (--seconds >= 0)
		return true;

	// get the next action from the command string
	return NextAction();
}
