#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "Constants.cpp"

const int PIN_STATE_NOT_INITIALIZED_LOW = -2;
const int PIN_STATE_NOT_INITIALIZED_HIGH = -1;
const int PIN_STATE_LOW = 1;
const int PIN_STATE_HIGH = 2;
const int PIN_STATE_DELAYED_HIGH = 3;
const int PIN_STATE_DELAYED_LOW = 4;
  
class DigitalPin
{
private:
  int _pin;
  short _state; // -1. Not initalized - 1. Off - 2. On - 3. Delayed turn on - 4. Delayed turn off
  int _delayTime;
  int _elapsedTime;
  Time* _timer;

public:
  
  DigitalPin(Time* timer, int pin, bool defautlState = false)
  {
    _timer = timer;
    _pin = pin;
    _state = defautlState ? PIN_STATE_NOT_INITIALIZED_HIGH : PIN_STATE_NOT_INITIALIZED_LOW;
  }

  void TurnOn(int delayMillis = 0)
  {
    if (delayMillis > 0)
    {
      _state = PIN_STATE_DELAYED_HIGH;
      _delayTime = delayMillis;
      _elapsedTime = 0;
    }
    else  
    {
      _state = PIN_STATE_HIGH;
      digitalWrite(_pin, HIGH);
    }
  }

  void TurnOff(int delayMillis = 0)
  {
    if (delayMillis > 0)
    {
      _state = PIN_STATE_DELAYED_LOW;
      _delayTime = delayMillis;
      _elapsedTime = 0;
    }
    else 
    {
      _state = PIN_STATE_LOW;
      digitalWrite(_pin, LOW);
    }
  }

  void OnSetup()
  {
    pinMode(_pin, OUTPUT); 
    if (_state == PIN_STATE_NOT_INITIALIZED_LOW) 
    {
      digitalWrite(_pin, LOW);
      _state = PIN_STATE_LOW;
    }
    else if (_state == PIN_STATE_NOT_INITIALIZED_HIGH) 
    {
      digitalWrite(_pin, HIGH);
      _state = PIN_STATE_HIGH;
    }   
  }
  
  void OnLoop()
  {
    if (_state == PIN_STATE_DELAYED_HIGH || _state == PIN_STATE_DELAYED_LOW)
    {
      _elapsedTime += _timer->DeltaTime;
      if (_elapsedTime >= _delayTime)
      {
        if (_state == PIN_STATE_DELAYED_HIGH) TurnOn();
        else if (_state == PIN_STATE_DELAYED_LOW) TurnOff();
      }
    } 
  }

  int State()
  {
    return _state;
  }
};
