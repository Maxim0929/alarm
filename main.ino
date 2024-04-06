
#define B1_PIN 29  // пин кнопки 1
#define B2_PIN 30  // пин кнопки 2
#define B3_PIN 31  // пин кнопки 3
#define B4_PIN 32  // пин кнопки 4
#define B5_PIN 33 // пин кнопки 5
#define B6_PIN 34  // пин кнопки 6

#define CLK 27  // encoder
#define DT 25   // encoder
#define SW 23 // encoder

#define ST_MAX 4//KA
#define ST_MAXE 3//KA

#define LED_PIN 49
#define LED_NUM 180


#define DAWN_TIME 20  // продолжительность рассвета (в минутах)
#define ALARM_TIMEOUT 80//KA

#define DAWN_MIN 50  // начальная яркость лампы (0 - 255) (для сетевых матриц начало света примерно с 50)
#define DAWN_MAX 500


#define DISK 0x50

#include <EEPROM.h>

#include "GyverEncoder.h"
Encoder enc(CLK, DT, SW);
#include "GyverTimer.h"
GTimer_ms clockTimer(1000), blinkTimer(500), backTimer(7000);
#include <Wire.h>
#include "led.h"
#include "GyverButton.h"

#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
RTC_DS3231 rtc;
DateTime now;
byte hour, min, sec;

byte hourA = 0, minA = 0, duration = 5, currentBr = 0;

bool isAlarm = 0;
GTimer_ms alarmTimer(((unsigned long)duration * (unsigned long)60 * (unsigned long)1000) / (unsigned long)255);

//#include "Ping_pong.h"
#include "display.h"
Display dsp;

#include <FastLED.h>
CRGB* leds;



bool showTime = 1;
bool flagMode = true;

byte stFirst = 0, stCurrent = 0, stSelect = ST_MAX + 1;
byte stFirstE = 0, stCurrentE = 0, stSelectE = ST_MAXE + 1;


bool timeSl = 0;

//Ping_pong ping_pong;

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

  dsp.setup();
  Serial.print("yes");
}

void loop() {
  encState = 0;
  if(enc.isClick()) encState = 1;
  if(enc.isLeft()) encState = 2;
  if(enc.isRight()) encState = 3;
  if(enc.isHolded()) encState = 4;
  if(enc.isLeftH()) encState = 5;
  if(enc.isRightH()) encState = 6;

  // if(stSelect != 0)
  //   updateClock();
  // if(!showTime){
  //   if(stSelect == ST_MAX + 1){
  //     if(encState == 2) changeStCurrent(0);
  //     if(encState == 3) changeStCurrent(1);
    
  //     if(encState == 1){ 
  //       stSelect = stCurrent;
  //       encState = 0;
  //       dsp.clear();
  //       if(stCurrent == 0) dsp.printTime(hour, min);
  //       if(stCurrent == 1) dsp.printTime(hourA, minA);
  //       if(stCurrent == 2) dsp.printNum(dsp.br);
  //       if(stCurrent == 3) dsp.printEffects(stCurrentE, stFirstE);
  //     }
  //   }
  // }else{
  //   if(encState == 4){
  //     showTime = 0; 
  //     dsp.clear();
  //     dsp.printSettings(stCurrent, stFirst);
  //   }
  // }

  // if(stSelect != ST_MAX + 1)
  //   switch(stSelect){
  //     case 0:
  //       setTime();
  //       break;
  //     case 1:
  //       setAlarm();
  //       break;
  //     case 2:
  //       setBr();
  //       break;
  //     case 3:
  //       StrEffects();
  //       break;
  //     case ST_MAX:
  //       dsp.clear();
  //       showTime = 1;
  //       dsp.printTime(hour, min);
  //       stSelect = ST_MAX + 1;
  //       break;
  //   }


  // if(isAlarm){ 
  //   alarm();
  // }
  // //updateStrip();

  // //if (mode == 7) {
  // //   if (flagMode){
  // //     ping_pong.newGame();
  // //     flagMode = false;
  // //   }
  // //   ping_pong.b1 = btn1.isSingle();
  // //   ping_pong.b2 = btn2.isSingle();
  // //   ping_pong.poolButtons();
  // //   ping_pong.gameRoutine();
  // //}
}

