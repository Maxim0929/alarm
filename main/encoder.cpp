#include "encoder.h"

MyEncoder::MyEncoder(): enc_(CLK, DT, SW){
  enc_.setType(TYPE2);
  enc_.setTickMode(AUTO);
  state_ = State::NONE;
}
bool MyEncoder::update(){
  if(enc_.isClick()) state_ = State::CLICK;
  else if(enc_.isHolded()) state_ = State::HOLD;
  else if(enc_.isLeftH()) state_ = State::LEFTH;
  else if(enc_.isRightH()) state_ = State::RIGHTH;
  else if(enc_.isLeft()) state_ = State::LEFT;
  else if(enc_.isRight()) state_ = State::RIGHT;
  else{
    state_ = State::NONE;
    return 0;
  }
  return 1;
}

