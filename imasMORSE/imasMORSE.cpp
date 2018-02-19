
// imasMORSE.cpp

// MORSE class for im@sClock

// First version: Fri Jun 24 23:30:21 JST 2016
// Last update: Sat Oct 22 16:52:19 JST 2016

// ---------------------------------------------------------

#include <Arduino.h>
#include "imasMORSE.h"

// ---------------------------------------------------------

#ifdef DEBUG_CODE_IMAS_MORSE
// extern char *debugLine(int, char*, int);	// declared in misc.ino
#define DEBUGLINE_TEMPLATE	"*DEBUG:imasMORSE_:........SX"
#define DEBUGLINE_CLNAMELEN	(10)		  // strlen("imasMORSE_")
#define DEBUGLINE_MESGLEN (8+DEBUGLINE_CLNAMELEN) // strlen("*DEBUG:imasMORSE_:")
#define DEBUGLINE(x)	Serial.print(debugLine((x), (char *)DEBUGLINE_TEMPLATE, DEBUGLINE_MESGLEN))
#endif // DEBUG_CODE_IMAS_MORSE

#ifdef SIMPLE_CLASS_TEST
int isin(char ch, char *str);
#endif // SIMPLE_CLASS_TEST

// =========================================================

// imasMORSE();
// imasMORSE(float speed, uint8_t config);
// imasMORSE(float speed, uint8_t config, uint8_t beepPin);

imasMORSEclass::imasMORSEclass() {
#ifndef SIMPLE_CLASS_TEST
  imasClassIndicatorSet(imasID_MORSE);
#endif // SIMPLE_CLASS_TEST
  _speed   = 100.0;
  _config  = CW_TEXT;
  _beepPin = 13;
  _preamble15[0] = '\0';
}

imasMORSEclass::imasMORSEclass(float s, uint8_t c) {
#ifndef SIMPLE_CLASS_TEST
  imasClassIndicatorSet(imasID_MORSE);
#endif // SIMPLE_CLASS_TEST
  _speed   = s;
  _config  = c;
  _beepPin = 13;
  _preamble15[0] = '\0';
}

imasMORSEclass::imasMORSEclass(float s, uint8_t c, uint8_t p) {
#ifndef SIMPLE_CLASS_TEST
  imasClassIndicatorSet(imasID_MORSE);
#endif // SIMPLE_CLASS_TEST
  _speed   = s;
  _config  = c;
  _beepPin = p;
  _preamble15[0] = '\0';
}

// ---------------------------------------------------------


// void init()
// void init(float speed)
// void init(float speed, uint8_t config)
// void init(float speed, uint8_t config, uint8_t beepPin)
// 機能：imasMORSE インスタンスを初期化する．
// 注意：

void imasMORSEclass::init() {
  //
  pinMode(_beepPin, OUTPUT);
}

void imasMORSEclass::init(float s) {
  _speed = s;
  pinMode(_beepPin, OUTPUT);
}

void imasMORSEclass::init(float s, uint8_t c) {
  _speed = s;
  _config = c;
  pinMode(_beepPin, OUTPUT);
}

void imasMORSEclass::init(float s, uint8_t c, uint8_t p) {
  _speed = s;
  _config = c;
  _beepPin = p;
  pinMode(_beepPin, OUTPUT);
}

// ---------------------------------------------------------

// void setSpeed(float speed)
// float getSpeed()
// void setConfig(uint8_t config)
// uint8_t getConfig()
// void setBeepPin(uint8_t beepPin)
// uint8_t getBeepPin()
// void setPreamble15(char *str)
// char* getPreamble15()
// 機能：imasMORSE インスタンスの諸パラメータを設定あるいは取得する．
// 注意：

void imasMORSEclass::setSpeed(float s) {
  _speed = s;
}

float imasMORSEclass::getSpeed() {
  return(_speed);
}


void imasMORSEclass::setConfig(uint8_t conf) {
  _config = conf;
}

uint8_t imasMORSEclass::getConfig() {
  return(_config);
}


void imasMORSEclass::setBeepPin(uint8_t p) {
  _beepPin = p;
}

uint8_t imasMORSEclass::getBeepPin() {
  return(_beepPin);
}

void imasMORSEclass::setPreamble15(char *str) {
  char *p;

  p = _preamble15;
  
  if (str == NULL) {
    *p = '\0';
    return;
  }

  for(int i = 0; i < 15; i++) {
    if (*str == '\0') {
      break;
    }
    *p = *str++;
  }
  *p = '\0';
}

