#pragma once
#ifndef _MENU_
#define _MENU_

#include "list.h"
#include "display.h"
#include "encoder.h"
#include "eprom.h"

struct Menu{

public:
  Menu() : list_(nullptr), modifySetting_(false), currSetting_(nullptr) {}
  void init();
  void update(State encState);
  List* list(){ return list_; }

private:
  List root_;
  List* list_;
  Display dsp_;
  Eprom epr_;

  bool modifySetting_;
  Setting* currSetting_;


  static constexpr uint8_t LISTS = 8;

  const String name_[LISTS] = {"MENU", "EFFECTS", "COLOUR", "FLOW", "COLOUR CHANGE", "RANDOM", "DRIFT", "SNAKE"};

  const uint8_t namesNumber_[LISTS] =  {6, 8, 3, 4, 5, 3, 3, 3};
  const uint8_t listsNumber_[LISTS] =  {1, 6, 0, 0, 0, 0, 0, 0};
  const uint8_t valuesNumber_[LISTS] = {2, 0, 2, 3, 4, 2, 2, 2};

  static constexpr uint8_t NAMES_NUMBER_MAX = 8;
  static constexpr uint8_t VALUES_NUMBER_MAX = 4;

  const String names_[LISTS][NAMES_NUMBER_MAX] = {
    {"EFFECTS", "DISPLAY BRIGHT.", "LED BRIGHTNESS", "ALARM", "TIME", "BACK"},
    {"COLOUR", "FLOW", "COLOUR CHANGE", "RANDOM", "DRIFT", "SNAKE", "OFF", "BACK"},
    {"COLOUR", "AMOUNT", "BACK"},
    {"COLOUR", "AMOUNT", "SPEED", "BACK"},
    {"COLOUR FROM","COLOUR TO", "AMOUNT", "SPEED", "BACK"},
    {"DELAY", "AMOUNT", "BACK"},
    {"DELAY", "AMOUNT", "BACK"},
    {"COLOUR", "AMOUNT", "BACK"}
  };

  const int maxValues_[LISTS][VALUES_NUMBER_MAX] = {
    {255, 255},
    {0},
    {359, 600},
    {359, 600, 255},
    {359, 359, 600, 200},
    {5000, 600},
    {255, 600},
    {359, 600}
  };

  void click();
  void listClick();
  bool modSetting(State encState, Setting* sett);
};
#endif





