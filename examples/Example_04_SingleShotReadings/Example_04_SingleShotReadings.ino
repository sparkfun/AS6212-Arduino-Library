/******************************************************************************
  Example_04_SingleShotReadings.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~ 

  This example puts the sensor into sleep mode.
  Then it takes a single shot reading every 5 seconds.
  The readings are printed to the terminal at 115200 baud.

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------

  Note, using single shot readings, this is the lowest power
  option for the sensor, because most of the time it is asleep,
  and then it only wakes up to take one reading on demand from
  the user.

  Also note, once you trigger a single shot reading, you must
  wait for the conversion to complete. You can do this by simply
  delaying about 150ms, or you can poll the singleShotStatus.
  SingleShotStatus is defined as so:
  0 = conversion finished
  1 = conversion ongoing

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

#include "SparkFun_AS6212_Qwiic.h"  //Click here to get the library: http://librarymanager/All#SparkFun_AS6212
#include <Wire.h>

AS6212 sensor;

//Initialize temperature variables as floats
float tempC;
float tempF;

void setup(){

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 4 - Single Shot Readings");

  Wire.begin();

  // Check to see if AS6212 Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x48, I2C-port:Wire)
  if (sensor.begin() == false)
  {
    Serial.println("AS6212 Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);      
  }; 

  sensor.sleepModeOn(); // go to sleep, this can take ~150ms
  delay(150);
}

void loop(){
  sensor.triggerSingleShotConversion(); // Note, you must be in sleep mode for this to work.

  // optional delay to allow sensor to finish it's single shot conversion
  //delay(150); 

  // optional polling of SS status
  while(sensor.getSingleShotStatus() == true) // 1 = conversion ongoing
  {
    Serial.print("."); // print some "dots" to know it's pollling correctly.
    delay(10); // avoid overtaxing I2C bus
  }

  tempC = sensor.readTempC();
  tempF = sensor.readTempF();

  Serial.println();
  Serial.print("Temperature (°C): ");
  Serial.print(tempC, 6);							//Reads out 6 characters of the temperature float
  Serial.print("\tTemperature (°F): ");
  Serial.println(tempF, 6);							//Reads out 6 characters of the temperature float

  delay(5000);
}
