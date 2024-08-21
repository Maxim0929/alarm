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
  void init(List& effects);

  void fill(uint8_t red, uint8_t green, uint8_t blue, int ledNum);
  void fill(uint8_t red, uint8_t green, uint8_t blue);
  void fill(float hsv, int ledNum);
  void fill(float hsv);

  void setBrightness(const uint8_t br);
  void clear();
  void updateStrip(List& effects, bool valueChanged);
  void HSVtoRGB(int hsv, uint8_t& R, uint8_t& G, uint8_t& B);

  bool setStripState(char* state);
  StripState getStripState();

private:
  CRGB* leds;
  StripState stripState;


  struct Flow{
  public:
    Flow();
    void init(List& effects);
    void update(CRGB* leds);
    void update(CRGB* leds, List& effects);
    unsigned long startTime;
    int colorHSV;
    uint8_t r, g, b;

  private:
    unsigned int speed;
    int start, amount;
  };

  Flow flow;
};
#endif