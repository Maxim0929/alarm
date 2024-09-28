#include "eprom.h"
#include "encoder.h"
#include "led.h"
#include "list.h"
#include "display.h"
#include "time.h"


#define MENULAST 5
#define EFFECTLAST 7
enum Func{TIME, SETTIME, SETALARM, NONEFUNC};
void listClick();
void initMenu(List & menu);
bool setVal(uint8_t encState, const List& list);
void update();
struct ListFunctions;


Led led;

MyEncoder enc;

Eprom epr;

List menu;
List* current;

Display dsp;

Time time;
Alarm alarm;

Func currentFunc = TIME;
#include "listFunctions.h"

ListFunctions listFunctions;

Value* settingValue = nullptr;
Metro backTimer = Metro(5000);

void setup(){
  Serial.begin(9600);
  initMenu();

  dsp.setup(menu.getValue("DISPLAY BRIGHT.")->getValue(0));
  led.init(menu.getValue("LED BRIGHTNESS")->getValue(0));
  current = nullptr;
  time.init();
  alarm.init(epr);
  dsp.printTime(time.getHour(), time.getMin());
}


void loop(){
  if(listFunctions.getFuncName() != "SETTIME"){//update time
    if(time.update()){
      if(current == nullptr)
        dsp.printTime(time.getHour(), time.getMin());
      if(listFunctions.getFuncName() != "SETALARM")// check alarm when time changed
        alarm.update(time, led, enc.getEncstate());
    }
  }

   if(alarm.getisAlarm())// update alarm when isAlarm = true
      alarm.update(time, led, enc.getEncstate());


  if(enc.update()){
    backTimer.reset();
    if(!listFunctions.update()){ // if no function is called

      if(settingValue == nullptr){ // if no value is setting
        update();
      }else{// set Value
        if(setVal(enc.getEncstate(), *settingValue, current->getCurrent() - current->getFirst())){
          if(settingValue->getName() == "DISPLAY BRIGHT.") dsp.setBrightness(settingValue->getValue(1)); // update display brightness
          if(settingValue->getName() == "LED BRIGHTNESS") led.setBrightness(settingValue->getValue(1)); // update display brightness
          settingValue = nullptr;
          led.updateStrip(*menu.getList("EFFECTS"), 1);
        }
      }
    }
  }


  //if(currentFunc == TIME)
   // led.updateStrip(*menu.next.getPtr("EFFECTS"), 0);

  if(backTimer.check() && current != nullptr){//go to time screen when nothing happend 5 sec
    current = nullptr;
    listFunctions.off();
    if(settingValue != nullptr){
      epr.write(settingValue->getName() + settingValue->getID(), settingValue->getValue(1));
      if(settingValue->getName() == "DISPLAY BRIGHT.") dsp.setBrightness(settingValue->getValue(1));// seperate func
      if(settingValue->getName() == "LED BRIGHTNESS") led.setBrightness(settingValue->getValue(1));
      led.updateStrip(*menu.getList("EFFECTS"), 1);
      settingValue = nullptr;
    }
    dsp.clear();
    dsp.printTime(time.getHour(), time.getMin());
  }
}


void update(){
  if(current == nullptr){ // if time screen
    if(enc.getEncstate() == 4){
      current = &menu;
      dsp.printList(*current, 1);
    }
  }else{
    if(enc.getEncstate() == 1){ // click in list
      if(current->getCurrent() < current->getListsNumber())  // click on list
        listClick();
      else if(current->getCurrent() < current->getListsNumber() + current->getValuesNumber()) // click on value
        settingValue = &current->getValues()[current->getCurrent() - current->getListsNumber()];
      else
        listFunctions.callFunction(current->getNames()[current->getCurrent()]); // click on function
    }
    else{
      current->update(enc.getEncstate(), dsp);
    }
  }
}


