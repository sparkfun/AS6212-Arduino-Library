/******************************************************************************
  Example_10_HardwareInterrupt.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~

  This example shows how to use a hardware interrupt.

  Connections:
  
  --------------------------
  Arduino   |     Sensor
  --------------------------
  
  Qwiic    -->>   Qwiic
  Pin 2    -->>   "ALRT/AD1"

  --------------------------

  The default interrupt mode is AS6212_MODE_COMPARATOR, but for this example
  we are going to try out AS6212_MODE_INTERRUPT.

  In interrupt mode, the alertStatus and Alert pin will become active quickly
  when the temp crosses the high threshold.

  It will then only go active quickly again if the temp crosses the low threshhold.

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

#include "SparkFun_AS6212_Qwiic.h" //Click here to get the library: http://librarymanager/All#SparkFun_AS6212
#include <Wire.h>

AS6212 sensor;

//Initialize temperature variables as floats
float tempF;

bool ledState = LOW;  // ledState used to set the LED

bool alertInterrupt;  // this is used to know if an hardware interrupt has happened
                      // ISR (aka interrupt service routine)

void setup() {
  attachInterrupt(digitalPinToInterrupt(2), ISR_alert, FALLING);

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 8 - Hardware Interrupt");

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

  // Mostly default settings here, except AS6212_MODE_INTERRUPT
  sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_250MS); // 4 times a sec
  sensor.setConsecutiveFaults(1); // valid options are 1,2,3 or 4.
  sensor.setAlertPolarity(AS6212_ALERT_ACTIVE_LOW);

  sensor.setInterruptMode(AS6212_MODE_INTERRUPT);
  //sensor.setInterruptMode(AS6212_MODE_COMPARATOR); // other valid mode option

  pinMode(LED_BUILTIN, OUTPUT); // used to show sketch is alive
}

void loop() {
  if (alertInterrupt == true) // alertInterrupt is only set via hardware interrupt
  {
    tempF = sensor.readTempF();
    Serial.println("Alert ACTIVE LOW!");
    Serial.print("Temperature (°F): ");
    Serial.println(tempF, 6);              //Reads out 6 characters of the temperature float
    alertInterrupt = false; // reset int variable
  }

  ledState = !ledState; // flip it and rip it brah.
  digitalWrite(LED_BUILTIN, ledState);
  delay(500); // we can use delays now, 
              // because we're using a hardware interrupt to catch the alert pin.
              // However, we do want to keep it quick, so out sketch is reactive.
}

void ISR_alert()
{
  alertInterrupt = true;
}

