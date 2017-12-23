#ifndef Timer_h
#define Timer_h
#include "Arduino.h"

class Timer{
public:
  bool triggerForMinute = false;
  void checkForMinute();
  bool triggerMinute();

private:
  int unsigned minute = 0;
};
#endif
