#define BR_PIN 8
#include <string.h>
#include <avr/pgmspace.h>

const char PROGMEM menu0[] = "SET TIME";
const char PROGMEM menu1[] = "SET ALARM";
const char PROGMEM menu2[] = "DISPLAY BRIGHTNESS";
const char PROGMEM menu3[] = "EFFECTS";
const char PROGMEM menu4[] = "BACK";
const char* const menu[] PROGMEM = {menu0, menu1, menu2, menu3, menu4};


const char PROGMEM effects0[] = "COLOUR";
const char PROGMEM effects1[] = " ";
const char PROGMEM effects2[] = " ";
const char PROGMEM effects3[] = "BACK";

const char* const effects[] PROGMEM = {effects0, effects1, effects2, effects3};

struct Display{
public:
  LiquidCrystal_I2C lcd;
  byte br = 50;
//   bool blinkM;
//   bool blinkH;
  Display(): lcd(0x27, 20 , 4){}
  void setup(byte hour, byte min);
  void printTime(byte hour, byte min);
  void printMenu(byte current, byte first);
  void updateMenu(byte encState, byte& menuCurrent, byte& menuFirst);
  void setBr(byte encState);

//   // }
//   // void printEffects(byte st_currentE, byte st_firstE){
//   //   char buffer[30];

//   //   for(int i = 0; i < 4; i++){
//   //     lcd.setCursor(0, i);
//   //     lcd.print(" ");
//   //   }
//   //   lcd.setCursor(0, st_currentE - st_firstE);
//   //   lcd.print(">");

//   //   for(int i = 0; i < 4; i++){
//   //     lcd.setCursor(1, i);
//   //     strcpy_P(buffer, (char*)pgm_read_word(&(effects[st_firstE + i])));
//   //     lcd.print(buffer);
//   //   }
//   // }
  void printNum(byte number);
    void clear(){ lcd.clear();}
private:
  byte custChar[8][8] = {
    {31, 31, 31, 31, 31, 0, 0, 0},//five up
    {0, 0, 0, 31, 31, 31, 31, 31},// five down
    {0, 0, 0, 0, 0, 0, 31, 31},// two down           2
    {31, 31, 0, 0, 0, 0, 0, 0},//two up
    {0, 0, 0, 0, 0, 7, 7, 7},//left up
    {7, 7, 7, 0, 0, 0, 0, 0},//left down
    {0, 0, 0, 0, 0, 28, 28, 28},//rigth up
    {28, 28, 28, 0, 0, 0, 0, 0}//right down
  };
  uint8_t bigNums[11][16] = {
    {255, 0, 0, 255, 255 ,254, 254, 255, 255, 254, 254, 255, 255, 1, 1, 255},
    {254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255},
    {0, 0, 0, 255, 2, 2, 2, 255, 255, 3, 3,3, 255, 1, 1, 1},
    {0, 0, 0, 255, 2, 2, 2, 255, 3, 3, 3, 255, 1, 1, 1, 255},
    {255, 254, 254, 255, 255, 2, 2, 255, 3, 3, 3, 255, 254, 254, 254, 255},
    {255, 0, 0, 0, 255, 2, 2, 2, 3, 3, 3, 255, 1, 1, 1, 255},
    {255, 0, 0, 0, 255, 2, 2, 2, 255, 3, 3, 255, 255, 1, 1, 255},
    {0, 0, 0, 255, 254, 254, 254, 255, 254, 254, 254, 255, 254, 254, 254, 255},
    {255, 0, 0, 255, 255, 2, 2, 255, 255, 3, 3, 255, 255, 1, 1, 255},
    {255, 0, 0, 255, 255, 2, 2, 255, 3, 3, 3, 255, 1, 1, 1, 255},
    {4, 6, 5, 7, 4, 6, 5, 7}
  };
  void printDsp(byte number, byte place);
};

void Display::printNum(byte number){
      printDsp(floor(number / 100), 1);
      printDsp(floor((number % 100) / 10), 2);
      printDsp(number % 10, 3);
}

void Display::setup(byte hour, byte min){
  pinMode(BR_PIN, OUTPUT);
  analogWrite(BR_PIN, br);
  lcd.init();
  lcd.backlight();
  for(int i = 0; i < 8; i++){
    lcd.createChar(i, custChar[i]);
  }
  printTime(hour, min);
}

void Display::printTime(byte hour, byte min){
  printDsp(floor(hour/10), 0);
  printDsp(hour%10, 1);
  printDsp(10, 0);
  printDsp(floor(min/10), 2);
  printDsp(min%10, 3);
}//hour 0-23, min 0-59

void Display::printMenu(byte current, byte first){
     char buffer[30];
 
     for(int i = 0; i < 4; i++){
       lcd.setCursor(0, i);
       lcd.print(" ");
     }
     lcd.setCursor(0, current - first);
     lcd.print(">");

     for(int i = 0; i < 4; i++){
      lcd.setCursor(1, i);
      strcpy_P(buffer, (char*)pgm_read_word(&(menu[first + i])));
      lcd.print(buffer);
     }
}

void Display::printDsp(byte number, byte place){
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

void Display::updateMenu(byte encState, byte& menuCurrent, byte& menuFirst){
  bool print = 0;
  bool clearDsp = 0;
  if(encState == 2){
    if(menuCurrent == 0){
      menuCurrent = MENULAST;
      menuFirst = MENULAST - 3;
      clearDsp = 1;
      print = 1;
    }else if(menuCurrent != menuFirst){
      menuCurrent--;
      print = 1;
    }else{
      menuCurrent--;
      menuFirst--;
      clearDsp = 1;
      print = 1;
    }
  }else if(encState == 3){
    if(menuCurrent == MENULAST){
      menuCurrent = 0;
      menuFirst = 0;
      clearDsp = 1;
      print = 1;
    }else if(menuCurrent != menuFirst + 3){
      menuCurrent++;
      print = 1;
    }else{
      menuCurrent++;
      menuFirst++;
      clearDsp = 1;
      print = 1;
    }
  }
  if(clearDsp) clear();
  if(print) printMenu(menuCurrent, menuFirst);
}

void Display::setBr(byte encState){
  switch(encState){
    case 2:
      if(br == 0) br = 255;
      else br--;
      printNum(br);
    break;
    case 3:
      if(br == 255) br = 0;
      else br++;
      printNum(br);
    break;
    case 5:
      if(br > 9) br-=10;
      printNum(br);
    break;
    case 6:
      if(br < 246) br+=10;
      printNum(br);
    break;
  }
}
































