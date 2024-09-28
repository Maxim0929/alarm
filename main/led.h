#pragma once
#ifndef _LED_
#define _LED_

#define LED_PIN 49
#define LED_NUM 600
#define UNCONNECTED_PIN A10

#include <FastLED.h>
#include <Arduino.h>
struct List;
enum StripState{COLOUR, FLOW, COLOUR_CHANGE, RANDOM, DRIFT, SNAKE, OFF};

struct Led{
public:
  Led();
  ~Led();
  void init(uint8_t ledBr);

  void fill(uint8_t red, uint8_t green, uint8_t blue, int ledNum);
  void fill(uint8_t red, uint8_t green, uint8_t blue);
  void fill(int hsv, int ledNum);
  // void fill(float hsv);

  void setBrightness(const uint8_t br);
  void clear();
  void updateStrip(List& effects, bool valueChanged);
  // void HSVtoRGB(int hsv, uint8_t& R, uint8_t& G, uint8_t& B);

  bool setStripState(String state);
  StripState getStripState();

private:
  CRGB* leds;
  StripState stripState;

  // struct Flow{
  //   public:
  //     Flow();
  //     void init(List* effects);
  //     void update(CRGB* leds);
  //     void update(CRGB* leds, List& effects);
  //     unsigned long startTime;
  //     int colorHSV;
  //     uint8_t r, g, b;

  //   private:
  //     unsigned int speed;
  //     int start, amount;
  // };
  // Flow flow;

  // struct ColorChange{
  //   public:
  //     ColorChange();

  //     void init(List* effects);
  //     void update(CRGB* leds);
  //     void update(CRGB* leds, List& effects);
      
  //     unsigned long startTime;
  //     int colorHSVfrom, colorHSVto, colorHSVcurrent;
  //     unsigned int speed;
  //     int amount;
  //     bool isUp;
  // };
  // ColorChange colorChange;

  // struct RandomEffect{
  //   public:
  //     RandomEffect();
  //     ~RandomEffect();

  //     void init(List* effects);
  //     void update(CRGB* leds);
  //     void update(CRGB* leds, List& effects);
  //     void clearPos(CRGB* leds);
  //     void generatePos();
  //     void showLeds(CRGB* leds);

  //     unsigned long startTime;
  //     unsigned int delay;
  //     int amount;
  //     unsigned int* ledsOnPos;
  // };
  // RandomEffect randomEffect;

  // struct Drift{
  //   private:
  //     struct Pixel{
  //       public:
  //         Pixel(){
  //           pos = 0;
  //           r = g = b = 0;
  //           direction = 0;
  //         }
  //         void clear(){
  //           r = g = b = 0;
  //           pos = 0;
  //           direction = 0;
  //         }
  //         unsigned int pos;
  //         uint8_t r, g, b;
  //         bool direction;
  //     };
  //   public:
  //     Drift();
  //     ~Drift();

  //     void init(List* effects);
  //     void update(CRGB* leds);
  //     void update(CRGB* leds, List& effects);
  //     void clearPos(CRGB* leds);
  //     void generatePos();
  //     void showLeds(CRGB* leds);

  //     unsigned long startTime;
  //     unsigned int delay;
  //     int amount;
  //     unsigned int* ledsOnPos;
  //     Pixel* pixels;
  // };
  // Drift drift;
};
#endif