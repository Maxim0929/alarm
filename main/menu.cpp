#include "menu.h"


void Menu::init(){
  List* curr = &root_;
  List* prev = nullptr;

  for(int i = 0; i < 8; ++i){
    curr->initAll(name_[i], names_[i], namesNumber_[i], listsNumber_[i], valuesNumber_[i], maxValues_[i], epr_);
    curr->setPrev(prev);
    if(i < 2){ // menu and effects
      prev = curr;
      curr = curr->lists();
    } 
    else{ // all effects sublists
      curr++;
    }
  }
  dsp_.setup();
  dsp_.printList(root_);
}

void Menu::update(State encState){

  if(modifySetting_){
    modifySetting_ = modSetting(encState, currSetting_);
    return;
  }

  // To main menu
  if(encState == State::HOLD){
    list_ = &root_;
    dsp_.printList(*list_);
    return;
  }
  // No active list (time screen)
  if(!list_) return;

  // Click inside list
  if(encState == State::CLICK){
    click();
    return;
  } 

  // Rotation
  if(encState == State::LEFT ||encState == State::RIGHT){
    list_->update(encState, dsp_);
  }

}

void Menu::click(){
  uint8_t cursorPos = list_->current();
  uint8_t listsNumber = list_->listsNumber();
  uint8_t settingsNumber = list_->settingsNumber();

  // click on list
  if(cursorPos < listsNumber){
    listClick();
    return;
  }
    
  // click on value
  if(cursorPos < listsNumber + settingsNumber){
    modifySetting_ = true;
    currSetting_ = &list_->settings()[cursorPos - listsNumber];
  }
  else{
    //listFunctions.callFunction(current->names()[cursorPos]); // click on function
  }
    
}


void Menu::listClick(){
  list_ = &list_->lists()[list_->current()];

  list_->setCurrent(0);
  list_->setFirst(0);

  dsp_.printList(*list_);

  // if(current->name() != "EFFECTS"){
  //   led.setStripState(current->getName());
  //   led.updateStrip(*menu.getList("EFFECTS"), 1);
  // }
  // }else if(next->getNodetype() == List::NodeType::VALUE){ // VALUE
  //   currentFunc = SETVAL;
  //   dsp.clear();
  //   dsp.printNum(next->getValue(1));


  // }else if(next->getNodetype() == List::NodeType::FUNCTION){
  //   if(next->getName() == "SETTIME"){
  //     currentFunc = SETTIME;
  //     dsp.clear();
  //     dsp.printTime(time.getHour(), time.getMin());

  //   }else if(next->getName() == "SETALARM"){
  //     currentFunc = SETALARM;
  //     dsp.clear();
  //     dsp.printTime(alarm.getHour(), alarm.getMin());

  //   }else if(next->getName() == "BACK"){
  //     if(current->getName() == "MENU"){
  //       dsp.clear();
  //       dsp.printTime(time.getHour(), time.getMin());
  //       currentFunc = TIME;
  //     }
  //     else {
  //       currentFunc = NONEFUNC;
  //       current = next->prev;
  //       current->setCurrent(0);
  //       current->setFirst(0);
  //       dsp.printList(*current);
  //     }
  //   }else if(next->getName() == "OFF"){
  //     led.setStripState("OFF");
  //     led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
  //   }
  // }
}

bool Menu::modSetting(State encState, Setting* sett){
  int val = sett->value();
  int maxValue = sett->maxValue();
  int settPos = list_->current() - list_->first();

  switch(encState){
    case State::CLICK:
      epr_.write(sett->name() + String(sett->id()), sett->value());
      return 0;

    case State::LEFT:
      if(val == 0) sett->setValue(maxValue);
      else sett->setValue(val - 1);
      dsp_.printVal(sett->value(), settPos);
      break;

    case State::RIGHT:
      if(val == maxValue) sett->setValue(0);
      else sett->setValue(val + 1);
      dsp_.printVal(sett->value(), settPos);
      break;

    case State::LEFTH:
      if(val > 9) sett->setValue(val - 10);
      dsp_.printVal(sett->value(), settPos);
      break;

    case State::RIGHTH:
      if(val < maxValue - 9) sett->setValue(val + 10);
      dsp_.printVal(sett->value(), settPos);
      break;
  }
  return 1;
}







