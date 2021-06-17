/************************************************************
  Brandon Williams
  AS6212 Register Addresses and Presets
  Creation Date: 07/15/20
  https://github.com/will2055/AS6212-Arduino-Library/src
  
  This file defines AS6212 registers and includes some configuration
  presets.
  
  Development environment specifics:
    IDE: Arduino 1.8.12
    Hardware Platform: Arduino Uno

  Special thanks to Madison Chodikov @ SparkFun Electronics
    for code examples from TMP117 Arduino Library
    (https://github.com/sparkfun/SparkFun_TMP117_Arduino_Library)
    
  Distributed as-is; no warranty is given.
************************************************************/

#ifndef __AS6212_Registers_H__
#define __AS6212_Registers_H__

enum AS6212_Register{

  //Internal Register Addresses
  TVAL      =       0x0,    //Temperature Register
  CONFIG    =       0x1,    //Configuration Register
  TLOW      =       0x2,    //Low Temperature Threshold
  THIGH     =       0x3,    //High Temperature Threshold
  
  //Helpful preset definitions for configuration register
  DEFAULTM  =       0x40A0,   //Default state
  SLEEPMODE =       0x41A0,   //Sleep Mode
  SLEEPSS   =       0xC1A0,   //Sleep Mode Single Shot

  /* Register values (MSB -> LSB) */
  SINGLESHOT	=	0x8000,	//15
  CFAULT_1		=	0x0800,	//12
  CFAULT_0		=	0x0400,	//11
  POLARITY		=	0x0200,	//10
  INTERRUPT		=	0x0100,	//9
  SLEEP			=	0x0080,	//8
  CONVER_RATE_1 =	0x0040,	//7
  CONVER_RATE_0 =	0x0020,	//6
  ALERT			=	0x0010,	//5
};

#endif