char *imasMORSEclass::getPreamble15() {
  return(_preamble15);
}


// ---------------------------------------------------------

struct DnD {	// Dot and Dash
  uint16_t t0:2;
  uint16_t t1:2;
  uint16_t t2:2;
  uint16_t t3:2;
  uint16_t t4:2;
  uint16_t t5:2;
  uint16_t t6:2;
  uint16_t t7:2;
} dataDnD[] = {
  /* index = 0..26 */
  { 3, 0, 0, 0, 0, 0, 0, 0 }, // 00:SP
  { 1, 2, 0, 0, 0, 0, 0, 0 }, // 01:A
  { 2, 1, 1, 1, 0, 0, 0, 0 }, // 02:B
  { 2, 1, 2, 1, 0, 0, 0, 0 }, // 03:C
  { 2, 1, 1, 0, 0, 0, 0, 0 }, // 04:D
  { 1, 0, 0, 0, 0, 0, 0, 0 }, // 05:E
  { 1, 1, 2, 1, 0, 0, 0, 0 }, // 06:F
  { 2, 2, 1, 0, 0, 0, 0, 0 }, // 07:G
  { 1, 1, 1, 1, 0, 0, 0, 0 }, // 08:H
  { 1, 1, 0, 0, 0, 0, 0, 0 }, // 09:I
  { 1, 2, 2, 2, 0, 0, 0, 0 }, // 10:J
  { 2, 1, 2, 0, 0, 0, 0, 0 }, // 11:K
  { 1, 2, 1, 1, 0, 0, 0, 0 }, // 12:L
  { 2, 2, 0, 0, 0, 0, 0, 0 }, // 13:M
  { 2, 1, 0, 0, 0, 0, 0, 0 }, // 14:N
  { 2, 2, 2, 0, 0, 0, 0, 0 }, // 15:O
  { 1, 2, 2, 1, 0, 0, 0, 0 }, // 16:P
  { 2, 2, 1, 2, 0, 0, 0, 0 }, // 17:Q
  { 1, 2, 1, 0, 0, 0, 0, 0 }, // 18:R
  { 1, 1, 1, 0, 0, 0, 0, 0 }, // 19:S
  { 2, 0, 0, 0, 0, 0, 0, 0 }, // 20:T
  { 1, 1, 2, 0, 0, 0, 0, 0 }, // 21:U
  { 1, 1, 1, 2, 0, 0, 0, 0 }, // 22:V
  { 1, 2, 2, 0, 0, 0, 0, 0 }, // 23:W
  { 2, 1, 1, 2, 0, 0, 0, 0 }, // 24:X
  { 2, 1, 2, 2, 0, 0, 0, 0 }, // 25:Y
  { 2, 2, 1, 1, 0, 0, 0, 0 }, // 26:Z
  /* index = 27..36 */
  { 2, 2, 2, 2, 2, 0, 0, 0 }, // 27:0
  { 1, 2, 2, 2, 2, 0, 0, 0 }, // 28:1
  { 1, 1, 2, 2, 2, 0, 0, 0 }, // 29:2
  { 1, 1, 1, 2, 2, 0, 0, 0 }, // 30:3
  { 1, 1, 1, 1, 2, 0, 0, 0 }, // 31:4
  { 1, 1, 1, 1, 1, 0, 0, 0 }, // 32:5
  { 2, 1, 1, 1, 1, 0, 0, 0 }, // 33:6
  { 2, 2, 1, 1, 1, 0, 0, 0 }, // 34:7
  { 2, 2, 2, 1, 1, 0, 0, 0 }, // 35:8
  { 2, 2, 2, 2, 1, 0, 0, 0 }, // 36:9
  /* index = 37..45 */
  { 1, 2, 1, 2, 1, 2, 0, 0 }, // 37:,
  { 2, 2, 1, 1, 2, 2, 0, 0 }, // 38:.
  { 1, 1, 2, 2, 1, 1, 0, 0 }, // 39:?
  { 2, 1, 2, 1, 2, 2, 0, 0 }, // 40:!
  { 2, 1, 1, 1, 2, 0, 0, 0 }, // 41:-
  { 2, 1, 1, 2, 1, 0, 0, 0 }, // 42:/
  { 1, 2, 2, 1, 2, 1, 0, 0 }, // 43:@
  { 2, 1, 2, 2, 1, 0, 0, 0 }, // 44:(
  { 2, 1, 2, 2, 1, 2, 0, 0 }, // 45:)
  /* index = 46..49 */
  { 1, 1, 1, 1, 1, 1, 1, 1 }, // 46:HH
  { 2, 1, 1, 1, 2, 0, 0, 0 }, // 47:BT
  { 1, 2, 1, 2, 1, 0, 0, 0 }, // 48:AR
  { 1, 1, 1, 2, 1, 2, 0, 0 }, // 49:VA
};


