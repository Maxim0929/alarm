#pragma once
#ifndef _LED_
#define _LED_

#define LED_PIN 49
#define LED_NUM 600

#include <FastLED.h>
#include <Arduino.h>
struct List;
enum StripState{COLOUR, FLOW, COLOUR_CHANGE, RANDOM, DRIFT, SNAKE, OFF};

struct Led{
public:
  Led();
  ~Led();
  void init();
  void fill(uint8_t red, uint8_t green, uint8_t blue, int ledNum);
  void fill(uint8_t red, uint8_t green, uint8_t blue);
  void fill(float hsv, int ledNum);
  void fill(float hsv);
  void setBrightness(const uint8_t br);
  void clear();
  void updateStrip(List& effects);
  bool setStripState(char* state);
  StripState getStripState();
private:
  CRGB* leds;
  StripState stripState;
  
};
#endif