#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "DigitalPin.cpp"
#include "TemperatureHumiditySensor.cpp"
#include "Constants.cpp"
class FanControl
{
  private:
    Time* _timer;
    DigitalPin* _fans;
    TemperatureHumiditySensor* _sensor;
    bool _cooling;
    int _elapsedTime;
    long _coolingElapsedTime;
    int _ventilateTime[2] = {7, 16};
    int _ventilateTimeCount = 2;
    int _lastVentilatedHour;
  
    const int CHECK_FREQUENCY = 10000;
    const double ON_TEMPERATURE = 28;
    const long ON_TIME = 180000; // 3 minutes

    bool VentilateTime(int hour)
    {
      for (int i = 0; i < _ventilateTimeCount; ++i)
        if (_ventilateTime[i] == hour) return true;
      return false;
    }

    void Check()
    {
      // Es hora de ventilar el ambiente?
      // Si se encuentra apagado, se chequea si hay que refrigerar
      if (!_cooling)
      {
        double temperature = _sensor->Temperature();
        int hour = _timer->GetTime().hour;
      
        if (VentilateTime(hour) && _lastVentilatedHour != hour)
        {
          TurnOn();
          _lastVentilatedHour = hour;
        }
        else if (temperature > ON_TEMPERATURE)
        {
          TurnOn();
        }
      }
    }

    void TurnOn()
    {
        _cooling = true;
        _coolingElapsedTime = 0;
        _fans->TurnOff();      
    }

    void TurnOff()
    {
        _cooling = false;
        _coolingElapsedTime = 0;
        _fans->TurnOn();
    }

    void Debug()
    {

    }
    
  public:
    FanControl(Time* timer, DigitalPin* fans, TemperatureHumiditySensor* sensor)
    {
      _timer = timer;
      _fans = fans;
      _sensor = sensor;
      _cooling = false;
      _elapsedTime = 0;
      _lastVentilatedHour = -1;
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

      if (_cooling)
      {
        _coolingElapsedTime += _timer->DeltaTime;
        if (_coolingElapsedTime >= ON_TIME)
        {
          TurnOff();
        }
      }
      
      if (DEBUG_FAN_CONTROL || DEBUG_EVERYTHING) Debug();
    }
};
