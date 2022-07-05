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

/*
  NOTE: Read for use for the most accurate readings from the sensor
  - Avoid heavy bypass traffic on the I2C bus for most accurate temperature readings
  - Use the highest available communication speeds
  - Use the minimal supply voltage acceptable for the system
*/

#include <Arduino.h>
#include <Wire.h>
#include "registers/AS6212_Registers.h"
#include "SparkFun_AS6212_Qwiic.h"

/* CONSTRUCTOR
    This function will use the main I2C port on the Arduino 
  by default, but this is configurable with the setBus function.
  This needs to be called when running the example sketches to
  initialize the sensor and be able to call to the library. 
*/

AS6212::AS6212()
{

}

/*
  Begin function. Sets the address for I2C communication.
  Returns True if checks pass.
 */

bool AS6212::begin(uint8_t sensorAddress, TwoWire &wirePort)
{
  _i2cPort = &wirePort;
  _deviceAddress = sensorAddress;

  _i2cPort->beginTransmission(_deviceAddress);
 
  if(_i2cPort->endTransmission() != 0){
    return false;
  }

  else{
    return true;
  }
}

uint8_t AS6212::getAddress()
{
  return _deviceAddress;
}

uint16_t AS6212::readRegister(uint8_t reg, uint8_t size)
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->endTransmission();
  
  _i2cPort->requestFrom(_deviceAddress, size);
  
  uint8_t dataBuffer[size];
  
  int16_t datac = 0;

  if(_i2cPort->available() <= 2)
  {
    for(size_t i = 0; i < size; i++) dataBuffer[i] = _i2cPort->read();
  }
  
  datac = ((dataBuffer[0] << 8) | dataBuffer[1]);

  return datac;
}

void AS6212::writeRegister(uint8_t reg, int16_t data)
{
  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->write(highByte(data));
  _i2cPort->write(lowByte(data));
  _i2cPort->endTransmission();
}

float AS6212::readTempC()
{

  int16_t digitalTempC = readRegister(TVAL,2);

  float finalTempC;

  if(digitalTempC < 32768)
  {
    finalTempC = digitalTempC * 0.0078125;
  }

  if(digitalTempC >= 32768)
  {
    finalTempC = ((digitalTempC - 1) * 0.0078125) * -1;
  }

  return finalTempC;
}

float AS6212::readTempF()
{  
  return C_to_F( readTempC() ); 
}

float AS6212::getTLowC()
{
	int16_t lowTemp = readRegister(TLOW,2);
	
	float temp;

	if(lowTemp < 32768){
		temp = lowTemp * 0.0078125;
	}

	if(lowTemp >= 32768){
		temp = ((lowTemp - 1) * 0.0078125) * -1;
	}
	
	return temp;
}

float AS6212::getTLowF()
{
  return C_to_F(getTLowC());
}

/*
 * Sets TLow Threshold, if temp drops below the threshold then 
 * interrupt is triggered.
 */
bool AS6212::setTLowC(int16_t lowLimit)
{        
    int16_t lowTemp = lowLimit / 0.0078125;
    writeRegister(TLOW, lowTemp);
    return true;
}

bool AS6212::setTLowF(int16_t lowLimit)
{
  float temp_float = float(lowLimit); // cast to float
  temp_float = F_to_C(temp_float); // convert to C
  return setTLowC(int16_t(temp_float)); // cast back to int16_t, set with C value & function
}

float AS6212::getTHighC()
{
	int16_t highTemp = readRegister(THIGH,2);
	
	float temp;

	if(highTemp < 32768){
		temp = highTemp * 0.0078125;
	}

	if(highTemp >= 32768){
		temp = ((highTemp - 1) * 0.0078125) * -1;
	}

	return temp;
}

float AS6212::getTHighF()
{
  return C_to_F(getTHighC());
}

bool AS6212::setTHighC(int16_t highLimit)
{
		int16_t highTemp = highLimit / 0.0078125;
		writeRegister(THIGH, highTemp);
		return true;
}

bool AS6212::setTHighF(int16_t highLimit)
{
  float temp_float = float(highLimit); // cast to float
  temp_float = F_to_C(temp_float); // convert to C
  return setTHighC(int16_t(temp_float)); // cast back to int16_t, set with C value & function
}

uint16_t AS6212::readConfig()
{
		return readRegister(CONFIG,2);
}

void AS6212::setConfig(uint16_t targetState)
{
		writeRegister(CONFIG, targetState);
}


