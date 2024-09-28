#include "eprom.h"

Eprom::Eprom(){
  if(FIRST) {
    EEPROM.write(0,0);
    Serial.println("WARNING in eporm.h FIRST = 1!");
  }
}

Eprom::~Eprom(){}


void Eprom::write(const String name, uint8_t value){
  if(COMPILE) Serial.println("epr.write\t");
  int address = 0;
  String crnName = "";


  while(EEPROM.read(address) != 0){// search if element with this name exists

    if(name.length() == EEPROM.read(address) - address - 1){

      for(int i = address + 1; i < EEPROM.read(address); i++){
        crnName += char(EEPROM.read(i));
      }
      
      if(name == crnName){
        if(COMPILE){
          Serial.print("found\t");
          Serial.println(name);
        }
        EEPROM.update(EEPROM.read(address), value);
        return;
      }else{
        address = EEPROM.read(address) + 1;
        crnName = "";
      }

    }else{
      address = EEPROM.read(address) + 1;
    }
  }
  
  if(EEPROM.read(address) == 0){
    if(COMPILE){
      Serial.print("not found\t");
      Serial.println(name);
    }

    EEPROM.update(address, name.length() + 1 + address); // value address // +1
    for(int i = address + 1, k = 0; i < EEPROM.read(address); i++, k++){
      EEPROM.write(i, name[k]);// value name
    }
    EEPROM.update(EEPROM.read(address), value); // value
    EEPROM.update(EEPROM.read(address) + 1, 0);
  }
  
}

uint8_t Eprom::read(const String name){
  if(COMPILE) Serial.println("epr.read\t");
  int address = 0;
  String crnName = "";
  while(EEPROM.read(address) != 0){// search if element with this name exists

    if(name.length() == EEPROM.read(address)- address - 1){

      for(int i = address + 1; i < EEPROM.read(address); i++){
        crnName += char(EEPROM.read(i));
      }

      if(name == crnName){
        if(COMPILE){
          Serial.print("found\t");
          Serial.println(name);
          Serial.println(EEPROM.read(EEPROM.read(address)));
        }
        return EEPROM.read(EEPROM.read(address));
      }else{
        address = EEPROM.read(address) + 1;
        crnName = "";
      }

    }else{
      address = EEPROM.read(address) + 1;
    }
  }
  if(COMPILE){
    Serial.print("not found\t");
    Serial.println(name);
  }

  if(FIRST) write(name, 0);

  return 0;
}


void Eprom::print(){
  int address = 0;
  while(EEPROM.read(address) != 0){
    Serial.print(EEPROM.read(address));
    for(int i = address + 1; i < EEPROM.read(address); i++){
      Serial.print(char(EEPROM.read(i)));
    }
    Serial.print(" ");
    Serial.println(EEPROM.read(EEPROM.read(address)));

    address = EEPROM.read(address) + 1;
  }
}
