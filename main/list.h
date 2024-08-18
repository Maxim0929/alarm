#pragma once
#ifndef _LIST_
#define _LIST_

#include<Arduino.h>
#include"display.h"
#include"time.h"

struct Display;
struct Time;
struct Alarm;

struct List{
public:
  enum NodeType{LIST, FUNCTION, VALUE, NONE};

  List();
  List(const char* name);
  List(const char* name, List *prev);
  List(const char* name, uint8_t value, NodeType nodeType);

  ~List();
  void initList(const char* name);
  void initList(const char* name, List *prev);
  void initList(const char* name, uint8_t value, NodeType nodeType);
  void List::initList(const char* name, uint8_t value, int maxValue);

  void update(uint8_t encState, const Display& dsp);

  uint8_t getCurrent();
  uint8_t getFirst();

  void setCurrent(uint8_t current);
  void setFirst(uint8_t first);

  int getValue(bool isReal);
  void setValue(int value);

  char* getName();
  void setName(char* name);

  NodeType getNodetype();

  uint8_t getID();

  int getMaxValue();

  List& operator=(const List& other);
  List* next;
  List* prev;
private:
  NodeType nodeType;
  static uint8_t first;
  static uint8_t current;
  static uint8_t IDcounter;
  int value;
  char* name;
  uint8_t ID;
  int maxValue;
};
#endif

