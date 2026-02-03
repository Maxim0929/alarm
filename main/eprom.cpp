#include "eprom.h"

Eprom::Eprom(){
  if(FIRST) {
    EEPROM.update(0, 0xFF);
    Serial.println("WARNING in eeprom.h FIRST = 1!");
  }
}

Eprom::~Eprom(){}


// val address, name[0], name[1], ... , val
void Eprom::write(const String& name, uint8_t value){
  if(COMPILE) Serial.println("epr.write()\t");

  int addr = searchAddress(name);

  //found
  int valAddr = EEPROM.read(addr);
  if(valAddr != 0xFF){
    EEPROM.update(valAddr, value);
    return;
  }
  // not found
  // cowerflow check
  if(addr + name.length() + 2 > EEPROM.length()){
    Serial.println("EEPROM overflow");
    return;
  }
  // write value address
  valAddr = name.length() + 1 + addr;
  EEPROM.update(addr, valAddr);

  for(int i = addr + 1; i < valAddr; i++){
    // write name
    EEPROM.write(i, name[i - addr - 1]);
  }
  // write value
  EEPROM.update(valAddr, value);
  // set last address to 0xFF
  EEPROM.update(valAddr + 1, 0xFF);
}



uint8_t Eprom::read(const String& name){
  if(COMPILE) Serial.println("epr.read()\t");

  int addr = searchAddress(name);

  if(EEPROM.read(addr) == 0xFF) return 0;

  return EEPROM.read(EEPROM.read(addr));
}



// return address of the value of the name. If this name does not exist return last address.
int Eprom::searchAddress(const String& name){
  int address = 0;

  while(EEPROM.read(address) != 0xFF){
    uint8_t valAddr = EEPROM.read(address);

    if(name.length() != valAddr - address - 1){
      address = valAddr + 1;
      continue;
    }

    bool found = true; 
    for(int i = address + 1; i < valAddr; ++i){
      if(char(EEPROM.read(i)) != name[i - address - 1]){
        address = valAddr + 1;
        found = false;
        break;
      }
    }

    if(!found) continue;
    if(COMPILE) Serial.println("Found " + name + " at address " + address);
    return address;
  }

  if(COMPILE) Serial.println(name + " not found");
  return address;
}



void Eprom::print(){
  int address = 0;

  while(EEPROM.read(address) != 0xFF){
    int valAddr = EEPROM.read(address);
    Serial.print(EEPROM.read(address));
    Serial.print(" ");

    for(int i = address + 1; i < valAddr; i++){
      Serial.print(char(EEPROM.read(i)));
    }
    Serial.print(" ");
    Serial.println(EEPROM.read(valAddr));

    address = valAddr + 1;
  }
}



















