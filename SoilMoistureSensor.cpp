#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "Constants.cpp"

class SoilMoistureSensor
{
private:
  Time* _timer;
  int _analogPin;
  float _moisture;
  int _elapsedTime;

  const int CHECK_FREQUENCY = 1000;

  void Check()
  {
    _moisture = analogRead(_analogPin);
    _moisture = (_moisture / 1024) * 100; 
    _moisture = (_moisture - 100) * (-1);
  }

  void Debug()
  {
    Serial.print("Moisture: ");
    Serial.println(_moisture);
  }
public:
  SoilMoistureSensor(Time* timer, int analogPin)
  {
    _timer = timer;
    _analogPin = analogPin;
  }

  float Moisture()
  {
    return _moisture;
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

    if (DEBUG_SOIL_MOISTURE_SENSOR || DEBUG_EVERYTHING) Debug();
  }
};
