#pragma once
#ifndef _LIST_
#define _LIST_

#include<Arduino.h>
#include"display.h"
#include"time.h"

struct Display;
struct Time;
struct Alarm;

// value
struct Value{
  public:

  Value();
  Value(String name, uint8_t value);

  int getValue(bool oneByte);
  void setValue(int value, bool oneByte);

  String getName();
  void setName(String name);

  void setMaxValue(int maxValue);
  int getMaxValue();

  uint8_t getID();

  private:
  int value;
  int maxValue;
  String name;
  uint8_t ID;
  static uint8_t  IDcounter;
};


// list

struct List{
  public:
  List();
  ~List();

  List& operator=(List& other);
  // init functions
  void initName(String name, String* names, uint8_t namesNumber);
  void initList(uint8_t listsNumber);
  void initVal(uint8_t valuesNumber, Eprom& epr, int maxValue[]);

  void initAll(String name, String* names, uint8_t namesNumber, uint8_t listsNumber, uint8_t valuesNumber, Eprom& epr, int maxValue[]);
  // other
  void update(uint8_t encState, const Display& dsp);
  void goBack();

  //geters and seters
  void setPrev(List* prev);
  List* getPrev();

  List* getList(String name);
  Value* getValue(String name);
  
  List* getLists();
  Value* getValues();
  String* getNames();
  uint8_t getNamesNumber();
  String getName();

  uint8_t getCurrent();
  void setCurrent(uint8_t current);

  uint8_t getFirst();
  void setFirst(uint8_t first);

  uint8_t getListsNumber();
  uint8_t getValuesNumber();



  private:
  List* lists;
  Value* values;
  String* names;
  
  List* prev;

  uint8_t namesNumber, listsNumber, valuesNumber;
  String name;

  static uint8_t first, current;
};

#endif

