#include "led.h"
#include "list.h"

Led::Led(){
  leds = new CRGB[LED_NUM];
  stripState = OFF;
}

Led::~Led(){delete[] leds;}

void Led::init(uint8_t ledBr){
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_NUM);
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  FastLED.setBrightness(ledBr);
  FastLED.show();
  // flow.init(menu->next.getPtr("EFFECTS"));
  // colorChange.init(menu->next.getPtr("EFFECTS"));
}

void Led::fill(uint8_t red, uint8_t green, uint8_t blue, int ledNum){
  fill_solid(leds, LED_NUM, CRGB(0, 0, 0));
  fill_solid(leds, ledNum, CRGB(red, green, blue));
  FastLED.show();
}
void Led::fill(uint8_t red, uint8_t green, uint8_t blue){fill(red, green, blue, LED_NUM); }

void Led::fill(int hsv, int ledNum){
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
// void Led::fill(float hsv){ fill(hsv, 600); }

// void Led::setBr(uint8_t br){
//   FastLED.setBrightness(br);
//   FastLED.show();
// }

void Led::setBrightness(const uint8_t br){
  FastLED.setBrightness(br);
  FastLED.show();
}

void Led::clear(){
  fill(0,0,0);
}

void Led::updateStrip(List& effects, bool valueChanged){
  if(stripState == COLOUR && valueChanged){
    fill(effects.getList("COLOUR")->getValue("COLOUR")->getValue(0),
         effects.getList("COLOUR")->getValue("AMOUNT")->getValue(0));

  }//else if(stripState == FLOW){
  //   if(valueChanged){
  //     flow.update(leds, effects);
  //     HSVtoRGB(flow.colorHSV, flow.r, flow.g, flow.b);
  //   }
  //   else{
  //     flow.update(leds);
  //   }

  // }else if(stripState == COLOUR_CHANGE){
  //   if(valueChanged){
  //     colorChange.update(leds, effects);
  //     fill(colorChange.colorHSVcurrent, colorChange.amount);
  //   }
  //   else{
  //     colorChange.update(leds);
  //     fill(colorChange.colorHSVcurrent, colorChange.amount);
  //   }
  //   FastLED.show();
  // }else if(stripState == RANDOM){
  //   if(valueChanged){
  //     randomEffect.update(leds, effects);
  //   }
  //   else{
  //     randomEffect.update(leds);
  //   }

  // }else if(stripState == OFF && valueChanged){
  //   fill(0,0,0);
  // }

}

// void Led::HSVtoRGB(int hsv, uint8_t& r, uint8_t& g, uint8_t& b){
//   float X = 1.0 - abs(fmod(hsv / 60.0, 2.0) - 1.0);
//   double R,G,B;
//   if(hsv < 60){ R = 1; G = X; B = 0; }
//   else if(hsv < 120){ R = X; G = 1; B = 0; }
//   else if(hsv < 180){ R = 0; G = 1; B = X; }
//   else if(hsv < 240){ R = 0; G = X; B = 1; }
//   else if(hsv < 300){ R = X; G = 0; B = 1; }
//   else if(hsv < 360){ R = 1; G = 0; B = X; }
//   r = round(R * 255);
//   g = round(G * 255);
//   b = round(B * 255);
// }

bool Led::setStripState(String state){
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
// Led::Flow::Flow(){
//   startTime = 0;
//   speed = 1;
//   amount = 1;
//   colorHSV = 0;
//   start = 0;
//   r = g = b = 0;
// }

// void Led::Flow::init(List *effects){
//   speed = effects->next.getPtr("FLOW")->next.getPtr("SPEED")->getValue(0);
//   amount = effects->next.getPtr("FLOW")->next.getPtr("AMOUNT")->getValue(0);
//   colorHSV = effects->next.getPtr("FLOW")->next.getPtr("COLOUR")->getValue(0);
//   start = 0;
// }

// void Led::Flow::update(CRGB* leds){
//   if(millis() - startTime > 1000 / speed){
//     int change = round((millis() - startTime) * speed / 1000.0);
//     startTime = millis();
//     start += change;
//     if(start >= LED_NUM) start -= 600;

//     for(int i = 0; i < change; i++){
//       if(start - i - 1 < 0)
//         leds[start - i - 1 + LED_NUM] = CRGB::Black;
//       else
//         leds[start - i - 1] = CRGB::Black;
//     }

//     for(int i = 0; i < amount; i++){
//       if(start + i >= LED_NUM)
//         leds[start + i - LED_NUM] = CRGB(r, g, b); 
//       else
//         leds[start + i] = CRGB(r, g, b); 
//     }
//     FastLED.show();
//   }
// }

// void Led::Flow::update(CRGB* leds, List& effects){
//   amount = effects.next.getPtr("FLOW")->next.getPtr("AMOUNT")->getValue(0);
//   colorHSV = effects.next.getPtr("FLOW")->next.getPtr("COLOUR")->getValue(0);

//   if(effects.next.getPtr("FLOW")->next.getPtr("SPEED")->getValue(0) != 0)
//     speed = effects.next.getPtr("FLOW")->next.getPtr("SPEED")->getValue(0);

//   startTime = millis();
//   update(leds);
// }

// //COLOUR CHANGE
// Led::ColorChange::ColorChange(){
//  colorHSVfrom = 0;
//  colorHSVto = 0;
//  startTime = 0;
//  speed = 1;
//  isUp = 1;
//  amount = 300; 
// }

// void Led::ColorChange::init(List *effects){
//   colorHSVfrom = effects->next.getPtr("COLOUR CHANGE")->next.getPtr("COLOUR FROM")->getValue(0);
//   colorHSVto = effects->next.getPtr("COLOUR CHANGE")->next.getPtr("COLOUR TO")->getValue(0);
//   amount = effects->next.getPtr("COLOUR CHANGE")->next.getPtr("AMOUNT")->getValue(0);
//   speed = effects->next.getPtr("COLOUR CHANGE")->next.getPtr("SPEED")->getValue(0);
// }

// void Led::ColorChange::update(CRGB* leds){
//   if(millis() - startTime > 1000 / speed){
//     unsigned int change = round(speed * (millis() - startTime) / 1000);
//     if(isUp){
//       if(colorHSVcurrent + change >= colorHSVto)
//         isUp = !isUp;
//       else
//         colorHSVcurrent += change;
//     }
//     else{
//       if(colorHSVcurrent - change <= colorHSVfrom)
//         isUp = !isUp;
//       else
//         colorHSVcurrent -= change;
//     }
//     startTime = millis();

//   }
// }

// void Led::ColorChange::update(CRGB* leds, List& effects){
//   colorHSVfrom = effects.next.getPtr("COLOUR CHANGE")->next.getPtr("COLOUR FROM")->getValue(0);
//   colorHSVto = effects.next.getPtr("COLOUR CHANGE")->next.getPtr("COLOUR TO")->getValue(0);
//   amount = effects.next.getPtr("COLOUR CHANGE")->next.getPtr("AMOUNT")->getValue(0);
//   colorHSVcurrent = colorHSVfrom;

//   if(effects.next.getPtr("COLOUR CHANGE")->next.getPtr("SPEED")->getValue(0) != 0)
//     speed = effects.next.getPtr("COLOUR CHANGE")->next.getPtr("SPEED")->getValue(0);

//   startTime = millis();
//   update(leds);
// }

// //RANDOM
// Led::RandomEffect::RandomEffect(){
//  startTime = 0;
//  delay = 1;
//  amount = 300;
//  ledsOnPos = new unsigned int[LED_NUM];
// }

// Led::RandomEffect::~RandomEffect(){
//   delete[] ledsOnPos;
// }

// void Led::RandomEffect::init(List *effects){
//   amount = effects->next.getPtr("RANDOM")->next.getPtr("AMOUNT")->getValue(0);
//   delay = effects->next.getPtr("RANDOM")->next.getPtr("DELAY")->getValue(0);
//   randomSeed(analogRead(UNCONNECTED_PIN));
// }

// void Led::RandomEffect::update(CRGB* leds){
//   if(millis() - startTime >= delay){
//     for(int i = 0; i < ceil(amount / 10.0); i++){
//       unsigned int index = random(0, amount);
//       leds[ledsOnPos[index]] = CRGB(0,0,0);
//       ledsOnPos[index] = random(0, LED_NUM);
//       leds[ledsOnPos[index]] = CRGB(random(0, 255), random(0, 255), random(0, 255));
//     }
//     FastLED.show();
  
//     startTime = millis();

//   }
// }

// void Led::RandomEffect::update(CRGB* leds, List& effects){
//   clearPos(leds);
//   amount = effects.next.getPtr("RANDOM")->next.getPtr("AMOUNT")->getValue(0);

//   if(effects.next.getPtr("RANDOM")->next.getPtr("DELAY")->getValue(0) != 0)
//     delay = effects.next.getPtr("RANDOM")->next.getPtr("DELAY")->getValue(0);
//   generatePos();
//   showLeds(leds);
//   startTime = millis();
//   update(leds);
// }
// void Led::RandomEffect::generatePos(){
//   for(int i = 0; i < amount; i++){
//     ledsOnPos[i] = random(0, LED_NUM);
//   }
// }

// void Led::RandomEffect::clearPos(CRGB* leds){
//   for(int i = 0; i < LED_NUM; i++){
//     if(i < amount) ledsOnPos[i] = 0;
//     leds[i] = CRGB(0,0,0);
//   }
//   FastLED.show();
// }

// void Led::RandomEffect::showLeds(CRGB* leds){
//   for(int i = 0; i < amount; i++){
//     leds[ledsOnPos[i]] = CRGB(random(0, 255), random(0, 255), random(0, 255));
//   }
// }

// //DRIFT
// Led::Drift::Drift(){
//  startTime = 0;
//  delay = 1;
//  amount = 5; 
//  pixels = new Pixel[LED_NUM]; 
// }

// Led::Drift::~Drift(){
//   delete[] pixels;
// }

// void Led::Drift::init(List *effects){
//   amount = effects->next.getPtr("DRIFT")->next.getPtr("AMOUNT")->getValue(0);
//   delay = effects->next.getPtr("DRIFT")->next.getPtr("DELAY")->getValue(0);
//   randomSeed(analogRead(UNCONNECTED_PIN));
// }

// void Led::Drift::update(CRGB* leds){
//   if(millis() - startTime >= delay){
//     for(int i = 0; i < amount; i++){
//       if(pixels[i].direction){
//         pixels[i].pos--;
//         if(pixels[i].pos < 0) pixels[i].pos = 255;
//       else
//         pixels[i].pos++;
//         if(pixels[i].pos > 255) pixels[i].pos = 0;
//       }
//       if(random(0,10) == 0) pixels[i].direction =  !pixels[i].direction;// 10% to switch dierection

//     }
  
//     startTime = millis();
//   }
// }

// void Led::Drift::update(CRGB* leds, List& effects){
//   clearPos(leds);
//   amount = effects.next.getPtr("DRIFT")->next.getPtr("AMOUNT")->getValue(0);

//   if(effects.next.getPtr("COLOUR CHANGE")->next.getPtr("DELAY")->getValue(0) != 0)
//     delay = effects.next.getPtr("COLOUR CHANGE")->next.getPtr("DELAY")->getValue(0);
//   generatePos();
//   showLeds(leds);
  

//   startTime = millis();
//   update(leds);
// }

// void Led::Drift::clearPos(CRGB* leds){
//   for(int i = 0; i < LED_NUM; i++){
//     if(i < amount) pixels[i].clear();
//     leds[i] = CRGB(0,0,0);
//   }
//   FastLED.show();
// }
// void Led::Drift::generatePos(){
//   for(int i = 0; i < amount; i++){
//     pixels[i].r = random(0, 255);
//     pixels[i].g = random(0, 255);
//     pixels[i].b = random(0, 255);
//     pixels[i].pos = random(0, LED_NUM);
//     pixels[i].direction = random(0, 2);
//   }
// }
// void Led::Drift::showLeds(CRGB* leds){
//   for(int i = 0; i < amount; i++){
//     leds[pixels[i].pos] = CRGB(pixels[i].r, pixels[i].g, pixels[i].b);
//   }
//   FastLED.show();
// }