struct DnD dataDnD2[] = {
  /* index = 00..04 */
  { 2, 2, 1, 2, 2, 0, 0, 0 }, // 00:あ
  { 1, 2, 0, 0, 0, 0, 0, 0 }, // 01:い
  { 1, 1, 2, 0, 0, 0, 0, 0 }, // 02:う
  { 2, 1, 2, 2, 2, 0, 0, 0 }, // 03:え
  { 1, 2, 1, 1, 1, 0, 0, 0 }, // 04:お
  /* index = .. */
  { 1, 2, 1, 1, 0, 0, 0, 0 }, // 05 :か
  { 2, 1, 2, 1, 1, 0, 0, 0 }, // 06 :き
  { 1, 1, 1, 2, 0, 0, 0, 0 }, // 07 :く
  { 2, 1, 2, 2, 0, 0, 0, 0 }, // 08 :け
  { 2, 2, 2, 2, 0, 0, 0, 0 }, // 09 :こ
  /* index = .. */
  { 2, 1, 2, 1, 2, 0, 0, 0 }, // 10:さ
  { 2, 2, 1, 2, 1, 0, 0, 0 }, // 11:し
  { 2, 2, 2, 1, 2, 0, 0, 0 }, // 12:す
  { 1, 2, 2, 2, 1, 0, 0, 0 }, // 13:せ
  { 2, 2, 2, 1, 0, 0, 0, 0 }, // 14:そ
  /* index = .. */
  { 2, 1, 0, 0, 0, 0, 0, 0 }, // 15:た
  { 1, 1, 2, 1, 0, 0, 0, 0 }, // 16:ち
  { 1, 2, 2, 1, 0, 0, 0, 0 }, // 17:つ
  { 1, 2, 1, 2, 2, 0, 0, 0 }, // 18:て  
  { 1, 1, 2, 1, 1, 0, 0, 0 }, // 19:と
  /* index = .. */
  { 1, 2, 1, 0, 0, 0, 0, 0 }, // 20:な
  { 2, 1, 2, 1, 0, 0, 0, 0 }, // 21:に
  { 1, 1, 1, 1, 0, 0, 0, 0 }, // 22:ぬ
  { 2, 2, 1, 2, 0, 0, 0, 0 }, // 23:ね
  { 1, 1, 2, 2, 0, 0, 0, 0 }, // 24:の
  /* index = .. */
  { 2, 1, 1, 1, 0, 0, 0, 0 }, // 25:は
  { 2, 2, 1, 1, 2, 0, 0, 0 }, // 26:ひ
  { 2, 2, 1, 1, 0, 0, 0, 0 }, // 27:ふ
  { 1, 0, 0, 0, 0, 0, 0, 0 }, // 38:へ
  { 2, 1, 1, 0, 0, 0, 0, 0 }, // 39:ほ
  /* index = .. */
  { 2, 1, 1, 2, 0, 0, 0, 0 }, // 30:ま
  { 1, 1, 2, 1, 2, 0, 0, 0 }, // 31:み
  { 2, 0, 0, 0, 0, 0, 0, 0 }, // 32:む
  { 2, 1, 1, 1, 2, 0, 0, 0 }, // 33:め
  { 2, 1, 1, 2, 1, 0, 0, 0 }, // 34:も
  /* index = .. */
  { 1, 2, 2, 0, 0, 0, 0, 0 }, // 35:や
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 36:
  { 2, 1, 1, 2, 2, 0, 0, 0 }, // 37:ゆ
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 38:
  { 2, 2, 0, 0, 0, 0, 0, 0 }, // 39:よ
  /* index = .. */
  { 1, 1, 1, 0, 0, 0, 0, 0 }, // 40:ら
  { 2, 2, 1, 0, 0, 0, 0, 0 }, // 41:り
  { 2, 1, 2, 2, 1, 0, 0, 0 }, // 42:る
  { 2, 2, 2, 0, 0, 0, 0, 0 }, // 43:れ
  { 1, 2, 1, 2, 0, 0, 0, 0 }, // 44:ろ
  /* index = .. */
  { 2, 1, 2, 0, 0, 0, 0, 0 }, // 45:わ
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 46:
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 47:
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 48:
  { 1, 2, 2, 2, 0, 0, 0, 0 }, // 49:を
  /* index = .. */
  { 1, 2, 1, 2, 1, 0, 0, 0 }, // 50:ん
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 51:
  { 2, 1, 1, 2, 2, 2, 0, 0 }, // 52:ホレ
  { 1, 1, 1, 2, 1, 0, 0, 0 }, // 53:ラタ
  { 0, 0, 0, 0, 0, 0, 0, 0 }, // 54:
#if 0
#endif // 0
};

