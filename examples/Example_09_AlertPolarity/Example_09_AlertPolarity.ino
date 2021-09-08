/******************************************************************************
  Example_09_AlertPolarity.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~

  This example utilizes the alert feature of the temp sensor,
  and shows how to set the alert polarity.

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------  

  We will setup the temp sensor to watch for a high and low threshhold.
  Then the Alert will go ACTIVE LOW when we cross the uppder threshhold.
  We can cause this effect be breathing on the sensor.

  Then the Alert status will remain active low until we lower the temp and 
  cross the lower threshold.
  If you let the sensor sit in room temp, it should lower enough to cause this
  effect.
  
  This sort of setup would be useful if you didn't want to know the exact temp 
  all the time, but wanted to monitor the alert pin to see if the temp goes too hot.

  To make this example easier to use and learn about alert statuses,
  We are going to set the temperature threshholds to values to 78F and 80F.
  This way we can breath on the sensor to cause it to cross the upper threshhold,
  Then let it settle back down to room temp to see it change the Alert Status correctly.

  Note, the alert status and the alert pin on the board will read the same thing.

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

  Serial.println("SparkFun AS6212 Qwiic Example 9 - Alert Polarity");

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

  // mostly default options here, except for alert polartiy
  sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_250MS); // 4 times a sec
  sensor.setConsecutiveFaults(1); // valid options are 1,2,3 or 4.
  sensor.setInterruptMode(AS6212_MODE_COMPARATOR);

  sensor.setAlertPolarity(AS6212_ALERT_ACTIVE_HIGH);
  //sensor.setAlertPolarity(AS6212_ALERT_ACTIVE_LOW); // other valid option (default:active low)
}

void loop() {
  tempF = sensor.readTempF();
  Serial.print("Temperature (°F): ");
  Serial.print(tempF, 6);              //Reads out 6 characters of the temperature float

  alertStatus = sensor.getAlertStatus();

  Serial.print("\talertStatus: ");
  Serial.print(alertStatus);

  if (alertStatus == HIGH) // alert is active high because we set it this way in setup()
  {
    Serial.print("\tAlert ACTIVE HIGH!");
  }

  Serial.println();
  delay(1000);
}
