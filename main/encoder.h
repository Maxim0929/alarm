#pragma once
#ifndef _ENCODER_
#define _ENCODER_

#define CLK 5
#define DT 4
#define SW 3

#include "GyverEncoder.h"
#include <Arduino.h>

enum State{
  NONE,
  CLICK,
  HOLD,
  LEFT,
  RIGHT,
  LEFTH,
  RIGHTH
};

struct MyEncoder{
public:
  MyEncoder();
  bool update();
  State state(){ return state_;}
private:
  Encoder enc_;
  State state_;
};
#endif