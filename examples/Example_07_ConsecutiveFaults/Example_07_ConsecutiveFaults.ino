/******************************************************************************
  Example_07_ConsecutiveFaults.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~

  This Example demonstrates how to set up a useful alert by controlling
  the Consecutive Faults and the conversion cycle time.

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------
  
  Note, we will also set up alert threshholds.

  With all this we can do a useful example where we require the temperature to
  rise above a high threshhold for 4 readings. And the sensor will only take
  a reading every 4 seconds. This means that the temperature must rise above 80F 
  for 16 seconds to cause an active alert.

  Note, for the alert status to become INactive,
  the temp must drop below the low threshhold for 16 seconds!

  Also note, because we are reading/printing the temp every second,
  and the conversion cycle time is set to 4 seconds,
  This means that the readings will only update every 4 readings.

  By adjusting the conversion cycle time and the consecutive faults,
  You can create many different alert setups to suit your project's needs.

  Note the alertStatus and the Alert output pin on the board are always the same.

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

bool alertStatus;

void setup() {

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 7 - CConsecutive Faults");

  Wire.begin();

  // Check to see if AS6212 Qwiic is present on the bus
  // Note, here we are calling begin() with no arguments = defaults (address:0x48, I2C-port:Wire)
  if (sensor.begin() == false)
  {
    Serial.println("AS6212 Qwiic failed to respond. Please check wiring and possibly the I2C address. Freezing...");
    while (1);
  };

  // check to see if the sensor might be in sleep mode (maybe from a previous arduino example)
  if (sensor.getSleepMode() == true)
  {
    Serial.println("Sensor was asleep, waking up now");
    sensor.sleepModeOff();
    delay(150); // wait for it to wake up
  }

  sensor.setDefaultSettings(); // return to default settings 
                               // in case they were set differenctly by a previous example
                               
  sensor.setTHighF(80); // set high threshhold
  sensor.setTLowF(77); // set low threshhold

  // Note, we could also set threshholds using the Celsius funtions like this:
  // sensor.setTHighC(27);
  // sensor.setTLowC(25);

  Serial.print("\tThighF: ");
  Serial.print(sensor.getTHighF(), 2);
  Serial.print("\tTlowF: ");
  Serial.println(sensor.getTLowF(), 2);

  sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_4000MS); // 1 time every 4 seconds

  // other options are as follows:
  // sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_125MS); // 8 times a sec
  // sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_250MS); // 4 times a sec
  // sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_1000MS); // 4 times a sec

  sensor.setConsecutiveFaults(4); // valid options are 1,2,3 or 4.
}

void loop() {
  tempF = sensor.readTempF();
  Serial.print("Temperature (°F): ");
  Serial.print(tempF, 6);              //Reads out 6 characters of the temperature float

  alertStatus = sensor.getAlertStatus();

  Serial.print("\talertStatus: ");
  Serial.print(alertStatus);

  if (alertStatus == false) // be default alertStatus is active low
  {
    Serial.print("\tAlert ACTIVE LOW!");
  }

  Serial.println();
  delay(1000);
}
