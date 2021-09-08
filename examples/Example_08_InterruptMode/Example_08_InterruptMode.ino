/******************************************************************************
  Example_08_InterruptMode.ino
  Example for the SparkFun Digital Temperature Sensor Breakout - AS6212 (Qwiic)
  License: This code is public domain but you buy us a beer if you use this and we meet someday (Beerware license).
  Authors: SparkFun Electronics, Pete Lewis, Brandon Williams
  Date Created: 8/17/2021
  ~

  This example shows how to change the interrupt mode. (aka "alert mode").

  Connections:
  --------------------------
  Arduino   |     Sensor
  --------------------------
  Qwiic    -->>   Qwiic
  --------------------------

  The default interrupt mode is AS6212_MODE_COMPARATOR, but for this example
  we are going to try out AS6212_MODE_INTERRUPT.

  In interrupt mode, the alertStatus and Alert pin will become active quickly
  when the temp crosses the high threshold.

  It will then only go active quickly again if the temp crosses the low threshhold.

  The time that it goes active is so short, that we need to omit any delays from our sketch,
  And we need to read the alert status as quickly as possible.

  Another (arguably better) way to use AS6212_MODE_INTERRUPT is to wire up a hardware
  interrupt from the board ALERT pin to your arduinos INT pin. 
  See example 10 for more info on this.

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

int ledState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated

void setup() {

  Serial.begin(115200);

  Serial.println("SparkFun AS6212 Qwiic Example 8 - Interrupt Mode");

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

  sensor.setConversionCycleTime(AS6212_CONVERSION_CYCLE_TIME_250MS); // 4 times a sec
  sensor.setConsecutiveFaults(1); // valid options are 1,2,3 or 4.

  sensor.setInterruptMode(AS6212_MODE_INTERRUPT);
  //sensor.setInterruptMode(AS6212_MODE_COMPARATOR); // other valid mode option


  pinMode(LED_BUILTIN, OUTPUT); // used to show sketch is alive
}

void loop() {
  if (sensor.getAlertStatus() == false) // be default alertStatus is active low
  {
    tempF = sensor.readTempF();
    Serial.println("Alert ACTIVE LOW!");
    Serial.print("Temperature (°F): ");
    Serial.println(tempF, 6);              //Reads out 6 characters of the temperature float
  }

  blink_no_delay(); // used to show sketch is alive
}



void blink_no_delay()
{
    // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN, ledState);
  }
}
