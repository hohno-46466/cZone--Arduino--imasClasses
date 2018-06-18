
// imasMORSE_20160706A.ino

// Last update: Thu Oct 20 06:33:11 JST 2016

// ---------------------------------------------------------

// SIMPLE_CLASS_TEST is defined in config.h

// ---------------------------------------------------------

#include <Arduino.h>
#include "config.h"
#ifndef SIMPLE_CLASS_TEST	// defined or undefined in config.h
#include "imasBASE.h"		// if not defined SIMPLE_CLASS_TEST, include this file.
#include "misc.h"
#endif // SIMPLE_CLASS_TEST
#include "imasMORSE.h"

imasMORSE im;

// ---------------------------------------------------------

void setup() {

  Serial.begin(57600);
  
  im.setSpeed(1.50);
  im.setBeepPin(13);
  im.setConfig(CW_TEXT|CW_DIO);
  im.setPreamble15("Test:");
  im.init();
}

void loop() {

  im.message("Hello, World.");

  delay(3000);
}

// ---------------------------------------------------------
