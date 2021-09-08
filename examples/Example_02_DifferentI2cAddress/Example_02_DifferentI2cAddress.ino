/******************************************************************************
  Example_02_DifferentI2cAddress.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~ 

  This example reads temperature from the sensor using a different I2C address
  and prints it to the terminal at 115200 baud.

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------

  Note, the I2C address can be set to a variety of addresses using the 4 address jumpers.

  In this example we are trying out 0x4A.
  To set this address, cut the AD0 default trace jumper to GND, 
  and then connect it to the SDA pad.

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

#include "SparkFun_AS6212_Qwiic.h" //Click here to get the library: http://librarymanager/All#SparkFun_AS6212
#include <Wire.h>

AS6212 sensor;

//Initialize temperature variables as floats
float tempC;
float tempF;

void setup(){

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 2 - Using a different I2C address (0x4A).");

  Wire.begin();

  // Check to see if AS6212 Qwiic is present on the bus
  // Note, here we are calling begin() with a non-default address (0x4A).
  // This could be any valid address you set your jumpers to on the board.
  if (sensor.begin(0x4A) == false)
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
  Serial.print(tempC, 6);							//Reads out 6 characters of the temperature float
  Serial.print("\tTemperature (°F): ");
  Serial.println(tempF, 6);							//Reads out 6 characters of the temperature float
  
  delay(1000);
}
