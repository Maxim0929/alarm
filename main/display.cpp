#include "display.h"
#include "list.h"
#include "eprom.h"


Display::Display(): lcd(0x27, 20 , 4){
}

void Display::printVal(int value, uint8_t pos){

  lcd.setCursor(17, pos);
  lcd.print("   ");

  if(value < 10) lcd.setCursor(19, pos);
  else if(value < 100) lcd.setCursor(18, pos);
  else lcd.setCursor(17, pos);

  lcd.print(value);
    
}

void Display::setup(uint8_t br){
  this->br = br;
  //Serial.println(br);
  pinMode(BR_PIN, OUTPUT);
  analogWrite(BR_PIN, br);
  lcd.init();
  lcd.backlight();
  for(int i = 0; i < 8; i++){
    lcd.createChar(i, custChar[i]);
  }
}


void Display::printList(const List& list, bool doClear){
  uint8_t max = 4;
  uint8_t first = list.getFirst();
  uint8_t current = list.getCurrent();
  uint8_t listsNumber = list.getListsNumber();
  uint8_t valuesNumber = list.getValuesNumber();
  int value = 0;
  if(doClear){
     clear();
  }else{
    for(int i = 0; i < 4; i++){
      lcd.setCursor(0, i);
      lcd.print(" ");
    }
  }

  lcd.setCursor(0, current - first);
  lcd.print(">");

  if(doClear){
    if(list.getNamesNumber() < 4) max = list.getNamesNumber();

    for(int i = 0; i < max; i++){
      lcd.setCursor(1, i);

      if(first + i < listsNumber || first + i >= listsNumber + valuesNumber){// if List
        lcd.print(list.getNames()[first + i]);

      }else if(first + i < listsNumber + valuesNumber){// if value
         lcd.print(list.getNames()[first + i]);

         value = list.getValues()[first + i - listsNumber].getValue(0);

         if(value < 10) lcd.setCursor(19, i);
         else if(value < 100) lcd.setCursor(18, i);
         else lcd.setCursor(17, i);

         lcd.print(value);
      }
    }
  }
}//list: o - menu, 1 - effects, 2 - trait
void Display::printList(const List& list){ printList(list, 1); }

void Display::printTime(uint8_t hour, uint8_t min){
  printDsp(floor(hour/10), 0);
  printDsp(hour%10, 1);
  printDsp(10, 0);
  printDsp(floor(min/10), 2);
  printDsp(min%10, 3);
}//hour 0-23, min 0-59

void Display::printDsp(uint8_t number, uint8_t place){
  if(number == 10){
    for(int i = 0; i < 8; i+=2){
      lcd.setCursor(9, i/2);
      lcd.print((char)bigNums[number][i]);
      lcd.setCursor(10, i/2);
      lcd.print((char)bigNums[number][i+1]);
    }
  }else{
    place *= 5;
    if(place/5 >= 2) place++;
      for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            lcd.setCursor(j + place, i);
            lcd.print((char)bigNums[number][i*4 + j]);
        }
      }
  }
  }
// place 0-3, number 0-10; 10 are dots

void Display::setBrightness(uint8_t br){
  analogWrite(BR_PIN, br);
}

void Display::clear(){ lcd.clear();}

