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

bool AS6212::begin(uint8_t sensorAddress, TwoWire &wirePort){
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

uint8_t AS6212::getAddress(){

  return _deviceAddress;
  
}

uint16_t AS6212::readRegister(uint8_t reg, uint8_t size){

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->endTransmission();
  
  _i2cPort->requestFrom(_deviceAddress, size);
  
  uint8_t dataBuffer[size];
  
  int16_t datac = 0;

  if(_i2cPort->available() <= 2){
    for(size_t i = 0; i < size; i++) dataBuffer[i] = _i2cPort->read();
  }
  
  datac = ((dataBuffer[0] << 8) | dataBuffer[1]);

  return datac;
}

void AS6212::writeRegister(uint8_t reg, int16_t data){

  _i2cPort->beginTransmission(_deviceAddress);
  _i2cPort->write(reg);
  _i2cPort->write(highByte(data));
  _i2cPort->write(lowByte(data));
  _i2cPort->endTransmission();

}

float AS6212::readTempC(){

  int16_t digitalTempC = readRegister(TVAL,2);

  float finalTempC;

  if(digitalTempC < 32768){
    finalTempC = digitalTempC * 0.0078125;
  }

  if(digitalTempC >= 32768){
    finalTempC = ((digitalTempC - 1) * 0.0078125) * -1;
  }

  return finalTempC;
}

float AS6212::readTempF()
{  
  return C_to_F( readTempC() ); 
}

float AS6212::getTLowC(){
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
 * interrupt is triggered. Pointer to error flag to for custom
 * error reporting.
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

bool AS6212::setTHighC(int16_t highLimit){
	
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

uint16_t AS6212::readConfig(){
	
		return readRegister(CONFIG,2);
	
}

void AS6212::setConfig(uint16_t targetState){
		
		writeRegister(CONFIG, targetState);
		
}


/* readAlert
* returns the alert bit status as a boolean
*/

bool AS6212::readAlert(){
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, 5);		
}

/* setConsecutiveFaults
*  sets the number of fault that need to happen in a row before alert is changed
*  valid settings are 1,2,3 or 4, but these correspont to other bit values
*  in the configuration register bits 11 and 12.
*/

void AS6212::setConsecutiveFaults(int faults){

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

    bitWrite(configReg, 11, configBit_11);
    bitWrite(configReg, 12, configBit_12);

    setConfig(configReg);
  }
}

/* readInterruptMode
* returns the interrupt mode bit status as a boolean
*/

bool AS6212::readInterruptMode(){
    uint16_t configReg = readRegister(CONFIG,2);
    return bitRead(configReg, 9);		
}

/* setInterruptMode
*  sets the interrupt mode bits in the config register
*/

void AS6212::setInterruptMode(bool mode){

    uint16_t configReg = readRegister(CONFIG,2);

    bitWrite(configReg, 9, mode);

    setConfig(configReg);
}

float AS6212::C_to_F(float tempC)
{
  return ( (tempC * (9.0 / 5.0)) + 32.0 );
}

float AS6212::F_to_C(float tempF)
{
  return ( (tempF - 32.0) * (5.0 / 9.0) );
}

//Sleep Single-Shot mode (0xC1A0) returns odd register value (FFFFC1A0)
