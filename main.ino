
#define B1_PIN 29  // пин кнопки 1
#define B2_PIN 30  // пин кнопки 2
#define B3_PIN 31  // пин кнопки 3
#define B4_PIN 32  // пин кнопки 4
#define B5_PIN 33 // пин кнопки 5
#define B6_PIN 34  // пин кнопки 6

#define CLK 27  // encoder
#define DT 25   // encoder
#define SW 23 // encoder

#define MENULAST 4
#define EFFECTLAST 3

#define LED_PIN 49
#define LED_NUM 600


#define DAWN_TIME 5  // продолжительность рассвета (в минутах)
#define ALARM_TIMEOUT 80//KA

#define BR_MAX 140
#define LED_ALARM 250


#define DISK 0x50

#include <EEPROM.h>

#include "GyverEncoder.h"
Encoder enc(CLK, DT, SW);
#include "GyverTimer.h"
GTimer_ms clockTimer(1000), backTimer(7000);
#include <Wire.h>
#include "led.h"
#include "GyverButton.h"

#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
RTC_DS3231 rtc;
DateTime now;
byte hour, min, sec;//Time

byte hourA = 0, minA = 0, alarmBr = 0;//duration in min

bool isAlarm = 0;
GTimer_ms alarmTimer(((unsigned long)DAWN_TIME * (unsigned long)60 * (unsigned long)1000) / (unsigned long)BR_MAX);

#include "Ping_pong.h"
#include "display.h"
Display dsp;

#include <FastLED.h>
CRGB* leds;

enum State{
  time_S,
  menu_S,
  setTime_S,
  setAlarm_S,
  setDspBr_S,
  effects_S
};
State state = time_S;

// bool showTime = 1;
// bool flagMode = true;

byte menuFirst = 0, menuCurrent = 0;//, stSelect = ST_MAX + 1;
//byte stFirstE = 0, stCurrentE = 0, stSelectE = ST_MAXE + 1;


bool setHour = 1;// 1- set Hour, 0 - set Min

// //Ping_pong ping_pong;

byte encState = 0;
void setup() {
   Serial.begin(9600);
  
   enc.setType(TYPE2);
   enc.setTickMode(AUTO);

   hourA = EEPROM.read(0);
   minA = EEPROM.read(1);
   dsp.br = EEPROM.read(2);

   leds = new CRGB[LED_NUM];
   FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
   fill_solid(leds, LED_NUM, CHSV(0, 0, 0));
   FastLED.show();

  if(!rtc.begin()){
      Serial.println("No RTC found!");
  }
  else{
    if(rtc.lostPower())
      rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
    now = rtc.now();
    hour = now.hour();
    min = now.minute();
    sec = now.second();
  }

   dsp.setup(hour, min);
}

void loop(){
  encState = 0;
  if(enc.isClick()) encState = 1;
  if(enc.isLeft()) encState = 2;
  if(enc.isRight()) encState = 3;
  if(enc.isHolded()) encState = 4;
  if(enc.isLeftH()) encState = 5;
  if(enc.isRightH()) encState = 6;

  switch(state){
    case time_S:
      if(encState == 4){
        state = menu_S;
        dsp.clear();
        dsp.printMenu(0,0);
      }
      else{
        updateClock();
        dsp.printTime(hour, min);
      }
    break;
    case menu_S:
      if(encState == 1) {
        switch(menuCurrent){
          case 0:
            state = setTime_S;
            dsp.clear();
            dsp.printTime(hour, min);
          break;
          case 1:
            state = setAlarm_S;
            dsp.clear();
            dsp.printTime(hourA, minA);
          break;
          case 2:
            state = setDspBr_S;
            dsp.clear();
            dsp.printNum(dsp.br);
          break;
          case 3:
            state = effects_S;
          break;
          case 4:
            state = time_S;
            dsp.clear();
            dsp.printTime(hour, min);
          break;
        }
      }
      dsp.updateMenu(encState, menuCurrent, menuFirst);
    break;
    case setTime_S:
      setTime();
    break;
    case setAlarm_S:
      setAlarm();
    break;
    case setDspBr_S:
      if(encState == 1){
        state = menu_S;
        EEPROM.write(2, dsp.br);
        dsp.clear();
        dsp.printMenu(menuCurrent, menuFirst);
      }else{ dsp.setBr(encState);}
    break;
    case effects_S:
    break;
  }
  if(isAlarm){
    if(encState == 1){
      fill_solid(leds, LED_NUM, CHSV(0,0,0));
      FastLED.show();
      isAlarm = 0;
    }
    if(alarmTimer.isReady() && alarmBr <= BR_MAX){
      alarmBr++;
      FastLED.setBrightness(alarmBr);
      FastLED.show();
    }
  }

//   //if (mode == 7) {
//   //   if (flagMode){
//   //     ping_pong.newGame();
//   //     flagMode = false;
//   //   }
//   //   ping_pong.b1 = btn1.isSingle();
//   //   ping_pong.b2 = btn2.isSingle();
//   //   ping_pong.poolButtons();
//   //   ping_pong.gameRoutine();
//   //}
}

