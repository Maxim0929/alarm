#include "display.h"
#include "list.h"
//#include "eprom.h"


Display::Display(): lcd_(0x27, 20 , 4){
}

void Display::printVal(int value, uint8_t pos){

  lcd_.setCursor(16, pos);
  lcd_.print("   "); // clear old value

  lcd_.setCursor(19 - (int)floor(log10(value)), pos);

  lcd_.print(value);
}

void Display::setup(uint8_t br){
  pinMode(BR_PIN, OUTPUT);
  analogWrite(BR_PIN, br);
  lcd_.init();
  lcd_.backlight();
  for(int i = 0; i < 8; i++){
    lcd_.createChar(i, custChar[i]);
  }
}


void Display::printList(const List& list, bool doClear){
  uint8_t first = list.first();
  uint8_t current = list.current();


  if(doClear){
    uint8_t listsNumber = list.listsNumber();
    uint8_t settingsNumber = list.settingsNumber();
    uint8_t max = (list.namesNumber() < 4) ? list.namesNumber() : 4;

    for(int i = 0; i < max; i++){
      
      // print names
      String space = "                   ";
      String text = list.names()[first + i];
      uint8_t spaceNum = 19 - text.length();

      lcd_.setCursor(0, i);
      lcd_.print(" " + text + space.substring(0, spaceNum));

      // add Value
      if(first + i >= listsNumber && first + i < listsNumber + settingsNumber){
        int value = list.settings()[first + i - listsNumber].value();
        printVal(value, i);
      }
    }
  }else{
    // clear cursor
    for(int i = 0; i < 4; i++){
      lcd_.setCursor(0, i);
      lcd_.print(" ");
    }
  }

  lcd_.setCursor(0, current - first);
  lcd_.print(">");
}//list: o - menu, 1 - effects, 2 - trait

void Display::printTime(uint8_t hour, uint8_t min){
  printNum(floor(hour/10), 0);
  printNum(hour%10, 1);
  printDots();
  printNum(floor(min/10), 2);
  printNum(min%10, 3);
}//hour 0-23, min 0-59


void Display::printDots(){
  for(int i = 0; i < 8; i+=1){
      int pos = (i % 2 == 0)? 9 : 10;
      lcd_.setCursor(pos, i/2);
      lcd_.print((char)bigNums[10][i]);
    }
}

void Display::printNum(uint8_t number, uint8_t place){
  place *= 5;
  if(place/5 >= 2) place++;

  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
        lcd_.setCursor(i + place, j);
        lcd_.print((char)bigNums[number][j*4 + i]);
    }
  }
}
// place 0-3, number 0-10;

void Display::setBrightness(uint8_t br){
  analogWrite(BR_PIN, br);
}

void Display::clear(){ lcd_.clear();}

