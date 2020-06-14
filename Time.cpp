#pragma once
#include <Arduino.h>
#include "Constants.cpp"
#include <Wire.h>
#include <ds3231.h>

class Time
{
private:
  int _lastMillis;
  int _actualMillis;
  struct ts _dateTime; 

  void Debug()
  {
    DS3231_get(&_dateTime);
    Serial.print("Date : ");
    Serial.print(_dateTime.mday);
    Serial.print("/");
    Serial.print(_dateTime.mon);
    Serial.print("/");
    Serial.print(_dateTime.year);
    Serial.print("\t Hour : ");
    Serial.print(_dateTime.hour);
    Serial.print(":");
    Serial.print(_dateTime.min);
    Serial.print(".");
    Serial.println(_dateTime.sec);
  }
  
public:
  int DeltaTime;
  
  Time()
  {
    _lastMillis = 0;
    DeltaTime = 0;
  }

  void OnSetup()
  {
    Wire.begin();
    DS3231_init(0);
    /*
    _dateTime.hour = 17; 
    _dateTime.min = 5;
    _dateTime.sec = 0;
    _dateTime.mday = 13;
    _dateTime.mon = 06;
    _dateTime.year = 2020;
   
    DS3231_set(_dateTime);
    */
  }

  void OnLoop()
  {
    _actualMillis = millis();
    DeltaTime = _actualMillis - _lastMillis;
    _lastMillis = _actualMillis;

    if (DEBUG_TIMER || DEBUG_EVERYTHING) Debug();
  }

  ts GetTime()
  {
    DS3231_get(&_dateTime);
    return _dateTime;
  }
};
