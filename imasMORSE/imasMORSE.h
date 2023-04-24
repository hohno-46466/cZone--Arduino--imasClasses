#ifndef __IMAS_MORSE_H__
#define __IMAS_MORSE_H__

// imasMORSE.h

// MORSE class for im@sClock

// First version: Fri Jun 24 23:31:23 JST 2016
// Prev update: Sat Oct 22 16:52:19 JST 2016
// New update: Wed Apr 19 07:30:16 JST 2023

// ---------------------------------------------------------

#include <Arduino.h>
#include "config.h"	// if you don't include imasBASE.h, you need this.
#ifndef SIMPLE_CLASS_TEST	// defined or undefined in config.h
#include "imasBASE.h"	// Base Class
#include "misc.h"
#endif // SIMPLE_CLASS_TEST

#define CW_TEXT	(0x01)
#define CW_DIO	(0x02)
#define CW_BEEP	(0x04)		// not supported now
#define CW_TONE	(0x08)		// not supported now

#define CW_VERBOSE (0x80)

// ---------------------------------------------------------

#ifdef SIMPLE_CLASS_TEST
class imasMORSEclass {
#else
class imasMORSEclass : public virtual imasBASEclass {
  // virtual を付けておかないと「多重継承にともなうあいまいさの問題」が生じる
  // http://exlight.net/devel/cpp/multiple_inheritance.html
#endif // SIMPLE_CLASS_TEST
 public:
  //
  imasMORSEclass();
  imasMORSEclass(float speed, uint8_t config);
  imasMORSEclass(float speed, uint8_t config, uint8_t beepPin);
  //
  void init();
  void init(float speed);
  void init(float speed, uint8_t config);
  void init(float speed, uint8_t config, uint8_t beepPin);
  //
  void setSpeed(float speed);
  float getSpeed();
  //
  void setConfig(uint8_t config);
  uint8_t getConfig();
  //
  void setDIOpin(uint8_t dioPin);
  uint8_t getDIOpin();
  //
  void setBeepPin(uint8_t beepPin);
  uint8_t getBeepPin();
  //
  void setTonePin(uint8_t tonePin);
  uint8_t getTonePin();
  // 
  void setPreamble15(char *str);
  char* getPreamble15();
  //
  void message(char *str);
  //
 private:
  uint8_t _config;
  uint8_t _dioPin;
  uint8_t _beepPin;
  uint8_t _tonePin;
  float   _speed;
  char	_preamble15[16];
};

// ---------------------------------------------------------

#endif // __IMAS_MORSE_H__
