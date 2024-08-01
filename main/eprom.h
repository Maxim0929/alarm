#pragma once
#ifndef _EPROM_
#define _EPROM_

#include <EEPROM.h>
#include <Arduino.h>
using namespace std;

struct Eprom{
public:
  Eprom();
  ~Eprom();
  void init(uint8_t &aHour, uint8_t &aMin, uint8_t &dBr);

  void write(const String& name, uint8_t value);
  uint8_t read(const String& name);
private:
};



#endif