#define DnD_SPEED_LL  (10)
#define DnD_SPEED_DEF (100)
#define DnD_SPEED_UL  (1000)

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
void imasMORSEclass::message(char *mesg) {

  char ch;
  int idx;

  int16_t s = DnD_SPEED_DEF / _speed;

  s = (s < DnD_SPEED_LL) ? DnD_SPEED_LL : (s > DnD_SPEED_UL) ? DnD_SPEED_UL : s;

  while(*mesg == ' ') {
    mesg++;
  }
  
  while(*mesg != 0x00) {
    ch = *mesg;

    if (ch == ' ') {
      /* idx = 0 */
      idx = 0;
    } else if ((ch >= 'a') && (ch <= 'z')) {
      /* idx = 1..26 */
      idx = 1 + (ch - 'a');
    } else if ((ch >= 'A') && (ch <= 'Z')) {
      /* idx = 1..26 */
      idx = 1 + (ch - 'A');
    } else if ((ch >= '0') && (ch <= '9')) {
      /* idx = 27..36 */
      idx = 27 + (ch - '0');
    } else {
      /* index = 37..45 */
      const char *tmpstr = ",.?!-/@()";
      int k = isin(ch, (char *)tmpstr);
      if (k < 0) {
	continue;
      }
      idx = 37 + k;
    }
    byte x[8];
    // x = (byte *)tmpstr;
    x[0] = dataDnD[idx].t0; x[1] = dataDnD[idx].t1;
    x[2] = dataDnD[idx].t2; x[3] = dataDnD[idx].t3;
    x[4] = dataDnD[idx].t4; x[5] = dataDnD[idx].t5;
    x[6] = dataDnD[idx].t6; x[7] = dataDnD[idx].t7;
    //
    
    if (_config & CW_TEXT) {
      if (_preamble15[0] != '\0') {
	Serial.print(_preamble15);
      }
    // Serial.print(F("Q:["));
      Serial.print(F("["));
      Serial.print(ch);
      Serial.print(F("] "));
    }

    for (int j = 0; j < 8; j++) {
      if (x[j] == 1) {
	// dot
	if (_config & (CW_TEXT|CW_VERBOSE)) { Serial.print('.'); }
	digitalWrite(_beepPin, HIGH);
	delay(s);
	digitalWrite(_beepPin, LOW);
	delay(s);

      } else if (x[j] == 2) {
	// dash
	if (_config & (CW_TEXT|CW_VERBOSE)) { Serial.print('-'); }
	digitalWrite(_beepPin, HIGH);
	delay(3*s);
	digitalWrite(_beepPin, LOW);
	delay(s);

      } else if (x[j] == 3) {
	// space
	if (_config & (CW_TEXT|CW_VERBOSE)) { Serial.print(' '); }
	delay(5*s);

      } else {
	delay(2*s);
	break;
      }
    }
    
    if (_config & (CW_TEXT|CW_VERBOSE)) { Serial.println(); }
    mesg++;
  }

  // return(0);
}

// =========================================================

#ifdef SIMPLE_CLASS_TEST

// int isin(char ch, char *str)
// 機能：ch が str の何文字目にあるかを返す．先頭が 0 文字目．
//       見つからなければ -1 を返す．
// 注意：misc.ino に同名同等の定義があるが，SIMPLE_CLASS_TEST の
// 　　　場合は misc.{ino,h} を使わずに以下で代替可能．

int isin(char ch, char *str) {
  char *ptr;
  int i;

  if (str == NULL) {
    return(-1);
  }

  i = 0;
  ptr = str;
  while((ptr != NULL) && (*ptr != 0x00)) {
    if (ch == *ptr) {return(i);}
    i++;
    ptr++;
  }

  return(-1);
}

#endif // SIMPLE_CLASS_TEST

// ---------------------------------------------------------
