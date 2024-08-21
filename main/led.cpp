#include "led.h"
#include "list.h"

Led::Led(){
  leds = new CRGB[LED_NUM];
  stripState = OFF;
}

Led::~Led(){delete[] leds;}

void Led::init(List& effects){
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  FastLED.setBrightness(128);
  FastLED.show();
  flow.init(effects);
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

void Led::updateStrip(List& effects, bool valueChanged){
  if(stripState == COLOUR && valueChanged){
    fill(effects.next[1].next[0].getValue(0), effects.next[1].next[1].getValue(0));
  }else if(stripState == FLOW){
    if(valueChanged){
      flow.update(leds, effects);
      HSVtoRGB(flow.colorHSV, flow.r, flow.g, flow.b);
    }
    else{
      flow.update(leds);
    }

  }else if(stripState == OFF && valueChanged){
    fill(0,0,0);
  }

}

void Led::HSVtoRGB(int hsv, uint8_t& R, uint8_t& G, uint8_t& B){
  float X = 1.0 - abs(fmod(hsv / 60.0, 2.0) - 1.0);
  if(hsv < 60){ R = 1; G = X; B = 0; }
  else if(hsv < 120){ R = X; G = 1; B = 0; }
  else if(hsv < 180){ R = 0; G = 1; B = X; }
  else if(hsv < 240){ R = 0; G = X; B = 1; }
  else if(hsv < 300){ R = X; G = 0; B = 1; }
  else if(hsv < 360){ R = 1; G = 0; B = X; }
  R = round(R * 255);
  G = round(G * 255);
  B = round(B * 255);
}

bool Led::setStripState(char* state){
  if(state == "COLOUR") stripState = COLOUR;
  else if(state == "FLOW") stripState = FLOW;
  else if(state == "COLOUR CHANGE") stripState = COLOUR_CHANGE;
  else if(state == "RANDOM") stripState = RANDOM;
  else if(state == "DRIFT") stripState = DRIFT;
  else if(state == "SNAKE") stripState = SNAKE;
  else if(state == "OFF") stripState = OFF;
  else return 1;

  return 0;
}
StripState Led::getStripState() {return stripState;}


//FLOW
Led::Flow::Flow(){//not tested
  startTime = 0;
  speed = 1;
  amount = 1;
  colorHSV = 0;
  start = 0;
  r = g = b = 0;
}

void Led::Flow::init(List &effects){//not tested
  speed = effects.next[2].next[2].getValue(0);
  amount = effects.next[2].next[1].getValue(0);
  colorHSV = effects.next[2].next[0].getValue(0);
  start = 0;
}

void Led::Flow::update(CRGB* leds){
  if(millis() - startTime > 1000 / speed){
    int change = round((millis() - startTime) * speed / 1000.0);
    startTime = millis();
    start += change;
    if(start >= LED_NUM) start -= 600;

    for(int i = 0; i < change; i++){
      if(start - i - 1 < 0)
        leds[start - i - 1 + LED_NUM] = CRGB::Black;
      else
        leds[start - i - 1] = CRGB::Black;
    }

    for(int i = 0; i < amount; i++){
      if(start + i >= LED_NUM)
        leds[start + i - LED_NUM] = CRGB(r, g, b); 
      else
        leds[start + i] = CRGB(r, g, b); 
    }
    FastLED.show();
  }
}

void Led::Flow::update(CRGB* leds, List& effects){
  amount = effects.next[2].next[1].getValue(0);
  colorHSV = effects.next[2].next[0].getValue(0);

  if(effects.next[2].next[2].getValue(0) != 0)
    speed = effects.next[2].next[2].getValue(0);

  startTime = millis();
  return update(leds);
}








