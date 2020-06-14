#pragma once
#include <MemoryFree.h>
#include "Time.cpp"
#include "DigitalPin.cpp"
#include "TemperatureHumiditySensor.cpp"
#include "SoilMoistureSensor.cpp"
#include "LightControl.cpp"
#include "FanControl.cpp"
#include "PumpControl.cpp"

// PINS
const int PIN_RELAY_LIGHTS = 3;
const int PIN_RELAY_FANS = 4;
const int PIN_RELAY_PUMP = 5;
const int PIN_MOISTURE_SENSOR = A0;
const int PIN_DHT_SENSOR = A1;

Time *Timer = new Time();

DigitalPin *Relay_Lights = new DigitalPin(Timer, PIN_RELAY_LIGHTS, true);
DigitalPin *Relay_Fans = new DigitalPin(Timer, PIN_RELAY_FANS, true);
DigitalPin *Relay_Pump = new DigitalPin(Timer, PIN_RELAY_PUMP, true);

TemperatureHumiditySensor *TemperatureHumidity = new TemperatureHumiditySensor(Timer, PIN_DHT_SENSOR);
SoilMoistureSensor *MoistureSensor = new SoilMoistureSensor(Timer, PIN_MOISTURE_SENSOR);
LightControl *LightController = new LightControl(Timer, Relay_Lights);
FanControl *FanController = new FanControl(Timer, Relay_Fans, TemperatureHumidity);
PumpControl *PumpController = new PumpControl(Timer, Relay_Pump, MoistureSensor);

void setup() 
{
  Serial.begin(9600);
  Timer->OnSetup();   
  Relay_Lights->OnSetup();
  Relay_Fans->OnSetup();
  Relay_Pump->OnSetup();
  TemperatureHumidity->OnSetup();
  MoistureSensor->OnSetup();
  LightController->OnSetup();
  FanController->OnSetup();
  PumpController->OnSetup();
}

void loop() 
{
  Timer->OnLoop();
  Relay_Lights->OnSetup();
  Relay_Fans->OnSetup();
  Relay_Pump->OnSetup();
  TemperatureHumidity->OnLoop();
  MoistureSensor->OnLoop();
  LightController->OnLoop();
  FanController->OnLoop();
  PumpController->OnLoop();

  if (DEBUG_MEMORY_RAM || DEBUG_EVERYTHING) 
  {
    Serial.print("Free RAM: ");
    Serial.println(freeMemory()); 
  }
}
