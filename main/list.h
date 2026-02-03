#pragma once
#ifndef _LIST_
#define _LIST_


#include <Arduino.h>
#include "display.h"
#include "encoder.h"
#include "eprom.h"
//#include"time.h"

struct Display;
struct Time;
struct Alarm;

// value
struct Setting{
public:

  Setting();
  Setting(String name, uint8_t value);

  int value(){return currentValue_;}
  void setValue(int value){currentValue_ = value;}

  String name(){ return name_;}
  void setName(String name){name_ = name;}

  int maxValue(){return maxValue_;}
  void setMaxValue(int maxValue){maxValue_ = maxValue;}
  

  uint8_t id(){return id_;}

private:
  int currentValue_;
  int maxValue_;
  String name_;
  uint8_t id_;
  static uint8_t  idCounter_;
};


// list

struct List{
public:
  List();
  ~List();

  List& operator=(List& other);
  // init functions
  void initName(const String &name, const String* names, const uint8_t namesNumber);
  void initList(uint8_t listsNumber);
  void initVal(uint8_t settingsNumber, const int maxValue[], Eprom& epr);

  void initAll(const String name, const String* names, const uint8_t namesNumber, const uint8_t listsNumber, const uint8_t settingsNumber, const int maxValue[], Eprom& epr);
  // other
  void update(State encState, const Display& dsp);
  void goBack();

  //geters and seters
  void setPrev(List* prev){prev_ = prev;}
  List* prev(){return prev_;}

  List* list(String name);
  Setting* setting(String name);
  
  List* lists(){return lists_;}
  Setting* settings(){return settings_;}
  String* names(){return names_;}
  
  String name(){return name_;}

  uint8_t current(){return current_;}
  void setCurrent(uint8_t current){current_ = current;}

  uint8_t first(){return first_;}
  void setFirst(uint8_t first){first_ = first;}

  uint8_t namesNumber(){return namesNumber_;}
  uint8_t listsNumber(){return listsNumber_;}
  uint8_t settingsNumber(){return settingsNumber_;}



private:
  List* lists_; // sublists
  Setting* settings_;// subsettings
  String* names_; // all sublists and subsettings names
  
  List* prev_;

  uint8_t namesNumber_;
  uint8_t listsNumber_;
  uint8_t settingsNumber_;
  String name_;

  uint8_t first_;
  uint8_t current_;
};

#endif

