/******************************************************************************
  SparkFun AS6212 Arduino Library Header File
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams, Madison Chodikov
  Date Created: 8/17/2021
  ~ 

  This file defines AS6212 core function definitions and
  initializers.

  Big thanks to Brandon Williams. This library was based off his 
  original library created 07/15/2020 and can be found here:
  https://github.com/will2055/AS6212-Arduino-Library/

  Special thanks to Madison Chodikov @ SparkFun Electronics
  for code examples from TMP117 Arduino Library
  (https://github.com/sparkfun/SparkFun_TMP117_Arduino_Library)
  
  Development environment specifics:
    IDE: Arduino 1.8.15
    Hardware Platform: SparkFun RedBoard Qwiic v20
    Product Version: v01
    
  Distributed as-is; no warranty is given.
******************************************************************************/

#ifndef __AS6212_H__
#define __AS6212_H__

#include <Wire.h>
#include <Arduino.h>
#include "registers/AS6212_Registers.h"

struct{
	uint8_t tlow_err_flag;
	uint8_t thigh_err_flag;
}ERROR_FLAGS;

class AS6212{
  public:
    AS6212();
    bool begin(uint8_t sensorAddress = 0x48, TwoWire &wirePort = Wire);
    uint8_t getAddress();
    
    // Celcius Functions
    float readTempC();
    float getTLowC();
    bool setTLowC(int16_t lowLimit);
    //bool setTLowC(float lowLimit);
    float getTHighC();
    bool setTHighC(int16_t highLimit);
    //bool setTHighC(float highLimit);

    // Fehrenheit Functions
    float readTempF();
    float getTLowF();
    bool setTLowF(int16_t lowLimit);
    //bool setTLowF(float lowLimit);
    float getTHighF();
    bool setTHighF(int16_t highLimit);
    //bool setTHighF(float highLimit);    

    // Config settings functions
    uint16_t readConfig();
    void setConfig(uint16_t targetState);
    bool readAlert();
    void setConsecutiveFaults(int faults);
    bool readInterruptMode();
    void setInterruptMode(bool mode);
  private:
    TwoWire *_i2cPort = NULL;
    uint8_t _deviceAddress;
    uint16_t readRegister(uint8_t reg, uint8_t size);
    void writeRegister(uint8_t reg, int16_t data);
    float C_to_F(float tempC);
    float F_to_C(float tempF);
};

#define AS6212_MODE_COMPARATOR 0
#define AS6212_MODE_INTERRUPT 1

#endif
