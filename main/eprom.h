#pragma once
#ifndef _EPROM_
#define _EPROM_
#define FIRST 0
#define COMPILE 0

#include <EEPROM.h>
#include <Arduino.h>
using namespace std;

struct Eprom{
public:
  Eprom();
  ~Eprom();

  void write(const String name, uint8_t value);
  uint8_t read(const String name);
  void print();
private:
};



#endif