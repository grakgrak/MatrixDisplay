#ifndef ACTIONS_H
#define ACTIONS_H
#include "matrix.h"


class TActionJob
{
  enum TJobState {SLEEPING, PAUSE, RUN, DONE};
public:
  void Set(const String &code);

  TJobState state = SLEEPING;  // Pause, run, done

  int action; // Prep, Work, Rest
  int seconds;
  int cycle;
  int limit;
  int16_t colour;

  bool Update();
  inline bool isRunning() { return state == RUN; }
  inline bool isPaused() { return state == PAUSE; }
  inline bool isDone() { return state == DONE; }
  inline bool isActive() { return state != SLEEPING; }
private:
  String _code;
  const char *_ptr;
  const char *_start;
  unsigned long _lastTick;
};


class TActionRenderer
{
public:
  bool Render();  // paints the action jobs onto the display

  void Set(const String &intro, const String &act1, const String &act2, const String &act3, const String &act4);

  String Intro; // intro command string
  String Outro; // outro command string
private:
  TActionJob jobs[4];

  int activeJobCount();
  int doneJobCount();

  const char *Interp(const char *code, int cycle, int limit);
  const char *pwr(const char *code, int action, int cycle, int limit);

  const char *text(const char *code);
  const char *brightness(const char *code);
  const char *repeat(const char *code);
};

extern TActionRenderer ActionRenderer;

#endif //ACTIONS_H