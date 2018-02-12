#include "actions.h"
#include "demos.h"

TActionRenderer ActionRenderer;

//--------------------------------------------------------------------
const char *getValue(const char *code, int &val)
{
    val = 0;
    while (*code >= '0' && *code <= '9')
        val = val * 10 + (*code++ - '0');

    return code;
}

//--------------------------------------------------------------------
void TActionJob::Set(const String &code)
{
    seconds = 0;
    _lastTick = 0;

    _code = code;
    _ptr = _code.c_str();

    _start = NULL;
    cycle = 0;
    limit = 0;

    state = (*_ptr == '\0') ? SLEEPING : RUN;
}
//--------------------------------------------------------------------
bool TActionJob::Update()
{
    if (state != RUN) // nothing to do if not running
        return false;

    // this is called frequently but we only do something every second
    if ((millis() - _lastTick) < 1000)
        return false;
    _lastTick = millis();

    // still working on the current action
    if (--seconds >= 0)
        return true;

    // get the next action from the command string
    while (true)
    {
        action = *_ptr; // get the current action to perform
        switch (action)
        {
        case 'p': // prep
        case 'w': // work
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
            state = DONE;
            return true;
        default: // ignore anything we dont recognise
            ++_ptr;
            break;
        }
    }
}

//--------------------------------------------------------------------
const char *TActionRenderer::brightness(const char *code)
{
    int level = 0;
    code = getValue(code, level);

    if (level > 10)
        level = 10;
    if (level < 1)
        level = 1;

    matrix.setBrightness(level);

    return code;
}

//--------------------------------------------------------------------
const char *TActionRenderer::text(const char *code)
{
    char buffer[128];

    char *b = buffer;
    while (*code && (*code != '\'') && (*code != '"'))
        *b++ = *code++;
    *b = '\0';

    Marquee(buffer, Colors::WHITE);

    return ++code;
}

//--------------------------------------------------------------------
const char *TActionRenderer::runDemo(const char *code)
{
    int demo = 0;
    code = getValue(code, demo);

    switch (demo)
    {
    case 1:
        wheaterDemo();
        break;
    case 2:
        lavaDemo();
        break;
    case 3:
        primitivesDemo();
        break;
    case 4:
        pongDemo();
        break;
    }

    return code;
}

//--------------------------------------------------------------------
const char *TActionRenderer::pwr(const char *code, int action, int cycle, int limit)
{
    int secs = 0;
    code = getValue(code, secs);

    for (int i = 0; i < secs; ++i)
    {
        FullPWR(action, secs - i, cycle, limit);
        vTaskDelay(1000);
    }
    return code;
}

//--------------------------------------------------------------------
const char *TActionRenderer::repeat(const char *code)
{
    int limit = 0;
    code = getValue(code, limit);

    const char *start = code;
    for (int cycle = 1; cycle <= limit; ++cycle)
    {
        code = Interp(start, cycle, limit);
    }

    return code;
}

//--------------------------------------------------------------------
const char *TActionRenderer::Interp(const char *code, int cycle, int limit)
{
    while (*code)
    {
        switch (*code++)
        {
        case 'p':
            code = pwr(code, 'p', cycle, limit);
            break;
        case 'w':
            code = pwr(code, 'w', cycle, limit);
            break;
        case 'r':
            code = pwr(code, 'r', cycle, limit);
            break;
        case '[':
            code = repeat(code);
            break;

        case '\'':
        case '"':
            code = text(code);
            break;
        case 'b':
            code = brightness(code);
        case 'D':
            code = runDemo(code);
            break;

        case ' ': // ignore spaces
        case '+':
            break;
        default:
            return code;
        }
    }
}

//--------------------------------------------------------------------
void TActionRenderer::Set(const String &intro, const String &act1, const String &act2, const String &act3, const String &act4)
{
    Serial.printf("Setting Jobs %s:%s:%s:%s:%s\n", intro.c_str(), act1.c_str(), act2.c_str(), act3.c_str(), act4.c_str());

    Intro = intro;
    jobs[0].Set(act1);
    jobs[1].Set(act2);
    jobs[2].Set(act3);
    jobs[3].Set(act4);
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
        Interp(Intro.c_str(), 0, 0);
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
                    FullPWR(j.action, j.seconds, j.cycle, j.limit);
                    break;
                case 2:
                    HalfPWR(idx, j.action, j.seconds, j.cycle, j.limit);
                    break;
                case 3:
                    ThirdPWR(idx, j.action, j.seconds, j.cycle, j.limit);
                    break;
                case 4:
                    QuarterPWR(idx, j.action, j.seconds, j.cycle, j.limit);
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
                jobs[i].Set("");
        }

        return true;
    }

    // run the out commands
    if (Outro.length() > 0)
    {
        Interp(Outro.c_str(), 0, 0);
        Outro = "";
        return true;
    }

    return false;
}
