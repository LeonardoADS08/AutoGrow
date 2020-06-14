#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "DigitalPin.cpp"
#include "Constants.cpp"

class LightControl
{
private:
  DigitalPin* _lights;
  Time* _timer;
  int _lightPeriods[6] = {17, 23, 0, 7};
  int _lightPeriodCount = 4;
  int _elapsedTime;
  
  const int CHECK_FREQUENCY = 10000;
  
  bool BetweenHours(int hour)
  {
    for (int i = 0; i < _lightPeriodCount / 2; ++i)
      if (hour >= _lightPeriods[i*2] && hour <= _lightPeriods[(i * 2) + 1]) return true;
    return false;
  }

  void TurnOnLights()
  {
    _lights->TurnOff();
  }

  void TurnOffLights()
  {
    _lights->TurnOn();
  }

  void Check()
  {
    
    int hour = _timer->GetTime().hour;
    if (BetweenHours(hour))
    {
      TurnOnLights();
    }
    else
    {
      TurnOffLights();
    }
  }

  void Debug()
  {
    int hour = _timer->GetTime().hour;
    Serial.print("LightController: ");
    Serial.print(_lightPeriods[0]);
    Serial.print(" >= ");
    Serial.print(hour);
    Serial.print(" <= ");
    Serial.print(_lightPeriods[1]);
    Serial.print(" || ");
    Serial.print(_lightPeriods[2]);
    Serial.print(" >= ");
    Serial.print(hour);
    Serial.print(" <= ");
    Serial.print(_lightPeriods[3]);
    if (BetweenHours(hour))
      Serial.println(" : ON");
    else
      Serial.println(" : OFF");
  }

public:
  LightControl(Time* timer, DigitalPin* lights)
  {
    _elapsedTime = 0;
    _timer = timer;
    _lights = lights;
  }

  void OnSetup()
  {
    this->Check();
  }

  void OnLoop()
  {
    this->_elapsedTime += _timer->DeltaTime;
    if (this->_elapsedTime >= CHECK_FREQUENCY)
    {
      this->_elapsedTime = 0;
      this->Check();
    }
    
    if (DEBUG_LIGHT_CONTROL || DEBUG_EVERYTHING) Debug();
  }
};