void updateClock(){
  if(clockTimer.isReady()){
      sec++;
      if(sec > 59){
        sec = 0;
        min++;
        //if(min < 60 && state ) dsp.printTime(hour, min);
        if(min == minA && hour == hourA){
          fill_solid(leds, LED_ALARM, CRGB::White);
          isAlarm = 1;
          alarmBr = 0;
          FastLED.setBrightness(alarmBr);
        }
      }
      if(min > 59){
        now = rtc.now();
        hour = now.hour();
        min = now.minute();
        sec = now.second();
        if(hour > 23) hour = 0;
        //if(showTime) dsp.printTime(hour, min);    
      }
    }
}
void setTime(){
  if(encState == 2){
    if(setHour){
      if(hour > 0) hour--;
      else hour = 23;
    }
    else{
      if(min > 0) min--;
      else min = 59;
    }
    dsp.printTime(hour, min);
  }
  if(encState == 3){
    if(setHour){
      if(hour < 23) hour++;
      else hour = 0;
    }
    else{
      if(min < 59) min++;
      else min = 0;
    }
    dsp.printTime(hour, min);
  }
  if(encState == 5){
    if(setHour){
      if(hour > 9) hour-=10;
    }
    else{
      if(min > 9) min-=10;
    }
    dsp.printTime(hour, min);
  }
  if(encState == 6){
    if(setHour){
      if(hour < 14) hour+=10;
    }
    else{
      if(min < 50) min+=10;
    }
    dsp.printTime(hour, min);
  }
  if(encState == 1){
    if(setHour) setHour = 0;
    else{
      state = menu_S;
      setHour = 1;
      rtc.adjust(DateTime(2000, 1, 1, hour, min, sec));
      dsp.clear();
      dsp.printMenu(menuCurrent, menuFirst);
    }
  }
}
void setAlarm(){
  if(encState == 2){
    if(setHour){
      if(hourA > 0) hourA--;
      else hourA = 23;
    }
    else{
      if(minA > 0) minA--;
      else minA = 59;
    }
    dsp.printTime(hourA, minA);
  }
  if(encState == 3){
    if(setHour){
      if(hourA < 23) hourA++;
      else hourA = 0;
    }
    else{
      if(minA < 59) minA++;
      else minA = 0;
    }
    dsp.printTime(hourA, minA);
  }
  if(encState == 5){
    if(setHour){
      if(hourA > 9) hourA-=10;
    }
    else{
      if(minA > 9) minA-=10;
    }
    dsp.printTime(hourA, minA);
  }
  if(encState == 6){
    if(setHour){
      if(hourA < 14) hourA+=10;
    }
    else{
      if(minA < 50) minA += 10;
    }
    dsp.printTime(hourA, minA);
  }
  if(encState == 1){
    if(setHour) setHour = 0;
    else{
      state = menu_S;
      setHour = 1;
      dsp.clear();
      dsp.printMenu(menuCurrent, menuFirst);
      EEPROM.write(0,hourA);
      EEPROM.write(1,minA);
    }
  }
}







