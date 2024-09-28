#pragma once
#ifndef _DISPLAY_
#define _DISPLAY_

#define BR_PIN 8


#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

struct List;
struct Eprom;


struct Display{

public:
  Display();
  void setup(uint8_t br);
  void printTime(uint8_t hour, uint8_t min);
  void printList(const List& list, bool doClear);
  void printList(const List& list);
  void setBrightness(uint8_t br);
  void printVal(int value, uint8_t pos);
  void clear();


private:
  LiquidCrystal_I2C lcd;
  uint8_t br;
  bool brFlag;
  uint8_t custChar[8][8] = {
    {31, 31, 31, 31, 31, 0, 0, 0},//five up
    {0, 0, 0, 31, 31, 31, 31, 31},// five down
    {0, 0, 0, 0, 0, 0, 31, 31},// two down           2
    {31, 31, 0, 0, 0, 0, 0, 0},//two up
    {0, 0, 0, 0, 0, 7, 7, 7},//left up
    {7, 7, 7, 0, 0, 0, 0, 0},//left down
    {0, 0, 0, 0, 0, 28, 28, 28},//rigth up
    {28, 28, 28, 0, 0, 0, 0, 0}//right down
  };
  uint8_t bigNums[11][16] = {
    {255, 0, 0, 255, 255 ,254, 254, 255, 255, 254, 254, 255, 255, 1, 1, 255},
    {254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255},
    {0, 0, 0, 255, 2, 2, 2, 255, 255, 3, 3,3, 255, 1, 1, 1},
    {0, 0, 0, 255, 2, 2, 2, 255, 3, 3, 3, 255, 1, 1, 1, 255},
    {255, 254, 254, 255, 255, 2, 2, 255, 3, 3, 3, 255, 254, 254, 254, 255},
    {255, 0, 0, 0, 255, 2, 2, 2, 3, 3, 3, 255, 1, 1, 1, 255},
    {255, 0, 0, 0, 255, 2, 2, 2, 255, 3, 3, 255, 255, 1, 1, 255},
    {0, 0, 0, 255, 254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255},
    {255, 0, 0, 255, 255, 2, 2, 255, 255, 3, 3, 255, 255, 1, 1, 255},
    {255, 0, 0, 255, 255, 2, 2, 255, 3, 3, 3, 255, 1, 1, 1, 255},
    {4, 6, 5, 7, 4, 6, 5, 7}
  };
  void printDsp(uint8_t number, uint8_t place);
};
#endif






































