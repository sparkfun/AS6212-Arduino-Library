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

#define AS6212_CONFIG_BIT_ALERT 5
#define AS6212_CONFIG_BIT_CONVERSION_RATE_0 6
#define AS6212_CONFIG_BIT_CONVERSION_RATE_1 7
#define AS6212_CONFIG_BIT_SLEEP_MODE 8
#define AS6212_CONFIG_BIT_INTERRUPT_MODE 9
#define AS6212_CONFIG_BIT_ALERT_POL 10
#define AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_0 11
#define AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_1 12
#define AS6212_CONFIG_BIT_SINGLE_SHOT 15

#define AS6212_MODE_COMPARATOR 0
#define AS6212_MODE_INTERRUPT 1

#define AS6212_CONVERSION_CYCLE_TIME_125MS 3
#define AS6212_CONVERSION_CYCLE_TIME_250MS 2
#define AS6212_CONVERSION_CYCLE_TIME_1000MS 1
#define AS6212_CONVERSION_CYCLE_TIME_4000MS 0

#define AS6212_ALERT_ACTIVE_HIGH 1
#define AS6212_ALERT_ACTIVE_LOW 0



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

    bool getAlertStatus();

    void setConsecutiveFaults(int faults); // valid options are 1, 2, 3 or 4
    uint8_t getConsecutiveFaults();

    void setInterruptMode(bool mode);
    bool getInterruptMode();

    void setConversionCycleTime(uint8_t cycleTime = AS6212_CONVERSION_CYCLE_TIME_250MS);
    uint16_t getConversionCycleTime(); // returns milliseconds (125/250/1000/4000)

    void setAlertPolarity(bool polarity);
    bool getAlertPolarity();

    void sleepModeOn();
    void sleepModeOff();
    bool getSleepMode();

    void triggerSingleShotConversion();
    bool getSingleShotStatus();

  private:
    TwoWire *_i2cPort = NULL;
    uint8_t _deviceAddress;
    uint16_t readRegister(uint8_t reg, uint8_t size);
    void writeRegister(uint8_t reg, int16_t data);
    float C_to_F(float tempC);
    float F_to_C(float tempF);
};



#endif