/* getAlertStatus
* returns the alert bit status as a boolean
*/

bool AS6212::getAlertStatus()
{
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, AS6212_CONFIG_BIT_ALERT);		
}

/* setConsecutiveFaults
*  sets the number of fault that need to happen in a row before alert is changed
*  valid settings are 1,2,3 or 4, but these correspont to other bit values
*  in the configuration register bits 11 and 12.
*/

void AS6212::setConsecutiveFaults(int faults)
{
  if ((faults > 4) || (faults < 1)) 
  {
     // discard out of range fault values
  } 
  else
  {
    faults = faults - 1; // consecutive faults value is stored in just 2 bits in the config reg,
    // so we must convert from "human readable" ints 1-4 to stored values (0-3).

    uint16_t configReg = readRegister(CONFIG,2);

    bool configBit_11 = bitRead(faults, 0);
    bool configBit_12 = bitRead(faults, 1);

    bitWrite(configReg, AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_0, configBit_11);
    bitWrite(configReg, AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_1, configBit_12);

    setConfig(configReg);
  }
}

/* getConsecutiveFaults
*  gets the number of fault that need to happen in a row before alert is changed
*  valid settings are 1,2,3 or 4, but these correspont to other bit values
*  in the configuration register bits 11 and 12.
*/

uint8_t AS6212::getConsecutiveFaults()
{
  int faults;

  uint16_t configReg = readRegister(CONFIG,2);

  bool consecutiveFaultsBit_0 = bitRead(configReg, AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_0);
  bool consecutiveFaultsBit_1 = bitRead(configReg, AS6212_CONFIG_BIT_CONSECUTIVE_FAULTS_1);

  bitWrite(faults, 0, consecutiveFaultsBit_0);
  bitWrite(faults, 1, consecutiveFaultsBit_1);

  faults = faults + 1; // consecutive faults value is stored in just 2 bits in the config reg,
  // so we must convert from stored values (0-3) to "human readable" values (1-4).

  return faults;
}

/* setInterruptMode
*  sets the interrupt mode bits in the config register
*  
*  Valid options are:
*  AS6212_MODE_COMPARATOR
*  AS6212_MODE_INTERRUPT
*/

void AS6212::setInterruptMode(bool mode)
{
    uint16_t configReg = readRegister(CONFIG,2);
    bitWrite(configReg, AS6212_CONFIG_BIT_INTERRUPT_MODE, mode);
    setConfig(configReg);
}

/* getInterruptMode
* returns the interrupt mode bit status as a boolean
*/

bool AS6212::getInterruptMode()
{
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, AS6212_CONFIG_BIT_INTERRUPT_MODE);		
}

/* setConversionCycleTime
*  sets the conversion cylce time (aka convertion rate) in the config register
*  valid settings are:
*
*  AS6212_CONVERSION_CYCLE_TIME_125MS
*  AS6212_CONVERSION_CYCLE_TIME_250MS
*  AS6212_CONVERSION_CYCLE_TIME_1000MS
*  AS6212_CONVERSION_CYCLE_TIME_4000MS
*/

void AS6212::setConversionCycleTime(uint8_t cycleTime)
{
  if ((cycleTime > 3) || (cycleTime < 0)) 
  {
     // discard out of range values
  } 
  else
  {
    uint16_t configReg = readRegister(CONFIG,2);

    bool configBit_6 = bitRead(cycleTime, 0);
    bool configBit_7 = bitRead(cycleTime, 1);

    bitWrite(configReg, AS6212_CONFIG_BIT_CONVERSION_RATE_0, configBit_6);
    bitWrite(configReg, AS6212_CONFIG_BIT_CONVERSION_RATE_1, configBit_7);

    setConfig(configReg);
  }
}

/* getConversionCycleTime
*  gets the conversion cylce time (aka convertion rate) in the config register
*  returns the cycle time value as milliseconds: (125/250/1000/4000)
*/