// void updateClock(){
//   if(clockTimer.isReady()){
//       sec++;
//       if(sec > 59){
//         sec = 0;
//         min++;
//         if(min < 60 && showTime) dsp.printTime(hour, min);
//         if(min == minA && hour == hourA){//min == minA && hour == hourA
//           fill_solid(leds, 180, CRGB::White);
//           isAlarm = 1;
//           currentBr = 0;
//         }
//       }
//       if(min > 59){
//         now = rtc.now();
//         hour = now.hour();
//         min = now.minute();
//         sec = now.second();
//         if(hour > 23) hour = 0;
//         if(showTime) dsp.printTime(hour, min);    
//       }
//     }
// }
// void changeStCurrent(bool isRight){
//     byte tmp = stFirst;
//     if(isRight){
//       if(stCurrent != ST_MAX){
//         stCurrent++;
//         if(stCurrent > 3) stFirst = stCurrent - 3;
//       }
//       else{
//         stCurrent = 0;
//         stFirst = 0;
//       }
//     }
//     if(!isRight){
//       if(stCurrent != 0){
//         stCurrent--;
//         if(stCurrent < stFirst) stFirst--;
//       }
//       else{
//         stCurrent = ST_MAX;
//         stFirst = ST_MAX - 3;
//       }
//     }
//     if(stFirst != tmp)
//       dsp.clear();
//     dsp.printSettings(stCurrent, stFirst);
// }
// void setTime(){
//   if(encState == 2){
//     if(timeSl){
//       if(min > 0) min--;
//       else min = 59;
//     }
//     else{
//       if(hour > 0) hour--;
//       else hour = 23;
//     }
//     dsp.printTime(hour, min);
//   }
//   if(encState == 3){
//     if(timeSl){
//       if(min < 59) min++;
//       else min = 0;
//     }
//     else{
//       if(hour < 23) hour++;
//       else hour = 0;
//     }
//     dsp.printTime(hour, min);
//   }
//   if(encState == 1){
//     if(!timeSl) timeSl = 1;
//     else{
//       stSelect = ST_MAX + 1;
//       timeSl = 0;
//       rtc.adjust(DateTime(2000, 1, 1, hour, min, sec));
//       dsp.clear();
//       dsp.printSettings(stCurrent, stFirst);
//     }
//   }
// }
// void setAlarm(){
//    if(encState == 2){
//     if(timeSl){
//       if(minA > 0) minA--;
//       else minA = 59;
//     }
//     else{
//       if(hourA > 0) hourA--;
//       else hourA = 23;
//     }
//     dsp.printTime(hourA, minA);
//   }
//   if(encState == 3){
//     if(timeSl){
//       if(minA < 59) minA++;
//       else minA = 0;
//     }
//     else{
//       if(hourA < 23) hourA++;
//       else hourA = 0;
//     }
//     dsp.printTime(hourA, minA);
//   }
//   if(encState == 1){
//     if(!timeSl) timeSl = 1;
//     else{
//       stSelect = ST_MAX + 1;
//       timeSl = 0;
//       dsp.clear();
//       dsp.printSettings(stCurrent, stFirst);
//       EEPROM.write(0,hourA);
//       EEPROM.write(1,minA);
//     }
//   }
// }
// void alarm(){
//   if(alarmTimer.isReady() && currentBr < 200){
//     currentBr++;
//     FastLED.setBrightness(currentBr);
//     FastLED.show();
//   }
//   if(encState == 1){
//     isAlarm = 0; 
//     currentBr = 0;
//     fill_solid(leds, 180, CRGB::Black);
//     FastLED.setBrightness(currentBr);
//     FastLED.show();
//   }
// }
// void setBr(){
//   if(encState == 1){
//     stSelect = ST_MAX + 1;
//     dsp.clear();
//     dsp.printSettings(stCurrent, stFirst);
//     EEPROM.write(2, dsp.br);
//   }
//   if(encState == 2){
//     dsp.br--;
//     analogWrite(BR_PIN, dsp.br);
//     dsp.printNum(dsp.br);
//   }
//   if(encState == 3){
//     dsp.br++;
//     analogWrite(BR_PIN, dsp.br);
//     dsp.printNum(dsp.br);
//   }
//   if(encState == 5){
//     dsp.br-=10;
//     analogWrite(BR_PIN, dsp.br);
//     dsp.printNum(dsp.br);
//   }
//   if(encState == 6){
//     dsp.br+=10;
//     analogWrite(BR_PIN, dsp.br);
//     dsp.printNum(dsp.br);
//   }
// }

// void StrEffects(){
//   while(stSelect != ST_MAX + 1){
//     encState = 0;
//     if(enc.isClick()) encState = 1;
//     if(enc.isLeft()) encState = 2;
//     if(enc.isRight()) encState = 3;
//     if(enc.isHolded()) encState = 4;
//     if(enc.isLeftH()) encState = 5;
//     if(enc.isRightH()) encState = 6;

//     if(encState == 2) changeStCurrentE(0);
//     if(encState == 3) changeStCurrentE(1);

//     if(encState == 1){
//       stSelectE = stCurrentE;
//       encState = 0;
//     }

//     if(stSelectE ==  ST_MAXE){
//       stSelect = ST_MAX + 1;
//       stSelectE = ST_MAXE + 1;
//       dsp.clear();
//       dsp.printSettings(stCurrent,stFirst);
//     }

//   }
// }

// void changeStCurrentE(bool isRight){
//     byte tmp = stFirstE;
//     if(isRight){
//       if(stCurrentE != ST_MAXE){
//         stCurrentE++;
//         if(stCurrentE > 3) stFirstE = stCurrentE - 3;
//       }
//       else{
//         stCurrentE = 0;
//         stFirstE = 0;
//       }
//     }
//     if(!isRight){
//       if(stCurrentE != 0){
//         stCurrentE--;
//         if(stCurrentE < stFirstE) stFirstE--;
//       }
//       else{
//         stCurrentE = ST_MAXE;
//         stFirstE = ST_MAXE- 3;
//       }
//     }
//     if(stFirstE != tmp)
//       dsp.clear();
//     dsp.printEffects(stCurrentE, stFirstE);
// }

// void updateStrip(){
// }





