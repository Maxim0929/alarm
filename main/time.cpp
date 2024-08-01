#include "time.h"
#include "led.h"


Time::Time(): clockTimer(60000){
}

void Time::init(){
    if(!rtc.begin()){
        Serial.println("No RTC found!");
    }
    else{
      if(rtc.lostPower())
        rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
      now = rtc.now();
      hour = now.hour();
      min = now.minute();
    }
    setHour = 1;
}
void Time::update(){
   if(clockTimer.isReady()){
    min++;
    if(min % 10 == 0){
      now = rtc.now();
      hour = now.hour();
      min = now.minute();
    }
    if(min > 59){
      hour++;
      if(hour > 23) hour = 0;
    }
  }
}

bool Time::set(const uint8_t encState){
  if(encState == 2){
    if(setHour){
      if(hour > 0) hour--;
      else hour = 23;
    }
    else{
      if(min > 0) min--;
      else min = 59;
    }
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
  }
  if(encState == 5){
    if(setHour){
      if(hour > 9) hour-=10;
    }
    else{
      if(min > 9) min-=10;
    }
  }
  if(encState == 6){
    if(setHour){
      if(hour < 14) hour+=10;
    }
    else{
      if(min < 50) min+=10;
    }
  }
  if(encState == 1){
    if(setHour) setHour = 0;
    else{
      setHour = 1;
      rtc.adjust(DateTime(2000, 1, 1, hour, min, 0));
      return 1;
    }
  }
  return 0;
}

uint8_t Time::getHour(){return hour;}
uint8_t Time::getMin(){return min;}



Alarm::Alarm(): clockTimer(((unsigned long)DAWN_TIME * (unsigned long)60* (unsigned long)1000) / (unsigned long)BR_MAX){
  isAlarm = 0;
  alarmBr = 0;
  setHour = 1;
}
void Alarm::init(const Eprom &eprom){
  hour = eprom.read("hourA");
  min = eprom.read("minA");
}

void Alarm::update(const Time& time, const Led& led, const uint8_t encState){
  if(isAlarm){
    if(encState == 1){
      led.fill(0,0,0);
      isAlarm = 0;
    }
    if(clockTimer.isReady() && alarmBr <= BR_MAX){
      alarmBr++;
      led.setBrightness(alarmBr);
    }
  }else if(time.getHour() == hour && time.getMin() == min){
    led.setBrightness(0);
    led.fill(255,255,255);
    isAlarm = 1;
  }
}

bool Alarm::set(const uint8_t encState, const Eprom& eprom){
  if(encState == 2){
    if(setHour){
      if(hour > 0) hour--;
      else hour = 23;
    }
    else{
      if(min > 0) min--;
      else min = 59;
    }
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
  }
  if(encState == 5){
    if(setHour){
      if(hour > 9) hour-=10;
    }
    else{
      if(min > 9) min-=10;
    }
  }
  if(encState == 6){
    if(setHour){
      if(hour < 14) hour+=10;
    }
    else{
      if(min < 50) min+=10;
    }
  }
  if(encState == 1){
    if(setHour) setHour = 0;
    else{
      setHour = 1;
      eprom.write("minA", min);
      eprom.write("hourA", hour);
      return 1;
    }
  }
  return 0;
}

uint8_t Alarm::getHour(){return hour;}
uint8_t Alarm::getMin(){return min;}
