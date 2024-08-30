#include "eprom.h"

Eprom::Eprom(){
  if(FIRST) EEPROM.write(0,0);
}

Eprom::~Eprom(){}


void Eprom::write(const String& name, uint8_t value){
  int address = 0;
  String crnName = "";
  while(EEPROM.read(address) != 0){// search if element with this name exists
    for(int i = address + 1; i < EEPROM.read(address); i++){
      crnName += char(EEPROM.read(i));
    }
    if(name == crnName){
      // Serial.print("found\t");
      // Serial.println(name);
      EEPROM.write(EEPROM.read(address), value);
      break;
    }else{
      address = EEPROM.read(address) + 1;
      crnName = "";
    }
  }
  if(EEPROM.read(address) == 0){
    // Serial.print("not found\t");
    // Serial.println(name);
    EEPROM.write(address, name.length() + 1 + address); // value address
    for(int i = address + 1, k = 0; i < EEPROM.read(address); i++, k++){
      EEPROM.write(i, name[k]);// value name
    }
    EEPROM.write(EEPROM.read(address), value); // value
    EEPROM.write(EEPROM.read(address) + 1, 0);
  }
  
}

uint8_t Eprom::read(const String& name){
  int address = 0;
  String crnName = "";
  while(EEPROM.read(address) != 0){// search if element with this name exists
    crnName = "";
    for(int i = address + 1; i < EEPROM.read(address); i++){
      crnName += char(EEPROM.read(i));
    }
    if(name == crnName){
      // Serial.print("found\t");
      // Serial.println(name);
      // Serial.println(EEPROM.read(EEPROM.read(address)));
      return EEPROM.read(EEPROM.read(address));
      break;
    }else{
      address = EEPROM.read(address) + 1;
    }
  }
  // Serial.print("not found\t");
  //   Serial.println(name);
  return 0;
}
