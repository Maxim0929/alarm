#include "led.h"

Led::Led(){
  leds = new CRGB[LED_NUM];
}

Led::~Led(){delete[] leds;}

void Led::init(){
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  FastLED.setBrightness(128);
  FastLED.show();
}

void Led::fill(uint8_t red, uint8_t green, uint8_t blue){
  fill_solid(leds, LED_NUM, CRGB(red, green, blue));
  FastLED.show();
}

void Led::setBrightness(const uint8_t br){
  FastLED.setBrightness(br);
  FastLED.show();
}

void Led::clear(){
  fill(0,0,0);
}