#pragma once
#include <Arduino.h>
#include "Time.cpp"
#include "Dependencies\\dht.h"
#include "Constants.cpp"

class TemperatureHumiditySensor
{
private:
  int _analogPin;
  dht _sensor;
  int _elapsedTime;
  Time* _timer;

  const int READ_FREQUENCY = 1000;

  void Debug()
  {
      Serial.print("Temperatura: ");
      Serial.print(Temperature(), 2);
      Serial.print(" - Humedad: ");
      Serial.println(Humidity(), 2);
  }
  
public:
  TemperatureHumiditySensor(Time* timer, uint8_t analogPin)
  {
    _timer = timer;
    _analogPin = analogPin;
    _elapsedTime = 0; 
  }

  double Humidity()
  {
    return _sensor.Humidity;
  }

  double Temperature()
  {
    return _sensor.Temperature;
  }

  void OnSetup()
  {
    _sensor.Read(_analogPin);
  }
  
  void OnLoop()
  {
    _elapsedTime += _timer->DeltaTime;
    if (_elapsedTime >= READ_FREQUENCY)
    {
      _elapsedTime = 0;
      _sensor.Read(_analogPin);
    }

    if (DEBUG_TEMPERATURE_HUMIDITY_SENSOR || DEBUG_EVERYTHING) Debug();
  }
};
