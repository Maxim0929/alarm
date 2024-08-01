#include "display.h"
#include "list.h"
#include "eprom.h"


Display::Display(): lcd(0x27, 20 , 4){
  br = 50;
}
void Display::printNum(uint8_t number){
      printDsp(floor(number / 100), 1);
      printDsp(floor((number % 100) / 10), 2);
      printDsp(number % 10, 3);
}

void Display::setup(){
  pinMode(BR_PIN, OUTPUT);
  analogWrite(BR_PIN, br);
  lcd.init();
  lcd.backlight();
  for(int i = 0; i < 8; i++){
    lcd.createChar(i, custChar[i]);
  }
}

void Display::printTime(uint8_t hour, uint8_t min){
  printDsp(floor(hour/10), 0);
  printDsp(hour%10, 1);
  printDsp(10, 0);
  printDsp(floor(min/10), 2);
  printDsp(min%10, 3);
}//hour 0-23, min 0-59

void Display::printList(const List& list, bool doClear){
  uint8_t max = 4;
  if(doClear){
     clear();
  }else{
    for(int i = 0; i < 4; i++){
      lcd.setCursor(0, i);
      lcd.print(" ");
    }
  }

  lcd.setCursor(0, list.getCurrent() - list.getFirst());
  lcd.print(">");
  if(doClear){
    if(list.getValue() < 4) max = 3;
    for(int i = 0; i < max; i++){
      lcd.setCursor(1, i);
      lcd.print(list.next[list.getFirst() + i].getName());
    }
  }
}//list: o - menu, 1 - effects, 2 - trait
void Display::printList(const List& list){ printList(list, 1); }

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

void Display::setBr(uint8_t br){
  analogWrite(BR_PIN, br);
}

void Display::clear(){ lcd.clear();}

