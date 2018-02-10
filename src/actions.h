#ifndef ACTIONS_H
#define ACTIONS_H
#include "matrix.h"
#include <queue>

class TActions
{
public:
  void AddAction(String code);
  String GetAction();

  const char *Interpret(const char *code, int cycle, int limit);

private:
  std::queue<String> _actions;
  bool _cancelAction = false;

  const char *text(const char *code);
  const char *prepare(const char *code, int cycle, int limit);
  const char *work(const char *code, int cycle, int limit);
  const char *rest(const char *code, int cycle, int limit);
  const char *brightness(const char *code);
  const char *repeat(const char *code);
  const char *runDemo(const char *code);
};

extern TActions actions;


class TActionJob
{
public:
  int state;  // Pause, run, cancel

  int action; // Prep, Work, Rest
  int secs;
  int cycle;
  int limit;
  int16_t colour;

  String code;
  void update();  // call every second
private:
  const char *prepare(const char *code, int cycle, int limit);
  const char *work(const char *code, int cycle, int limit);
  const char *rest(const char *code, int cycle, int limit);
  const char *repeat(const char *code);
};


class TActionRenderer
{
public:
  void AddFeed(TActionJob &feedJob);
  void DeleteFeed(TActionJob &feedJob);

  bool HasJobs()  { return _feedJobs.empty() == false; }
private:
  std::vector<TActionJob> _feedJobs;
};

#endif //ACTIONS_H