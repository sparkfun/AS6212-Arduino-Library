/******************************************************************************
  Example_03_Sleep.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~ 

  This example reads temperature from the sensor once.
  Then puts the sensor to sleep for 5 seconds.
  Then wakes it up and takes another reading.
  Then loops this cycle forever.
  The readings are printed to the terminal at 115200 baud.

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------

  Note, this example uses default settings on the sensor, 
  So when it is woken, it's in continous reading mode.

  For lowest power settings, see the Single Shot Mode example.

  Also note, after we wake up the sensor from sleeping,
  We must put in a delay to allow for the sensor to wake up
  and finish it's first conversion. 150ms is enough.
  If we read immediately after waking up, then the temp will not
  be recent, and you will actually be seeing the last valid conversion 
  (from 5 seconds ago).

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

  Serial.println("SparkFun AS6212 Qwiic Example 2 - Sleep");

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
  sensor.sleepModeOff(); // turn sleep off, (aka wake up!)

  delay(150); // allow sensor to wake up and finish it's first conversion

  tempC = sensor.readTempC();
  tempF = sensor.readTempF();

  Serial.println();
  Serial.print("Temperature (°C): ");
  Serial.print(tempC, 6);							//Reads out 6 characters of the temperature float
  Serial.print("\tTemperature (°F): ");
  Serial.println(tempF, 6);							//Reads out 6 characters of the temperature float
  
  sensor.sleepModeOn(); // go to sleep, this can take ~150ms

  delay(5000);
}
