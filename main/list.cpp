#include "list.h"


// Setting struct
uint8_t Setting::idCounter_ = 0;

Setting::Setting(): name_(""), currentValue_(0), maxValue_(255){
  id_ = idCounter_;
  idCounter_++;
}

Setting::Setting(String name, uint8_t value): Setting() { name_ = name; currentValue_ = value; }

List::List(): settings_(nullptr), lists_(nullptr), names_(nullptr), prev_(nullptr), settingsNumber_(0), listsNumber_(0), namesNumber_(0), name_(""), first_(0), current_(0){
}

List::~List(){
  Serial.print(name_);
  Serial.println(": List destructor was called");
  delete[] names_;
  delete[] settings_;
  delete[] lists_;
}

List& List::operator=(List& other){
  this->lists_ = other.lists_;
  this->settings_ = other.settings_;
  this->names_ = other.names_;

  this->listsNumber_ = listsNumber_;
  this->settingsNumber_ = settingsNumber_;
  this->namesNumber_ = namesNumber_;

  this->name_ = other.name_;

  other.lists_ = nullptr;
  other.settings_ = nullptr;
  other.names_ = nullptr;
  Serial.println("be careful operator = was called!!!");
  return *this;

}



void List::initName(const String &name, const String* names, const uint8_t namesNumber){
  name_ = name;
  names_ = names;
  namesNumber_ = namesNumber;
}

void List::initList(uint8_t listsNumber){
  if(listsNumber + settingsNumber_ > namesNumber_){
    Serial.println("In function List::initList: valuesNumber + listsNumber can not be bigger than namesNumber");
    return;
  }

  listsNumber_ = listsNumber;
  lists_ = new List[listsNumber_];
}


void List::initVal(uint8_t settingsNumber, const int maxValue[], Eprom& epr){

  if(listsNumber_ + settingsNumber > namesNumber_){
    Serial.println("In function List::initVal: valuesNumber_ + listsNumber_ can not be bigger than namesNumber_");
    return;
  }

  settingsNumber_ = settingsNumber;

  settings_ = new Setting[settingsNumber_];

  for(int i = 0; i < settingsNumber_; i++){
    settings_[i].setName(names_[i + listsNumber_]);
    settings_[i].setMaxValue(maxValue[i]);

    int val = epr.read(names_[i + listsNumber_] + String(settings_[i].id()));
    settings_[i].setValue(val);
  }

}


void List::initAll(const String name, const String* names, const uint8_t namesNumber, const uint8_t listsNumber, const uint8_t settingsNumber, const int maxValue[], Eprom& epr){
  initName(name, names, namesNumber);
  
  if(listsNumber > 0) initList(listsNumber);
  if(settingsNumber > 0) initVal(settingsNumber, maxValue, epr);
  
}


void List::update(State encState, const Display& dsp){
  uint8_t last = namesNumber_ - 1;

  if(encState == State::LEFT) current_--;
  else if(encState == State::RIGHT) current_++;
  else return;

  if(current_ == 255) current_ = last; // uint8_t current. -1 = 255
  else if(current_ > last) current_ = 0;

  bool windowMoved = false;

  if(current_ < first_){
    first_ = current_;
    windowMoved = true;
  }else if(current_ >= first_ + DISPLAY_LINES){
    first_ = current_ - DISPLAY_LINES + 1;
    windowMoved = true;
  }

  dsp.printList(*this, windowMoved);
}




List* List::list(String name){
  for(int i = 0; i < listsNumber_; i++){
    if(names_[i] == name) 
      return &lists_[i];
  }
  return nullptr;
}

Setting* List::setting(String name){
  for(int i = 0; i < settingsNumber_; i++){
    if(names_[i + listsNumber_] == name) 
      return &settings_[i];
  }

  return nullptr;
}















