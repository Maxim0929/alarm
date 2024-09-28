#include "encoder.h"

MyEncoder::MyEncoder(): enc(CLK, DT, SW){
  enc.setType(TYPE2);
  enc.setTickMode(AUTO);
}
bool MyEncoder::update(){
  if(enc.isClick()) encState = 1;
  else if(enc.isLeft()) encState = 2;
  else if(enc.isRight()) encState = 3;
  else if(enc.isHolded()) encState = 4;
  else if(enc.isLeftH()) encState = 5;
  else if(enc.isRightH()) encState = 6;
  else{
    encState = 0;
    return 0;
  }
  return 1;
}

uint8_t MyEncoder::getEncstate(){
  return encState;
}
