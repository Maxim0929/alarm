#pragma once
#ifndef _TIME_
#define _TIME_

#define BR_MAX 110
#define DAWN_TIME 5  // продолжительность рассвета (в минутах)
#define ALARM_LED_NUM 340

#include <RTClib.h>
#include <Metro.h>
#include <Arduino.h>

#include"led.h"
#include"eprom.h"
struct Led;
struct Eprom;

struct Time{
public:
  Time();
  void init();
  bool update();
  bool set(const uint8_t encState);
  uint8_t getHour();
  uint8_t getMin();
  bool setHour;
private:
  Metro clockTimer;
  RTC_DS3231 rtc;
  uint8_t hour;
  uint8_t min;
  DateTime now;
};

struct Alarm{
public:
  Alarm();
  void init(const Eprom &eprom);
  bool set(const uint8_t encState, const Eprom& eprom);
  void update(const Time& time, const Led& led, const uint8_t encState);
  uint8_t getHour();
  uint8_t getMin();
  bool getisAlarm();
private:
  Metro clockTimer;
  bool isAlarm;
  uint8_t alarmBr;
  uint8_t hour;
  uint8_t min;
  bool setHour;
};
#endif