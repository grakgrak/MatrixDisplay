#include "actions.h"
#include "demos.h"

TActions actions;

const char *TActions::text(const char *code)
{
    char buffer[128];

    int rpt = 0;
    while (*code >= '0' && *code <= '9')
        rpt = rpt * 10 + (*code++ - '0');
    if (rpt == 0)
        rpt = 1;

    char *b = buffer;
    while (*code && (*code != '\'') && (*code != '"'))
        *b++ = *code++;
    *b = '\0';

    for(int i = 0; i < rpt; ++i)
    {
        marquee(buffer, RGB(127, 127, 127));
        if (_cancelAction)
            return "";
    }

    return ++code;
}

const char *TActions::prepare(const char *code, int cycle, int limit)
{
    int secs = 0;
    while (*code >= '0' && *code <= '9')
        secs = secs * 10 + (*code++ - '0');

    for (int i = 0; i < secs; ++i)
    {
        if (_cancelAction)
            return "";
        displayPWR("Prep", RGB(0, 0, 127), secs - i, cycle, limit);
        vTaskDelay(1000);
    }
    return code;
}

const char *TActions::work(const char *code, int cycle, int limit)
{
    int secs = 0;
    while (*code >= '0' && *code <= '9')
        secs = secs * 10 + (*code++ - '0');

    for (int i = 0; i < secs; ++i)
    {
        if (_cancelAction)
            return "";
        displayPWR("Work", RGB(127, 0, 0), secs - i, cycle, limit);
        vTaskDelay(1000);
    }
    return code;
}

const char *TActions::rest(const char *code, int cycle, int limit)
{
    int secs = 0;
    while (*code >= '0' && *code <= '9')
        secs = secs * 10 + (*code++ - '0');

    for (int i = 0; i < secs; ++i)
    {
        if (_cancelAction)
            return "";
        if (cycle == 0 || (cycle < limit)) // skip the last rest period
            displayPWR("Rest", RGB(0, 127, 0), secs - i, cycle, limit);
        vTaskDelay(1000);
    }

    return code;
}

const char *TActions::brightness(const char *code)
{
    int level = 0;
    while (*code >= '0' && *code <= '9')
        level = level * 10 + (*code++ - '0');

    if (level > 10)
        level = 10;
    if (level < 1)
        level = 1;

    matrix.setBrightness(level);

    return code;
}

const char *TActions::repeat(const char *code)
{
    int limit = 0;
    while (*code >= '0' && *code <= '9')
        limit = limit * 10 + (*code++ - '0');

    const char *start = code;
    for (int cycle = 1; cycle <= limit; ++cycle)
    {
        code = Interpret(start, cycle, limit);
        if(_cancelAction)
            return "";
    }

    return code;
}

const char *TActions::runDemo(const char *code)
{
    int demo = 0;
    while (*code >= '0' && *code <= '9')
        demo = demo * 10 + (*code++ - '0');

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

const char *TActions::Interpret(const char *code, int cycle, int limit)
{
    while (*code)
    {
        if (_cancelAction)
            return "";

        switch (*code++)
        {
        case 'p':
            code = prepare(code, cycle, limit);
            break;
        case 'w':
            code = work(code, cycle, limit);
            break;
        case 'r':
            code = rest(code, cycle, limit);
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

void TActions::AddAction(String code)
{
    vTaskSuspendAll();
    _cancelAction = true;
    _actions.push(code);
    xTaskResumeAll();
}

String TActions::GetAction()
{
    String code;

    vTaskSuspendAll();
    if (_actions.size() > 0)
    {
        code = _actions.front();
        _actions.pop();
        _cancelAction = false;
    }
    xTaskResumeAll();

    return code;
}