void listClick(){
  current = &current->getLists()[current->getCurrent()];

  current->setCurrent(0);
  current->setFirst(0);
  dsp.printList(*current);
  if(current->getName() != "EFFECTS"){
    led.setStripState(current->getName());
    led.updateStrip(*menu.getList("EFFECTS"), 1);
  }
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









// void ListFunctions::setVal(){

//       dsp.printList(*current);
//       led.updateStrip(*menu.next.getPtr("EFFECTS"), 1);
// }





void initMenu(){
  uint8_t namesNumber, listsNumber, valuesNumber;
  //menu
  namesNumber = 6;
  listsNumber = 1;
  valuesNumber = 2;


  String* names = new String[namesNumber];
  names[0] = "EFFECTS";
  names[1] = "DISPLAY BRIGHT.";
  names[2] = "LED BRIGHTNESS";
  names[3] = "SETALARM";
  names[4] = "SETTIME";
  names[5] = "BACK";

  int maxValues[4];
  maxValues[0] = 255;
  maxValues[1] = 255;
 // epr.print();

  menu.initAll("MENU", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);



  // effects
  namesNumber = 8;
  listsNumber = 6;
  valuesNumber = 0;

  names = new String[namesNumber];
  names[0] = "COLOUR";
  names[1] = "FLOW";
  names[2] = "COLOUR CHANGE";
  names[3] = "RANDOM";
  names[4] = "DRIFT";
  names[5] = "SNAKE";
  names[6] = "OFF";
  names[7] = "BACK";


  current = menu.getLists();
  if(current != nullptr)
    current[0].initAll("EFFECTS", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[0].setPrev(&menu);


  current = menu.getList("EFFECTS")->getLists();
  if(current != nullptr){


    // colour
    namesNumber = 3;
    listsNumber = 0;
    valuesNumber = 2;

    names = new String[namesNumber];
    names[0] = "COLOUR";
    names[1] = "AMOUNT";
    names[2] = "BACK";

    maxValues[0] = 359;
    maxValues[1] = 600;

    current[0].initAll("COLOUR", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[0].setPrev(menu.getList("EFFECTS"));


    // flow
    namesNumber = 4;
    listsNumber = 0;
    valuesNumber = 3;

    names = new String[namesNumber];
    names[0] = "COLOUR";
    names[1] = "AMOUNT";
    names[2] = "SPEED";
    names[3] = "BACK";

    maxValues[0] = 359;
    maxValues[1] = 600;
    maxValues[2] = 255;

    if(current != nullptr)
      current[1].initAll("FLOW", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[1].setPrev(menu.getList("EFFECTS"));



    // colour change
    namesNumber = 5;
    listsNumber = 0;
    valuesNumber = 4;

    names = new String[namesNumber];
    names[0] = "COLOUR FROM";
    names[1] = "COLOUR TO";
    names[2] = "AMOUNT";
    names[3] = "SPEED";
    names[4] = "BACK";

    maxValues[0] = 359;
    maxValues[1] = 359;
    maxValues[2] = 600;
    maxValues[3] = 200;// not sure

    if(current != nullptr)
      current[2].initAll("COLOUR CHANGE", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[2].setPrev(menu.getList("EFFECTS"));



    // random
    namesNumber = 3;
    listsNumber = 0;
    valuesNumber = 2;

    names = new String[namesNumber];
    names[0] = "DELAY";
    names[1] = "AMOUNT";
    names[2] = "BACK";

    maxValues[0] = 5000;
    maxValues[1] = 600;

    if(current != nullptr)
      current[3].initAll("RANDOM", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[3].setPrev(menu.getList("EFFECTS"));



    // drift
    namesNumber = 3;
    listsNumber = 0;
    valuesNumber = 2;

    names = new String[namesNumber];
    names[0] = "DELAY";
    names[1] = "AMOUNT";
    names[2] = "BACK";

    maxValues[0] = 255;
    maxValues[1] = 600;

    if(current != nullptr)
      current[4].initAll("DRIFT", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[4].setPrev(menu.getList("EFFECTS"));



    // snake
    namesNumber = 3;
    listsNumber = 0;
    valuesNumber = 2;

    names = new String[namesNumber];
    names[0] = "COLOUR";
    names[1] = "AMOUNT";
    names[2] = "BACK";

    maxValues[0] = 359;
    maxValues[1] = 600;

    if(current != nullptr)
      current[5].initAll("SNAKE", names, namesNumber, listsNumber, valuesNumber, epr, maxValues);
    current[5].setPrev(menu.getList("EFFECTS"));

  }

}

bool setVal(uint8_t encState, const Value& value, uint8_t pos){
  int val = value.getValue(0);
  int maxValue = value.getMaxValue();
  switch(encState){
    case 1:
      epr.write(value.getName() + value.getID(), value.getValue(1));
      return 1;
    break;
    case 2:
      if(val == 0) value.setValue(maxValue, 0);
      else value.setValue(val - 1, 0);
      dsp.printVal(value.getValue(0), pos);
    break;
    case 3:
      if(val == maxValue) value.setValue(0, 0);
      else value.setValue(val + 1, 0);
      dsp.printVal(value.getValue(0), pos);
    break;
    case 5:
      if(val > 9) value.setValue(val - 10, 0);
      dsp.printVal(value.getValue(0), pos);
    break;
    case 6:
      if(val < maxValue - 9) value.setValue(val + 10, 0);
      dsp.printVal(value.getValue(0), pos);
    break;
  }
  return 0;
}









