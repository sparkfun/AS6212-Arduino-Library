/******************************************************************************
  Example_06_ConversionCycletime.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~

  This Example demonstrates how to set the Conversion Cycle Time.

  We will utilize this funtion: setConversionCycleTime(uint8_t cycleTime)
  valid options are:

  AS6212_CONVERSION_CYCLE_TIME_125MS
  AS6212_CONVERSION_CYCLE_TIME_250MS
  AS6212_CONVERSION_CYCLE_TIME_1000MS
  AS6212_CONVERSION_CYCLE_TIME_4000MS

  This setting is most useful when utilizing the alert feature of the sensor.

  For this example, we will set the Conversion Cycle Time to as slow as possible,
  4000MS (1 time every 4 seconds), To get a slower reaction time on teh alert status.

  Note, DEFAULT is 250MS (4 times a sec)

  Then we will read the alertStatus every second. This will only update every 4 seconds,
  So in this way, it will require the temperature to rise above the high threshhold and
  stay up there through a conversion.

  Note, because we are reading the temperature at a rate of once per second, this
  is faster than our conversion rate, and so the readings will only show a new
  reading every 4 readings.

  The alert settings are the same as in Example_05_Alert.ino. See that
  for more info on setting up the alert feature.

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

bool alertStatus;

void setup() {

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 6 - Conversion Cycle Time");

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
