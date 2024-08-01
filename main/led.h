#pragma once
#ifndef _LED_
#define _LED_

#define LED_PIN 49
#define LED_NUM 600

#include <FastLED.h>
#include <Arduino.h>

struct Led{
public:
  Led();
  ~Led();
  void init();
  void fill(uint8_t red, uint8_t green, uint8_t blue);
  void setBrightness(const uint8_t br);
  void clear();
private:
  CRGB* leds;
};
#endif