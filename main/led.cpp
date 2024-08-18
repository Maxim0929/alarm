#include "led.h"
#include "list.h"

Led::Led(){
  leds = new CRGB[LED_NUM];
  stripState = OFF;
}

Led::~Led(){delete[] leds;}

void Led::init(){
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  FastLED.setBrightness(128);
  FastLED.show();
}

void Led::fill(uint8_t red, uint8_t green, uint8_t blue, int ledNum){
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  fill_solid(leds, ledNum, CRGB(red, green, blue));
  FastLED.show();
}
void Led::fill(uint8_t red, uint8_t green, uint8_t blue){fill(red, green, blue, LED_NUM); }

void Led::fill(float hsv, int ledNum){
  float X = 1.0 - abs(fmod(hsv / 60.0, 2.0) - 1.0);
  float R,G,B;
  if(hsv < 60){ R = 1; G = X; B = 0; }
  else if(hsv < 120){ R = X; G = 1; B = 0; }
  else if(hsv < 180){ R = 0; G = 1; B = X; }
  else if(hsv < 240){ R = 0; G = X; B = 1; }
  else if(hsv < 300){ R = X; G = 0; B = 1; }
  else if(hsv < 360){ R = 1; G = 0; B = X; }
  R = round(R * 255);
  G = round(G * 255);
  B = round(B * 255);
  fill(R, G, B, ledNum);
}
void Led::fill(float hsv){ fill(hsv, 600); }

void Led::setBrightness(const uint8_t br){
  FastLED.setBrightness(br);
  FastLED.show();
}

void Led::clear(){
  fill(0,0,0);
}

void Led::updateStrip(List& effects){

  if(stripState == COLOUR){
    fill(effects.next[1].next[0].getValue(0), effects.next[1].next[1].getValue(0));
  }else if(stripState == FLOW){

  }else if(stripState == OFF){
    fill(0,0,0);
  }

}

bool Led::setStripState(char* state){
  if(state == "COLOUR") stripState = COLOUR;
  else if(state == "COLOUR CHANGE") stripState = COLOUR_CHANGE;
  else if(state == "RANDOM") stripState = RANDOM;
  else if(state == "DRIFT") stripState = DRIFT;
  else if(state == "SNAKE") stripState = SNAKE;
  else if(state == "OFF") stripState = OFF;
  else return 1;

  return 0;
}

StripState Led::getStripState() {return stripState;}