uint16_t AS6212::getConversionCycleTime()
{
  int cycleTime;

  uint16_t configReg = readRegister(CONFIG,2);

  bool conversionRateBit_0 = bitRead(configReg, AS6212_CONFIG_BIT_CONVERSION_RATE_0);
  bool conversionRateBit_1 = bitRead(configReg, AS6212_CONFIG_BIT_CONVERSION_RATE_1);

  bitWrite(cycleTime, 0, conversionRateBit_0);
  bitWrite(cycleTime, 1, conversionRateBit_1);

  // conversion rate value is stored in just 2 bits in the config reg,
  // so we must convert from stored values (0-3) to "human readable" values (125/250/1000/4000).
  if(cycleTime == AS6212_CONVERSION_CYCLE_TIME_125MS) return 125;
  if(cycleTime == AS6212_CONVERSION_CYCLE_TIME_250MS) return 250;
  if(cycleTime == AS6212_CONVERSION_CYCLE_TIME_1000MS) return 1000;
  if(cycleTime == AS6212_CONVERSION_CYCLE_TIME_4000MS) return 4000;
  return 0; // gotta return something to avoid compiler error
}

/* setAlertPolarity
*  sets the alert polarity bit in the config register

The polarity bit configures the polarity of the ALERT output. If the polarity bit is cleared, the ALERT
output is low active while it becomes high active if the polarity bit is set to ‘1’.

*/

void AS6212::setAlertPolarity(bool polarity)
{
    uint16_t configReg = readRegister(CONFIG,2);
    bitWrite(configReg, AS6212_CONFIG_BIT_ALERT_POL, polarity);
    setConfig(configReg);
}

/* getAlertPolarity
* returns the alert polarity bit status as a boolean
*/

bool AS6212::getAlertPolarity()
{
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, AS6212_CONFIG_BIT_ALERT_POL);		
}

/* sleepModeOn
*  sets the sleep mode bit in the config register
*  This can take 120ms + a conversion time (around 150ms total)
*  Note, this also triggers a SS conversion as recommended at DS section 6.2.4
*/

void AS6212::sleepModeOn()
{
    uint16_t configReg = readRegister(CONFIG,2);
    bitWrite(configReg, AS6212_CONFIG_BIT_SLEEP_MODE, 1);
    bitWrite(configReg, AS6212_CONFIG_BIT_SINGLE_SHOT, 1); // trigger SS
    setConfig(configReg);
}

/* sleepModeOff
*  clears the sleep mode bit in the config register
*  Note, after reseting the SM bit to 0,
*  the device enters the continuous conversion mode.
*/

void AS6212::sleepModeOff()
{
    uint16_t configReg = readRegister(CONFIG,2);
    bitWrite(configReg, AS6212_CONFIG_BIT_SLEEP_MODE, 0);
    setConfig(configReg);
}

/* getSleepMode
* returns the sleep mode bit status as a boolean
*/

bool AS6212::getSleepMode()
{
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, AS6212_CONFIG_BIT_SLEEP_MODE);		
}

/* triggerSingleShotConversion
*  sets the SS mode bit in the config register
*  Note, you must be in sleep mode for this to work
*/

void AS6212::triggerSingleShotConversion()
{
    uint16_t configReg = readRegister(CONFIG,2);
    // only do this, if we are in sleep mode
    if (bitRead(configReg, AS6212_CONFIG_BIT_SLEEP_MODE) == true)
    {
      bitWrite(configReg, AS6212_CONFIG_BIT_SINGLE_SHOT, 1); // trigger SS
      setConfig(configReg);
    }
}

/* getSingleShotStatus
*  returns the SS mode bit status as a boolean
*  0 = No conversion ongoing/ conversion finished
*  1 = Start single shot conversion / conversion ongoing
*/

bool AS6212::getSingleShotStatus()
{
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, AS6212_CONFIG_BIT_SINGLE_SHOT);		
}

float AS6212::C_to_F(float tempC)
{
  return ( (tempC * (9.0 / 5.0)) + 32.0 );
}

float AS6212::F_to_C(float tempF)
{
  return ( (tempF - 32.0) * (5.0 / 9.0) );
}

/* setDefaultSettings
*  Sets up the sensor with the following default settings:
*
*  Sleep Mode: OFF
*  Conversion Rate (aka Conversion cycletime): 250MS (aka 4 per second)
*  Interrupt Mode: COMPARATOR
*  Alert Polarity: ACTIVE LOW
*  Consecutive Faults: 1
*  TLOW: 75C
*  THIGH: 80C
*/

void AS6212::setDefaultSettings()
{
  sleepModeOff();
  setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_250MS);
  setInterruptMode(AS6212_MODE_COMPARATOR);
  setAlertPolarity(AS6212_ALERT_ACTIVE_LOW);
  setConsecutiveFaults(1);
  setTLowC(75);
  setTHighC(80);
}
