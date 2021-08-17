/******************************************************************************
  Example_01_BasicReadings.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~ 

  This example reads temperature from the sensor 
  and prints it to the terminal at 115200 baud.

  Note, there are separate functions to read in Celcius
  or Fahrenheit.

  Also, the functions in this library returns float values!

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

#include "SparkFun_AS6212_Qwiic.h"
#include <Wire.h>

AS6212 sensor;

//Initialize temperature variables as floats
float tempC;
float tempF;

void setup(){

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 1 - Basic Readings");

  Wire.begin();

  // Check to see if AS6212 Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x48, I2C-port:Wire)
  if (sensor.begin() == false)
  {
    Serial.println("AS6212 Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 
}

void loop(){
  tempC = sensor.readTempC();
  tempF = sensor.readTempF();

  Serial.println();
  Serial.print("Temperature (°C): ");
  Serial.println(tempC, 6);							//Reads out 6 characters of the temperature float
  Serial.print("Temperature (°F): ");
  Serial.println(tempF, 6);							//Reads out 6 characters of the temperature float
  
  delay(1000);
}
