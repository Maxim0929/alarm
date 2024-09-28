#pragma once
#ifndef _ENCODER_
#define _ENCODER_

#define CLK 27
#define DT 25
#define SW 23

#include "GyverEncoder.h"
#include <Arduino.h>

struct MyEncoder{
public:
  MyEncoder();
  bool update();
  uint8_t getEncstate();
private:
  Encoder enc;
  uint8_t encState;
};
#endif