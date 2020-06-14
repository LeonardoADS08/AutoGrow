#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "DigitalPin.cpp"
#include "SoilMoistureSensor.cpp"
#include "Constants.cpp"

class PumpControl
{
private:
  Time* _timer;
  DigitalPin* _pump;
  SoilMoistureSensor* _sensor;
  int _wateringHours[2] = {7, 21};
  int _wateringHoursCount = 2;
  int _elapsedTime;
  int _wateringElapsedTime;
  bool _watering;
  int _lastHourCheck;
  
  const int CHECK_FREQUENCY = 30000;
  const float DESIRED_MOISTURE = 80;
  const float WATERING_TIME = 3000;

  void TurnOn()
  {
    _watering = true;
    _wateringElapsedTime = 0;
    _pump->TurnOff();
  }

  void TurnOff()
  {
    _pump->TurnOn();
    _watering = false;
  }

  bool WateringHour(int hour)
  {
    for(int i = 0; i < _wateringHoursCount; ++i)
      if (_wateringHours[i] == hour) return true;
    return false;
  }

  void Check()
  {
    float moisture = _sensor->Moisture();
    int actualHour = _timer->GetTime().hour;
    if (_lastHourCheck != actualHour && WateringHour(actualHour))
    {
      _lastHourCheck = actualHour;
      TurnOn();
    }
  }

  void Debug()
  {
    
  }
  
public:
  PumpControl(Time* timer, DigitalPin* pump, SoilMoistureSensor* sensor)
  {
    _timer = timer;
    _pump = pump;
    _sensor = sensor;
    _elapsedTime = 0;
    _watering = false;
    _lastHourCheck = -1;
  }

  void OnSetup()
  {
    Check();
  }

  void OnLoop()
  {
    _elapsedTime += _timer->DeltaTime;
    if (_elapsedTime >= CHECK_FREQUENCY)
    {
      _elapsedTime = 0;
      Check();
    }

    if (_watering)
    {
      _wateringElapsedTime += _timer->DeltaTime;
      if (_wateringElapsedTime >= WATERING_TIME)
      {
        TurnOff();
      }
    }

    if (DEBUG_PUMP_CONTROL || DEBUG_EVERYTHING) Debug();
  }
};
