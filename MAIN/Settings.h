#pragma once
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "CONFIG.h"
#include "DS3231.h" // подключаем часы реального времени
#include "LoRa.h" // LoRa
#include "CoreTransport.h" // SIM800
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class SettingsClass
{
  public:
    SettingsClass();

    void begin();
  
  private:

  
};
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern SettingsClass Settings;
